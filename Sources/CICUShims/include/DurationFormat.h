#ifndef DurationFormat_h
#define DurationFormat_h

#include "Defines.h"
#include "TimeComponents.h"
#include "Result.h"

#ifdef __cplusplus
extern "C" {
#endif

_CICUResultTypeDefinition(Formatting, const char * CICUNonnull);

typedef enum cicu_enum_extensibility(open) {
    CICUDurationFormatWidthNumeric,
    CICUDurationFormatWidthNarrow,
    CICUDurationFormatWidthShort,
} CICUDurationFormatWidth;

extern CICUFormattingResult cicu_formatDuration(CICUTimeComponents components,
                                                cicu_nonnull const char * CICUNonnull localeIdentifier,
                                                CICUDurationFormatWidth width);

#ifdef __cplusplus
}
#endif

#endif /* DurationFormat_h */
