//
//  TakeTapPayment.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 10/22/25.
//  Copyright © 2025 Fattmerchant. All rights reserved.
//

import Foundation
import ProximityReader

/// A use case for taking a payment using Apple's Tap to Pay on iPhone functionality
///
/// **Requirements:**
/// - iOS 17.4 or later
/// - Compatible iPhone device (iPhone XS or later)
/// - Tap to Pay on iPhone must be set up in the device settings
///
/// **Important:** Always check `TakeTapPayment.isSupported()` before attempting to use this feature
/// to provide appropriate user messaging for unsupported devices or iOS versions.
class TakeTapPayment {

    typealias Exception = TakeTapPaymentException

    var tapDriver: TapDriver
    var invoiceRepository: InvoiceRepository
    var customerRepository: CustomerRepository
    var paymentMethodRepository: PaymentMethodRepository
    var transactionRepository: TransactionRepository
    var request: TransactionRequest
    var signatureProvider: SignatureProviding?
    weak var transactionUpdateDelegate: TransactionUpdateDelegate?
    weak var userNotificationDelegate: UserNotificationDelegate?

    init(
        tapDriver: TapDriver,
        invoiceRepository: InvoiceRepository,
        customerRepository: CustomerRepository,
        paymentMethodRepository: PaymentMethodRepository,
        transactionRepository: TransactionRepository,
        request: TransactionRequest,
        signatureProvider: SignatureProviding?,
        transactionUpdateDelegate: TransactionUpdateDelegate?,
        userNotificationDelegate: UserNotificationDelegate?
    ) {

        self.tapDriver = tapDriver
        self.invoiceRepository = invoiceRepository
        self.customerRepository = customerRepository
        self.paymentMethodRepository = paymentMethodRepository
        self.transactionRepository = transactionRepository
        self.request = request
        self.signatureProvider = signatureProvider
        self.transactionUpdateDelegate = transactionUpdateDelegate
        self.userNotificationDelegate = userNotificationDelegate
    }

    func start(
        completion: @escaping (Transaction) -> Void,
        failure: @escaping (OmniException) -> Void
    ) {
        // Check iOS version first
        guard #available(iOS 17.4, *) else {
            failure(Exception.iOSVersionNotSupported)
            return
        }

        self.getOrCreateInvoice(failure) { (createdInvoice) in
            self.takeTapToPayPayment(
                signatureProvider: self.signatureProvider,
                transactionUpdateDelegate: self.transactionUpdateDelegate,
                userNotificationDelegate: self.userNotificationDelegate,
                failure
            ) { (tapToPayResult) in

                // This is a callback that voids the transaction and calls the fail block
                let voidAndFail: (OmniException) -> Void = { exception in
                    // By the time this is invoked, the NMI transaction went through fine but something happened while doing
                    // one of the calls to Omni. Since the transaction is pending confirmation, then we need to void it *before*
                    // invoking the failure block. That way the customer gets their money back
                    self.tapDriver.void(transactionResult: tapToPayResult) {
                        _ in
                        failure(exception)
                    }
                }

                self.createCustomer(tapToPayResult, voidAndFail) {
                    (createdCustomer) in
                    self.createPaymentMethod(
                        for: createdCustomer,
                        tapToPayResult,
                        voidAndFail
                    ) { (createdPaymentMethod) in
                        self.updateInvoice(
                            createdInvoice,
                            with: createdPaymentMethod,
                            and: createdCustomer,
                            voidAndFail
                        ) { (updatedInvoice) in
                            self.createTransaction(
                                result: tapToPayResult,
                                paymentMethod: createdPaymentMethod,
                                customer: createdCustomer,
                                invoice: updatedInvoice,
                                voidAndFail
                            ) { completedTransaction in

                                // Make sure the transaction from Omni has an id. This should be true pretty much all the time
                                guard
                                    let transactionId = completedTransaction.id
                                else {
                                    voidAndFail(
                                        TakeTapPaymentException
                                            .couldNotCreateTransaction(
                                                detail: nil
                                            )
                                    )
                                    return
                                }

                                // If the transaction is a pre-auth, then we don't need to capture it
                                if self.request.preauth {
                                    completion(completedTransaction)
                                    return
                                }

                                self.tapDriver.capture(
                                    transaction: completedTransaction
                                ) { (success) in
                                    if success {
                                        completion(completedTransaction)
                                    } else {
                                        /* We couldn't capture the transaction. So void the NMI transaction and mark it failed on Omni */

                                        // Mark omni transaction failed
                                        let failedTransaction =
                                            completedTransaction
                                        failedTransaction.success = false
                                        failedTransaction.message =
                                            "Error capturing the transaction"

                                        // Fail the transaction in omni
                                        self.transactionRepository.update(
                                            model: failedTransaction,
                                            id: transactionId,
                                            completion: { _ in
                                                voidAndFail(
                                                    TakeTapPaymentException
                                                        .couldNotCaptureTransaction
                                                )
                                                return
                                            },
                                            error: { _ in
                                                voidAndFail(
                                                    TakeTapPaymentException
                                                        .couldNotCaptureTransaction
                                                )
                                                return
                                            }
                                        )
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    internal func createTransaction(
        result: TransactionResult,
        paymentMethod: PaymentMethod,
        customer: Customer,
        invoice: Invoice,
        _ failure: @escaping (OmniException) -> Void,
        _ completion: @escaping (Transaction) -> Void
    ) {
        let transactionToCreate = Transaction()

        guard let paymentMethodId = paymentMethod.id else {
            failure(
                Exception.couldNotUpdateInvoice(
                    detail: "Payment method id is required"
                )
            )
            return
        }

        guard let lastFour = getLastFour(for: result.maskedPan) else {
            failure(
                Exception.couldNotCreatePaymentMethod(
                    detail: "Could not retrieve masked pan"
                )
            )
            return
        }

        guard let transactionMeta = createTransactionMeta(from: result) else {
            failure(
                Exception.couldNotCreateTransaction(
                    detail: "Could not generate transaction meta json"
                )
            )
            return
        }

        var gatewayResponseJson: JSONValue?

        if let authCode = result.authCode, result.source.lowercased() == "nmi" {
            let gatewayResponse = [
                "gateway_specific_response_fields": [
                    "nmi": [
                        "authcode": authCode
                    ]
                ]
            ]

            gatewayResponseJson = gatewayResponse.jsonValue()
        }

        guard let customerId = customer.id else {
            failure(
                Exception.couldNotCreateTransaction(
                    detail: "Customer id is required"
                )
            )
            return
        }

        guard let invoiceId = invoice.id else {
            failure(
                Exception.couldNotCreateTransaction(
                    detail: "Invoice id is required"
                )
            )
            return
        }

        transactionToCreate.paymentMethodId = paymentMethodId
        transactionToCreate.total = request.amount.dollars()
        transactionToCreate.success = result.success ?? false
        transactionToCreate.lastFour = lastFour
        transactionToCreate.meta = transactionMeta
        transactionToCreate.type = "charge"
        transactionToCreate.method = "card"
        transactionToCreate.source = "iOS|CPSDK|TapToPay"
        transactionToCreate.customerId = customerId
        transactionToCreate.invoiceId = invoiceId
        transactionToCreate.response = gatewayResponseJson
        transactionToCreate.token = result.externalId
        transactionToCreate.message = result.message
        transactionToCreate.isRefundable = false
        transactionToCreate.isVoidable = false

        // Mark the transaction as pre-auth, if necessary
        if request.preauth {
            transactionToCreate.type = "pre_auth"
            transactionToCreate.preAuth = true
            transactionToCreate.isCaptured = 0
            transactionToCreate.isVoidable = true
        }

        transactionRepository.create(
            model: transactionToCreate,
            completion: completion,
            error: failure
        )
    }

    /// Creates a JSONValue object from the transactionResult, including only the items that make up the TransactionMeta
    fileprivate func createTransactionMeta(
        from transactionResult: TransactionResult
    ) -> JSONValue? {
        var dict = [String: JSONValue?]()

        // Add tap to pay specific metadata
        dict["tapToPayDevice"] = JSONValue(UIDevice.current.name)

        if let externalId = transactionResult.externalId {
            dict["nmiTransactionId"] = JSONValue(externalId)
        }

        if let gatewayResponse = transactionResult.gatewayResponse {
            dict["gatewayResponse"] = JSONValue(gatewayResponse)
        }

        if let staxLineItemResponse = transactionResult.request?.staxLineItems {
            dict["staxLineItems"] = JSONValue(staxLineItemResponse)
        }

        if let subtotal = transactionResult.request?.subtotal {
            dict["subtotal"] = JSONValue(subtotal)
        }

        if let tax = transactionResult.request?.tax {
            dict["tax"] = JSONValue(tax)
        }

        if let memo = transactionResult.request?.memo {
            dict["memo"] = JSONValue(memo)
        }

        if let reference = transactionResult.request?.reference {
            dict["reference"] = JSONValue(reference)
        }

        if let tip = transactionResult.request?.tip {
            dict["tip"] = JSONValue(tip)
        }

        return dict.jsonValue()
    }

    /// Takes a payment using Tap to Pay
    private func takeTapToPayPayment(
        signatureProvider: SignatureProviding?,
        transactionUpdateDelegate: TransactionUpdateDelegate?,
        userNotificationDelegate: UserNotificationDelegate?,
        _ failure: @escaping (OmniException) -> Void,
        _ completion: @escaping (TransactionResult) -> Void
    ) {

        self.request.transactionPOI = "TAP_TO_MOBILE"

        tapDriver.performTransaction(
            with: self.request,
            signatureProvider: signatureProvider,
            transactionUpdateDelegate: transactionUpdateDelegate,
            userNotificationDelegate: userNotificationDelegate,
            completion: completion
        )
    }

    // Reusing these methods from TakeMobileReaderPayment as they work the same way

    /// Creates the json meta for the invoice as modeled in the given transaction request
    internal static func createInvoiceMeta(from request: TransactionRequest)
        -> JSONValue?
    {
        var dict = [String: JSONValue?]()

        if let subtotal = request.subtotal {
            dict["subtotal"] = JSONValue(subtotal)
        } else {
            dict["subtotal"] = JSONValue(request.amount.dollars())
        }

        if let tax = request.tax {
            dict["tax"] = JSONValue(tax)
        }

        if let memo = request.memo {
            dict["memo"] = JSONValue(memo)
        }

        if let reference = request.reference {
            dict["reference"] = JSONValue(reference)
        }

        if let tip = request.tip {
            dict["tip"] = JSONValue(tip)
        }

        if let shippingAmount = request.shippingAmount {
            dict["shippingAmount"] = JSONValue(shippingAmount)
        }

        if let poNumber = request.poNumber {
            dict["poNumber"] = JSONValue(poNumber)
        }

        if let staxLineItems = request.staxLineItems {
            dict["staxLineItems"] = JSONValue(staxLineItems)
        }

        return dict.jsonValue()
    }

    fileprivate func updateInvoice(
        _ invoice: Invoice,
        with paymentMethod: PaymentMethod,
        and customer: Customer,
        _ failure: @escaping (OmniException) -> Void,
        completion: @escaping (Invoice) -> Void
    ) {
        let newInvoice = Invoice()

        guard let id = invoice.id else {
            failure(
                Exception.couldNotUpdateInvoice(
                    detail: "Invoice id is required"
                )
            )
            return
        }

        guard let paymentMethodId = paymentMethod.id else {
            failure(
                Exception.couldNotUpdateInvoice(
                    detail: "Payment method id is required"
                )
            )
            return
        }

        guard let customerId = customer.id else {
            failure(
                Exception.couldNotUpdateInvoice(
                    detail: "Customer id is required"
                )
            )
            return
        }

        newInvoice.customerId = customerId
        newInvoice.paymentMethodId = paymentMethodId
        invoiceRepository.update(
            model: newInvoice,
            id: id,
            completion: completion,
            error: failure
        )
    }

    fileprivate func getLastFour(for maskedPan: String?) -> String? {
        guard
            let maskedPan = maskedPan,
            maskedPan.count > 4,
            let lastFourIdx = maskedPan.index(
                maskedPan.endIndex,
                offsetBy: -4,
                limitedBy: maskedPan.startIndex
            )
        else {
            return nil
        }

        return String(maskedPan.suffix(from: lastFourIdx))
    }

    fileprivate func createPaymentMethod(
        for customer: Customer,
        _ result: TransactionResult,
        _ failure: @escaping (OmniException) -> Void,
        completion: @escaping (PaymentMethod) -> Void
    ) {
        let paymentMethodToCreate = PaymentMethod(customer: customer)

        guard let customerId = customer.id else {
            failure(
                Exception.couldNotCreateCustomer(
                    detail: "Customer id is required"
                )
            )
            return
        }

        guard let lastFour = getLastFour(for: result.maskedPan) else {
            failure(
                Exception.couldNotCreatePaymentMethod(
                    detail: "Could not retrieve masked pan"
                )
            )
            return
        }

        guard let cardType = result.cardType else {
            failure(
                Exception.couldNotCreateCustomer(
                    detail: "Card type is required"
                )
            )
            return
        }

        paymentMethodToCreate.cardExp = result.cardExpiration
        paymentMethodToCreate.customerId = customerId
        paymentMethodToCreate.method = PaymentMethodType.card
        paymentMethodToCreate.cardLastFour = lastFour
        paymentMethodToCreate.cardType = cardType
        paymentMethodToCreate.personName =
            "\(customer.firstname) \(customer.lastname)"
        paymentMethodToCreate.tokenize = false
        paymentMethodToCreate.paymentToken = result.paymentToken

        // When the payment method was tokenized, we want to use the
        // createTokenizedPaymentMethod method since it tells Omni to save the token
        if paymentMethodToCreate.paymentToken != nil {
            paymentMethodRepository.createTokenizedPaymentMethod(
                model: paymentMethodToCreate,
                completion: completion,
                error: failure
            )
        } else {
            paymentMethodRepository.create(
                model: paymentMethodToCreate,
                completion: completion,
                error: failure
            )
        }
    }

    fileprivate func createCustomer(
        _ transactionResult: TransactionResult,
        _ failure: @escaping (OmniException) -> Void,
        _ completion: @escaping (Customer) -> Void
    ) {
        let customerToCreate = Customer(
            firstName: "TAP",
            lastName: "CUSTOMER"
        )

        // For tap to pay, we may get customer details from the card
        if let firstName = transactionResult.cardHolderFirstName,
            !firstName.isEmpty
        {
            customerToCreate.firstname = firstName
        }

        if let lastName = transactionResult.cardHolderLastName,
            !lastName.isEmpty
        {
            customerToCreate.lastname = lastName
        }

        // Check transaction source for contactless transactions
        if let transactionSource = transactionResult.transactionSource,
            transactionSource.lowercased().contains("contactless")
                || transactionSource.lowercased().contains("tap")
        {
            if customerToCreate.firstname == "TAP" {
                customerToCreate.firstname = "Mobile Device"
            }
        }

        customerRepository.create(
            model: customerToCreate,
            completion: completion,
            error: failure
        )
    }

    /// Gets the invoice with the id in the transaction request or creates a new one
    internal func getOrCreateInvoice(
        _ failure: @escaping (OmniException) -> Void,
        _ completion: @escaping (Invoice) -> Void
    ) {
        // If an invoiceId was given in the transaction request, we should verify that an invoice with that id exists
        if let invoiceId = request.invoiceId {
            guard !invoiceId.isEmpty else {
                failure(TakeTapPaymentException.invoiceIdCannotBeBlank)
                return
            }
            invoiceRepository.getById(id: invoiceId, completion: completion) {
                (error) in
                failure(TakeTapPaymentException.invoiceNotFound)
            }
        } else {
            let invoiceToCreate = Invoice()
            invoiceToCreate.total = request.amount.dollars()
            invoiceToCreate.url = "https://fattpay.com/#/bill"

            guard let invoiceMetaJson = Self.createInvoiceMeta(from: request)
            else {
                failure(
                    Exception.couldNotCreateInvoice(
                        detail: "Error generating json for meta"
                    )
                )
                return
            }

            invoiceToCreate.meta = invoiceMetaJson
            invoiceRepository.create(
                model: invoiceToCreate,
                completion: { createdInvoice in
                    guard createdInvoice.id?.isEmpty != true else {
                        return failure(
                            TakeTapPaymentException.couldNotCreateInvoice(
                                detail: nil
                            )
                        )
                    }
                    completion(createdInvoice)
                },
                error: failure
            )
        }
    }
}

extension TakeTapPayment {
    /// Checks if Tap to Pay is supported on the current device and iOS version
    static func isSupported() -> (supported: Bool, reason: String?) {
        if #available(iOS 17.4, *) {
            return (true, nil)
        } else {
            return (false, "Tap to Pay on iPhone requires iOS 17.4 or later")
        }
    }
}
