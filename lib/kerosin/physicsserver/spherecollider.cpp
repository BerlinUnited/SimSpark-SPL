#include "spherecollider.h"

using namespace kerosin;

SphereCollider::SphereCollider() :
Collider()
{
}

void SphereCollider::SetRadius(float r)
{
	if (mODEGeom)
		dGeomSphereSetRadius(mODEGeom, r);
}

bool SphereCollider::ConstructInternal()
{
	if (!Collider::ConstructInternal()) return false;

	// create a unit sphere
	mODEGeom = dCreateSphere(0, 1.0f);

	return (mODEGeom != 0);
}