import Foundation

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

    public static func <(lhs: Self, rhs: Self) -> Bool {
        lhs != rhs && (lhs.hour ?? 0, lhs.minute ?? 0, lhs.second ?? 0) < (rhs.hour ?? 0, rhs.minute ?? 0, rhs.second ?? 0)
    }
}
