import Foundation
import Testing
import CICUShims
import DurationFormatting

@Suite
struct DurationFormattingTests {
    @Test(arguments: [
        (locale: "en_US", expected: "3h 24m 42s"),
        (locale: "de_DE", expected: CICUMajorVersion > 77 ? "3h, 24 Min. und 42 Sek." : CICUMajorVersion > 60 ? "3 Std., 24 Min. und 42 Sek." : "3 Std., 24 Min. und 42 s"),
        (locale: "fr_FR", expected: CICUMajorVersion > 60 ? "3h 24min 42s" : "3h 24 min 42s"),
        (locale: "es_ES", expected: CICUMajorVersion > 60 && CICUMajorVersion < 67 ? "3 h 24 min 42 s" : "3h 24min 42s"),
        (locale: "it_IT", expected: "3h 24min 42s"),
        (locale: "it_CH", expected: "3h 24min 42s"),
        (locale: "fr_CH", expected: CICUMajorVersion > 60 ? "3h 24min 42s" : "3h 24 min 42s"),
        (locale: "de_CH", expected: CICUMajorVersion > 77 ? "3h, 24 Min. und 42 Sek." : CICUMajorVersion > 60 ? "3 Std., 24 Min. und 42 Sek." : "3 Std., 24 Min. und 42 s"),
    ])
    func fullComponentsWithDefaults(locale: String, expected: String) {
        let formatter = DurationFormatter(locale: Locale(identifier: locale))
        #expect(formatter.format(TimeComponents(hour: 3, minute: 24, second: 42)) == expected)
    }

    @Test(arguments: [
        (locale: "en_US", expected: "3h 42s"),
        (locale: "de_DE", expected: CICUMajorVersion > 77 ? "3h, 42 Sek." : CICUMajorVersion > 60 ? "3 Std., 42 Sek." : "3 Std., 42 s"),
        (locale: "fr_FR", expected: "3h 42s"),
        (locale: "es_ES", expected: CICUMajorVersion > 60 && CICUMajorVersion < 67 ? "3 h 42 s" : "3h 42s"),
        (locale: "it_IT", expected: "3h 42s"),
        (locale: "it_CH", expected: "3h 42s"),
        (locale: "fr_CH", expected: "3h 42s"),
        (locale: "de_CH", expected: CICUMajorVersion > 77 ? "3h, 42 Sek." : CICUMajorVersion > 60 ? "3 Std., 42 Sek." : "3 Std., 42 s"),
    ])
    func partialComponentsWithDefaults(locale: String, expected: String) {
        let formatter = DurationFormatter(locale: Locale(identifier: locale))
        #expect(formatter.format(TimeComponents(hour: 3, minute: nil, second: 42)) == expected)
    }

    @Test(arguments: [
        (locale: "en_US", expected: "3h 0m 42s"),
        (locale: "de_DE", expected: CICUMajorVersion > 77 ? "3h, 0 Min. und 42 Sek." : CICUMajorVersion > 60 ? "3 Std., 0 Min. und 42 Sek." : "3 Std., 0 Min. und 42 s"),
        (locale: "fr_FR", expected: CICUMajorVersion > 60 ? "3h 0min 42s" : "3h 0 min 42s"),
        (locale: "es_ES", expected: CICUMajorVersion > 60 && CICUMajorVersion < 67 ? "3 h 0 min 42 s" : "3h 0min 42s"),
        (locale: "it_IT", expected: "3h 0min 42s"),
        (locale: "it_CH", expected: "3h 0min 42s"),
        (locale: "fr_CH", expected: CICUMajorVersion > 60 ? "3h 0min 42s" : "3h 0 min 42s"),
        (locale: "de_CH", expected: CICUMajorVersion > 77 ? "3h, 0 Min. und 42 Sek." : CICUMajorVersion > 60 ? "3 Std., 0 Min. und 42 Sek." : "3 Std., 0 Min. und 42 s"),
    ])
    func partialComponentsNotDroppingZeros(locale: String, expected: String) {
        let formatter = DurationFormatter(locale: Locale(identifier: locale), dropZeros: false)
        #expect(formatter.format(TimeComponents(hour: 3, minute: 0, second: 42)) == expected)
    }
}
