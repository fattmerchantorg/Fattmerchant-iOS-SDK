import Foundation

/// Service for interacting with the NMI (Network Merchants Inc.) API.
/// Provides convenient methods for processor management and transaction queries.
public final class NMIService: Sendable {
  /// The HTTP client used to make API requests
  private let httpClient: NMIHttpClientProtocol
  
  /// The security key for query API authentication
  private let securityKey: String
  
  /// Creates a new NMI service.
  /// - Parameters:
  ///   - apiKey: The NMI API key for authenticated requests.
  ///   - securityKey: The security key for query API requests.
  ///   - baseURL: The base URL for the NMI API. Defaults to production.
  public init(
    apiKey: String,
    securityKey: String,
    baseURL: URL = URL(string: "https://secure.nmi.com")!
  ) {
    self.httpClient = NMIHttpClient(baseURL: baseURL, apiKey: apiKey)
    self.securityKey = securityKey
  }
  
  /// Internal initializer for dependency injection (useful for testing).
  /// - Parameters:
  ///   - httpClient: The HTTP client to use for requests.
  ///   - securityKey: The security key for query API requests.
  internal init(httpClient: NMIHttpClientProtocol, securityKey: String) {
    self.httpClient = httpClient
    self.securityKey = securityKey
  }
  
  // MARK: - Processor Operations
  
  /// Adds a processor or value-added service to a merchant account.
  /// - Parameter request: The processor configuration details.
  /// - Returns: The processor response containing the processor ID and status.
  /// - Throws: `NMINetworkError` if the request fails.
  ///
  /// Example:
  /// ```swift
  /// let request = NMIAddProcessorRequest(
  ///     merchantId: "12345",
  ///     processorType: "authorize.net",
  ///     login: "loginId",
  ///     transactionKey: "transKey"
  /// )
  /// let response = try await nmiService.addProcessor(request: request)
  /// ```
  public func addProcessor(request: NMIAddProcessorRequest) async throws -> NMIProcessorResponse {
    let apiRequest = NMIApiRequest<NMIProcessorResponse>(
      path: "/api/v4/processors",
      method: .post,
      body: request
    )
    
    return try await httpClient.perform(apiRequest)
  }
  
  // MARK: - Transaction Query Operations
  
  /// Fetches the credit card expiration date for a transaction.
  /// Uses the Query API endpoint.
  /// - Parameter transactionId: The ID of the transaction to query.
  /// - Returns: The credit card expiration date (format: MMYY) if found, nil otherwise.
  /// - Throws: `NMINetworkError` if the request fails.
  ///
  /// Example:
  /// ```swift
  /// let expiration = try await nmiService.getTransactionCcExpiration(transactionId: "123456")
  /// print("Card expires: \(expiration ?? "N/A")")
  /// ```
  public func getTransactionCcExpiration(transactionId: String) async throws -> String? {
    // NMI Query API uses a different endpoint and format (form-encoded)
    let baseUrl = "https://secure.nmi.com/api/query.php"
    let urlString = "\(baseUrl)?security_key=\(securityKey)&transaction_id=\(transactionId)"
    
    guard let url = URL(string: urlString) else {
      throw NMINetworkError.invalidURL
    }
    
    var request = URLRequest(url: url)
    request.httpMethod = "POST"
    
    return try await withCheckedThrowingContinuation { continuation in
      let session = URLSession(configuration: .default)
      let task = session.dataTask(with: request) { data, response, error in
        if let error = error {
          continuation.resume(throwing: NMINetworkError.underlying(error))
          return
        }
        
        guard let data = data else {
          continuation.resume(returning: nil)
          return
        }
        
        // Parse XML response to extract expiration date
        let parser = ChipDnaXMLTransactionParser()
        parser.parseExpirationDate(from: data, transactionId: transactionId) { expiration in
          continuation.resume(returning: expiration)
        }
      }
      
      task.resume()
    }
  }
  
  /// Retrieves merchant processors and value-added services details.
  /// Returns all services that are active, offered, or in a free trial state for a merchant.
  /// - Parameters:
  ///   - maxResults: Number of max results (1-1000).
  ///   - merchantIds: Optional array of merchant IDs to filter results.
  ///   - offset: Optional offset for pagination.
  /// - Returns: Report containing processor and service details with pagination info.
  /// - Throws: `NMINetworkError` if the request fails.
  ///
  /// Example:
  /// ```swift
  /// let response = try await nmiService.getProcessorsReport(
  ///     maxResults: "1000",
  ///     merchantIds: ["33", "33453"],
  ///     offset: "0"
  /// )
  /// if let processors = response.results {
  ///     for processor in processors {
  ///         print("Service: \(processor.serviceId ?? "N/A")")
  ///     }
  /// }
  /// ```
  public func getProcessorsReport(
    maxResults: String,
    merchantIds: [String]? = nil,
    offset: String? = nil
  ) async throws -> NMIProcessorsReportResponse {
    let requestBody = NMIGetProcessorsReportRequest(
      maxResults: maxResults,
      merchantIds: merchantIds,
      offset: offset
    )
    
    let apiRequest = NMIApiRequest<NMIProcessorsReportResponse>(
      path: "/v4/processors/reports",
      method: .post,
      body: requestBody
    )
    
    return try await httpClient.perform(apiRequest)
  }
  
  // MARK: - Helper Methods
  
  /// Validates that the NMI service is properly configured.
  /// - Returns: `true` if the service has valid credentials.
  public func isConfigured() -> Bool {
    return !securityKey.isEmpty
  }
}
