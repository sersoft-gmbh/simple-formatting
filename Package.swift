// swift-tools-version: 5.6
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription
import Foundation

let package = Package(
    name: "simple-formatting",
    products: [
        .library(name: "DurationFormatting",
                 targets: ["DurationFormatting"]),
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .systemLibrary(
            name: "CICUCommon",
            pkgConfig: "icu-uc",
            providers: [
                .apt(["libicu-dev"]),
                .brew(["icu4c"]),
            ]),
        .systemLibrary(
            name: "CICUI18N",
            pkgConfig: "icu-i18n",
            providers: [
                .apt(["libicu-dev"]),
                .brew(["icu4c"]),
            ]),
        .systemLibrary(
            name: "CICUIO",
            pkgConfig: "icu-io",
            providers: [
                .apt(["libicu-dev"]),
                .brew(["icu4c"]),
            ]),
        .target(
            name: "CICUShims",
            dependencies: [
                "CICUCommon",
                "CICUI18N",
//                "CICUIO",
            ]),
        .target(
            name: "DurationFormatting",
            dependencies: ["CICUShims"]),
        .testTarget(
            name: "DurationFormattingTests",
            dependencies: [
                "CICUShims",
                "DurationFormatting",
            ]),
    ],
    cxxLanguageStandard: .cxx14
)

if ProcessInfo.processInfo.environment["ENABLE_DOCC_SUPPORT"] == "1" {
    package.dependencies.append(.package(url: "https://github.com/apple/swift-docc-plugin", from: "1.0.0"))
}
