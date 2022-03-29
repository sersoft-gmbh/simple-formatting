#ifndef TimeComponents_h
#define TimeComponents_h

#include <stdbool.h>
#include "Defines.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int value;
    bool isNull;
} CICUTimeComponent;

extern const CICUTimeComponent CICUTimeComponentMake(int value) cicu_swift_name("CICUTimeComponent.init(_:)");
extern const CICUTimeComponent CICUTimeComponentMakeNullingZero(int value) cicu_swift_name("CICUTimeComponent.init(nullingZeroOf:)");
extern const CICUTimeComponent CICUTimeComponentNull cicu_swift_name("CICUTimeComponent.null");

typedef struct {
    CICUTimeComponent hours;
    CICUTimeComponent minutes;
    CICUTimeComponent seconds;
} CICUTimeComponents;

#ifdef __cplusplus
}
#endif

#endif /* TimeComponents_h */
