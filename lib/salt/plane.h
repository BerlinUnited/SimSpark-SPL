/*
	Plane - Plane Math Classes

	A mathematical plane is modelled by this class. This can be used to classify
	points against planes (in front, on plane, etc...). Several useful functions
	for constructing a plane are provided.

	NOTE:

	HISTORY:
		11.06.01 - MK
			- Initial version

	TODO:

	TOFIX:
*/


#ifndef PLANE_H__
#define PLANE_H__

#include "defines.h"
#include "vector.h"
#include "bounds.h"


namespace salt
{

// a side on the plane
enum EPlaneSide
{
	PLANESIDE_BACK			=	0,
	PLANESIDE_ONPLANE		=	1,
	PLANESIDE_FRONT			=	2,
	PLANESIDE_SPLIT			=	3,
	PLANESIDE_FORCE_32BIT	=	0xffffffff	// used to force the enum to be 32-bit wide
};


// a dominant plane
enum EPlane
{
	PLANE_XY			=	0,
	PLANE_XZ			=	1,
	PLANE_YZ			=	2,
	PLANE_FORCE_32BIT	=	0xffffffff	// used to force the enum to be 32-bit wide
};


/*
	Plane

	A plane is defined by the formula. Ax+By+Cz+D=0. The vector formed by the coefficients
	<A,B,C> is the normal vector to the plane. In order to specify a plane in 3D you need
	three distinct points.
*/

class Plane
{
// Members
public:
	Vector3f	normal;
	float		d;

// Methods
public:
	f_inline Plane() {}		// empty constructor ... performance
	// construct plane from normal and a point on the plane
	f_inline Plane(const Vector3f& norm, const Vector3f& pnt)						{ normal=norm; d=-((norm.x()*pnt.x()) + (norm.y()*pnt.y()) + (norm.z()*pnt.z())); }
	// construct plane from normal and constant D. If normal is a unit vector, then D is the distance to the origin
	f_inline Plane(const Vector3f& norm, const float D)								{ normal=norm; d=D; }
	// construct plane from 3 distinct points
	f_inline Plane(const Vector3f& v1,  const Vector3f &v2, const Vector3f &v3)		{ normal=(v2-v1).Cross(v3-v1).Normalized(); d=-normal.Dot(v1); }
	
	// inline functions
	f_inline EPlaneSide	GetOrientation(const Vector3f &v, float delta=0.0001f) const	{ float dist=normal.Dot(v)+d; if (dist<-delta) return PLANESIDE_BACK; if (dist>delta) return PLANESIDE_FRONT; return PLANESIDE_ONPLANE;  }
	f_inline EPlane		GetDominantPlane() const									{ return (gAbs(normal.y()) > gAbs(normal.x()) ? (gAbs(normal.z()) > gAbs(normal.y()) ? PLANE_XY : PLANE_XZ) : (gAbs(normal.z()) > gAbs(normal.x()) ? PLANE_XY : PLANE_YZ)); }
	f_inline float		GetDistanceTo(const Vector3f &v) const						{ return normal.Dot(v) + d; }

	EPlaneSide ClassifyBox(const AABB3& bb) const;
	void Normalize();

	f_inline void Set(const Vector3f& norm, const Vector3f& pnt)						{ normal=norm; d=-((norm.x()*pnt.x()) + (norm.y()*pnt.y()) + (norm.z()*pnt.z())); }
	f_inline void Set(const Vector3f& norm, const float D)							{ normal=norm; d=D; }
	f_inline void Set(const Vector3f& v1,   const Vector3f &v2, const Vector3f &v3)	{ normal=(v2-v1).Cross(v3-v1).Normalized(); d=-normal.Dot(v1); }

	f_inline const Plane&	operator=(const Plane& p)								{ normal=p.normal; d=p.d; return *this; }
};

}

#endif //PLANE_H__