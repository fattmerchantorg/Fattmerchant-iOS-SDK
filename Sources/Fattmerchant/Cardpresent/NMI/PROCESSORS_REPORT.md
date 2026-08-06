# Get Merchant Processors and Value Added Services Details

## Overview

This endpoint returns all services that are active, offered, or in a free trial state for a merchant, or a list of merchants. It provides comprehensive details about processors and value-added services that you can see in the partner portal.

## Endpoint

**POST** `https://secure.nmi.com/api/v4/processors/reports`

## Authentication

Requires NMI v4 API key in the `Authorization` header.

## Request Parameters

| Parameter | Type | Required | Description |
|-----------|------|----------|-------------|
| `maxResults` | String | Yes | Number of max results in response. Must be in range from 1 to 1000. |
| `merchantIds` | [String] | No | Array of merchant IDs to filter results. |
| `offset` | String | No | Start point for the responses (for pagination). |

## Response Structure

The response is a paginated report containing:

- **Report metadata**: `reportId`, `offset`, `maxResults`, `totalResults`, `hasMore`
- **Pagination links**: `first`, `last`, `next`, `previous`
- **Results array**: Array of processor/service objects

Each processor/service object includes:

- Basic info: `id`, `processorType`, `processorName`, `serviceId`, `merchantId`
- Configuration: `mcc`, `accountClassification`, `status`, `condition`
- Limits: `maxTicketAmount`, `maxMonthlyVolume`, `applyToAllCardType`
- Settings: `enableDuplicateChecking`, `allowMerchantOverride`, `duplicateTime`
- Payment details: `paymentTypes`, `currencies`, `descriptor`
- Requirements: `requiredFields` object with extensive field requirements
- Processor-specific: `processorFields`, `extraDetails`, `faceConfiguration`
- Settlement: `settlementTime`, `settlementTimezone`
- Metadata: `created`, `updated`, `freeTrialEndDate`, `freeTrialDaysLeft`

## Usage Example

```swift
import Foundation

let nmiService = NMIService(
    apiKey: "your_v4_api_key",
    securityKey: "your_security_key"
)

// Get processors for specific merchants
Task {
    do {
        let response = try await nmiService.getProcessorsReport(
            maxResults: "1000",
            merchantIds: ["33", "33453"],
            offset: "0"
        )
        
        print("Total Results: \(response.totalResults ?? 0)")
        
        if let processors = response.results {
            for processor in processors {
                print("Service: \(processor.serviceId ?? "N/A")")
                print("Status: \(processor.status ?? "N/A")")
                print("Type: \(processor.processorType ?? "N/A")")
                
                if let paymentTypes = processor.paymentTypes {
                    print("Accepts: \(paymentTypes.joined(separator: ", "))")
                }
            }
        }
        
        // Handle pagination
        if let hasMore = response.hasMore, hasMore {
            let nextPage = try await nmiService.getProcessorsReport(
                maxResults: "1000",
                merchantIds: ["33", "33453"],
                offset: "\(response.offset ?? 0 + 1000)"
            )
        }
        
    } catch let error as NMINetworkError {
        print("Error: \(error.detail)")
    }
}
```

## Response Example

```json
{
  "objectType": "report",
  "reportId": 155,
  "offset": 0,
  "maxResults": 1000,
  "totalResults": 17,
  "hasMore": false,
  "links": {
    "first": "https://secure.nmi.com/api/v4/processors/reports/155?maxResults=1000",
    "last": "https://secure.nmi.com/api/v4/processors/reports/155?offset=0&maxResults=1000"
  },
  "results": [
    {
      "objectType": "processor",
      "id": 12,
      "processorType": "feature",
      "serviceId": "acu",
      "merchantId": "33",
      "status": "active",
      "condition": "mandatory",
      "created": "2018-03-09T16:03:48+00:00",
      "updated": "2023-03-24T18:39:07+00:00"
    },
    {
      "objectType": "processor",
      "id": 440,
      "processorType": "card",
      "processorId": "tsys",
      "processorName": "tsys",
      "serviceId": "tsys",
      "merchantId": "33453",
      "mcc": "5999",
      "accountClassification": "ecommerce",
      "maxMonthlyVolume": "100000.00",
      "status": "active",
      "paymentTypes": ["visa", "mastercard", "amex", "discover", "diners", "jcb"],
      "currencies": ["USD"]
    }
  ]
}
```

## Processor Types

- **card**: Credit/debit card processors
- **feature**: Value-added services (ACU, fraud prevention, etc.)
- **ach**: ACH/bank account processors

## Status Values

- **active**: Processor is active and processing
- **pending**: Processor setup is pending

## Condition Values

- **mandatory**: Required for merchant account
- **offered**: Available but optional
- **notOffered**: Not available for this merchant

## Common Services

| Service ID | Description |
|------------|-------------|
| `acu` | Automatic Card Updater |
| `tsys` | TSYS payment processor |
| `authorize.net` | Authorize.Net gateway |
| `stripe` | Stripe payment processor |

## Use Cases

1. **Merchant Onboarding**: Check which processors/services are configured for a merchant
2. **Service Management**: View all available and active services
3. **Billing Reconciliation**: Get processor details for billing purposes
4. **Compliance Audits**: Review processor configurations and settings
5. **Support**: Troubleshoot merchant configurations

## Important Notes

⚠️ **Never use real API keys when testing**
- Always use test merchant accounts for testing
- Use test API keys, never production keys during development

⚠️ **Response Size**
- Responses can be quite lengthy if the merchant has several processors
- Use pagination for large datasets
- Consider filtering by `merchantIds` to reduce response size

⚠️ **Rate Limiting**
- Be mindful of API rate limits
- Implement appropriate error handling and retries

## References

- [Official NMI Documentation](https://docs.nmi.com/reference/get-merchant-processors-and-value-added-services-details)
- [NMI Partner Portal](https://secure.nmi.com)
