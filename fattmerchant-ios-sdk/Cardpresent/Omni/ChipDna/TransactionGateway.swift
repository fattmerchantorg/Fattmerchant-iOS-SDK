import Foundation

/// Communicates with Transaction Gateway via the Direct Post API
/// https://fattmerchant.transactiongateway.com/merchants/resources/integration/integration_portal.php
internal class TransactionGateway {

  /// The base url of Transaction Gateway
  static let baseUrl = "https://secure.nmi.com/api/query.php"

  /// Fetches the CCExpiration for a transaction from TransactionGateway
  /// - Parameters:
  ///   - transactionId: the id of the Transaction
  ///   - securityKey: authentication for Transaction Gateway
  ///   - completion: a block to run once finished. Gets the cc expiration, if found
  static func getTransactionCcExpiration(securityKey: String, transactionId: String, completion: @escaping (String?) -> Void) {
    let session = URLSession(configuration: URLSessionConfiguration.default)
    let url = URL(string: "\(baseUrl)?security_key=\(securityKey)&transaction_id=\(transactionId)")!
    var request = URLRequest(url: url)
    request.httpMethod = "POST"

    let dataTask = session.dataTask(with: request) { (data, _, error) in
      guard error == nil, let data = data else {
        completion(nil)
        return
      }

      let parser = ChipDnaXMLTransactionParser()
      parser.parseExpirationDate(from: data, transactionId: transactionId, completion: completion)
    }

    dataTask.resume()
  }

}
