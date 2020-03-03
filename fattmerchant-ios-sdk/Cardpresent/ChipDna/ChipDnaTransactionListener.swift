//
//  ChipDnaTransactionListener.swift
//  Fattmerchant
//
//  Created by Tulio Troncoso on 1/17/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation

#if targetEnvironment(simulator)

#else
class ChipDnaTransactionListener: NSObject {

  /// Called when ChipDna finishes a transaction
  var onFinished: ((CCParameters) -> Void)?

  /// Makes this listener bind to the transaction events ChipDna emits
  func bindToChipDna() {
    ChipDnaMobile.addTransactionUpdateTarget(self, action: #selector(onTransactionUpdate(parameters:)))
    ChipDnaMobile.addTransactionFinishedTarget(self, action: #selector(onTransactionFinished(parameters:)))
    ChipDnaMobile.addDeferredAuthorizationTarget(self, action: #selector(onDeferredAuthorization(parameters:)))
    ChipDnaMobile.addSignatureVerificationTarget(self, action: #selector(onSignatureVerification(parameters:)))
    ChipDnaMobile.addVoiceReferralTarget(self, action: #selector(onVoiceReferral(parameters:)))
    ChipDnaMobile.addPartialApprovalTarget(self, action: #selector(onPartialApprove(parameters:)))
    ChipDnaMobile.addForcedAcceptanceTarget(self, action: #selector(onForcedAcceptance(parameters:)))
    ChipDnaMobile.addIdVerificationTarget(self, action: #selector(onIdVerification(parameters:)))
  }

  /// Makes this listener stop listening to transaction events from ChipDna
  func detachFromChipDna() {
    ChipDnaMobile.removeTransactionUpdateTarget(self)
    ChipDnaMobile.removeTransactionFinishedTarget(self)
    ChipDnaMobile.removeDeferredAuthorizationTarget(self)
    ChipDnaMobile.removeSignatureVerificationTarget(self)
    ChipDnaMobile.removeVoiceReferralTarget(self)
    ChipDnaMobile.removePartialApprovalTarget(self)
    ChipDnaMobile.removeForcedAcceptanceTarget(self)
    ChipDnaMobile.removeIdVerificationTarget(self)
  }

  @objc fileprivate func onTransactionUpdate(parameters: CCParameters) {

  }

  @objc fileprivate func onTransactionFinished(parameters: CCParameters) {
    onFinished?(parameters)
  }

  @objc fileprivate func onDeferredAuthorization(parameters: CCParameters) {

  }

  @objc fileprivate func onSignatureVerification(parameters: CCParameters) {
    let approveSignatureParams = CCParameters()
    approveSignatureParams.setValue(CCValueTrue, forKey: CCParamResult)
    ChipDnaMobile.sharedInstance().continueSignatureVerification(approveSignatureParams)
  }

  @objc fileprivate func onVoiceReferral(parameters: CCParameters) {

  }

  @objc fileprivate func onPartialApprove(parameters: CCParameters) {

  }

  @objc fileprivate func onForcedAcceptance(parameters: CCParameters) {

  }

  @objc fileprivate func onIdVerification(parameters: CCParameters) {

  }

}
#endif
