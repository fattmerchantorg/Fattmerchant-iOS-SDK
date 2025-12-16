Pod::Spec.new do |s|
  s.name         = 'Fattmerchant'
  s.version      = '2.6.0'
  s.summary      = 'Fattmerchant iOS SDK'
  s.homepage     = 'https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK'
  s.license      = { :type => 'Apache License, Version 2.0', :text => "© #{ Date.today.year } Fattmerchant, inc" }
  s.authors      = { 'Fattmerchant' => 'techteam@fattmerchant.com' }

  # Source
  s.source       = { :git => 'https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK.git', :branch => "release/#{s.version}", :tag => s.version.to_s }

  # Platforms
  s.ios.deployment_target = '15.0'
  s.swift_versions = ['5.7']

  # Source files layout matches SPM target path
  s.source_files = 'Sources/Fattmerchant/**/*.{h,m,swift}'
  s.public_header_files = 'Sources/Fattmerchant/include/**/*.h'
  s.preserve_paths = [
    'ChipDnaMobile/IDTech.xcframework',
    'ChipDnaMobile/BBPOSFrameworks',
    'ChipDnaMobile/CardEaseXml.xcframework',
    'ChipDnaMobile/ChipDnaMobileAPI.xcframework',
    'ChipDnaMobile/SqlCipher-4.6.1.xcframework'
  ]

  # Vendored binaries to mirror SPM binary targets
  s.vendored_frameworks = [
    'ChipDnaMobile/BBPOSFrameworks/BBDevice-BT-3.27.0.xcframework',
    'ChipDnaMobile/BBPOSFrameworks/BBDeviceOTA-1.6.13.xcframework',
    'ChipDnaMobile/CardEaseXml.xcframework',
    'ChipDnaMobile/ChipDnaMobileAPI.xcframework',
    'ChipDnaMobile/IDTech.xcframework',
    'ChipDnaMobile/SqlCipher-4.6.1.xcframework'
  ]

  # System frameworks and libraries to mirror SPM linkerSettings
  s.frameworks = [
    'Foundation',
    'UIKit',
    'AVFoundation',
    'ExternalAccessory',
    'CoreBluetooth',
    'CoreAudio',
    'AudioToolbox',
    'MediaPlayer',
    'CoreLocation',
    'CoreData',
    'MessageUI',
    'SystemConfiguration',
    'Security'
  ]
  s.libraries = 'z'

  # Build settings
  s.pod_target_xcconfig = {
    'OTHER_LDFLAGS' => '-Wl,-force_load'
  }

  # CocoaPods has dropped bitcode/VALID_ARCHS usage; do not restrict architectures here.
end
