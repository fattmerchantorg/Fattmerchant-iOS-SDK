//
//  AsyncFilter.swift
//  fattmerchant-ios-sdk
//
//  Created by Tulio Troncoso on 5/10/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

/// Filters `items` by executing async function `predicate` on each item
///
/// This is useful when you need a list filtered, but the decision to filter an item out is an asynchronous one. Think
/// about filtering a list of random emails to ones that actually exist. You'd need to actually send an email, an
/// async operation, and see if it gets delivered. Only after knowing if its been delivered can you say that you want it
/// in the list
/// - Parameters:
///   - items: the items to filter
///
///   - predicate: a function that takes a single item and returns a function that takes a completion block. This
///   completion block takes a Bool and returns Void. The value passed to the completion block will be the deciding
///   factor in whether or not the item gets filtered out.
///
///   - completion: a function that gets the list of filtered items
public func filter<T>(items: [T], predicate: @escaping (T) -> (@escaping (Bool) -> Void) -> Void, completion: @escaping ([T]) -> Void) {
  guard !items.isEmpty else {
    completion(items)
    return
  }

  DispatchQueue.global(qos: .default).async {
    var result: [T] = []
    let dispatchGroup = DispatchGroup()
    let semaphore = DispatchSemaphore(value: 1)

    items.forEach { item in
      dispatchGroup.enter()
      let addToListIfSuccessful = { (success: Bool) in
        if success {
          semaphore.wait()
          result.append(item)
          semaphore.signal()
        }
        dispatchGroup.leave()
      }

      predicate(item)(addToListIfSuccessful)
    }

    dispatchGroup.wait()
    completion(result)
  }
}

/// Filters `items` by executing async function `predicate` on each item
///
/// This is useful when you need a list filtered, but the decision to filter an item out is an asynchronous one. Think
/// about filtering a list of random emails to ones that actually exist. You'd need to actually send an email, an
/// async operation, and see if it gets delivered. Only after knowing if its been delivered can you say that you want it
/// in the list
/// - Parameters:
///   - items: the items to filter
///
///   - predicate: a function that takes a single item and a completion block. This completion block takes a Bool and
///   returns Void. The value passed to the completion block will be the deciding factor in whether or not the item gets
///   filtered out.
///
///   - completion: a function that gets the list of filtered items
public func filter<T>(items: [T], predicate: @escaping (T, @escaping (Bool) -> Void) -> Void, completion: @escaping ([T]) -> Void) {
  guard !items.isEmpty else {
    completion(items)
    return
  }

  DispatchQueue.global(qos: .default).async {
    var result: [T] = []
    let dispatchGroup = DispatchGroup()
    let semaphore = DispatchSemaphore(value: 1)

    items.forEach { item in
      dispatchGroup.enter()
      let addToListIfSuccessful = { (success: Bool) in
        if success {
          semaphore.wait()
          result.append(item)
          semaphore.signal()
        }
        dispatchGroup.leave()
      }

      predicate(item, addToListIfSuccessful)
    }

    dispatchGroup.wait()
    completion(result)
  }
}
