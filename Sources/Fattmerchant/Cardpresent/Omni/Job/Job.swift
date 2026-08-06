protocol Job: Actor {
  associatedtype ResultType

  func start() async -> JobResult<ResultType>
}
