/// Implementation of CustomerRepository that communicates with the Stax API.
final class CustomerRepositoryImpl: CustomerRepository {
    private let httpClient: StaxHttpClient
    
    init(httpClient: StaxHttpClient) {
        self.httpClient = httpClient
    }
    
    func getCustomer(id: String) async throws -> Customer {
        let request = StaxApiRequest<Customer>(
            path: "/customer/\(id)",
            method: .get
        )
        return try await httpClient.perform(request)
    }
    
    func createCustomer(_ request: CustomerRequest) async throws -> Customer {
        let request = StaxApiRequest<Customer>(
            path: "/customer",
            method: .post,
            body: request
        )
        return try await httpClient.perform(request)
    }
    
    func updateCustomer(id: String, request: CustomerRequest) async throws -> Customer {
        let request = StaxApiRequest<Customer>(
            path: "/customer/\(id)",
            method: .put,
            body: request
        )
        return try await httpClient.perform(request)
    }
    
    func deleteCustomer(id: String) async throws {
        // Using Void as the Response type since we don't expect a response body
        let request = StaxApiRequest<Void>(
            path: "/customer/\(id)",
            method: .delete
        )
        try await httpClient.perform(request)
    }
}
