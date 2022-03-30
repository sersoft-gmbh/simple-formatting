#include <assert.h>
#include <stdint.h>
#include <vector>
#include <unicode/uvernum.h>
#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/locid.h>
#include <unicode/fieldpos.h>
#include <unicode/numberformatter.h>
#include <unicode/measfmt.h>
//#include <unicode/msgfmt.h>
//#include <unicode/rbnf.h>

#define CICU_CAN_USE_NUMBER_FORMATTER (U_ICU_VERSION_MAJOR_NUM >= 67)

#define CICU_NO_SWIFT_ANNOTATIONS
#include "include/DurationFormat.h"
#include "ResultInternal.h"
#undef CICU_NO_SWIFT_ANNOTATIONS

_CICUResultTypeImpl(Formatting, const char * CICUNonnull, true);

CICUFormattingResult _cicu_makeResultFromUnicodeString(UErrorCode status, icu::UnicodeString str) {
    if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
    // FIXME: Is there a way to use (const) char directly?
    std::string outBuf;
    str.toUTF8String(outBuf);
    char *coutBuf = (char *)calloc(outBuf.length() + 1, sizeof(char));
    auto copied = outBuf.copy(coutBuf, outBuf.length());
    assert(copied == outBuf.length() && "Incomplete string copy!");
    return CICUFormattingResultMake(status, coutBuf);
}

CICUFormattingResult _cicu_formatWithNumericMeasureFormat(CICUTimeComponents timeComponents,
                                                          icu::Locale locale,
                                                          UMeasureFormatWidth measureFormatWidth) {
    UErrorCode status = U_ZERO_ERROR;

    auto measureFormat = icu::MeasureFormat(locale, measureFormatWidth, status);
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
    measureFormat.formatMeasures(measures.data(), (int32_t)measures.size(), result, pos, status);
    return _cicu_makeResultFromUnicodeString(status, result);
}

#if CICU_CAN_USE_NUMBER_FORMATTER
CICUFormattingResult _cicu_formatWithNumberFormatter(CICUTimeComponents components,
                                                     icu::Locale locale,
                                                     UNumberUnitWidth unitWidth) {
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
        .unit(unit)
        .unitWidth(unitWidth);

    auto formattedNum = numberFormatter.formatDouble(formattable, status);
    if (U_FAILURE(status)) return CICUFormattingResultMakeFailure(status);
    auto result = formattedNum.toString(status);
    return _cicu_makeResultFromUnicodeString(status, result);
}
#endif

CICUFormattingResult cicu_formatDuration(CICUTimeComponents components,
                                         cicu_nonnull const char * CICUNonnull localeIdentifier,
                                         CICUDurationFormatWidth width) {
    auto locale = icu::Locale(localeIdentifier);
    switch (width) {
        case CICUDurationFormatWidthNumeric:
            return _cicu_formatWithNumericMeasureFormat(components, locale, UMEASFMT_WIDTH_NUMERIC);
#if CICU_CAN_USE_NUMBER_FORMATTER
        case CICUDurationFormatWidthNarrow:
            return _cicu_formatWithNumberFormatter(components, locale, UNUM_UNIT_WIDTH_NARROW);
        case CICUDurationFormatWidthShort:
            return _cicu_formatWithNumberFormatter(components, locale, UNUM_UNIT_WIDTH_SHORT);
#else
        case CICUDurationFormatWidthNarrow:
            return _cicu_formatWithNumericMeasureFormat(components, locale, UMEASFMT_WIDTH_NARROW);
        case CICUDurationFormatWidthShort:
            return _cicu_formatWithNumericMeasureFormat(components, locale, UMEASFMT_WIDTH_SHORT);
#endif
    }
}

/*
// These deliver invalid results for non-EN locales
void _cicu_formatWithMessageFormat(double seconds, icu::Locale locale) {
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

void _cicu_formatWithRuleBasedNumberFormat(double seconds, icu::Locale locale) {
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
*/
