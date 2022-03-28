#include <vector>
#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <unicode/numberformatter.h>
#include <unicode/measfmt.h>
//#include <unicode/msgfmt.h>
//#include <unicode/rbnf.h>

#define CICU_NO_SWIFT_ANNOTATIONS
#include "include/DurationFormat.h"
#include "ResultInternal.h"
#undef CICU_NO_SWIFT_ANNOTATIONS

_CICUResultTypeImpl(Formatting, const char * CICUNonnull, false);

CICUFormattingResult _cicu_makeResultFromUnicodeString(UErrorCode status, icu::UnicodeString str) {
    if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
    // FIXME: Is there a way to use const char directly?
    std::string outBuf;
    str.toUTF8String(outBuf);
    char *coutBuf = (char *)calloc(outBuf.length() + 1, sizeof(char));
    auto copied = outBuf.copy(coutBuf, outBuf.length());
    assert(copied == outBuf.length() || "Incomplete string copy!");
    return CICUFormattingResultMake(status, coutBuf);
}

CICUFormattingResult _cicu_formatWithNumericMeasureFormat(CICUTimeComponents timeComponents, icu::Locale locale) {
    UErrorCode status = U_ZERO_ERROR;

    auto measureFormat = icu::MeasureFormat(locale, UMEASFMT_WIDTH_NUMERIC, status);
    if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);

    std::vector<icu::Measure> measures;
    measures.reserve(3);
    if (!timeComponents.hours.isNull) {
        auto unit = icu::MeasureUnit::createHour(status);
        if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
        auto measure = icu::Measure(timeComponents.hours.value, unit, status);
        if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
        measures.push_back(measure);
    }
    if (!timeComponents.minutes.isNull) {
        auto unit = icu::MeasureUnit::createMinute(status);
        if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
        auto measure = icu::Measure(timeComponents.minutes.value, unit, status);
        if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
        measures.push_back(measure);
    }
    if (!timeComponents.seconds.isNull || measures.empty()) {
        auto unit = icu::MeasureUnit::createSecond(status);
        if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
        auto measure = icu::Measure(timeComponents.seconds.isNull ? 0 : timeComponents.seconds.value, unit, status);
        if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
        measures.push_back(measure);
    }

    icu::UnicodeString result;
    icu::FieldPosition pos;
    measureFormat.formatMeasures(measures.data(), measures.size(), result, pos, status);
    return _cicu_makeResultFromUnicodeString(status, result);
}

CICUFormattingResult _cicu_formatWithNumberFormatter(CICUTimeComponents components, icu::Locale locale, bool formatShort) {
    UErrorCode status = U_ZERO_ERROR;

    double formattable;
    std::string identifier = "";
    if (!components.hours.isNull) {
        identifier.append("hour");
        formattable = (double)components.hours.value;
    }
    if (!components.minutes.isNull) {
        if (identifier.empty()) {
            identifier.append("minute");
            formattable = (double)components.minutes.value;
        } else {
            identifier.append("-and-minute");
            formattable += (double)components.minutes.value / 60.0;
        }
    }
    if (!components.seconds.isNull) {
        if (identifier.empty()) {
            identifier.append("second");
            formattable = (double)components.seconds.value;
        } else {
            identifier.append("-and-second");
            if (components.hours.isNull) {
                formattable += (double)components.seconds.value / 60.0;
            } else {
                formattable += (double)components.seconds.value / 60.0 / 60.0;
            }
        }
    }
    if (identifier.empty()) {
        identifier.append("second");
        formattable = 0.0;
    }

    auto unit = icu::MeasureUnit::forIdentifier(identifier, status);
    if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);

    auto numberFormatter = icu::number::NumberFormatter::withLocale(locale)
//        .usage("hour-and-minute-and-second")
        .unitWidth(formatShort ? UNUM_UNIT_WIDTH_SHORT : UNUM_UNIT_WIDTH_NARROW)
        .unit(unit);

    auto formattedNum = numberFormatter.formatDouble(formattable, status);
    if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
    auto result = formattedNum.toString(status);
    return _cicu_makeResultFromUnicodeString(status, result);
}

CICUFormattingResult cicu_formatDuration(CICUTimeComponents components,
                                         cicu_nonnull const char * CICUNonnull localeIdentifier,
                                         CICUDurationFormatWidth width) {
    auto locale = icu::Locale(localeIdentifier);
    switch (width) {
        case CICUDurationFormatWidthNumeric:
            return _cicu_formatWithNumericMeasureFormat(components, locale);
        case CICUDurationFormatWidthNarrow:
            return _cicu_formatWithNumberFormatter(components, locale, false);
        case CICUDurationFormatWidthShort:
            return _cicu_formatWithNumberFormatter(components, locale, true);
    }
}

/*
typedef struct {
    double seconds;
    double minutes;
    double hours;
} timeparts_t;

inline timeparts_t splitSeconds(double seconds) {
    double secondsPart, minutesPart, hoursPart;
    if (seconds >= 60.0) {
        minutesPart = (int)(seconds / 60.0);
    } else {
        minutesPart = 0.0;
    }
    if (minutesPart >= 60.0) {
        hoursPart = (int)(minutesPart / 60.0);
        minutesPart -= hoursPart * 60.0;
    } else {
        hoursPart = 0.0;
    }
    secondsPart = seconds - (minutesPart * 60.0) - (hoursPart * 60.0 * 60.0);
    return {
        secondsPart,
        minutesPart,
        hoursPart
    };
}

void testNumberFormatter(double seconds, icu::Locale locale) {
    UErrorCode status = U_ZERO_ERROR;

    double divisor;
    std::string identifier = "";
    auto timeParts = splitSeconds(seconds);
    if (timeParts.hours > 0) {
        identifier.append("hour");
        divisor = 60 * 60;
    }
    if (timeParts.minutes > 0) {
        if (identifier.empty()) {
            identifier.append("minute");
            divisor = 60;
        } else {
            identifier.append("-and-minute");
        }
    }
    if (timeParts.seconds > 0) {
        if (identifier.empty()) {
            identifier.append("second");
            divisor = 1;
        } else {
            identifier.append("-and-second");
        }
    }

    auto unit = icu::MeasureUnit::forIdentifier(identifier, status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to create unit: " << u_errorName(status) << std::endl;
        return;
    }

    auto numberFormatter = icu::number::NumberFormatter::withLocale(locale)
//        .usage("hour-and-minute-and-second")
        .unitWidth(UNUM_UNIT_WIDTH_NARROW)
        .unit(unit);

    auto formattedNum = numberFormatter.formatDouble(seconds / divisor, status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to format number: " << u_errorName(status) << std::endl;
        return;
    }
    auto result = formattedNum.toString(status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to convert to string: " << u_errorName(status) << std::endl;
        return;
    }
    std::cout << result << std::endl;
}

void testMeasureFormat(double seconds, icu::Locale locale) {
    UErrorCode status = U_ZERO_ERROR;

    auto measureFormat = icu::MeasureFormat(locale, UMEASFMT_WIDTH_NARROW, status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to create measure format: " << u_errorName(status) << std::endl;
        return;
    }

    auto unitSecond = icu::MeasureUnit::createSecond(status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to create measure unit second: " << u_errorName(status) << std::endl;
        return;
    }
    auto unitMinute = icu::MeasureUnit::createMinute(status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to create measure unit minute: " << u_errorName(status) << std::endl;
        return;
    }
    auto unitHour = icu::MeasureUnit::createHour(status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to create measure unit hour: " << u_errorName(status) << std::endl;
        return;
    }

    auto timeParts = splitSeconds(seconds);
    std::vector<icu::Measure> measures;
    measures.reserve(3);
    if (timeParts.hours > 0) {
        auto hoursMeasure = icu::Measure(timeParts.hours, unitHour, status);
        if (U_FAILURE(status)) {
            std::cerr << "Failed to create measure for hours: " << u_errorName(status) << std::endl;
            return;
        }
        measures.push_back(hoursMeasure);
    }
    if (timeParts.minutes > 0) {
        auto minutesMeasure = icu::Measure(timeParts.minutes, unitMinute, status);
        if (U_FAILURE(status)) {
            std::cerr << "Failed to create measure for minutes: " << u_errorName(status) << std::endl;
            return;
        }
        measures.push_back(minutesMeasure);
    }
    if (timeParts.seconds > 0) {
        auto secondsMeasure = icu::Measure(timeParts.seconds, unitSecond, status);
        if (U_FAILURE(status)) {
            std::cerr << "Failed to create measure for seconds: " << u_errorName(status) << std::endl;
            return;
        }
        measures.push_back(secondsMeasure);
    }

    icu::UnicodeString result;
    icu::FieldPosition pos;
    measureFormat.formatMeasures(measures.data(), measures.size(), result, pos, status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to format measures: " << u_errorName(status) << std::endl;
        return;
    }
    std::cout << result << std::endl;
}

void testMessageFormat(double seconds, icu::Locale locale) {
    UErrorCode status = U_ZERO_ERROR;
    auto messageFormat = icu::MessageFormat("{0,duration}", locale, status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to create message format: " << u_errorName(status) << std::endl;
        return;
    }
    icu::UnicodeString result;
    icu::Formattable args[] = {seconds};
    icu::FieldPosition pos;
    messageFormat.format(args, 1, result, pos, status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to format message: " << u_errorName(status) << std::endl;
        return;
    }
    std::cout << result << std::endl;
}

void testRuleBasedNumberFormat(double seconds, icu::Locale locale) {
    UErrorCode status = U_ZERO_ERROR;
    auto numberFormat = icu::RuleBasedNumberFormat(icu::URBNF_DURATION, locale, status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to create rule based number format: " << u_errorName(status) << std::endl;
        return;
    }
    icu::UnicodeString result;
    numberFormat.format(seconds, result, status);
    if (U_FAILURE(status)) {
        std::cerr << "Failed to format message: " << u_errorName(status) << std::endl;
        return;
    }
    std::cout << result << std::endl;
}

void test() {
    icu::Locale locale = "de_DE";

    double seconds = 10.5;
    seconds += 23.0 * 60.0;
    seconds += 3.0 * 60.0 * 60.0;

    std::cout << "Number Formatter: ";// << std::endl;
    testNumberFormatter(seconds, locale);
    std::cout << "Measure Format: ";// << std::endl;
    testMeasureFormat(seconds, locale);
    std::cout << "Messsage Format: ";// << std::endl;
    testMessageFormat(seconds, locale);
    std::cout << "Rule Based Number Format: ";// << std::endl;
    testRuleBasedNumberFormat(seconds, locale);
}
*/
