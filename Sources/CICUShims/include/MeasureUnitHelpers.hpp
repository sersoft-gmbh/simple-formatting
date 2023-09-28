#ifndef MeasureUnitHelpers_h
#define MeasureUnitHelpers_h

#include <string>
#include <unicode/utypes.h>
#include <unicode/measunit.h>

namespace CICUShims {
    static inline icu::MeasureUnit measureUnitForIdentifier(const std::string& identifier, UErrorCode &status) {
        return icu::MeasureUnit::forIdentifier(identifier, status);
    }
}

#endif /* MeasureUnitHelpers_h */
