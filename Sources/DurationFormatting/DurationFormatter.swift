import Foundation
@_implementationOnly import CICUShims

public struct TimeComponents {
    public var hour: Int?
    public var minute: Int?
    public var second: Int?

    @inlinable
    public var dateComponents: DateComponents {
        .init(hour: hour, minute: minute, second: second)
    }

    public init(hour: Int? = nil, minute: Int? = nil, second: Int? = nil) {
        self.hour = hour
        self.minute = minute
        self.second = second
    }

    @inlinable
    public init(dateComponents: DateComponents) {
        self.init(hour: dateComponents.hour,
                  minute: dateComponents.minute,
                  second: dateComponents.second)
    }

    fileprivate func _cicuTimeComponents(nullingZeros: Bool) -> CICUTimeComponents {
        func _component(for value: Int?) -> CICUTimeComponent {
            value.map(numericCast).map(nullingZeros ? CICUTimeComponent.init(nullingZeroOf:) : CICUTimeComponent.init(_:)) ?? .null
        }
        return .init(hours: _component(for: hour),
                     minutes: _component(for: minute),
                     seconds: _component(for: second))
    }
}

public struct DurationFormatter {
    public enum Width {
        case numeric, short, narrow

        fileprivate var _cicuFormatWidth: CICUDurationFormatWidth {
            switch self {
            case .numeric: return .numeric
            case .short: return .short
            case .narrow: return .narrow
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
        let result = cicu_formatDuration(timeComponents._cicuTimeComponents(nullingZeros: dropZeros),
                                         locale.identifier,
                                         width._cicuFormatWidth)
        defer { result.free() }
        guard result.isSuccess else { return nil }
        return String(cString: result.value)
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
