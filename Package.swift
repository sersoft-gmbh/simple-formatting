// swift-tools-version: 5.6
// The swift-tools-version declares the minimum version of Swift required to build this package.

import PackageDescription

let package = Package(
    name: "simple-formatting",
    products: [
        .library(name: "SimpleDurationFormatting",
                 targets: ["SimpleDurationFormatting"]),
    ],
    targets: [
        // Targets are the basic building blocks of a package. A target can define a module or a test suite.
        // Targets can depend on other targets in this package, and on products in packages this package depends on.
        .systemLibrary(
            name: "CICUCommon",
            pkgConfig: "icu-uc",
            providers: [
                .apt(["icu4c"]),
                .brew(["icu4c"]),
            ]),
        .systemLibrary(
            name: "CICUData",
            pkgConfig: "icu-data",
            providers: [
                .apt(["icu4c"]),
                .brew(["icu4c"]),
            ]),
        .systemLibrary(
            name: "CICUI18N",
            pkgConfig: "icu-i18n",
            providers: [
                .apt(["icu4c"]),
                .brew(["icu4c"]),
            ]),
        .systemLibrary(
            name: "CICUIO",
            pkgConfig: "icu-io",
            providers: [
                .apt(["icu4c"]),
                .brew(["icu4c"]),
            ]),
        .target(
            name: "CICUShims",
            dependencies: [
                "CICUCommon",
                "CICUData",
                "CICUI18N",
//                "CICUIO",
            ]),
        .target(
            name: "SimpleDurationFormatting",
            dependencies: ["CICUShims"]),
        .testTarget(
            name: "SimpleDurationFormattingTests",
            dependencies: ["SimpleDurationFormatting"]),
    ],
    cxxLanguageStandard: .cxx14
)
