enum JobResult<T> {
  case success(T)
  case failure(OmniException)
}
