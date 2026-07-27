#if !targetEnvironment(simulator)
struct ChipDnaSearchArgs: MobileReaderDriverSearchArgs {
  let allowed: [ChipDnaConnectionTypes]
}
#endif
