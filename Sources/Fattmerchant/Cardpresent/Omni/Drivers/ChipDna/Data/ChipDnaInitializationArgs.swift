#if !targetEnvironment(simulator)
struct ChipDnaInitializationArgs: MobileReaderDriverInitializationArgs {
  let appId: String
  let keys: NMIDetails
  let environment: ChipDnaEnvironment
}
#endif
