// swift-tools-version: 5.7
import PackageDescription

let package = Package(
    name: "Fattmerchant",
    platforms: [
        .iOS(.v15)
    ],
    products: [
        .library(
            name: "Fattmerchant",
            targets: ["Fattmerchant"]
        ),
    ],
    targets: [
        .binaryTarget(
            name: "BBDevice-BT-3.27.0",
            path: "Frameworks/BBPOSFrameworks/BBDevice-BT-3.27.0.xcframework"
        ),
        .binaryTarget(
            name: "BBDeviceOTA-1.6.13",
            path: "Frameworks/BBPOSFrameworks/BBDeviceOTA-1.6.13.xcframework"
        ),
        .binaryTarget(
            name: "CardEaseXml",
            path: "Frameworks/CardEaseXml.xcframework"
        ),
        .binaryTarget(
            name: "ChipDnaMobileAPI",
            path: "Frameworks/ChipDnaMobileAPI.xcframework"
        ),
        .binaryTarget(
            name: "CloudCommerce",
            path: "Frameworks/CloudCommerce.xcframework"
        ),
        .binaryTarget(
            name: "IDTech",
            path: "Frameworks/IDTech.xcframework"
        ),
        .binaryTarget(
            name: "SqlCipher-4.6.1",
            path: "Frameworks/SqlCipher-4.6.1.xcframework"
        ),
        
        .target(
            name: "Fattmerchant",
            dependencies: [
                "BBDevice-BT-3.27.0",
                "BBDeviceOTA-1.6.13",
                "CardEaseXml",
                "ChipDnaMobileAPI",
                "CloudCommerce",
                "IDTech",
                "SqlCipher-4.6.1"
            ],
            path: "Sources/Fattmerchant",
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
                .linkedLibrary("z"),
                .unsafeFlags(["-Wl,-force_load"])
            ]
        ),
        
        .testTarget(
            name: "FattmerchantTests",
            dependencies: ["Fattmerchant"],
            path: "Tests/FattmerchantTests",
            exclude: ["Info.plist"]
        ),
    ]
)
