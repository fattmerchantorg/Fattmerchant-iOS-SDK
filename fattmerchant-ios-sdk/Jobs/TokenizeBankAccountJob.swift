import Foundation

/// Tokenizes a `BankAccount` object into a Stax `PaymentMethod` chargable in the future
class TokenizeBankAccountJob: Job {
  typealias Output = PaymentMethod
  
  private let bank: BankAccount
  
  public init(bank: BankAccount) {
    self.bank = bank
  }
  
  func start() async throws -> PaymentMethod {
    guard let http = Services.resolve(StaxHttpService.self) else {
      throw StaxNetworkingException.serviceNotInitialized
    }

    // Attempt to make Customer via Stax API
    let potentialCustomer = Customer(fullName: bank.personName)
    let customer = try await http.postCustomer(customer: potentialCustomer)
    
    // Make PaymentMethod from Customer and BankAccount
    let potentialPaymentMethod = PaymentMethod(bank: bank, customer: customer)
    let paymentMethod = try await http.postPaymentMethod(paymentMethod: potentialPaymentMethod)

    return paymentMethod
  }
}
