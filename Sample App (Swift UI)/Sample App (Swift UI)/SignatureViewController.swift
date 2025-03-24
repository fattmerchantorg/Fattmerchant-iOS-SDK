import SwiftUI
import Fattmerchant

struct SignatureProvider: SignatureProviding {
    
    // The method to provide the signature
    func signatureRequired(completion: @escaping (String) -> Void) {
        // Simulating signature request
        DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
            completion("signaturedata")
        }
    }
}

