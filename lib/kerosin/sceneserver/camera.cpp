#include "camera.h"
#include <salt/matrix.h>
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace kerosin;
using namespace salt;

Camera::Camera() :
BaseNode()
{
	mViewTransform.Identity();

	mFOV	= 60.0f;
	mZNear	= 1.0f;
	mZFar	= 2000.0f;
	mX		= 0;
	mY		= 0;
	mWidth	= 640;
	mHeight	= 480;
	
	SetName("camera");
}

Camera::~Camera()
{
}

/*!
	This is some dangerous looking code, but it's not as bad as it seems.
	The code extracts a frustum given the projection and view transforms.
	It is really fast and very generic because of that.
*/
void Camera::DescribeFrustum(Frustum& frustum) const
{
    // concatenate projection and view transform
    Matrix frustumMatrix = mProjectionTransform * mViewTransform;

    // Get plane parameters
    float *m= frustumMatrix.m;
   
    Plane *p = &frustum.mPlanes[Frustum::PI_RIGHT];
	p->normal.Set(m[3]-m[0], m[7]-m[4], m[11]-m[8]);
	p->d = m[15]-m[12];
	
	p = &frustum.mPlanes[Frustum::PI_LEFT];
	p->normal.Set(m[3]+m[0], m[7]+m[4], m[11]+m[8]);
	p->d = m[15]+m[12];

	p = &frustum.mPlanes[Frustum::PI_BOTTOM];
	p->normal.Set(m[3]+m[1], m[7]+m[5], m[11]+m[9]);
	p->d = m[15]+m[13];

	p = &frustum.mPlanes[Frustum::PI_TOP];
	p->normal.Set(m[3]-m[1], m[7]-m[5], m[11]-m[9]);
	p->d = m[15]-m[13];

	p = &frustum.mPlanes[Frustum::PI_NEAR];
	p->normal.Set(m[3]-m[2], m[7]-m[6], m[11]-m[10]);
	p->d = m[15]-m[14];

	p = &frustum.mPlanes[Frustum::PI_FAR];
	p->normal.Set(m[3]+m[2], m[7]+m[6], m[11]+m[10]);
	p->d = m[15]+m[14];

	// Normalize all plane normals
	for(int i=0;i<6;++i)
	{
		frustum.mPlanes[i].Normalize();
	}

	// set base position
	frustum.mBasePos = GetWorldTransform().Pos();
}

void Camera::Bind()
{
    mViewTransform	= GetWorldTransform();
    mViewTransform.InvertRotationMatrix();

    mProjectionTransform.CalcInfiniteFrustum(-mHalfWorldWidth, mHalfWorldWidth, -mHalfWorldHeight, mHalfWorldHeight, mZNear);
}

void Camera::OnLink()
{
	int xRes, yRes;
	GetScript()->GetVariable("Viewport.xRes", xRes);
	GetScript()->GetVariable("Viewport.yRes", yRes);

	mWidth = xRes;
	mHeight = yRes;
}

void Camera::UpdateHierarchyInternal()
{
	// make sure values are within bounds
	gClamp(mFOV, 10.0f, 170.0f);

	mHalfWorldWidth		= mZNear * (float)tan(gDegToRad(mFOV*0.5f));
	mHalfWorldHeight	= mHalfWorldWidth * (mHeight/(float)mWidth);
}
