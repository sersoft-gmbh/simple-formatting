//import XCTest
//import DurationFormatting
//
//final class DurationParsingTests: XCTestCase {
//    private func _testTimeComponents(_ timeComponents: TimeComponents,
//                                     with formatter: DurationFormatter,
//                                     expecting result: String,
//                                     file: StaticString = #filePath,
//                                     line: UInt = #line) {
//        let formatted = formatter.format(timeComponents)
//        XCTAssertNotNil(formatted, file: file, line: line)
//        XCTAssertEqual(formatted, result, file: file, line: line)
//    }
//
//    /// Tests `TimeComponents(hour: 3, minute: 24, second: 42)`
//    private func _testFullComponentsWithDefaults(inLocale localeIdentifier: String,
//                                                 expecting result: String,
//                                                 file: StaticString = #filePath,
//                                                 line: UInt = #line) {
//        _testTimeComponents(TimeComponents(hour: 3, minute: 24, second: 42),
//                            with: .init(locale: Locale(identifier: localeIdentifier)),
//                            expecting: result,
//                            file: file, line: line)
//    }
//
//    /// Tests `TimeComponents(hour: 3, minute: nil, second: 42)`
//    private func _testPartialComponentsWithDefaults(inLocale localeIdentifier: String,
//                                                    expecting result: String,
//                                                    file: StaticString = #filePath,
//                                                    line: UInt = #line) {
//        _testTimeComponents(TimeComponents(hour: 3, minute: nil, second: 42),
//                            with: .init(locale: Locale(identifier: localeIdentifier)),
//                            expecting: result,
//                            file: file, line: line)
//    }
//
//    /// Tests `TimeComponents(hour: 3, minute: 0, second: 42)` by not dropping zeros
//    private func _testPartialComponentsNotDroppingZeros(inLocale localeIdentifier: String,
//                                                        expecting result: String,
//                                                        file: StaticString = #filePath,
//                                                        line: UInt = #line) {
//        _testTimeComponents(TimeComponents(hour: 3, minute: 0, second: 42),
//                            with: .init(locale: Locale(identifier: localeIdentifier), dropZeros: false),
//                            expecting: result,
//                            file: file, line: line)
//    }

    // MARK: Full Components
//    func testFullComponentsWithDefaultsInEN_US() {
//        let parser = DurationFormatter(locale: Locale(identifier: "en_US"),
//                                       width: .numeric,
//                                       dropZeros: true)
//        let components = parser.parse("3")
//        XCTAssertEqual(components, .init(hour: 3, minute: nil, second: nil))
//    }

//    func testFullComponentsWithDefaultsInDE_DE() {
//        _testFullComponentsWithDefaults(inLocale: "de_DE",
//                                        expecting: CICUMajorVersion > 60 ? "3 Std., 24 Min. und 42 Sek." : "3 Std., 24 Min. und 42 s")
//    }
//
//    func testFullComponentsWithDefaultsInFR_FR() {
//        _testFullComponentsWithDefaults(inLocale: "fr_FR",
//                                        expecting: CICUMajorVersion > 60 ? "3h 24min 42s" : "3h 24 min 42s")
//    }
//
//    func testFullComponentsWithDefaultsInES_ES() {
//        _testFullComponentsWithDefaults(inLocale: "es_ES",
//                                        expecting: CICUMajorVersion > 60 && CICUMajorVersion < 67 ? "3 h 24 min 42 s" : "3h 24min 42s")
//    }
//
//    func testFullComponentsWithDefaultsInIT_IT() {
//        _testFullComponentsWithDefaults(inLocale: "it_IT", expecting: "3h 24min 42s")
//    }
//
//    func testFullComponentsWithDefaultsInIT_CH() {
//        _testFullComponentsWithDefaults(inLocale: "it_CH", expecting: "3h 24min 42s")
//    }
//
//    func testFullComponentsWithDefaultsInFR_CH() {
//        _testFullComponentsWithDefaults(inLocale: "fr_CH",
//                                        expecting: CICUMajorVersion > 60 ? "3h 24min 42s" : "3h 24 min 42s")
//    }
//
//    func testFullComponentsWithDefaultsInDE_CH() {
//        _testFullComponentsWithDefaults(inLocale: "de_CH",
//                                        expecting: CICUMajorVersion > 60 ? "3 Std., 24 Min. und 42 Sek." : "3 Std., 24 Min. und 42 s")
//    }
//
//    // MARK: Partial Components
//    func testPartialComponentsWithDefaultsInEN_US() {
//        _testPartialComponentsWithDefaults(inLocale: "en_US", expecting: "3h 42s")
//    }
//
//    func testPartialComponentsWithDefaultsInDE_DE() {
//        _testPartialComponentsWithDefaults(inLocale: "de_DE",
//                                           expecting: CICUMajorVersion > 60 ? "3 Std., 42 Sek." : "3 Std., 42 s")
//    }
//
//    func testPartialComponentsWithDefaultsInFR_FR() {
//        _testPartialComponentsWithDefaults(inLocale: "fr_FR", expecting: "3h 42s")
//    }
//
//    func testPartialComponentsWithDefaultsInES_ES() {
//        _testPartialComponentsWithDefaults(inLocale: "es_ES",
//                                           expecting: CICUMajorVersion > 60 && CICUMajorVersion < 67 ? "3 h 42 s" : "3h 42s")
//    }
//
//    func testPartialComponentsWithDefaultsInIT_IT() {
//        _testPartialComponentsWithDefaults(inLocale: "it_IT", expecting: "3h 42s")
//    }
//
//    func testPartialComponentsWithDefaultsInIT_CH() {
//        _testPartialComponentsWithDefaults(inLocale: "it_CH", expecting: "3h 42s")
//    }
//
//    func testPartialComponentsWithDefaultsInFR_CH() {
//        _testPartialComponentsWithDefaults(inLocale: "fr_CH", expecting: "3h 42s")
//    }
//
//    func testPartialComponentsWithDefaultsInDE_CH() {
//        _testPartialComponentsWithDefaults(inLocale: "de_CH",
//                                           expecting: CICUMajorVersion > 60 ? "3 Std., 42 Sek." : "3 Std., 42 s")
//    }
//
//    // MARK: Partial Components Not Dropping Zeros
//    func testPartialComponentsNotDroppingZerosInEN_US() {
//        _testPartialComponentsNotDroppingZeros(inLocale: "en_US", expecting: "3h 0m 42s")
//    }
//
//    func testPartialComponentsNotDroppingZerosInDE_DE() {
//        _testPartialComponentsNotDroppingZeros(inLocale: "de_DE",
//                                               expecting: CICUMajorVersion > 60 ? "3 Std., 0 Min. und 42 Sek." : "3 Std., 0 Min. und 42 s")
//    }
//
//    func testPartialComponentsNotDroppingZerosInFR_FR() {
//        _testPartialComponentsNotDroppingZeros(inLocale: "fr_FR",
//                                               expecting: CICUMajorVersion > 60 ? "3h 0min 42s" : "3h 0 min 42s")
//    }
//
//    func testPartialComponentsNotDroppingZerosInES_ES() {
//        _testPartialComponentsNotDroppingZeros(inLocale: "es_ES",
//                                               expecting: CICUMajorVersion > 60 && CICUMajorVersion < 67 ? "3 h 0 min 42 s" : "3h 0min 42s")
//    }
//
//    func testPartialComponentsNotDroppingZerosInIT_IT() {
//        _testPartialComponentsNotDroppingZeros(inLocale: "it_IT", expecting: "3h 0min 42s")
//    }
//
//    func testPartialComponentsNotDroppingZerosInIT_CH() {
//        _testPartialComponentsNotDroppingZeros(inLocale: "it_CH", expecting: "3h 0min 42s")
//    }
//
//    func testPartialComponentsNotDroppingZerosInFR_CH() {
//        _testPartialComponentsNotDroppingZeros(inLocale: "fr_CH",
//                                               expecting: CICUMajorVersion > 60 ? "3h 0min 42s" : "3h 0 min 42s")
//    }
//
//    func testPartialComponentsNotDroppingZerosInDE_CH() {
//        _testPartialComponentsNotDroppingZeros(inLocale: "de_CH",
//                                               expecting: CICUMajorVersion > 60 ? "3 Std., 0 Min. und 42 Sek." : "3 Std., 0 Min. und 42 s")
//    }
//}
