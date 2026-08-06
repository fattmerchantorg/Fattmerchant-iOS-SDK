extension StaxInvoice {

  internal static func createMeta(from request: StaxTransactionRequest) throws -> JSONCodable {
    var meta: [String:JSONCodable] = [:]

    if let subtotal = request.subtotal {
      meta["subtotal"] = JSONCodable.double(subtotal)
    } else {
      meta["subtotal"] = JSONCodable.double(request.amount.dollars())
    }

    if let tax = request.tax { meta["tax"] = JSONCodable.double(tax); }
    if let memo = request.memo { meta["memo"] = JSONCodable.string(memo); }
    if let reference = request.reference { meta["reference"] = JSONCodable.string(reference); }
    if let tip = request.tip { meta["tip"] = JSONCodable.double(tip); }
    if let shippingAmount = request.shippingAmount { meta["shippingAmount"] = JSONCodable.double(shippingAmount); }
    if let poNumber = request.poNumber { meta["poNumber"] = JSONCodable.string(poNumber); }

    if let lineItems = request.lineItems {
      var codables = [] as [JSONCodable]
      for item in lineItems {
        codables.append(try JSONCodable.encode(item))
      }
      meta["lineItems"] = JSONCodable.array(codables)
    }

    return JSONCodable.object(meta)
  }
}
