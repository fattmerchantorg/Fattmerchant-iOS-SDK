import Foundation
import Combine

class LogMessagesManager: ObservableObject {
    static let shared = LogMessagesManager() // Singleton instance
    
    @Published var logMessages: String = ""  // This will hold the log text
    

    // Append a log message and update the UI
    func log(_ message: String) {
        DispatchQueue.main.async {
            let timestamp = self.timestamp()
            let logMessage = "\n\(timestamp) | \(message)"
            
            // Append the log message to existing logs (with a newline)
            self.logMessages += logMessage
        }
    }

    // Example function to generate timestamp (you can customize this format)
    private func timestamp() -> String {
        let formatter = DateFormatter()
        formatter.dateFormat = "HH:mm:ss"
        return formatter.string(from: Date())
    }
}

