public import Foundation
internal import CICUShims

fileprivate extension CICUTimeComponent {
    func value(droppingZero dropZero: Bool) -> Int? {
        isNull || (dropZero && value == .zero) ? nil : numericCast(value)
    }
}

public struct TimeComponents: Hashable, Comparable, Sendable {
    public var hour: Int?
    public var minute: Int?
    public var second: Int?

    @inlinable
    public var dateComponents: DateComponents {
        .init(hour: hour, minute: minute, second: second)
    }

    public init(hour: Int? = nil,
                minute: Int? = nil,
                second: Int? = nil) {
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

    init(_cicuTimeComponents timeComponents: CICUTimeComponents, dropZeros: Bool) {
        self.init(hour: timeComponents.hours.value(droppingZero: dropZeros),
                  minute: timeComponents.minutes.value(droppingZero: dropZeros),
                  second: timeComponents.seconds.value(droppingZero: dropZeros))
    }

    func _cicuTimeComponents(nullingZeros: Bool) -> CICUTimeComponents {
        func _component(for value: Int?) -> CICUTimeComponent {
            value.map(numericCast).map(nullingZeros ? CICUTimeComponent.init(nullingZeroOf:) : CICUTimeComponent.init(_:)) ?? .null
        }
        return .init(hours: _component(for: hour),
                     minutes: _component(for: minute),
                     seconds: _component(for: second))
    }

    public static func <(lhs: Self, rhs: Self) -> Bool {
        lhs != rhs && (lhs.hour ?? 0, lhs.minute ?? 0, lhs.second ?? 0) < (rhs.hour ?? 0, rhs.minute ?? 0, rhs.second ?? 0)
    }
}
