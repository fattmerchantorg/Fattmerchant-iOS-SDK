// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "ChipDnaMobileKit",
    platforms: [.iOS(.v12)],
    products: [
        .library(name: "ChipDnaMobileKit", targets: ["ChipDnaMobileKit"])
    ],
    targets: [
        .target(
            name: "ChipDnaMobileKit",
            dependencies: [
                .target(name: "IDTechResources"),
                .target(name: "ChipDnaMobile"),
                .target(name: "IDTech"),
                .target(name: "BBDeviceBT"),
                .target(name: "BBDeviceOTA"),
            ],
            path: "Sources/ChipDnaMobileKit",
            sources: ["Dummy.swift","Reexports.swift"],
            linkerSettings: [
                .unsafeFlags(["-ObjC"])
            ]
        ),
        .target(
            name: "IDTechResources",
            path: "Sources/IDTechResources",
            sources: ["Dummy.swift"],
            resources: [.copy("IDTech.bundle")]
        ),
        .binaryTarget(
            name: "ChipDnaMobile",
            path: "Binaries/ChipDnaMobile.xcframework"
        ),
        .binaryTarget(
            name: "IDTech",
            path: "Binaries/IDTechStatic.xcframework"
        ),
        .binaryTarget(name: "BBDeviceBT", path: "Binaries/BBDevice-BT-3.27.0.xcframework"),
        .binaryTarget(name: "BBDeviceOTA", path: "Binaries/BBDeviceOTA-1.6.13.xcframework"),
    ]
)
