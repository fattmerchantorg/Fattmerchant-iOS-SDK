import XCTest

@testable import StaxApi
final class StaxApiTests: XCTestCase {

  func testGet() async throws {
    let api = StaxApi(token)
    let response = try await api.getCustomers()
    print(response.count)
  }
}
