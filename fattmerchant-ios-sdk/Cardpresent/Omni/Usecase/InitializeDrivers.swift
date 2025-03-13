import Foundation



class InitializeDrivers {

  var mobileReaderDriverRepository: MobileReaderDriverRepository
  var args: [String:Any]

  init(mobileReaderDriverRepository: MobileReaderDriverRepository, args: [String:Any]) {
    self.mobileReaderDriverRepository = mobileReaderDriverRepository
    self.args = args
  }

  func start(completion: @escaping (Bool) -> Void, failure: @escaping (OmniException) -> Void) {
    let drivers = mobileReaderDriverRepository.getDrivers()
    
    // Make sure we have at least one available driver
    guard !drivers.isEmpty else {
      failure(InitializeDriversException.noMobileReadersFound)
      return
    }
    
    // Right now we only have 1 driver. For simplicity, initialize the 1st driver.
    guard let driver = drivers.first, drivers.count == 1 else {
      failure(InitializeDriversException.incorrectMobileReaderSettings)
      return
    }
    
    #if targetEnvironment(simulator)
    let args = MockInitializationArgs(appId: args["appId"])
    #else
    let args = ChipDnaInitializationArgs(appId: args["appId"] as! String, keys: args["nmi"] as! NMIDetails)
    #endif
    
    driver.initialize(args: args) { isSuccessful in completion(isSuccessful) }
  }
}
