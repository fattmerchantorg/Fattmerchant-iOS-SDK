extension TransactionResult {
  internal func createTransactionMeta() -> JSONCodable {
    var meta: [String:JSONCodable] = [:]

    #if !targetEnvironment(simulator)
    if source.contains(ChipDnaDriver.source) {
      if let user = userReference { meta["nmiUserRef"] = JSONCodable.string(user) }
      if let local = localId { meta["cardEaseReference"] = JSONCodable.string(local) }
      if let external = externalId { meta["nmiTransactionId"] = JSONCodable.string(external) }
    }
    #endif

    if let gateway = gatewayResponse { meta["gatewayResponse"] = JSONCodable.string(gateway) }
    if let subtotal = request?.subtotal { meta["subtotal"] = JSONCodable.double(subtotal) }
    if let tax = request?.tax { meta["tax"] = JSONCodable.double(tax) }
    if let memo = request?.memo { meta["memo"] = JSONCodable.string(memo) }
    if let reference = request?.reference { meta["reference"] = JSONCodable.string(reference) }
    if let tip = request?.tip { meta["tip"] = JSONCodable.double(tip) }
    
    if let items = request?.staxLineItems {
      var array: [JSONCodable] = []
      for item in items {
        if let codable = try? JSONCodable.encode(item) {
          array.append(codable)
        }
      }
      meta["lineItems"] = JSONCodable.array(array)
    }

    return JSONCodable.object(meta)
  }
}
