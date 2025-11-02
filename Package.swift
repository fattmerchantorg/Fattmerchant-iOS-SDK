// swift-tools-version: 5.7
import PackageDescription

let package = Package(
    name: "Fattmerchant",
    platforms: [.iOS(.v15)],
    products: [
        .library(name: "Fattmerchant", targets: ["Fattmerchant"])
    ],
    dependencies: [
        // ✅ point to committed path, not build output
        .package(path: "Vendor/ChipDnaMobileKit")
    ],
    targets: [
        .target(
            name: "Fattmerchant",
            dependencies: [
                // ✅ use the dependency package name exactly as in its manifest
                .product(name: "ChipDnaMobileKit", package: "ChipDnaMobileKit")
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
            ]
        )
    ]
)
