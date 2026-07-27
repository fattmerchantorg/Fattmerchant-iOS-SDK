import Foundation

// MARK: - NMI Service Usage Examples
//
// This file contains example code snippets demonstrating how to use the NMIService.
// These examples are for documentation purposes and are not meant to be compiled.

/*
 
 // MARK: - Example 1: Initialize the NMI Service
 
 let nmiService = NMIService(
     apiKey: "your_nmi_api_key_here",
     securityKey: "your_security_key_here"
 )
 
 // For testing with a different environment:
 let testBaseURL = URL(string: "https://test.secure.nmi.com")!
 let testNMIService = NMIService(
     apiKey: "test_api_key",
     securityKey: "test_security_key",
     baseURL: testBaseURL
 )
 
 // MARK: - Example 2: Add a Processor
 
 Task {
     let request = NMIAddProcessorRequest(
         merchantId: "merchant_12345",
         processorType: "authorize.net",
         login: "api_login_id",
         transactionKey: "transaction_key_here",
         settings: [
             "test_mode": "1",
             "cvv_required": "1"
         ]
     )
     
     do {
         let response = try await nmiService.addProcessor(request: request)
         
         if response.status == "success" {
             print("✅ Processor added successfully!")
             print("Processor ID: \(response.processorId ?? "N/A")")
         } else {
             print("❌ Failed to add processor")
             print("Message: \(response.message ?? "No message")")
         }
     } catch let error as NMINetworkError {
         handleNMIError(error)
     } catch {
         print("Unexpected error: \(error)")
     }
 }
 
 // MARK: - Example 3: Query Transaction for CC Expiration
 
 Task {
     do {
         let expiration = try await nmiService.getTransactionCcExpiration(
             transactionId: "transaction_123456"
         )
         
         if let exp = expiration {
             print("Card expiration: \(exp)")
             // Format is typically MMYY (e.g., "1225" for December 2025)
         } else {
             print("No expiration data found for transaction")
         }
     } catch {
         print("Error querying transaction: \(error)")
     }
 }
 
 // MARK: - Example 4: Error Handling
 
 func handleNMIError(_ error: NMINetworkError) {
     switch error {
     case .invalidURL:
         print("❌ Invalid URL - check your API configuration")
         
     case .invalidResponse:
         print("❌ Invalid response from NMI server")
         
     case .httpError(let statusCode):
         print("❌ HTTP Error: \(statusCode)")
         switch statusCode {
         case 401:
             print("Unauthorized - check your API key")
         case 404:
             print("Resource not found")
         case 500...599:
             print("Server error - try again later")
         default:
             print("Unexpected status code")
         }
         
     case .decodingError(let decodingError):
         print("❌ Failed to decode response: \(decodingError.localizedDescription)")
         
     case .underlying(let underlyingError):
         print("❌ Network error: \(underlyingError.localizedDescription)")
         
     case .apiError(let message):
         print("❌ API Error: \(message)")
     }
 }
 
 // MARK: - Example 5: Migration from TransactionGateway
 
 // OLD CODE (deprecated):
 /*
 TransactionGateway.getTransactionCcExpiration(
     securityKey: "security_key_here",
     transactionId: "123456"
 ) { expiration in
     if let exp = expiration {
         print("Expiration: \(exp)")
     }
 }
 */
 
 // NEW CODE (recommended):
 Task {
     let nmiService = NMIService(
         apiKey: "api_key_here",
         securityKey: "security_key_here"
     )
     
     do {
         let expiration = try await nmiService.getTransactionCcExpiration(
             transactionId: "123456"
         )
         
         if let exp = expiration {
             print("Expiration: \(exp)")
         }
     } catch {
         print("Error: \(error)")
     }
 }
 
 // MARK: - Example 6: Using with Omni SDK
 
 class MyPaymentProcessor {
     private let nmiService: NMIService
     
     init(apiKey: String, securityKey: String) {
         self.nmiService = NMIService(
             apiKey: apiKey,
             securityKey: securityKey
         )
     }
     
     func processPayment(transactionId: String) async throws {
         // Query the transaction to get card details
         let expiration = try await nmiService.getTransactionCcExpiration(
             transactionId: transactionId
         )
         
         guard let exp = expiration else {
             throw NSError(
                 domain: "PaymentError",
                 code: 1,
                 userInfo: [NSLocalizedDescriptionKey: "No card expiration found"]
             )
         }
         
         print("Processing payment with card expiring: \(exp)")
         // Continue with payment processing...
     }
     
     func setupProcessor(merchantId: String) async throws {
         let request = NMIAddProcessorRequest(
             merchantId: merchantId,
             processorType: "authorize.net",
             login: "login_id",
             transactionKey: "trans_key"
         )
         
         let response = try await nmiService.addProcessor(request: request)
         
         guard response.status == "success" else {
             throw NSError(
                 domain: "ProcessorError",
                 code: 2,
                 userInfo: [NSLocalizedDescriptionKey: response.message ?? "Failed to add processor"]
             )
         }
         
         print("Processor setup complete: \(response.processorId ?? "N/A")")
     }
 }
 
 // MARK: - Example 7: Get Processors Report
 
 Task {
     do {
         // Get report for specific merchants
         let response = try await nmiService.getProcessorsReport(
             maxResults: "1000",
             merchantIds: ["33", "33453"],
             offset: "0"
         )
         
         print("📊 Processors Report")
         print("Total Results: \(response.totalResults ?? 0)")
         print("Has More: \(response.hasMore ?? false)")
         
         // Check pagination
         if let hasMore = response.hasMore, hasMore {
             print("\n⏭️ More results available")
             if let nextLink = response.links?.next {
                 print("Next page: \(nextLink)")
             }
         }
         
         // Process results
         if let results = response.results {
             print("\n🔍 Found \(results.count) processor(s)/service(s):")
             
             for (index, processor) in results.enumerated() {
                 print("\n--- Processor \(index + 1) ---")
                 print("Type: \(processor.processorType ?? "N/A")")
                 print("Name: \(processor.processorName ?? "N/A")")
                 print("Service ID: \(processor.serviceId ?? "N/A")")
                 print("Status: \(processor.status ?? "N/A")")
                 print("Condition: \(processor.condition ?? "N/A")")
                 print("Merchant ID: \(processor.merchantId ?? "N/A")")
                 
                 // Display payment types
                 if let paymentTypes = processor.paymentTypes, !paymentTypes.isEmpty {
                     print("Accepted Cards: \(paymentTypes.joined(separator: ", "))")
                 }
                 
                 // Display currencies
                 if let currencies = processor.currencies, !currencies.isEmpty {
                     print("Currencies: \(currencies.joined(separator: ", "))")
                 }
                 
                 // Display limits
                 if let maxTicket = processor.maxTicketAmount, !maxTicket.isEmpty {
                     print("Max Ticket: $\(maxTicket)")
                 }
                 if let maxMonthly = processor.maxMonthlyVolume, !maxMonthly.isEmpty {
                     print("Max Monthly: $\(maxMonthly)")
                 }
                 
                 // Display settlement info
                 if let settlementTime = processor.settlementTime {
                     let timezone = processor.settlementTimezone ?? "UTC"
                     print("Settlement: \(settlementTime) \(timezone)")
                 }
             }
         }
         
         // Example: Get all processors without merchant filter
         print("\n\n📋 Getting all processors...")
         let allProcessors = try await nmiService.getProcessorsReport(
             maxResults: "100"
         )
         print("Total processors: \(allProcessors.totalResults ?? 0)")
         
     } catch let error as NMINetworkError {
         print("❌ Error fetching processors report:")
         handleNMIError(error)
     } catch {
         print("❌ Unexpected error: \(error)")
     }
 }
 
 // MARK: - Example 8: Pagination through Processors Report
 
 func fetchAllProcessors(
     nmiService: NMIService,
     merchantIds: [String]? = nil
 ) async throws -> [NMIProcessorsReportResponse.ProcessorService] {
     var allProcessors: [NMIProcessorsReportResponse.ProcessorService] = []
     var currentOffset = 0
     let maxResults = 100
     
     repeat {
         let response = try await nmiService.getProcessorsReport(
             maxResults: "\(maxResults)",
             merchantIds: merchantIds,
             offset: "\(currentOffset)"
         )
         
         if let results = response.results {
             allProcessors.append(contentsOf: results)
         }
         
         guard let hasMore = response.hasMore, hasMore else {
             break
         }
         
         currentOffset += maxResults
         
     } while true
     
     print("✅ Fetched \(allProcessors.count) total processors")
     return allProcessors
 }
 
 // Usage:
 Task {
     let allProcessors = try await fetchAllProcessors(
         nmiService: nmiService,
         merchantIds: ["33", "33453"]
     )
     
     // Process all processors
     for processor in allProcessors {
         print("Processor: \(processor.serviceId ?? "N/A")")
     }
 }
 
 // MARK: - Example 9: Filtering Processors by Status
 
 Task {
     do {
         let response = try await nmiService.getProcessorsReport(
             maxResults: "1000"
         )
         
         guard let processors = response.results else {
             print("No processors found")
             return
         }
         
         // Filter by status
         let activeProcessors = processors.filter { $0.status == "active" }
         let pendingProcessors = processors.filter { $0.status == "pending" }
         
         print("📊 Status Summary:")
         print("Active: \(activeProcessors.count)")
         print("Pending: \(pendingProcessors.count)")
         
         // Filter by processor type
         let cardProcessors = processors.filter { $0.processorType == "card" }
         let features = processors.filter { $0.processorType == "feature" }
         
         print("\n📊 Type Summary:")
         print("Card Processors: \(cardProcessors.count)")
         print("Features/Services: \(features.count)")
         
         // Find specific service
         if let acu = processors.first(where: { $0.serviceId == "acu" }) {
             print("\n✅ ACU Service found:")
             print("Status: \(acu.status ?? "N/A")")
             print("Condition: \(acu.condition ?? "N/A")")
         }
         
     } catch {
         print("Error: \(error)")
     }
 }
 
 // MARK: - Example 10: Validation
 
 func validateNMIService(_ service: NMIService) -> Bool {
     guard service.isConfigured() else {
         print("⚠️ NMI Service is not properly configured")
         return false
     }
     
     print("✅ NMI Service is configured and ready")
     return true
 }
 
 */
