import Foundation

extension StaxHttpService {
  var encoder: JSONEncoder {
    let enc = JSONEncoder()
    enc.keyEncodingStrategy = .convertToSnakeCase
    return enc
  }

  var decoder: JSONDecoder {
    let dec = JSONDecoder()
    dec.keyDecodingStrategy = .convertFromSnakeCase
    return dec
  }
}
