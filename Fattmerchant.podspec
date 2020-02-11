Pod::Spec.new do |s|
  s.name = 'Fattmerchant'
  s.version = '1.3.0'
  s.license = { :type => 'Apache License, Version 2.0', :text => "© #{ Date.today.year } Fattmerchant, inc" }
  s.summary = 'Fattmerchant iOS SDK'
  s.homepage = 'https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK'
  s.authors = { 'Fattmerchant' => 'techteam@fattmerchant.com' }
  s.source = { :git => 'https://github.com/fattmerchantorg/Fattmerchant-iOS-SDK.git', :tag => s.version }

  s.ios.deployment_target = '8.0'
  s.osx.deployment_target = '10.10'
  s.tvos.deployment_target = '9.0'
  s.watchos.deployment_target = '2.0'

  s.source_files = "fattmerchant-ios-sdk/**/*.{h, m, swift, a}"
end
