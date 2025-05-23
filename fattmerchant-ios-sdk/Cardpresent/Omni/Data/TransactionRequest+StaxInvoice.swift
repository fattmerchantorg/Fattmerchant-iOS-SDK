extension TransactionRequest {
  internal func createInvoiceMeta() -> JSONCodable {
    var meta: [String: JSONCodable] = [:]

    if let subtotal = subtotal {
      meta["subtotal"] = JSONCodable.double(subtotal)
    } else {
      meta["subtotal"] = JSONCodable.double(amount.dollars())
    }

    if let tax = tax { meta["tax"] = JSONCodable.double(tax); }
    if let memo = memo { meta["memo"] = JSONCodable.string(memo); }
    if let reference = reference { meta["reference"] = JSONCodable.string(reference); }
    if let tip = tip { meta["tip"] = JSONCodable.double(tip); }
    if let shipping = shippingAmount { meta["shippingAmount"] = JSONCodable.double(shipping); }
    if let po = poNumber { meta["poNumber"] = JSONCodable.string(po); }

    if let items = staxLineItems {
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
                                            
