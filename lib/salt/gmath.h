#ifndef GMATH_H__
#define GMATH_H__

#include "defines.h"
#include <math.h>

namespace salt
{

const static float gPI		= ((float)3.1415926535);
const static float g2PI		= ((float)6.283185307);
const static float gHalfPI	= ((float)1.570796326794895);

template <typename TYPE>
f_inline TYPE gSqrt(const TYPE &v)
{
	return (TYPE)sqrt(v);
}

template <class TYPE>
f_inline TYPE gClamp(TYPE &val, TYPE min, TYPE max)
{
	if (val<min) val=min; if (val>max) val=max; return val;
}

template <class TYPE>
f_inline TYPE gMin(TYPE a, TYPE b)
{
	if (a<b) return a; return b;
}

template <class TYPE>
f_inline TYPE gMax(TYPE a, TYPE b)
{
	if (a>b) return a; return b;
}

template <class TYPE>
f_inline TYPE gFloor(TYPE a)
{
	return (TYPE)floor(a);
}

template <class TYPE> 
f_inline TYPE	gCeil(TYPE a)
{
	return ceil(a);
}

template <class TYPE> 
f_inline TYPE	gAbs(TYPE a)
{
	return (TYPE)fabs(a);
}

template <class TYPE> 
f_inline TYPE	gNeg(TYPE a)
{
	return -a;
}

template <class TYPE>
f_inline TYPE gCos(TYPE a)
{
	return (TYPE)cos(a);
}

template <class TYPE>
f_inline TYPE	gSin(TYPE a)
{
	return (TYPE)sin(a);
}

template <class TYPE> 
f_inline TYPE	gArcCos(TYPE a)
{ 
	return acos(a); 
}

template <class TYPE> 
f_inline TYPE	gArcSin(TYPE a)
{ 
	return asin(a); 
}

template <class TYPE> 
f_inline TYPE	gArcTan(TYPE a)
{ 
	return atan(a); 
}

template <class TYPE> 
f_inline TYPE	gArcTan2(TYPE a, TYPE b)
{ 
	return atan2(a, b); 
}

template <class TYPE> 
f_inline TYPE	gPow(TYPE a, TYPE b)
{ 
	return pow(a, b); 
}

template <class TYPE> 
f_inline void gSwap(TYPE &a, TYPE &b)
{ 
	TYPE temp = a;
	a = b;
	b = temp;
}

// some math conversion functions
template <class TYPE> 
f_inline float gDegToRad(TYPE deg)
{
	return ((float)deg) * (gPI / 180.0f);
}

template <class TYPE> 
f_inline float gRadToDeg(TYPE rad)
{
	return ((float)rad) * (180.0f/gPI);
}

template <class TYPE>
f_inline bool	gInRange(const TYPE& val, const TYPE& low, const TYPE& high)
{
	return ((val>=low) && (val<=high));
}

} //namespace salt

#endif //GMATH_H__