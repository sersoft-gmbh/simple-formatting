#ifndef Result_h
#define Result_h

#include "Defines.h"

#define __CICU_CAT(A, B) A ## B
#define _CICU_CAT(A, B) __CICU_CAT(A, B)
#define __STR(A) #A
#define _STR(A) __STR(A)

#define _CICUUnderline _

#define _CICUResultTypeName(NamePart) CICU ## NamePart ## Result
#define _CICUResultTypeFunctionName(NamePart, Suffix) _CICU_CAT(_CICUResultTypeName(NamePart), Suffix)

#define _CICUResultTypeDefinition(NamePart, InnerType) \
struct _CICU_CAT(_CICUUnderline, _CICUResultTypeName(NamePart)); \
typedef struct _CICU_CAT(_CICUUnderline, _CICUResultTypeName(NamePart)) * CICUNonnull _CICUResultTypeName(NamePart) cicu_swift_newtype(struct); \
extern const bool _CICUResultTypeFunctionName(NamePart, IsSuccess)(_CICUResultTypeName(NamePart) result) cicu_swift_name("getter:"_STR(_CICUResultTypeName(NamePart))".isSuccess(self:)"); \
extern cicu_nullable InnerType _CICUResultTypeFunctionName(NamePart, GetResult)(_CICUResultTypeName(NamePart) result) cicu_swift_name("getter:"_STR(_CICUResultTypeName(NamePart))".value(self:)"); \
extern cicu_nonnull const char * CICUNonnull _CICUResultTypeFunctionName(NamePart, ErrorDescription)(_CICUResultTypeName(NamePart) result) cicu_swift_name("getter:"_STR(_CICUResultTypeName(NamePart))".errorDescription(self:)"); \
extern const void _CICUResultTypeFunctionName(NamePart, Free)(_CICUResultTypeName(NamePart) result) cicu_swift_name(_STR(_CICUResultTypeName(NamePart))".free(self:)");

#endif /* Result_h */
