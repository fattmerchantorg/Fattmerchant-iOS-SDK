Pod::Spec.new do |s|
  s.name = 'Fattmerchant'
  s.version = '2.6.0'
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
    :branch => "release/#{s.version}",
    :tag => s.version.to_s
  }
  s.ios.deployment_target = '15.0'
  s.swift_versions = ['5']
  s.source_files = "fattmerchant-ios-sdk/**/*.{h,m,swift}"
  s.public_header_files = "fattmerchant-ios-sdk/Vendor/ChipDnaMobile/*.h"
  s.preserve_paths = 
    "Frameworks/IDTech.xcframework",
    "Frameworks/BBPOSFrameworks"
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
  s.vendored_frameworks =
    'Frameworks/IDTech.xcframework',
    'Frameworks/BBPOSFrameworks/BBDevice-BT-3.27.0.xcframework',
    'Frameworks/BBPOSFrameworks/BBDeviceOTA-1.6.13.xcframework'
  s.pod_target_xcconfig = {
    'ENABLE_BITCODE' => 'NO',
    'OTHER_LDFLAGS' => '-lz',
    'VALID_ARCHS[sdk=iphonesimulator*]' => '', # No simulator support
    'VALID_ARCHS[sdk=iphoneos*]' => 'arm64 arm64e'
  }
  s.user_target_xcconfig = {
    'VALID_ARCHS[sdk=iphonesimulator*]' => '', # No simulator support
    'VALID_ARCHS[sdk=iphoneos*]' => 'arm64 arm64e'
  }
end
