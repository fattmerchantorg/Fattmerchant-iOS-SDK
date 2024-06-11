import Foundation

protocol StaxHttpService {
  init(_ networking: NetworkService)
  func getSelf() async throws -> StaxSelf
  func getCardReaderSettings() async throws -> CardReaderSettings
  func getInvoice(id: String) async throws -> Invoice
  func postVoidOrRefund() async -> Void
  func postCaptureTransaction() async -> Void
  func postCustomer(customer: Customer) async throws -> Customer
  func postInvoice(invoice: Invoice) async throws -> Invoice
  func putInvoice(invoice: Invoice, id: String) async throws -> Invoice
  func postPaymentMethod(paymentMethod: PaymentMethod) async throws -> PaymentMethod
  func postPaymentMethodToken(paymentMethod: PaymentMethod) async throws -> PaymentMethod
  func postTransaction(transaction: Transaction) async throws -> Transaction
  func putTransaction(transaction: Transaction, id: String) async throws -> Transaction
}
