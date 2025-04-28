import SwiftUI
import Fattmerchant

struct ContentView: View {
    @State private var totalAmount: String = "$0.01"
    @State private var staxService = StaxService.shared
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
                    TextField("", text: $totalAmount)
                        .keyboardType(.numberPad)
                        .font(.system(size: 38, weight: .bold, design: .monospaced))
                        .multilineTextAlignment(.center)
                        .foregroundColor(Color(.lightGray))
                        .padding()
                        .frame(height: 46)
                        .cornerRadius(10)
                        .padding(.top, 30)
                        .onChange(of: totalAmount) { newValue in
                            totalAmount = formatCurrencyInput(newValue)
                        }
                    
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
                                if !staxService.isOmniInitialized {
                                    Button("Initialize") {
                                        staxService.initializeOmni()
                                    }
                                    .padding()
                                    .frame(height: 30)
                                }
                                
                                Button("Initialize w/Ephemeral Token") {
                                    staxService.initializeEphemeralOmni()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Connect Reader") {
                                    staxService.searchForReaders()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Disconnect Reader") {
                                    staxService.disconnectReader()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Get Reader Info") {
                                    staxService.getReaderInfo()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Refund Payment") {
                                    fetchTransactions()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Take Payment with Reader") {
                                    staxService.takePayment(amountText: totalAmount)
                                }
                                .bold()
                                .padding()
                                .frame(height: 30)
                                
                                Button("Capture Last Transaction") {
                                    staxService.captureLastTransaction()
                                }
                                .bold()
                                .padding()
                                .frame(height: 30)
                                
                                Button("Preauth Payment with Reader") {
                                    staxService.takePayment(amountText: totalAmount, preauth: true)
                                }
                                .bold()
                                .padding()
                                .frame(height: 30)
                                
                                Button("Void Last Transaction") {
                                    staxService.voidLastTransaction()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Cancel transaction") {
                                    staxService.cancelTransaction()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Tokenize Card") {
                                    staxService.tokenizeCard()
                                    }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Tokenize Bank Account") {
                                    staxService.tokenizeBankAccount()
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Take Payment with Card") {
                                    staxService.payWithCard(amountText: totalAmount)
                                }
                                .padding()
                                .frame(height: 30)
                                
                                Button("Take Payment with Bank Account") {
                                    staxService.payWithBankAccount()
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
        Task {
            do {
                let transactions = try await withCheckedThrowingContinuation { continuation in
                    staxService.fetchTransactions(completion: { transactions in
                        continuation.resume(returning: transactions)
                    }, error: { err in
                        continuation.resume(throwing: err)
                    })
                }

                await MainActor.run {
                    self.transactions = transactions
                    if !transactions.isEmpty {
                        sheetManager.present(sheet: .transactionPicker)
                    } else {
                        logMessagesManager.log("No transactions available.")
                    }
                }
            } catch {
                await MainActor.run {
                    logMessagesManager.log("Error fetching transactions: \(error)")
                }
            }
        }
    }
    
    private func formatCurrencyInput(_ input: String) -> String {
        // Remove non-digits
        let digits = input.replacingOccurrences(of: "[^0-9]", with: "", options: .regularExpression)

        guard let cents = Int(digits) else {
            return "$0.00"
        }

        let dollars = Double(cents) / 100.0
        return String(format: "$%.2f", dollars)
    }
    
    // Refund a selected transaction
    private func refund(_ transaction: Fattmerchant.Transaction) {
        // Pass totalAmount to OmniInitializer's refund method
        staxService.refund(transaction: transaction, totalTextInput: totalAmount, completion: {
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
