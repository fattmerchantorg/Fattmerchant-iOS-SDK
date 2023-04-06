import XCTest

@testable import StaxApi
final class StaxApiTests: XCTestCase {
  
  func testCustomerApi() async throws {
    let api = StaxApi(token)
    
    let foo = Customer(fullName: "John Smith")
    XCTAssert(foo.firstname.elementsEqual("John"), "Customer first name parsed incorrectly")
    XCTAssert(foo.lastname.elementsEqual("Smith"), "Customer last name parsed incorrectly")

    guard let bar = try await api.createCustomer(customer: foo) else {
      XCTAssert(false, "Could not create customer; API failure")
      return
    }
    XCTAssertNotNil(bar.id, "Customer does not have Stax ID")
    XCTAssert(bar.firstname.elementsEqual("John"))
    XCTAssert(bar.lastname.elementsEqual("Smith"))

    guard let baz = try await api.getCustomer(id: bar.id!) else {
      XCTAssert(false, "Could not get customer; API failure")
      return
    }
    baz.firstname = "Jane"
    XCTAssert(bar.id == baz.id)
    XCTAssert(baz.firstname.elementsEqual("Jane"))
    XCTAssert(baz.lastname.elementsEqual("Smith"))

    guard let qux = try await api.updateCustomer(id: baz.id!, customer: baz) else {
      XCTAssert(false, "Could not update customer; API failure")
      return
    }
    XCTAssert(baz.id == qux.id)
    XCTAssert(qux.firstname.elementsEqual("Jane"))
    XCTAssert(qux.lastname.elementsEqual("Smith"))
    
    guard let final = try await api.deleteCustomer(id: qux.id!) else {
      XCTAssert(false, "Could not delete customer; API failure")
      return
    }
  }
  
  func testInvoiceApi() async throws {
    let api = StaxApi(token)
    
    let foo = Invoice(total: 0.01)
    
    guard let bar = try await api.createInvoice(invoice: foo) else {
      XCTAssert(false, "Could not create invoice; API failure")
      return
    }
    XCTAssertNotNil(bar.id)
    XCTAssertEqual(bar.total, 0.01)
    
    guard let baz = try await api.getInvoice(id: bar.id!) else {
      XCTAssert(false, "Could not get invoice; API failure")
      return
    }
    XCTAssertNotNil(baz.id)
    XCTAssertEqual(baz.id, bar.id)
    XCTAssertEqual(baz.total, 0.01)
    
    baz.total = 0.05
    guard let qux = try await api.updateInvoice(id: baz.id!, invoice: baz) else {
      XCTAssert(false, "Could not update invoice; API failure")
      return
    }
    XCTAssertNotNil(qux.id)
    XCTAssertEqual(qux.id, baz.id)
    XCTAssertEqual(qux.total, 0.05)
    
    guard let final = try await api.deleteInvoice(id: qux.id!) else {
      XCTAssert(false, "Could not delete customer; API failure")
      return
    }
  }
  
  func testItemApi() async throws {
    let api = StaxApi(token)
    
    let foo = Item(name: "Item")
    
    guard let bar = try await api.createItem(item: foo) else {
      XCTAssert(false, "Could not create item; API failure")
      return
    }
    XCTAssertNotNil(bar.id)
    XCTAssertEqual(bar.item, "Item")
    
    guard let baz = try await api.getItem(id: bar.id!) else {
      XCTAssert(false, "Could not get item; API failure")
      return
    }
    XCTAssertNotNil(baz.id)
    XCTAssertEqual(baz.id, bar.id)
    XCTAssertEqual(baz.item, "Item")
    
    baz.item = "New Name"
    guard let qux = try await api.updateItem(id: baz.id!, item: baz) else {
      XCTAssert(false, "Could not update invoice; API failure")
      return
    }
    XCTAssertNotNil(qux.id)
    XCTAssertEqual(qux.id, baz.id)
    XCTAssertEqual(qux.item, "New Name")
    
    guard let final = try await api.deleteItem(id: qux.id!) else {
      XCTAssert(false, "Could not delete customer; API failure")
      return
    }
  }
}
