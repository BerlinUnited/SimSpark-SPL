#ifndef VECTOR_H__
#define VECTOR_H__

/*! 
	$Id: vector.h,v 1.1 2003/04/30 11:29:34 fruit Exp $

	Here we define/complete the vector definitions of TVector into specific
	vector types (2d float, 3d float, etc...).

	HISTORY:
		12.08.2002 MK
			- initial version

*/

#include "tvector.h"

namespace salt
{

class Vector2f : public TVector2<float, Vector2f>
{
	//
	// Methods
	//
public:
	/// Construction/Destruction/Assignment
	f_inline 	Vector2f()					: TVector2<float, Vector2f>() { }
	f_inline 	Vector2f(float x, float y)	: TVector2<float, Vector2f>(x, y) { }
	explicit	Vector2f(const float *f)	: TVector2<float, Vector2f>() { SetData(f);  }
	f_inline 	Vector2f(const Vector2f &v)	: TVector2<float, Vector2f>(v) { }
	explicit	Vector2f(float f)			: TVector2<float, Vector2f>() { Fill(f); }
};

class Vector3f : public TVector3<float, Vector3f>
{
	//
	// Methods
	//
public:
	/// Construction/Destruction/Assignment
	f_inline 	Vector3f()							: TVector3<float, Vector3f>() { }
	f_inline 	Vector3f(float x, float y, float z)	: TVector3<float, Vector3f>(x, y, z) { }
	explicit	Vector3f(const float *f)			: TVector3<float, Vector3f>() { SetData(f);  }
	f_inline 	Vector3f(const Vector3f &v)			: TVector3<float, Vector3f>(v) { }
	explicit	Vector3f(float f)					: TVector3<float, Vector3f>() { Fill(f); }

	f_inline	Vector3f Reflect(const Vector3f &normal)
	{
		float fac = 2.0f * (normal.x() * x() + normal.y() * y() + normal.z() * z());
		return Vector3f(fac * normal.x()-x(),
						fac * normal.y()-y(),
						fac * normal.z()-z());
	}
};

} //namespace salt

#endif //VECTOR_H__