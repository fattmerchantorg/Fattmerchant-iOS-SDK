import SwiftUI
import Fattmerchant

struct TransactionPicker: View {
    var transactions: [Fattmerchant.Transaction]
    var onTransactionChosen: (Fattmerchant.Transaction) -> Void
    var onCancel: (() -> Void)?
    

    @SwiftUICore.Environment(\.presentationMode) var presentationMode

    var body: some View {
        VStack {
            if transactions.isEmpty {
                Text("No transactions available.")
                    .font(.title2)
                    .padding()
            } else {
                List(transactions, id: \.id) { transaction in
                    VStack(alignment: .leading) {
                        Text(pretty(transaction: transaction))
                            .font(.body)
                        Text("Transaction ID: \(transaction.id ?? "")")
                            .font(.caption)
                            .foregroundColor(.gray)
                    }
                    .onTapGesture {
                        onTransactionChosen(transaction)
                        presentationMode.wrappedValue.dismiss()  // Dismiss when transaction is chosen
                    }
                }
            }
        }
        .navigationBarTitle("Choose a Transaction", displayMode: .inline)
        .navigationBarItems(trailing: Button("Cancel") {
            onCancel?()
            presentationMode.wrappedValue.dismiss()  // Dismiss the sheet
        })
    }

    private func pretty(transaction: Fattmerchant.Transaction) -> String {
        return "$\(String(format: "%.2f", transaction.total ?? 0.0)) \(transaction.type ?? "") to \(transaction.method ?? "") ending in \(transaction.lastFour ?? "")"
    }
}

