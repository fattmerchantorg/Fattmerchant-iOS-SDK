Pod::Spec.new do |s|
  s.name = 'Fattmerchant'
  s.version = '2.4.0'
  s.license = {
    :type => 'Apache License, Version 2.0',
    :text => "© #{ Date.today.year } Fattmerchant, inc"
  }
  s.summary = 'Fattmerchant iOS SDK'
  s.homepage = 'https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK'
  s.authors = {
    'Fattmerchant' => 'techteam@fattmerchant.com'
  }
  s.source = {
    :git => 'https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK.git',
    :branch => "release/#{s.version}"
  }
  s.ios.deployment_target = '12.0'
  s.swift_versions = ['5']  
  s.source_files = "fattmerchant-ios-sdk/**/*.{h,m,swift}"
  s.public_header_files = "fattmerchant-ios-sdk/Vendor/ChipDnaMobile/*.h"
  s.preserve_paths = 
    "fattmerchant-ios-sdk/Vendor/ChipDnaMobile/IDTech.xcframework",
    "fattmerchant-ios-sdk/Vendor/ChipDnaMobile/BBPOSFrameworks"
  s.library = 'sqlite3'
  s.frameworks = 
    'UIKit',
    'AVFoundation',
    'MediaPlayer',
    'CoreAudio',
    'ExternalAccessory',
    'CoreBluetooth',
    'AudioToolbox',
    'ExternalAccessory'
  s.vendored_libraries = 
    'fattmerchant-ios-sdk/Vendor/ChipDnaMobile/libChipDnaMobileAPI.a',
    'fattmerchant-ios-sdk/Vendor/ChipDnaMobile/libCardEaseXml.a',
    'fattmerchant-ios-sdk/Vendor/ChipDnaMobile/SQLCipher/libsqlcipher-4.5.0.a'
  s.vendored_frameworks =
    'fattmerchant-ios-sdk/Vendor/ChipDnaMobile/IDTech.xcframework',
    'fattmerchant-ios-sdk/Vendor/ChipDnaMobile/BBPOSFrameworks/BBDevice-BT-3.27.0.xcframework',
    'fattmerchant-ios-sdk/Vendor/ChipDnaMobile/BBPOSFrameworks/BBDeviceOTA-1.6.13.xcframework'
  s.pod_target_xcconfig = { 
    'ENABLE_BITCODE' => 'NO',
    'OTHER_LDFLAGS' => '-lz',
    'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64'
  }
  s.user_target_xcconfig = {
    'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64'
  }
end
