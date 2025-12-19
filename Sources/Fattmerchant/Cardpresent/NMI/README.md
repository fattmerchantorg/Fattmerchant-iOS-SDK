# NMI Service

This directory contains the NMI (Network Merchants Inc.) API integration for the Fattmerchant iOS SDK.

## Overview

The NMI Service provides a clean, type-safe interface for interacting with the NMI payment gateway API. It supports both modern async/await patterns (iOS 15+) and callback-based patterns for older iOS versions.

## Architecture

The service is structured with the following components:

- **NMIService**: Main service class providing high-level API methods
- **NMIHttpClient**: HTTP client for making network requests
- **NMIApiRequest**: Type-safe request model
- **NMIModels**: Data models for requests and responses
- **NMINetworkError**: Error types specific to NMI API operations
- **NMIHttpMethod**: HTTP method enumeration

## Usage

### Initialization

```swift
let nmiService = NMIService(
    apiKey: "your_api_key",
    securityKey: "your_security_key"
)
```

### Add Processor

Add a processor or value-added service to a merchant account:

```swift
let request = NMIAddProcessorRequest(
    merchantId: "12345",
    processorType: "authorize.net",
    login: "loginId",
    transactionKey: "transKey"
)

do {
    let response = try await nmiService.addProcessor(request: request)
    print("Processor ID: \(response.processorId ?? "N/A")")
    print("Status: \(response.status)")
} catch {
    print("Error adding processor: \(error)")
}
```

### Query Transaction

Get credit card expiration date for a transaction:

```swift
do {
    let expiration = try await nmiService.getTransactionCcExpiration(
        transactionId: "123456"
    )
    if let exp = expiration {
        print("Card expires: \(exp)")
    }
} catch {
    print("Error querying transaction: \(error)")
}
```

### Get Processors Report

Retrieve details about merchant processors and value-added services:

```swift
// Get report for specific merchants
let response = try await nmiService.getProcessorsReport(
    maxResults: "1000",
    merchantIds: ["33", "33453"],
    offset: "0"
)

if let results = response.results {
    for processor in results {
        print("Processor: \(processor.processorName ?? "Unknown")")
        print("Service ID: \(processor.serviceId ?? "N/A")")
        print("Status: \(processor.status ?? "N/A")")
        
        // Check payment types
        if let paymentTypes = processor.paymentTypes {
            print("Accepts: \(paymentTypes.joined(separator: ", "))")
        }
    }
}

// Get all processors (no merchant filter)
let allProcessors = try await nmiService.getProcessorsReport(
    maxResults: "100"
)

// Handle pagination
if let hasMore = response.hasMore, hasMore {
    print("More results available")
    if let nextOffset = response.offset {
        let nextPage = try await nmiService.getProcessorsReport(
            maxResults: "100",
            offset: "\(nextOffset + 100)"
        )
    }
}
```

## API Endpoints

### Processor Management

- **POST /api/v4/processors** - Add a processor or value-added service
  - Documentation: https://docs.nmi.com/reference/add-processor-service

- **POST /api/v4/processors/reports** - Get merchant processors and services details
  - Documentation: https://docs.nmi.com/reference/get-merchant-processors-and-value-added-services-details

### Transaction Queries

- **POST /api/query.php** - Query transaction details
  - Used for fetching transaction information like CC expiration dates

## Migration Guide

### From TransactionGateway

The old `TransactionGateway` class has been deprecated. To migrate:

**Old Code:**
```swift
TransactionGateway.getTransactionCcExpiration(
    securityKey: securityKey,
    transactionId: transactionId
) { expiration in
    // Handle result
}
```

**New Code:**
```swift
let nmiService = NMIService(apiKey: apiKey, securityKey: securityKey)

Task {
    do {
        let expiration = try await nmiService.getTransactionCcExpiration(
            transactionId: transactionId
        )
        // Handle result
    } catch {
        // Handle error
    }
}
```

## Error Handling

The service uses `NMINetworkError` for all error cases:

```swift
do {
    let response = try await nmiService.addProcessor(request: request)
} catch let error as NMINetworkError {
    switch error {
    case .invalidURL:
        print("Invalid URL")
    case .invalidResponse:
        print("Invalid response from server")
    case .httpError(let code):
        print("HTTP error: \(code)")
    case .decodingError(let error):
        print("Failed to decode response: \(error)")
    case .underlying(let error):
        print("Network error: \(error)")
    case .apiError(let message):
        print("API error: \(message)")
    }
}
```

## Testing

For testing purposes, you can inject a mock HTTP client:

```swift
let mockClient = MockNMIHttpClient()
let service = NMIService(httpClient: mockClient, securityKey: "test_key")
```

## Requirements

- iOS 13.0+
- Swift 5.0+
- Async/await support for iOS 15.0+ (fallback to callback-based for older versions)

## Future Enhancements

The following NMI API endpoints could be added in the future:

- Transaction processing
- Subscription management
- Customer vault operations
- Batch processing
- Reporting endpoints

## References

- [NMI API Documentation](https://docs.nmi.com/reference)
- [Add Processor Service](https://docs.nmi.com/reference/add-processor-service)
