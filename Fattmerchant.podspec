Pod::Spec.new do |s|
  s.name = 'Fattmerchant'
  s.version = '2.1.0'
  s.license = { :type => 'Apache License, Version 2.0', :text => "© #{ Date.today.year } Fattmerchant, inc" }
  s.summary = 'Fattmerchant iOS SDK'
  s.homepage = 'https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK'
  s.authors = { 'Fattmerchant' => 'techteam@fattmerchant.com' }
  s.source = { :git => 'https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK.git', :tag => s.version }

  s.ios.deployment_target = '9.0'
  s.swift_versions = ['4.0', '4.2', '5.2']  
  s.source_files = "Sources/Fattmerchant-iOS-SDK/**/*.{h,m,swift}", "Sources/Fattmerchant/Vendored-Frameworks/AnyPay/**/*.{h,m}"

  s.frameworks = 'UIKit', 'AVFoundation', 'MediaPlayer', 'CoreAudio', 'ExternalAccessory', 'CoreBluetooth', 'AudioToolbox'

  s.vendored_libraries = 'Sources/Fattmerchant-iOS-SDK/Cardpresent/ChipDnaMobile/libChipDnaMobileAPI.a', 'Sources/Fattmerchant/Cardpresent/ChipDnaMobile/libsqlcipher-4.0.1.a', 'Sources/Fattmerchant/Cardpresent/ChipDnaMobile/libCardEaseXml.a', 'Sources/Fattmerchant/Cardpresent/ChipDnaMobile/libLumberjack.a'
  
  s.vendored_frameworks = 'Sources/Vendored-Frameworks/AnyPay/AnyPay.framework'

  s.pod_target_xcconfig = { 
    'ENABLE_BITCODE' => 'NO',
    'OTHER_LDFLAGS' => '-lz'
  }

  s.pod_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }
  s.user_target_xcconfig = { 'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'arm64' }

end
