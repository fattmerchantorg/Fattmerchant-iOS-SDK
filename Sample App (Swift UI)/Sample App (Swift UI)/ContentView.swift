import SwiftUI
import Fattmerchant

struct ContentView: View {
    @State private var totalAmount: String = "$0.01"
    @State private var omniInitializer = OmniInitializer.shared
    @ObservedObject var logMessagesManager = LogMessagesManager.shared
    @State private var scrollToBottom = false
    @State private var lastLogMessageCount = 0

    var body: some View {
        NavigationView {
            GeometryReader { geometry in
                VStack {
                    // TextField for entering amount
                    TextField("$0.01", text: $totalAmount)
                        .font(.system(size: 38, weight: .bold, design: .monospaced))
                        .multilineTextAlignment(.center)
                        .padding()
                        .frame(height: 46)
                        .cornerRadius(10)
                        .padding(.top, 30)
                    
                    ScrollViewReader { proxy in
                        ScrollView {
                            VStack(spacing: 12) {
                                Text(logMessagesManager.logMessages)
                                    .id("last")  // Unique identifier for the last log message
                                    .multilineTextAlignment(.leading)
                                    .padding(.top, 5)
                                    .frame(width: geometry.size.width * 0.9)
                                    .cornerRadius(10)
                                    .padding(.bottom, 20)
                            }
                            .onChange(of: logMessagesManager.logMessages) { newLogMessages in
                                // Only scroll when the number of log messages increases to avoid excessive scrolling
                                if newLogMessages.count > lastLogMessageCount {
                                    withAnimation {
                                        proxy.scrollTo("last", anchor: .bottom)
                                    }
                                    lastLogMessageCount = newLogMessages.count  // Update the count after scrolling
                                }
                            }
                        }
                        .frame(height: geometry.size.height * 0.3)
                        .padding(.top, 20)
                    
                    
                        // Buttons ScrollView
                        ScrollView {
                            VStack(spacing: 12) {
                                // Buttons for various actions
                                if !omniInitializer.isOmniInitialized {
                                Button("Initialize") {
                                    omniInitializer.initializeOmni() }
                                .padding()
                                .frame(height: 30)
                                }

                                
                                Button("Initialize w/Ephemeral Token") {
                                    Task {
                                        do {
                                            // Log the event
                                            logMessagesManager.log("Initializing with Ephemeral Token")
                                            try OmniInitializer.shared.initializeEphemeralOmni()
                                        } catch {
                                            // Handle any errors that occur during the asynchronous operation
                                            logMessagesManager.log("Error initializing Omni with ephemeral token: \(error)")
                                        }
                                    }
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Connect Reader") {
                                    guard !omniInitializer.isConnecting else { return }
                                    omniInitializer.searchForReaders()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Disconnect Reader") {
                                    OmniInitializer.shared.disconnectReader()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Get Reader Info") {
                                    OmniInitializer.shared.getReaderInfo()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Refund Payment") {
                                    logMessagesManager.log("Refund Payment tapped")
                                    print("Refund Payment tapped")
                                }
                                .padding()
                                .frame(height: 30)
                                Button("Take Payment with Reader") {
                                    logMessagesManager.log("Take Payment with Reader tapped")
                                    print("Take Payment with Reader tapped")
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Capture Last Transaction") {
                                    logMessagesManager.log("Capture Last Transaction tapped")
                                    print("Capture Last Transaction tapped")
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Preauth Payment with Reader") {
                                    logMessagesManager.log("Preauth Payment with Reader tapped")
                                    print("Preauth Payment with Reader tapped")
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Void Last Transaction") {
                                    logMessagesManager.log("Void Last Transaction tapped")
                                    print("Void Last Transaction tapped")
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Cancel transaction") {
                                    logMessagesManager.log("Cancel transaction tapped")
                                    print("Cancel transaction tapped")
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Tokenize Card") {
                                    logMessagesManager.log("Tokenize Card tapped")
                                    print("Tokenize Card tapped")
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Tokenize Bank Account") {
                                    logMessagesManager.log("Tokenize Bank Account tapped")
                                    print("Tokenize Bank Account tapped")
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Take Payment with Card") {
                                    logMessagesManager.log("Take Payment with Card tapped")
                                    print("Take Payment with Card tapped")
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Take Payment with Bank Account") {
                                    logMessagesManager.log("Take Payment with Bank Account tapped")
                                    print("Take Payment with Bank Account tapped")
                                }
                                .padding()
                                .frame(height: 30)
                            }
                            .padding()
                        }
                    }
                    .frame(maxHeight: .infinity)
                }
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

