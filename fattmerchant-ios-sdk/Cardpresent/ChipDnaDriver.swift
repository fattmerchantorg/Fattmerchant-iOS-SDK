//
//  ChipDnaDriver.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 12/9/19.
//  Copyright © 2019 Fattmerchant. All rights reserved.
//

import Foundation
import shared

class ChipDnaDriver: MobileReaderDriver {

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

  func searchForReaders(args: [String: Any]) -> [MobileReader]? {
    let params = CCParameters()
    params.setValue(CCValueBluetooth, forKey: CCParamPinPadConnectionType)

//    ChipDnaMobile.addConnectAndConfigureFinishedTarget(self, action: #selector("onConnectAndConfigure"))

    return nil
  }

  @objc func onConnectAndConfigure(parameters: CCParameters) {

  }

  /*
 log("Search for readers")
 val parameters = Parameters().apply {
 add(ParameterKeys.SearchConnectionTypeBluetooth, ParameterValues.TRUE)
 }
 ChipDnaMobile.getInstance().clearAllAvailablePinPadsListeners()

 val pinPads = suspendCancellableCoroutine<List<SelectablePinPad>> { cont ->
 val availablePinPadsListener: IAvailablePinPadsListener? = null
 ChipDnaMobile.getInstance().addAvailablePinPadsListener { params ->
 log("Inside IAvailablePinPadsListener")
 val availablePinPadsXml = params?.getValue(ParameterKeys.AvailablePinPads)
 val pinPads = deserializePinPads(availablePinPadsXml!!)
 availablePinPadsListener?.let { ChipDnaMobile.getInstance().removeAvailablePinPadsListener(it) }

 cont.resume(pinPads)
 }

 ChipDnaMobile.getInstance().getAvailablePinPads(parameters)
 }

 return pinPads.map {
 mapPinPadToMobileReader(it)
 }
 */


}
