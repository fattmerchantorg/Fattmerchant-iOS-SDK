import Foundation

protocol Job {
  associatedtype Output
  func start() async throws -> Output
}
