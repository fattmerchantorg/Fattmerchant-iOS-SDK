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
        .target(
            name: "Fattmerchant",
            dependencies: [],
            path: "Sources/Fattmerchant",
            publicHeadersPath: "include",
            cSettings: [
                .headerSearchPath("include"),
                .define("SWIFT_PACKAGE")
            ]
        ),
        .testTarget(
            name: "FattmerchantTests",
            dependencies: ["Fattmerchant"],
            path: "Tests/FattmerchantTests"
        ),
    ]
)
