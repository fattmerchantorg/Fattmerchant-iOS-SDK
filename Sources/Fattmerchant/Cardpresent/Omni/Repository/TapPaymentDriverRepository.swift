@available(iOS 17.4, *)
class TapPaymentDriverRepository {
    static let shared = TapPaymentDriverRepository()
    

    func all() -> [TapDriver] {
      #if targetEnvironment(simulator)
        return [MockTapDriver()]
      #else
        return [ChipDnaDriver.shared]
      #endif
    }
}
