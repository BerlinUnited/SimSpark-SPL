#ifndef GMATH_H__
#define GMATH_H__

#include "defines.h"
#include <math.h>

template <typename TYPE>
f_inline TYPE gSqrt(const TYPE &v) {
	return TYPE(sqrt(v));
}

#endif //GMATH_H__