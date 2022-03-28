#ifndef ResultInternal_h
#define ResultInternal_h

#include "include/Result.h"

#define _CICUResultTypeImpl(NamePart, InnerType, IsPtrType) \
struct _CICU_CAT(_CICUUnderline, _CICUResultTypeName(NamePart)) { \
    UErrorCode icuCode;\
    cicu_nullable InnerType inner;\
};\
const _CICUResultTypeName(NamePart) _CICUResultTypeFunctionName(NamePart, Make)(UErrorCode icuCode, InnerType inner) {\
    auto result = (_CICUResultTypeName(NamePart))calloc(1, sizeof(_CICU_CAT(_CICUUnderline, _CICUResultTypeName(NamePart)))); \
    result->icuCode = icuCode;\
    result->inner = inner;\
    return result;\
}\
const _CICUResultTypeName(NamePart) _CICUResultTypeFunctionName(NamePart, MakeFailure)(UErrorCode icuCode) {\
    auto result = (_CICUResultTypeName(NamePart))calloc(1, sizeof(_CICU_CAT(_CICUUnderline, _CICUResultTypeName(NamePart)))); \
    result->icuCode = icuCode;\
    return result;\
}\
const bool _CICUResultTypeFunctionName(NamePart, IsSuccess)(_CICUResultTypeName(NamePart) result) {\
    return U_SUCCESS(result->icuCode);\
}\
cicu_nullable InnerType _CICUResultTypeFunctionName(NamePart, GetResult)(_CICUResultTypeName(NamePart) result) {\
    return result->inner;\
}\
const char * CICUNonnull _CICUResultTypeFunctionName(NamePart, ErrorDescription)(_CICUResultTypeName(NamePart) result) {\
    return u_errorName(result->icuCode);\
}\
const void _CICUResultTypeFunctionName(NamePart, Free)(_CICUResultTypeName(NamePart) result) {\
    if (IsPtrType && result->inner != NULL) free((void *)result->inner);\
    free(result);\
}

#endif /* ResultInternal_h */
