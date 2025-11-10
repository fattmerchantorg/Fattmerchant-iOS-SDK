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

# Sources (matches SPM target path)
s.source_files = 'Sources/Fattmerchant/**/*.{h,m,swift}'
s.public_header_files = 'Sources/Fattmerchant/include/**/*.h'

# Resources required at runtime (IDTech bundle)
s.resources = [
'ChipDnaMobileKit/Sources/IDTechResources/IDTech.bundle/IDTech.bundle'
]

# Preserve vendored content
s.preserve_paths = [
'ChipDnaMobileKit/Binaries/BBDevice-BT-3.27.0.xcframework',
'ChipDnaMobileKit/Binaries/BBDeviceOTA-1.6.13.xcframework',
'ChipDnaMobileKit/Binaries/ChipDnaMobile.xcframework',
'ChipDnaMobileKit/Binaries/CloudCommerce.xcframework',
'ChipDnaMobileKit/Binaries/IDTechStatic.xcframework',
'ChipDnaMobileKit/Sources/IDTechResources/IDTech.bundle/IDTech.bundle'
]

# Vendored frameworks to mirror SPM binary targets
s.vendored_frameworks = [
'ChipDnaMobileKit/Binaries/BBDevice-BT-3.27.0.xcframework',
'ChipDnaMobileKit/Binaries/BBDeviceOTA-1.6.13.xcframework',
'ChipDnaMobileKit/Binaries/ChipDnaMobile.xcframework',
'ChipDnaMobileKit/Binaries/CloudCommerce.xcframework',
'ChipDnaMobileKit/Binaries/IDTechStatic.xcframework'
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
end