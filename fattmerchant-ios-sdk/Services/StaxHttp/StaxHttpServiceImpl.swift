import Foundation

class StaxHttpServiceImpl: StaxHttpService {
  private let client: NetworkService
  
  required init(_ networking: any NetworkService) {
    self.client = networking
  }
  
  /// Makes a `GET` call to the `/self` endpoint in the Stax API
  func getSelf() async throws -> StaxSelf {
    return try await request(path: "/self", method: .get)
  }
  
  func getCardReaderSettings() async throws -> CardReaderSettings {
    return try await request(path: CardReaderSettings.resourceEndpoint(), method: .get)
  }
  
  func getInvoice(id: String) async throws -> Invoice {
    return try await request(path: "/invoice/\(id)", method: .get)
  }
  
  func postVoidOrRefund() async {
    
  }
  
  func postCaptureTransaction() async {
    
  }
  
  func postCustomer(customer: Customer) async throws -> Customer {
    let data = try? self.encoder.encode(customer)
    return try await request(path: "/customer", body: data, method: .post)
  }
  
  func postInvoice(invoice: Invoice) async throws -> Invoice {
    let data = try? self.encoder.encode(invoice)
    return try await request(path: "/invoice", body: data, method: .post)
  }
  
  func putInvoice(invoice: Invoice, id: String) async throws -> Invoice {
    let data = try? self.encoder.encode(invoice)
    return try await request(path: "/invoice/\(id)", body: data, method: .put)
  }
  
  func postPaymentMethod(paymentMethod: PaymentMethod) async throws -> PaymentMethod {
    let data = try? self.encoder.encode(paymentMethod)
    return try await request(path: "/payment-method", body: data, method: .post)
  }
  
  func postPaymentMethodToken(paymentMethod: PaymentMethod) async throws -> PaymentMethod {
    let data = try? self.encoder.encode(paymentMethod)
    return try await request(path: "/payment-method/token", body: data, method: .post)
  }
  
  func postTransaction(transaction: Transaction) async throws -> Transaction {
    let data = try? self.encoder.encode(transaction)
    return try await request(path: "/transaction", body: data, method: .post)
  }
  
  func putTransaction(transaction: Transaction, id: String) async throws -> Transaction {
    let data = try? self.encoder.encode(transaction)
    return try await request(path: "/transaction/\(id)", body: data, method: .put)
  }
  
  private func request<T>(path: String, body: Data? = nil, method: HttpMethod) async throws -> T where T: Codable {
    guard let base = Environment.LIVE.baseUrl() else {
      throw URLError(.badURL)
    }
    
    let request = client.generateURLRequest("\(base)\(path)", method, body)
    let (isSuccess, response) = await withCheckedContinuation { continuation in
      client.fetch(request: request) { (a, b) in
        continuation.resume(returning: (a, b))
      }
    }
    
    guard let data = response as? Data else {
      throw URLError(.badServerResponse)
    }

    do {
      let result = try self.decoder.decode(T.self, from: data)
      return result
    } catch {
      // When the API throws an error it, returns json in the following structure
      // {
      //  "method": [
      //    "The selected method is invalid."
      //  ]
      // }
      guard let json = try? JSONSerialization.jsonObject(with: data, options: []) else {
        throw StaxNetworkingException.couldNotParseResponse(nil)
      }
      
      // It's some kind of JSON, parse out the common Stax JSON forms
      switch json {
        case let map as [String: [String]]:
          let errorStrings = map.values.flatMap { $0 }
          throw StaxNetworkingException.unknown(errorStrings.first ?? nil)
        case let arr as [String]:
          throw StaxNetworkingException.unknown(arr.first ?? nil)
        default:
          throw StaxNetworkingException.couldNotParseResponse(nil)
      }
    }
  }
}
