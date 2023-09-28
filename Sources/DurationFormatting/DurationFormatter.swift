#if canImport(Darwin)
import Foundation
#else
@preconcurrency import Foundation // Locale is not Sendable on Linux
#endif

package import CxxStdlib
package import CICUCommon
package import CICUI18N
package import CICUShims

extension icu.UnicodeString {
    fileprivate func toString(_ status: inout UErrorCode) -> String {
        guard isEmpty() == 0 else { return String() }
        let charCount = countChar32(0, .max)
        let cchar = UnsafeMutablePointer<CChar>.allocate(capacity: Int(charCount) + 1)
        defer { cchar.deallocate() }
        extract(cchar, charCount, nil, &status)
        return String(cString: cchar)
    }
}

extension icu.MeasureFormat {
    fileprivate borrowing func formatMeasures(_ measures: Array<icu.Measure>, _ status: inout UErrorCode) -> icu.UnicodeString {
        var result = icu.UnicodeString()
        var pos = icu.FieldPosition()
        __formatMeasuresUnsafe(measures, .init(measures.count), &result, &pos, &status)
        return result
    }
}

extension UErrorCode {
    @inline(__always)
    fileprivate var isFailure: Bool { U_FAILURE(self) != 0 }
}

public struct DurationFormatter: Sendable {
    public enum Width: Sendable, Hashable {
        case numeric, short, narrow
    }

    public var locale: Locale
    public var width: Width
    public var dropZeros: Bool

    public init(locale: Locale = .current,
                width: Width = .narrow,
                dropZeros: Bool = true) {
        self.locale = locale
        self.width = width
        self.dropZeros = dropZeros
    }

    private func _formatNumeric(components: TimeComponents,
                                locale: icu.Locale,
                                width: UMeasureFormatWidth) -> String? {
        var status = U_ZERO_ERROR

        let measureFormat = icu.MeasureFormat(locale, width, &status)
        guard !status.isFailure else { return nil }

        var measures = Array<icu.Measure>()
        measures.reserveCapacity(3)
        if let hours = components.hour, !dropZeros || hours != 0 {
            let unit = icu.MeasureUnit.createHour(&status)
            guard !status.isFailure else { return nil }
            let measure = icu.Measure(.init(Int32(hours)), unit, &status)
            guard !status.isFailure else { return nil }
            measures.append(measure)
        }
        if let minutes = components.minute, !dropZeros || minutes != 0 {
            let unit = icu.MeasureUnit.createMinute(&status)
            guard !status.isFailure else { return nil }
            let measure = icu.Measure(.init(Int32(minutes)), unit, &status)
            guard !status.isFailure else { return nil }
            measures.append(measure)
        }
        if let seconds = components.second, !dropZeros || seconds != 0 {
            let unit = icu.MeasureUnit.createSecond(&status)
            guard !status.isFailure else { return nil }
            let measure = icu.Measure(.init(Int32(seconds)), unit, &status)
            guard !status.isFailure else { return nil }
            measures.append(measure)
        }

        let result = measureFormat.formatMeasures(measures, &status)
        guard !status.isFailure else { return nil }

        let string = result.toString(&status)
        guard !status.isFailure else { return nil }
        return string
    }

    private func _formatWithNumberFormatter(components: TimeComponents,
                                            locale: icu.Locale,
                                            width: UNumberUnitWidth) -> String? {
        var status = U_ZERO_ERROR

        var formattable = Double()
        var identifier: std.string = ""
        if let hour = components.hour, !dropZeros || hour != 0 {
            identifier += "hour"
            formattable = Double(hour)
        }
        if let minute = components.minute, !dropZeros || minute != 0 {
            if identifier.isEmpty {
                identifier += "minute"
                formattable = Double(minute)
            } else {
                identifier += "-and-minute"
                formattable += Double(minute) / 60
            }
        }
        if let second = components.second, !dropZeros || second != 0 {
            if identifier.isEmpty {
                identifier += "second"
                formattable = Double(second)
            } else {
                identifier += "-and-second"
                if components.hour == nil {
                    formattable += Double(second) / 60
                } else {
                    formattable += Double(second) / 60 / 60
                }
            }
        }
        if identifier.isEmpty {
            identifier += "second"
        }

        let unit = CICUShims.measureUnitForIdentifier(identifier, &status)
        // This currently fails. Likely because the string is somehow incorrectly constructed.
        // let unit = icu.MeasureUnit.forIdentifier(icu.StringPiece(identifier), &status)
        guard !status.isFailure else { return nil }

        let formatter = icu.number.NumberFormatter
            .withLocale(locale)
            //.usage("hour-and-minute-and-second")
            .unit(unit)
            .unitWidth(width)

        let formatted = CICUShims.formatDoubleWithNumberFormatter(formatter, formattable, &status)
        guard !status.isFailure else { return nil }

        let result = formatted.toString(&status)
        guard !status.isFailure else { return nil }
        return result
    }

    public func format(_ timeComponents: TimeComponents) -> String? {
        let locale = icu.Locale(locale.identifier, nil, nil, nil)
        switch width {
        case .numeric:
            return _formatNumeric(components: timeComponents, locale: locale, width: UMEASFMT_WIDTH_NUMERIC)
        case .narrow: 
            return _formatWithNumberFormatter(components: timeComponents, locale: locale, width: UNUM_UNIT_WIDTH_NARROW)
        case .short:
            return _formatWithNumberFormatter(components: timeComponents, locale: locale, width: UNUM_UNIT_WIDTH_SHORT)
        }
    }

    @inlinable
    public func format(_ dateComponents: DateComponents) -> String? {
        format(.init(dateComponents: dateComponents))
    }

    public func formatDuration(from start: Date, to end: Date) -> String? {
        format(locale.calendar.dateComponents([.hour, .minute, .second], from: start, to: end))
    }

    @inlinable
    public func formatDuration(_ duration: TimeInterval, from start: Date = Date(timeIntervalSinceReferenceDate: 0)) -> String? {
        formatDuration(from: start, to: start.addingTimeInterval(duration))
    }

//    public func parse(_ string: String) -> TimeComponents? {
//        let result = cicu_parseDuration(string, locale.identifier, width._cicuFormatWidth);
//        defer { result.free() }
//        guard result.isSuccess else { return nil }
//        return .init(_cicuTimeComponents: result.value, dropZeros: dropZeros)
//    }
}
