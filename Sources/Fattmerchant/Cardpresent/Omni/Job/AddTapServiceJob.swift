import Foundation

/// Job to check if the merchant has the 'taptombl' service active and activate it if not
actor AddTapServiceJob: Job {
    private let nmiService: NMIService
    private let merchantId: String
    
    init(nmiService: NMIService, merchantId: String) {
        self.nmiService = nmiService
        self.merchantId = merchantId
    }
    
    func start() async -> JobResult<Bool> {
        do {
            // Check if the merchant has the 'taptombl' service
            let reportResponse = try await nmiService.getProcessorsReport(
                maxResults: "100",
                merchantIds: [merchantId]
            )
            
            // Check if 'taptombl' service exists and is active
            if let services = reportResponse.results {
                for service in services {
                    if service.serviceId == "taptombl" {
                        // Check if service is active
                        if service.status?.lowercased() == "active" {
                            // Service is already active
                            return JobResult.success(true)
                        }
                    }
                }
            }
            
            // Service not found or not active, need to add/activate it
            let addProcessorRequest = NMIAddProcessorRequest(
                merchantId: merchantId,
                serviceId: "taptombl",
                condition: "offered",
                status: "active"
            )
            
            let addResponse = try await nmiService.addProcessor(request: addProcessorRequest)
            
            if addResponse.status.lowercased() == "success" {
                return JobResult.success(true)
            } else {
                let errorMessage = addResponse.message ?? "Failed to add Tap to Mobile service"
                return JobResult.failure(
                    OmniGeneralException.message("NMI Error: \(errorMessage)")
                )
            }
            
        } catch let error as NMINetworkError {
            // Convert NMI network error to Omni exception
            let errorMessage: String
            switch error {
            case .invalidURL:
                errorMessage = "Invalid NMI URL"
            case .invalidResponse:
                errorMessage = "Invalid NMI response"
            case .httpError(let statusCode):
                errorMessage = "HTTP error \(statusCode)"
            case .decodingError(let underlying):
                errorMessage = "Failed to decode response: \(underlying.localizedDescription)"
            case .underlying(let underlying):
                errorMessage = "Network failure: \(underlying.localizedDescription)"
            case .apiError(let message):
                errorMessage = "NMI API error: \(message)"
            }
            
            return JobResult.failure(
                OmniGeneralException.message(errorMessage)
            )
        } catch {
            return JobResult.failure(
                OmniGeneralException.message("Unexpected error: \(error.localizedDescription)")
            )
        }
    }
}
