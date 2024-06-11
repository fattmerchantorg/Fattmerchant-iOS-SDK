import Foundation

/// Tokenizes a `CreditCard` object into a Stax `PaymentMethod` chargable in the future
class TokenizeCreditCardJob: Job {
  typealias Output = PaymentMethod
  
  private let card: CreditCard
  
  public init(card: CreditCard) {
    self.card = card
  }
  
  func start() async throws -> PaymentMethod {
    guard let http = Services.resolve(StaxHttpService.self) else {
      throw StaxNetworkingException.serviceNotInitialized
    }

    // Attempt to make Customer via Stax API
    let potentialCustomer = Customer(fullName: card.personName)
    let customer = try await http.postCustomer(customer: potentialCustomer)
    
    // Make PaymentMethod from Customer and CreditCard
    let potentialPaymentMethod = PaymentMethod(card: card, customer: customer)
    let paymentMethod = try await http.postPaymentMethod(paymentMethod: potentialPaymentMethod)

    return paymentMethod
  }
}
