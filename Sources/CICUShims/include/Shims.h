#ifndef Shims_h
#define Shims_h

#include <string>
#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/numberformatter.h>
#include <unicode/measunit.h>

static inline icu::UnicodeString formatDoubleWithNumberFormatter(icu::number::LocalizedNumberFormatter const& formatter,
                                                                 double const formattable,
                                                                 UErrorCode &status) {
    return formatter.formatDouble(formattable, status).toString(status);
}

static inline icu::MeasureUnit measureUnitForIdentifier(const std::string& identifier, UErrorCode &status) {
    return icu::MeasureUnit::forIdentifier(identifier, status);
}

#endif /* Shims_h */
