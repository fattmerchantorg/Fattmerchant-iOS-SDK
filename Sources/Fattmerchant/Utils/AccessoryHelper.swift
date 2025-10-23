import Foundation
import ExternalAccessory

public class AccessoryHelper {
  // A list of known device models to check against
  fileprivate static let devices = ["VP3350"]
  
  public static func isIdTechConnected() -> Bool {
    let accessories = EAAccessoryManager.shared().connectedAccessories
    for accessory in accessories {
      if accessory.isVp3350() {
        return true
      }
    }
    return false
  }
  
  private let delegate: UsbAccessoryDelegate
  
  public init(delegate: UsbAccessoryDelegate) {
    self.delegate = delegate
    EAAccessoryManager.shared().registerForLocalNotifications()
    NotificationCenter.default.addObserver(self, selector: #selector(onConnected), name: .EAAccessoryDidConnect, object: nil)
    NotificationCenter.default.addObserver(self, selector: #selector(onDisconnected), name: .EAAccessoryDidDisconnect, object: nil)
  }
  
  deinit {
    EAAccessoryManager.shared().unregisterForLocalNotifications()
    NotificationCenter.default.removeObserver(self)
  }

  
  @objc private func onConnected(notification: NSNotification) {
    guard let connected = notification.userInfo?[EAAccessoryKey] as? EAAccessory else { return }
    if connected.isVp3350() {
      delegate.onUsbAccessoryConnected()
    }
  }
  
  @objc private func onDisconnected(notification: NSNotification) {
    guard let disconnected = notification.userInfo?[EAAccessoryKey] as? EAAccessory else { return }
    if disconnected.isVp3350() {
      delegate.onUsbAccessoryDisconnected()
    }
  }
}

extension EAAccessory {
  fileprivate func isVp3350() -> Bool {
    let trimmed = name.trimmingCharacters(in: .whitespacesAndNewlines)
    return AccessoryHelper.devices.contains(trimmed)
  }
}
