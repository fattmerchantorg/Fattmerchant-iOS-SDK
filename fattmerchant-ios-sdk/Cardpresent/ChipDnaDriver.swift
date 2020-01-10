//
//  ChipDnaDriver.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 12/9/19.
//  Copyright © 2019 Fattmerchant. All rights reserved.
//

import Foundation

class ChipDnaDriver {

  struct SelectablePinPad {
    var name: String
    var connectionType: String
  }

  /// Initializes ChipDna
  func initialize(args: [String: Any]) -> Bool {
    // Initialize
    let parameters = CCParameters()
    parameters.setValue("password", forKey: CCParamPassword)
    parameters.setValue(CCValueTrue, forKey: CCParamAutoConfirm)
    let initResult = ChipDnaMobile.initialize(parameters)

    // Set creds
    let properties = CCParameters()
    properties.setValue("apikey", forKey: CCParamApiKey)
    properties.setValue("appid", forKey: CCParamApplicationIdentifier)
    properties.setValue(CCValueEnvironmentLive, forKey: CCParamEnvironment)
    let credsResult = ChipDnaMobile.sharedInstance()?.setProperties(properties)

    // Check if something went wrong
    return true
  }

//  func searchForReaders(args: [String: Any]) -> [MobileReader]? {
//    let params = CCParameters()
//    params.setValue(CCValueBluetooth, forKey: CCParamPinPadConnectionType)
//
////    ChipDnaMobile.addConnectAndConfigureFinishedTarget(self, action: #selector("onConnectAndConfigure"))
//
//    return nil
//  }

  @objc func onConnectAndConfigure(parameters: CCParameters) {

  }

}
