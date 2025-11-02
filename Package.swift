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
        )
    ],
    targets: [
        // -------- Vendor binaries folded into THIS package ----------
        .binaryTarget(
            name: "ChipDnaMobile",
            path: "Vendor/ChipDnaMobileKit/Binaries/ChipDnaMobile.xcframework"
        ),
        .binaryTarget(
            name: "CloudCommerce",
            path: "Vendor/ChipDnaMobileKit/Binaries/CloudCommerce.xcframework"
        ),
        .binaryTarget(
            name: "BBDeviceBT",
            path: "Vendor/ChipDnaMobileKit/Binaries/BBDevice-BT-3.27.0.xcframework"
        ),
        .binaryTarget(
            name: "BBDeviceOTA",
            path: "Vendor/ChipDnaMobileKit/Binaries/BBDeviceOTA-1.6.13.xcframework"
        ),
        // IMPORTANT: this is the header-less static XCFramework we built in the script
        .binaryTarget(
            name: "IDTech", // keep name "IDTech" if your wrapper expects it
            path: "Vendor/ChipDnaMobileKit/Binaries/IDTechStatic.xcframework"
        ),

        // -------- IDTech resources as Swift target ----------
        .target(
            name: "IDTechResources",
            path: "Vendor/ChipDnaMobileKit/Sources/IDTechResources",
            sources: ["Dummy.swift"],               // keep a placeholder source
            resources: [
                .copy("IDTech.bundle")              // ships / loads IDTech.bundle
            ]
        ),

        // -------- Wrapper that re-exports ChipDnaMobile and links others ----------
        .target(
            name: "ChipDnaMobileKit",
            dependencies: [
                .target(name: "IDTechResources"),
                .target(name: "ChipDnaMobile"),
                .target(name: "IDTech"),
                .target(name: "CloudCommerce"),
                .target(name: "BBDeviceBT"),
                .target(name: "BBDeviceOTA")
            ],
            path: "Vendor/ChipDnaMobileKit/Sources/ChipDnaMobileKit",
            sources: ["Dummy.swift", "Reexports.swift"],
            linkerSettings: [
                // Required by ObjC-only symbols inside static libs
                .unsafeFlags(["-ObjC"])
            ]
        ),

        // -------- Your SDK target now depends on the wrapper ----------
        .target(
            name: "Fattmerchant",
            dependencies: [
                .target(name: "ChipDnaMobileKit")
            ],
            path: "Sources/Fattmerchant",
            exclude: ["Info.plist"],
            publicHeadersPath: "include",
            cSettings: [
                .headerSearchPath("include"),
                .define("SWIFT_PACKAGE"),
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
        )
    ]
)
