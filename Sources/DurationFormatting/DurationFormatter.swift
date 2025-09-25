public import Foundation
internal import CICUShims

public struct DurationFormatter: Sendable {
    public enum Width: Sendable, Hashable {
        case numeric, short, narrow

        fileprivate var _cicuFormatWidth: CICUDurationFormatWidth {
            switch self {
            case .numeric: .numeric
            case .short: .short
            case .narrow: .narrow
            }
        }
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

    public func format(_ timeComponents: TimeComponents) -> String? {
#if compiler(>=6.2)
        let result = unsafe cicu_formatDuration(timeComponents._cicuTimeComponents(nullingZeros: dropZeros),
                                                locale.identifier,
                                                width._cicuFormatWidth)
#else
        let result = cicu_formatDuration(timeComponents._cicuTimeComponents(nullingZeros: dropZeros),
                                         locale.identifier,
                                         width._cicuFormatWidth)
#endif
        defer { result.free() }
        guard result.isSuccess else { return nil }
#if compiler(>=6.2)
        return unsafe String(cString: result.value)
#else
        return String(cString: result.value)
#endif
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
}
