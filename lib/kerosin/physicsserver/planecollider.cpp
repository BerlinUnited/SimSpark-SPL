#include "planecollider.h"

using namespace kerosin;

PlaneCollider::PlaneCollider() :
Collider()
{
}

void PlaneCollider::SetParams(float a, float b, float c, float d)
{
	if (mODEGeom)
		dGeomPlaneSetParams(mODEGeom, a, b, c, d);
}

bool PlaneCollider::ConstructInternal()
{
	if (!Collider::ConstructInternal()) return false;

	// a plane with normal pointing up, going through the origin
	mODEGeom = dCreatePlane(0, 0, 1, 0, 0);

	return (mODEGeom != 0);
}
