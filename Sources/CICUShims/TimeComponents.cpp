#include "include/TimeComponents.h"

const CICUTimeComponent CICUTimeComponentMake(int value) { return {value, false}; }
const CICUTimeComponent CICUTimeComponentMakeNullingZero(int value) { return {value, value == 0}; }
const CICUTimeComponent CICUTimeComponentNull = {0, true};
