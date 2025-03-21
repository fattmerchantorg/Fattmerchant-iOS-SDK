import SwiftUI

struct ContentView: View {
    @State private var totalAmount: String = "$0.01"
    
    var body: some View {
        NavigationView {
            VStack {
                // TextField for entering amount
                TextField("$0.01", text: $totalAmount)
                    .font(.system(size: 38, weight: .bold, design: .monospaced))
                    .multilineTextAlignment(.center)
                    .padding()
                    .frame(height: 46)
                    .background(Color.gray.opacity(0.1))
                    .cornerRadius(10)
                    .padding(.top, 20)
                
                ScrollView {
                    VStack(spacing: 12) {
                        // Text View (equivalent to the large static text)
                        Text("Info here")
                            .padding()
                            .frame(height: 200)

                        // Buttons
                        Button("Initialize") {
                            print("Initialize tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Initialize w/Ephemeral Token") {
                            print("Initialize w/Ephemeral Token tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Connect Reader") {
                            print("Connect Reader tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Disconnect Reader") {
                            print("Disconnect Reader tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Get Reader Info") {
                            print("Get Reader Info tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Refund Payment") {
                            print("Refund Payment tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Take Payment with Reader") {
                            print("Take Payment with Reader tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Capture Last Transaction") {
                            print("Capture Last Transaction tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Preauth Payment with Reader") {
                            print("Preauth Payment with Reader tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Void Last Transaction") {
                            print("Void Last Transaction tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Cancel transaction") {
                            print("Cancel transaction tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Tokenize Card") {
                            print("Tokenize Card tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Tokenize Bank Account") {
                            print("Tokenize Bank Account tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Take Payment with Card") {
                            print("Take Payment with Card tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)

                        Button("Take Payment with Bank Account") {
                            print("Take Payment with Bank Account tapped")
                        }
                        .padding()
                        .frame(height: 30)
                        .background(Color.blue.opacity(0.1))
                        .cornerRadius(10)
                    }
                    .padding()
                }
                .frame(maxHeight: .infinity)
                .padding(.top, 20)
            }
        }
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView()
    }
}

