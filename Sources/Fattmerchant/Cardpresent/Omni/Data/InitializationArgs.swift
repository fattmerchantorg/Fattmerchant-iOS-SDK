public struct InitializationArgs {
  /// The Bundle Identifier for your Application.
  public let applicationId: String
  
  /// An ephemeral token for use with the Stax API. Obtain with `GET /ephemeral` route in the Stax API.
  public var ephemeralToken: String
  
  public init(applicationId: String, ephemeralToken: String) {
    self.applicationId = applicationId
    self.ephemeralToken = ephemeralToken
  }
}
