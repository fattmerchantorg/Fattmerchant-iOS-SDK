//
//  TapConnectionStatus.swift
//  fattmerchant-ios-sdk
//
//  Created by Hassan Nazari on 11/5/25.
//  Copyright © 2025 Fattmerchant. All rights reserved.
//

import Foundation

/// High-level connection/configuration states for Tap to Pay on iPhone.
///
/// These statuses are surfaced to client apps via `TapConnectionStatusDelegate` while the SDK
/// connects to and configures Tap to Pay using the underlying ChipDNA integration.
/// They correspond to configuration update events (CCParamConfigurationUpdate) and the final
/// connect-and-configure result emitted by ChipDNA.
///
/// Typical sequence:
/// 1) `.connecting` — the connection/configuration flow has started
/// 2) Update phases such as `.tms_update`, `.tap_config_check`, `.tap_config_update`
/// 3) `.registering` — the app/device is registering for Tap to Pay
/// 4) `.connected` — Tap to Pay is ready to accept payments
public enum TapConnectionStatus: String {
    /// The SDK has started the Tap to Pay connect-and-configure process.
    ///
    /// Maps from ChipDNA updates like `CCValueConnecting` and `CCValueConnectionAndConfigureStarted`.
    case connecting

    /// Tap to Pay is fully connected and ready to use.
    ///
    /// Emitted after ChipDNA connect-and-configure finishes successfully (`CCParamResult == CCValueTrue`).
    case connected

    /// The device is applying updates from the Terminal Management System (TMS).
    ///
    /// Maps from ChipDNA `CCValuePerformingTmsUpdate`.
    case tms_update

    /// The SDK is checking the Tap to Pay configuration on the device.
    ///
    /// Maps from ChipDNA `CCValueCheckingTapToMobileConfig`.
    case tap_config_check

    /// The SDK is applying Tap to Pay configuration updates on the device.
    ///
    /// Maps from ChipDNA `CCValueUpdatingTapToMobileConfig`.
    case tap_config_update

    /// The app/device is registering Tap to Pay with the payment platform.
    ///
    /// Maps from ChipDNA `CCValueRegistering`.
    case registering
}
