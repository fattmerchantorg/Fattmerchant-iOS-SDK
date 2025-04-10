import SwiftUI
import Fattmerchant

struct ContentView: View {
    @State private var totalAmount: String = "$0.01"
    @State private var omniInitializer = OmniInitializer.shared
    @ObservedObject var logMessagesManager = LogMessagesManager.shared
    @State private var scrollToBottom = false
    @State private var lastLogMessageCount = 0
    @State private var debounceTimer: Timer? = nil
    @State private var transactions: [Fattmerchant.Transaction] = [] // This would be fetched
    @State private var selectedTransaction: Fattmerchant.Transaction? = nil
    @StateObject private var sheetManager = SheetManager()
    
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
                                // Debouncing state update
                                debounceTimer?.invalidate()  // Cancel the previous timer if it's still active
                                
                                debounceTimer = Timer.scheduledTimer(withTimeInterval: 0.3, repeats: false) { _ in
                                    // Only scroll when the number of log messages increases to avoid excessive scrolling
                                    if newLogMessages.count > lastLogMessageCount {
                                        // Dispatch to the main thread to ensure UI updates are handled correctly
                                        DispatchQueue.main.async {
                                            withAnimation {
                                                proxy.scrollTo("last", anchor: .bottom)
                                            }
                                        }
                                        lastLogMessageCount = newLogMessages.count  // Update the count after scrolling
                                    }
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
                                        omniInitializer.initializeOmni()
                                    }
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
                                    fetchTransactions()
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
                        .frame(maxHeight: .infinity)
                    }
                }
                .sheet(isPresented: $sheetManager.isPresented) {
                    if sheetManager.activeSheet == .transactionPicker, !transactions.isEmpty {
                        TransactionPicker(
                            transactions: transactions,
                            onTransactionChosen: { transaction in
                                selectedTransaction = transaction
                                refund(transaction)
                                sheetManager.dismiss()
                            },
                            onCancel: {
                                sheetManager.dismiss()
                            }
                        )
                    }
                }
            }
        }
    }
   
    private func fetchTransactions() {
        OmniInitializer.shared.fetchTransactions(completion: { transactions in
            DispatchQueue.main.async {
                self.transactions = transactions
                if !transactions.isEmpty {
                    sheetManager.present(sheet: .transactionPicker)
                } else {
                    logMessagesManager.log("No transactions available.")
                }
            }
        }, error: { err in
            DispatchQueue.main.async {
                logMessagesManager.log("Error fetching transactions: \(err.localizedDescription)")
            }
        })
    }
    
    // Refund a selected transaction
    private func refund(_ transaction: Fattmerchant.Transaction) {
        // Pass totalAmount to OmniInitializer's refund method
        OmniInitializer.shared.refund(transaction: transaction, totalTextInput: totalAmount, completion: {
            logMessagesManager.log("Refunded transaction successfully")
        }, error: { errorMessage in
            logMessagesManager.log("Error refunding transaction: \(errorMessage)")
        })
    }
}

    struct ContentView_Previews: PreviewProvider {
        static var previews: some View {
            ContentView()
        }
    }
    

    class SheetManager: ObservableObject {
        enum ActiveSheet {
            case transactionPicker
        }

        @Published var isPresented: Bool = false
        @Published var activeSheet: ActiveSheet? = nil

    func present(sheet: ActiveSheet) {
        activeSheet = sheet
        isPresented = true
    }

    func dismiss() {
        isPresented = false
        activeSheet = nil
    }
}
