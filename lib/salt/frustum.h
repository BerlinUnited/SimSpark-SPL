/*	$Id: frustum.h,v 1.1 2003/04/30 14:21:48 fruit Exp $

	Frustum

	This class implements a frustum, which is an arbitrary volume in space (possibly infinite)
	defined by a set of planes. This class also provides some 'special' frustra construction
	methods. 

	NOTE:

	HISTORY:
		11.06.01 - MK
			- Initial version
		23.03.02 - MK
			- Intersection/Inclusion-tests


	TODO:
		- Create frustum from a (view)point and a polygon
		- Create a view frustum given field-of-views

	TOFIX:
*/

#ifndef FRUSTUM_H__
#define FRUSTUM_H__

#include "defines.h"
#include "plane.h"
#include "bounds.h"

namespace salt
{

class Frustum
{
// Members
public:
	enum ePlaneID
	{
		PI_NEAR		= 0,
		PI_LEFT		= 1,
		PI_RIGHT	= 2,
		PI_FAR		= 3,
		PI_BOTTOM	= 4,
		PI_TOP		= 5
	};
	enum eFrustumSide
	{
		FS_INSIDE  = 0,
		FS_OUTSIDE = 1,
		FS_SPLIT   = 2
	};

	
	Vector3f	mBasePos;			// Position from where the frustum is cast
	Plane		mPlanes[6];			// 6 Plane equations

// Methods
public:
	void Dump() const;
	void Set(const Matrix& worldTransform, float fov=60.0f, float zNear=0.1f, float zFar=2000.0f, float aspect=0.75f);
	eFrustumSide Intersects(const AABB3& bb) const;
};

} // namespace salt

#endif //FRUSTUM_H__