import Foundation

/// Parses information from a chipdna transaction formatted in XML
class NmiXmlParser: NSObject, XMLParserDelegate {

  /// The expiration date of the card used for the transaction
  var ccExpirationDate: String?

  /// The value of the element that the parser is currently parsing
  var currentValue: String = ""

  /// The name of the element that the parser is currently parsing
  var currentElementName: String = ""

  /// The ID of the transaction we intend to parser out of the XML
  var targetTransactionId: String = ""

  /// `true` when parser element is within the target transaction. `false` otherwise
  ///
  /// The XML can have more objects than the target transaction
  var parsingTargetTransaction: Bool = false

  /// The XMLParser doing the parsing work
  internal var parser: XMLParser?

  /// A block to run once finished
  var completion: ((String?) -> Void)?

  /// Parses the expiration date from the XML
  /// - Parameters:
  ///   - xml: an xml-formatted Transaction from Transaction Gateway
  ///   - completion: a block to run once finished. If the expiration date was retrieved, it is passed here. Nil
  ///   otherwise
  internal func parseExpirationDate(from transactionXml: Data, transactionId: String, completion: @escaping (String?) -> Void) {
    self.completion = completion

    // Make sure transaction id is not empty
    guard !transactionId.isEmpty else {
      completion(nil)
      return
    }

    // Set the target transaction id
    targetTransactionId = transactionId

    // Create parser
    let parser = XMLParser(data: transactionXml)
    self.parser = parser
    parser.delegate = self
    parser.parse()
  }

  /// Stops the parsing and executes the completion block
  func finish() {
    parser?.abortParsing()
    completion?(ccExpirationDate)
  }

  func parser(_ parser: XMLParser, didStartElement elementName: String, namespaceURI: String?,
              qualifiedName qName: String?, attributes attributeDict: [String: String] = [:]) {
    currentElementName = elementName
    currentValue = ""
  }

  func parser(_ parser: XMLParser, didEndElement elementName: String, namespaceURI: String?, qualifiedName qName: String?) {
    // If we finished parsing a transaction and we were parsing the target transaction, then finish
    if currentElementName == "transaction" && parsingTargetTransaction {
      finish()
      return
    }

    // If we finished parsing a transaction id, then check if this transaction id corresponds to the target transaction
    if currentElementName == "transaction_id" {
      parsingTargetTransaction = currentValue == targetTransactionId
      return
    }

    // Make sure we are parsing the target transaction. We don't care about the other objects
    guard parsingTargetTransaction else {
      return
    }

    if currentElementName == "cc_exp" {
      ccExpirationDate = currentValue
      return
    }
  }

  func parser(_ parser: XMLParser, foundCharacters string: String) {
    currentValue += string
  }

  func parserDidEndDocument(_ parser: XMLParser) {
    finish()
  }
}
