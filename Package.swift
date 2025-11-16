// swift-tools-version: 5.7
import PackageDescription

let package = Package(
    name: "Fattmerchant",
    platforms: [.iOS(.v15)],
    products: [
        .library(name: "Fattmerchant", type: .dynamic, targets: ["Fattmerchant"]),
    ],
    targets: [
        .target(
            name: "Fattmerchant",
            dependencies: [
                .target(name: "IDTechResources"),
                .target(name: "ChipDnaMobile"),
                .target(name: "IDTech"),
                .target(name: "BBDeviceBT"),
                .target(name: "BBDeviceOTA"),
            ],
            path: "Sources/Fattmerchant",
            exclude: ["Info.plist"],
            publicHeadersPath: "include",
            cSettings: [
                .headerSearchPath("include"),
                .define("SWIFT_PACKAGE")
            ],
            linkerSettings: [
                .linkedFramework("Foundation"),
                .linkedFramework("UIKit"),
                .linkedFramework("AVFoundation"),
                .linkedFramework("ExternalAccessory"),
                .linkedFramework("CoreBluetooth"),
                .linkedFramework("CoreAudio"),
                .linkedFramework("AudioToolbox"),
                .linkedFramework("MediaPlayer"),
                .linkedFramework("CoreLocation"),
                .linkedFramework("CoreData"),
                .linkedFramework("MessageUI"),
                .linkedFramework("SystemConfiguration"),
                .linkedFramework("Security"),
                .linkedLibrary("z")
            ],
            
        ),
        .target(
            name: "IDTechResources",
            path: "Sources/IDTechResources",
            sources: ["Dummy.swift"],
            resources: [.copy("IDTech.bundle")]
        ),
        .binaryTarget(
            name: "ChipDnaMobile",
            path: "Vendor/ChipDnaMobileKit/Binaries/ChipDnaMobile.xcframework"
        ),
        .binaryTarget(
            name: "IDTech",
            path: "Vendor/ChipDnaMobileKit/Binaries/IDTechStatic.xcframework"
        ),
        .binaryTarget(name: "BBDeviceBT", path: "Vendor/ChipDnaMobileKit/Binaries/BBDevice-BT-3.27.0.xcframework"),
        .binaryTarget(name: "BBDeviceOTA", path: "Vendor/ChipDnaMobileKit/Binaries/BBDeviceOTA-1.6.13.xcframework"),
    ]
)
