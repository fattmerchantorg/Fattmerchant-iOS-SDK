import Foundation
import Fattmerchant
import SwiftUI

@Observable
class OmniInitializer: TransactionUpdateDelegate, UserNotificationDelegate, MobileReaderConnectionStatusDelegate, UsbAccessoryDelegate {
    static let shared = OmniInitializer()
    private var omni: Omni
    private let apiKey: String = ""
    var isOmniInitialized: Bool = false
    var availableReaders: [MobileReader] = []
    var connectedReader: MobileReader? = nil
    var isConnecting = false
    var logMessages: [String] = []
    private var isInitializing: Bool = false
    
   private init() {
       omni = Omni()
       omni.transactionUpdateDelegate = self
       omni.userNotificationDelegate = self
       omni.mobileReaderConnectionUpdateDelegate = self
       omni.usbAccessoryDelegate = self
       if (omni.isInitialized == false) {
           initializeOmni()
       }
    }

    // Now we define initializeOmni as a method that launches a Task
      public func initializeOmni() {
    
          LogMessagesManager.shared.log("Attempting initialization...")

          // Check if already initializing to avoid multiple initializations
          guard !isInitializing else { return }
          
          // Mark the process as initializing
          isInitializing = true
          
          // Launch a new Task to run the async initialization
          Task {
              do {
                  // Await Omni initialization inside the Task
                  try await withCheckedThrowingContinuation { continuation in
                      omni.initialize(params: self.initParams(), completion: {
                          self.isInitializing = false
                          LogMessagesManager.shared.log("Omni Initialized")
                          continuation.resume()  // Successfully finished, resume continuation
                      }, error: { error in
                          self.isInitializing = false
                          LogMessagesManager.shared.log("Error: \(error.localizedDescription)")
                          continuation.resume(throwing: error)  // Handle error and resume continuation
                      })
                  }
              } catch {
                  // Handle error during initialization if needed
                  LogMessagesManager.shared.log("Initialization failed: \(error.localizedDescription)")
              }
          }
      }

    public func initializeEphemeralOmni() {
        LogMessagesManager.shared.log("Attempting to initialize Omni with Ephemeral Token...")

        // Ensure we are not already initializing
        guard !isInitializing else { return }
        
        isInitializing = true

        // Launch a Task to handle the async operation
        Task {
            do {
                guard let url = URL(string: "https://apiprod.fattlabs.com/ephemeral") else {
                    throw URLError(.badURL)  // Handle invalid URL
                }
                
                var request = URLRequest(url: url)
                request.httpMethod = "GET"
                request.setValue("application/json", forHTTPHeaderField: "Accept")
                request.setValue("Bearer \(apiKey)", forHTTPHeaderField: "Authorization")
                request.timeoutInterval = 5
                
                // Perform the async URLSession call
                let (data, _) = try await URLSession.shared.data(for: request)
                
                // Parse the response
                guard let json = try JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let token = json["token"] as? String else {
                    throw URLError(.cannotParseResponse)
                }
                
                // Initialize Omni with the token
                try await withCheckedThrowingContinuation { continuation in
                    self.omni.initialize(params: self.initParams(token: token), completion: {
                        LogMessagesManager.shared.log("Omni Initialized with Ephemeral Token")
                        self.isInitializing = false
                        continuation.resume()  // Complete the async flow successfully
                    }, error: { error in
                        LogMessagesManager.shared.log("Error: \(error.message)")
                        self.isInitializing = false
                        continuation.resume(throwing: error)  // Handle error and resume
                    })
                }
            } catch {
                LogMessagesManager.shared.log("Initialization failed: \(error.localizedDescription)")
                self.isInitializing = false  // Mark as finished even if failed
            }
        }
    }


    private func initParams(token: String? = nil) -> Omni.InitParams {
           return Omni.InitParams(appId: "fmiossample", apiKey: token ?? apiKey, environment: Environment.LIVE)
       }

    // Function to search for available readers and connect to the first one
       func searchForReaders() {
           guard omni.isInitialized else {
               LogMessagesManager.shared.log("Omni is not initialized yet.")
               return
           }

           LogMessagesManager.shared.log("Searching for available readers...")

           // Wrapping the completion handler in a Task
           Task {
               omni.getAvailableReaders(completion: { readers in
                   if readers.isEmpty {
                       LogMessagesManager.shared.log("No readers found")
                   } else {
                       LogMessagesManager.shared.log("Found readers:")
                       for reader in readers {
                           LogMessagesManager.shared.log(reader.name) // Log each reader's name
                       }
                       
                       // Connect to the first reader if available
                       if let firstReader = readers.first {
                           self.connectReader(reader: firstReader)
                       }
                   }
               }, error: { error in
                   LogMessagesManager.shared.log("Error: \(error.localizedDescription)")
               })
           }
       }
       
       // Function to connect to a chosen reader
       func connectReader(reader: MobileReader) {
           isConnecting = true
           LogMessagesManager.shared.log("Trying to connect to \(reader.name)")

           // Wrap the completion-based connect method in a task
           Task {
               omni.connect(reader: reader, completion: { connectedReader in
                   self.connectedReader = connectedReader
                   self.isConnecting = false
                   LogMessagesManager.shared.log("Connected reader: \(connectedReader.name)")
               }, error: { error in
                   LogMessagesManager.shared.log("Couldn't connect to \(reader.name): \(error.localizedDescription)")
                   self.isConnecting = false
               })
           }
       }

    func disconnectReader() {
        Task {
            // Attempt to get the connected reader asynchronously using completion handlers
            omni.getConnectedReader(completion: { reader in
                guard let reader = reader else {
                    LogMessagesManager.shared.log("No connected reader found")
                    return
                }
                
                // Attempt to disconnect asynchronously using completion handlers
                self.omni.disconnect(reader: reader, completion: { success in
                    LogMessagesManager.shared.log(success ? "Successfully disconnected reader" : "Could not disconnect reader")
                }, error: { error in
                    LogMessagesManager.shared.log(error.detail ?? "Error disconnecting")
                })
            }, error: { error in
                LogMessagesManager.shared.log(error.detail ?? "Error getting connected reader")
            })
        }
    }

    // Function to get information about the connected reader
    func getReaderInfo() {
        log("Trying to get info about the connected reader")
        omni.getConnectedReader(completion: { reader in
            self.log("Done")
        }, error: { error in
            self.log(error.detail ?? "Huh")
        })
    }

    // Function to log messages (will be displayed in the UI)
    func log(_ message: String) {
        LogMessagesManager.shared.log(message)
        logMessages.append(message)  // Add the message to the UI log
    }

    // MobileReaderConnectionStatusDelegate method
    func mobileReaderConnectionStatusUpdate(status: MobileReaderConnectionStatus) {
        log("Mobile reader \(status.rawValue)")
    }

    // Delegate methods for other updates
    func onTransactionUpdate(transactionUpdate: TransactionUpdate) {
        LogMessagesManager.shared.log("Transaction Update: \(transactionUpdate)")
    }

    func onUserNotification(userNotification: UserNotification) {
        LogMessagesManager.shared.log("User Notification: \(userNotification)")
    }

    func onUsbAccessoryConnected() {
        LogMessagesManager.shared.log("USB Accessory Connected")
    }

    func onUsbAccessoryDisconnected() {
        LogMessagesManager.shared.log("USB Accessory Disconnected")
    }
}

