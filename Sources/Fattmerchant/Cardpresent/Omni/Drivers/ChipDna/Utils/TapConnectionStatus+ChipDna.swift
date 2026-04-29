#if !targetEnvironment(simulator)
import Foundation

extension TapConnectionStatus {

    /// Initializes a `TapConnectionStatus` from a ChipDNA configuration update string.
    ///
    /// During the Tap to Pay on iPhone connect-and-configure flow, ChipDNA emits a variety of
    /// configuration update messages via `CCParamConfigurationUpdate`. This initializer maps the
    /// subset we care about into high-level `TapConnectionStatus` values that are surfaced via
    /// `TapConnectionStatusDelegate`.
    ///
    /// Mappings:
    /// - `CCValueConnecting`, `CCValueConnectionAndConfigureStarted` -> `.connecting`
    /// - `CCValuePerformingTmsUpdate` -> `.tms_update`
    /// - `CCValueCheckingTapToMobileConfig` -> `.tap_config_check`
    /// - `CCValueUpdatingTapToMobileConfig` -> `.tap_config_update`
    /// - `CCValueRegistering` -> `.registering`
    ///
    /// Any other ChipDNA configuration update will return `nil` and be ignored.
    ///
    /// - Parameter configUpdate: The ChipDNA configuration update string (`CCParamConfigurationUpdate` value).
    init?(chipDnaConfigurationUpdate configUpdate: String) {
        switch configUpdate {
        case CCValueConnecting, CCValueConnectionAndConfigureStarted:
            self = .connecting

        case CCValuePerformingTmsUpdate:
            self = .tms_update

        case CCValueCheckingTapToMobileConfig:
            self = .tap_config_check

        case CCValueUpdatingTapToMobileConfig:
            self = .tap_config_update

        case CCValueRegistering:
            self = .registering

        default:
            return nil
        }
    }
}
#endif
