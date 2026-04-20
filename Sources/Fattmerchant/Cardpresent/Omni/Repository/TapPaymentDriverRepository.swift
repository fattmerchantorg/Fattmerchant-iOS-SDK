@available(iOS 17.4, *)
class TapPaymentDriverRepository {
    static let shared = TapPaymentDriverRepository()
    

    func all() -> [TapDriver] {
      #if targetEnvironment(simulator)
        return [MockDriver()]
      #else
        return [ChipDnaDriver.shared]
      #endif
    }
}
