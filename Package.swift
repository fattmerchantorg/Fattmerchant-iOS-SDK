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
            path: "Sources/Fattmerchant"
        ),
        .testTarget(
            name: "FattmerchantTests",
            dependencies: ["Fattmerchant"],
            path: "Tests/FattmerchantTests"
        ),
    ]
)

