#ifndef TVECTOR_H__
#define TVECTOR_H__

/*! \class TVector
	$Id: tvector.h,v 1.1 2003/04/30 11:29:34 fruit Exp $
	
	TVector
	
	TVector is a template class for vector implementations. It abstracts away
	the number of elements and their type.

	TVector2 is a 2D version
	TVector3 is a 3D version

	HISTORY:
		12.08.2002 MK
			- initial version

*/

#include "defines.h"
#include "gmath.h"

namespace salt
{

template <typename DATATYPE, int ELEMENTS, typename TYPE>
class TVector
{
	//
	// functions
	//
public:
	TVector() {}

	// accessors
	f_inline DATATYPE&			operator[](int row)				{	return El(row);	}
	f_inline const DATATYPE&	operator[](int row) const		{	return El(row);	}
	f_inline DATATYPE&			Get(int row)					{	return El(row);	}
	f_inline const DATATYPE&	Get(int row) const				{	return El(row);	}

	// Direct pointer access to the data member ... use with care!!!
	f_inline const TYPE&	SetData(const DATATYPE *copy);
	f_inline DATATYPE*			GetData()						{	return mData;	}

	f_inline const TYPE&	Fill(const DATATYPE &fill);			// Fill vector with value 'fill'
	f_inline TYPE&			Zero();							// Zero out this vector

	// operators
	f_inline const TYPE		operator+(const TYPE &v) const; // VECTORA + VECTORB
	f_inline const TYPE		operator-(const TYPE &v) const; // VECTORA - VECTORB
	f_inline const TYPE		operator*(const DATATYPE &v) const; // VECTORA * FLOAT (for example)
	f_inline const TYPE		operator/(const DATATYPE &v) const;	// VECTORA / FLOAT (for example)

	f_inline TYPE&			operator+=(const TYPE &v);		// VECTORA+=VECTORB
	f_inline TYPE&			operator-=(const TYPE &v);		// VECTORA-=VECTORB
	f_inline TYPE&			operator*=(const DATATYPE &v);		// VECTORA*=FLOAT (for example)
	f_inline TYPE&			operator/=(const DATATYPE &v);		// VECTORA/=FLOAT (for example)

	f_inline TYPE			operator-() const;				// -VECTORA
	f_inline bool			operator==(const TYPE &v)const;	// VECTORA == VECTORB
	f_inline bool			operator!=(const TYPE &v)const;	// VECTORA != VECTORB

	f_inline DATATYPE			Dot(const TYPE &v) const;
	f_inline const TYPE&	Normalize();
	f_inline TYPE			Normalized() const;

	f_inline DATATYPE			SquareLength() const;			// Squared length of the vector
	f_inline DATATYPE			Length() const					{ return gSqrt(SquareLength()); }

	//! returns XX significant axis helper function (return between 0 and ELEMENTS)
	f_inline int			GetLeastSignificantAxis() const;
	f_inline int			GetMostSignificantAxis() const;

	//! interpolation routines
	f_inline TYPE			LinearInterpolate(const TYPE &to, float delta) const;
	f_inline TYPE			NormalizedLinearInterpolate(const TYPE &to, float delta) const
	{
		return LinearInterpolate(to, delta).Normalize();
	}
protected:
	// Copy constructor:
	TVector(const TYPE &v)
	{
		for (int i=0; i<ELEMENTS; i++)
			mData[i] = v[i];
	}

	// Element accessor
	const DATATYPE&	El(int index) const
	{
		return mData[index];
	}
	DATATYPE&		El(int index)
	{
		return mData[index];
	}

	//
	// members
	//
private:
	DATATYPE	mData[ELEMENTS];
};

// TVector2 ... 2D Vector
template <typename DATATYPE, class TYPE>
class TVector2 : public TVector<DATATYPE, 2, TYPE>
{
public:
	// Default Constructor
	TVector2() : TVector<DATATYPE, 2, TYPE>() {};

	// Init constructor with two elements
	TVector2(DATATYPE x, DATATYPE y) : TVector<DATATYPE, 2, TYPE>() { Set(x, y); }


	// Element Access operators
	f_inline DATATYPE&			x()					{ return El(0); }
	f_inline const DATATYPE&	x() const			{ return El(0); }
	f_inline DATATYPE&			y()					{ return El(1); }
	f_inline const DATATYPE&	y() const			{ return El(1); }
	
	// Set operator
	f_inline const TYPE& Set(const DATATYPE &x, const DATATYPE &y)
	{
		El(0) = x;
		El(1) = y;
		return *static_cast<TYPE*>(this);
	}
};


// TVector3 ... 3D Vector
template <typename DATATYPE, class TYPE>
class TVector3 : public TVector<DATATYPE, 3, TYPE>
{
public:
	// Default Constructor
	TVector3() : TVector<DATATYPE, 3, TYPE>() {};

	// Init constructor
	TVector3(const DATATYPE &x, const DATATYPE &y, const DATATYPE &z) : TVector<DATATYPE, 3, TYPE>() { Set(x, y, z); }

	// Element Access operators
	f_inline DATATYPE&			x()					{ return El(0); }
	f_inline const DATATYPE&	x() const			{ return El(0); }
	f_inline DATATYPE&			y()					{ return El(1); }
	f_inline const DATATYPE&	y() const			{ return El(1); }
	f_inline DATATYPE&			z()					{ return El(2); }
	f_inline const DATATYPE&	z() const			{ return El(2); }

	// Cross product - returns a new vector
	template <typename DATATYPE, class TYPE>
	const TYPE Cross(const TVector<DATATYPE, 3, TYPE> &v) const
	{
		// Create a new one
		TYPE r;
		r[0] = El(1) * v[2] - El(2) * v[1];
		r[1] = El(2) * v[0] - El(0) * v[2];
		r[2] = El(0) * v[1] - El(1) * v[0];
		return r;
	}

	//
	// Set operator
	//
	const TYPE& Set(const DATATYPE &x, const DATATYPE &y, const DATATYPE &z)
	{
		El(0) = x;
		El(1) = y;
		El(2) = z;
		return *static_cast<TYPE*>(this);
	}

};

// Set operator for any vector. Just with a pointer
template <typename DATATYPE, int ELEMENTS, typename TYPE>
inline const TYPE	&TVector<DATATYPE, ELEMENTS, TYPE>::SetData(const DATATYPE *copy) {
	for (int i=0; i<ELEMENTS; i++)
		mData[i] = copy[i];
	return *static_cast<TYPE*>(this);
}

// fill vector with value 'fill'
template <typename DATATYPE, int ELEMENTS, class TYPE> 
f_inline const TYPE &TVector<DATATYPE, ELEMENTS, TYPE>::Fill(const DATATYPE& fill)
{
	for (int c=0; c < ELEMENTS; c++)
		mData[c] = fill;
	return *static_cast<TYPE*>(this);
}

// fill vector with zeros
template <typename DATATYPE, int ELEMENTS, class TYPE> 
f_inline TYPE &TVector<DATATYPE, ELEMENTS, TYPE>::Zero()
{
	for (int c=0; c < ELEMENTS; c++)
		mData[c] = DATATYPE(0);
	return *static_cast<TYPE*>(this);
}

// DATATYPE * vector
template <typename DATATYPE, int ELEMENTS, class TYPE> 
f_inline const TYPE operator*(const DATATYPE &f, const TVector<DATATYPE, ELEMENTS, TYPE> &vec)
{
	return vec * f; 
}

// vector addition
// this + v return new Vector
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline const TYPE TVector<DATATYPE, ELEMENTS, TYPE>::operator+(const TYPE &v) const
{
	TYPE r;
	for (int c=0; c < ELEMENTS; c++)
		r[c]=mData[c] + v[c];
	return r;
}

// vector subtraction
// this - v return new Vector
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline const TYPE TVector<DATATYPE, ELEMENTS, TYPE>::operator-(const TYPE &v) const
{
	TYPE r;
	for (int c=0; c < ELEMENTS; c++)
		r[c]=mData[c] - v[c];
	return r;
}

// scale
// this * DATATYPE return new Vector
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline const TYPE TVector<DATATYPE, ELEMENTS, TYPE>::operator*(const DATATYPE &v) const
{
	TYPE r;
	for (int c=0; c < ELEMENTS; c++)
		r[c]=mData[c] * v;
	return r;
}

// division
// this / DATATYPE return new Vector
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline const TYPE TVector<DATATYPE, ELEMENTS, TYPE>::operator/(const DATATYPE &v) const
{
	TYPE r;
	for (int c=0; c < ELEMENTS; c++)
		r[c]=mData[c] / v;
	return r;
}

// this += v returns reference to first vector
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline TYPE &TVector<DATATYPE, ELEMENTS, TYPE>::operator+=(const TYPE &v)
{
	for (int c=0; c<ELEMENTS; c++)
		mData[c] += v[c];
	return *static_cast<TYPE*>(this);
}

// this -= v returns reference to first vector
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline TYPE &TVector<DATATYPE, ELEMENTS, TYPE>::operator-=(const TYPE &v)
{
	for (int c=0; c<ELEMENTS; c++)
		mData[c] -= v[c];
	return *static_cast<TYPE*>(this);
}

// this *= DATATYPE returns reference to first vector
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline TYPE &TVector<DATATYPE, ELEMENTS, TYPE>::operator*=(const DATATYPE &v)
{
	for (int c=0; c<ELEMENTS; c++)
		mData[c] *= v;
	return *static_cast<TYPE*>(this);
}

// this /= DATATYPE returns reference to first vector
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline TYPE &TVector<DATATYPE, ELEMENTS, TYPE>::operator/=(const DATATYPE &v)
{
	for (int c=0; c<ELEMENTS; c++)
		mData[c] *= v;
	return *static_cast<TYPE*>(this);
}

// unary minus
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline TYPE TVector<DATATYPE, ELEMENTS, TYPE>::operator-() const
{
	TYPE r;
	for (int c=0; c < ELEMENTS; c++)
		r[c] = -mData[c];
	return r;
}

// equality 
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline bool TVector<DATATYPE, ELEMENTS, TYPE>::operator==(const TYPE &v) const
{
	return (0==memcmp(this, &v, sizeof(*this)));
}

// inequality 
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline bool TVector<DATATYPE, ELEMENTS, TYPE>::operator!=(const TYPE &v) const
{
	return (0!=memcmp(this, &v, sizeof(*this)));
}

// generic dot product
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline DATATYPE TVector<DATATYPE, ELEMENTS, TYPE>::Dot(const TYPE &v) const
{
	DATATYPE r = mData[0] * v[0];
	for (int c=1; c < ELEMENTS; c++)
		r += mData[c] * v[c];
	return r;
}

// Normalize vector
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline const TYPE &TVector<DATATYPE, ELEMENTS, TYPE>::Normalize()
{
	DATATYPE length		= Length();
	DATATYPE lengthInv	= DATATYPE(DATATYPE(1) / length);
	for (int c=0; c<ELEMENTS; c++)
		mData[c] *= lengthInv;
	return *static_cast<TYPE*>(this);
}

template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline TYPE TVector<DATATYPE, ELEMENTS, TYPE>::Normalized() const
{
	TYPE r;
	DATATYPE length		= Length();
	DATATYPE lengthInv	= DATATYPE(DATATYPE(1) / length);
	for (int c=0; c<ELEMENTS; c++)
		r[c] = mData[c] * lengthInv;
	return r;
}

// Squared length determination:
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline DATATYPE TVector<DATATYPE, ELEMENTS, TYPE>::SquareLength() const
{
	DATATYPE r = mData[0] * mData[0];
	for (int c=1; c<ELEMENTS; c++)
		r += mData[c] * mData[c];
	return r;
}

// Linear Interpolate. Interpolated from one vector to the other
template <typename DATATYPE, int ELEMENTS, typename TYPE>
f_inline TYPE TVector<DATATYPE, ELEMENTS, TYPE>::LinearInterpolate(const TYPE &to, float t) const
{
	float it = 1.0f - t;
	TYPE r;
	for (int c=0; c<ELEMENTS; c++) 
		r[c] = mData[c] * it + to[c] * t;
	return r;
}

} //namespace salt

#endif //TVECTOR_H__