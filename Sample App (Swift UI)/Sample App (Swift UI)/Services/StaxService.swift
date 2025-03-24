import Foundation
import Fattmerchant
import SwiftUI


class OmniInitializer: NSObject, TransactionUpdateDelegate, UserNotificationDelegate, MobileReaderConnectionStatusDelegate, UsbAccessoryDelegate {
    
    private var omni: Omni?
    private let apiKey: String = ""  // Replace with your actual API key
    @Published var isInitialized = false  // For binding with SwiftUI views

    override init() {
        super.init()
        omni = Omni()
        // signatureProvider is not required for initialization
        omni?.transactionUpdateDelegate = self
        omni?.userNotificationDelegate = self
        omni?.mobileReaderConnectionUpdateDelegate = self
        omni?.usbAccessoryDelegate = self
    }

    // This method is for initializing Omni
    public func initializeOmni() {
        // initialize Omni SDK and set delegates
        LogMessagesManager.shared.log("Attempting initialization...")

        omni?.initialize(params: initParams(), completion: {
            self.isInitialized = true // Indicate that Omni has been initialized
            LogMessagesManager.shared.log("Omni Initialized")
        }, error: { (error) in
            if let initialized = self.omni?.isInitialized, initialized {
                self.isInitialized = true // Hide the button if already initialized
                LogMessagesManager.shared.log("Omni Initialized")
            }
            LogMessagesManager.shared.log("Error: \(error.localizedDescription)") // Log the error message
        })
    }

    // Function for initializing Omni with ephemeral token (used in asynchronous operations)
    func initializeEphemeralOmni() async throws {
        // Example API call for ephemeral token
        guard let url = URL(string: "https://apiprod.fattlabs.com/ephemeral") else {
            throw URLError(.badURL)  // Throw an error if the URL is invalid
        }
        
        var request = URLRequest(url: url)
        request.httpMethod = "GET"
        request.setValue("application/json", forHTTPHeaderField: "Accept")
        request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
        request.timeoutInterval = 5
        
        // Use `try await` to fetch data asynchronously
        let (data, _) = try await URLSession.shared.data(for: request)
        
        guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
              let token = json["token"] as? String else {
            throw URLError(.cannotParseResponse)  // Throw an error if token parsing fails
        }
        
        // Initialize Omni with the ephemeral token
        self.omni?.initialize(params: self.initParams(token: token), completion: {
            LogMessagesManager.shared.log("Omni Initialized with Ephemeral Token")
        }, error: { error in
            LogMessagesManager.shared.log("Error: \(error.message)")
        })
    }

    // Private helper method to get Omni initialization parameters
    private func initParams(token: String? = nil) -> Omni.InitParams {
        return Omni.InitParams(appId: "fmiossample", apiKey: token ?? apiKey, environment: Environment.LIVE)
    }
    
    // TransactionUpdateDelegate method implementation
    func onTransactionUpdate(transactionUpdate: TransactionUpdate) {
        // Handle transaction updates here, for example, update UI or log
        LogMessagesManager.shared.log("Hi")
    }

    // Example of how you can handle logging and sending updates to SwiftUI views
    func log(_ message: String) {
        LogMessagesManager.shared.log(message)
    }

    // Methods for the other delegates
    func onUserNotification(userNotification: UserNotification) {
        LogMessagesManager.shared.log("User Notification: \(userNotification)")
    }

    func mobileReaderConnectionStatusUpdate(status: MobileReaderConnectionStatus) {
        LogMessagesManager.shared.log("Mobile Reader Connection Status: \(status)")
    }

    func onUsbAccessoryConnected() {
        LogMessagesManager.shared.log("USB Accessory Connected")
    }

    func onUsbAccessoryDisconnected() {
        LogMessagesManager.shared.log("USB Accessory Disconnected")
    }

}

