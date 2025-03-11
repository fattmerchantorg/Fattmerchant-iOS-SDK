// swift-tools-version:5.3

import PackageDescription

let package = Package(
    name: "Fattmerchant",
    platforms: [
      .iOS(.v13)
    ],
    products: [
      .library(
        name: "Fattmerchant",
        targets: ["Fattmerchant"])
    ],
    dependencies: [],
    targets: [
      .systemLibrary(name: "ChipDnaMobile", pkgConfig: "ChipDnaMobile"),
      .binaryTarget(
        name: "BBDevice-BT",
        path: "fattmerchant-ios-sdk/Vendor/ChipDnaMobile/BBPOSFrameworks/BBDevice-BT-3.27.0.xcframework"
      ),
      .binaryTarget(
        name: "BBDeviceOTA",
        path: "fattmerchant-ios-sdk/Vendor/ChipDnaMobile/BBPOSFrameworks/BBDeviceOTA-1.6.13.xcframework"
      ),
      .binaryTarget(
        name: "IDTech",
        path: "fattmerchant-ios-sdk/Vendor/ChipDnaMobile/IDTech.xcframework"
      ),
      .target(
        name: "Fattmerchant",
        path: "fattmerchant-ios-sdk",
        sources: ["Utils", "Cardpresent", "Models", "Networking.swift"],
        publicHeadersPath: "fattmerchant-ios-sdk"
      ),
    ],
    cLanguageStandard: .c11,
    cxxLanguageStandard: .cxx14
)
