#ifndef NumberFormatterHelpers_h
#define NumberFormatterHelpers_h

#include <unicode/utypes.h>
#include <unicode/unistr.h>
#include <unicode/numberformatter.h>

namespace CICUShims {
    static inline icu::UnicodeString formatDoubleWithNumberFormatter(icu::number::LocalizedNumberFormatter const& formatter,
                                                                     double const formattable,
                                                                     UErrorCode &status) {
        return formatter.formatDouble(formattable, status).toString(status);
    }
}

#endif /* NumberFormatterHelpers_h */
