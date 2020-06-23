//
//  AsyncFilterTests.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 5/10/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import XCTest

class AsyncFilterTests: XCTestCase {

  func testFilterEmptyList() {
    let filteredEmptyList = expectation(description: "Get empty list back")

    let items: [String] = []
    filter(items: items, predicate: { _, _ in
      XCTFail("Trying to run predicate, when it should fail before this")
    }, completion: { (filtered: [String]) in
      assert(filtered.isEmpty)
      filteredEmptyList.fulfill()
    })

    wait(for: [filteredEmptyList], timeout: 10.0)
  }

  func testFiltersUsingPredicate() {
    let itemsAreFiltered = expectation(description: "Items were filtered successfully")

    let items: [String] = ["a", "", "b", ""]
    let expectedItems: [String] = ["a", "b"]

    filter(items: items, predicate: asyncIsNotEmpty, completion: { (filtered: [String]) in
      XCTAssertEqual(filtered.sorted(), expectedItems.sorted())
      itemsAreFiltered.fulfill()
    })

    wait(for: [itemsAreFiltered], timeout: 10.0)
  }

  func testFiltersUsingPredicate2() {
    let itemsAreFiltered = expectation(description: "Items were filtered successfully")

    let items: [Word] = [Word("Hello"), Word("yo"), Word("")]

    filter(items: items, predicate: { $0.valueNotEmpty }) { (filtered: [Word]) in
      XCTAssertEqual(filtered.count, 2)
      XCTAssert(filtered.contains(Word("Hello")))
      XCTAssert(filtered.contains(Word("yo")))
      XCTAssertFalse(filtered.contains(Word("")))
      itemsAreFiltered.fulfill()
    }

    wait(for: [itemsAreFiltered], timeout: 10.0)
  }

  /// This class is made purely for testing purposes
  fileprivate class Word: Equatable {

    static func == (lhs: AsyncFilterTests.Word, rhs: AsyncFilterTests.Word) -> Bool {
      return lhs.value == rhs.value
    }

    var value: String = "Hello"

    init(_ val: String) {
      self.value = val
    }

    func valueNotEmpty(completion: @escaping (Bool) -> Void) {
      DispatchQueue.global(qos: .background).async {
        usleep(2000 * UInt32.random(in: 1..<100)) // sleep for some amount of time, less than one second
        completion(!self.value.isEmpty)
      }
    }
  }

  /// Emulates an async operation
  fileprivate func asyncIsNotEmpty(string: String, completion: @escaping (Bool) -> Void) {
    DispatchQueue.global(qos: .background).async {
      usleep(2000 * UInt32.random(in: 1..<100)) // sleep for some amount of time, less than one second
      completion(!string.isEmpty)
    }
  }

}
