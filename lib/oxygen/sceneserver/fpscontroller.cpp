#include "fpscontroller.h"
#include "../physicsserver/body.h"

using namespace boost;
using namespace kerosin;
using namespace salt;

FPSController::FPSController() :
BaseNode()
{
	mHAngle = 0.0f;
	mVAngle = 0.0f;

	mForward	= false;
	mBackward	= false;
	mLeft		= false;
	mRight		= false;
	mUp			= false;
	mDown		= false;
}

FPSController::~FPSController()
{
}

void FPSController::PrePhysicsUpdateInternal(float deltaTime)
{

	// determine force direction
	Vector3f force(0,0,0);

	if(mForward)	force.z() -= 1.0f;
	if(mBackward)	force.z() += 1.0f;
	if(mRight)		force.x() += 1.0f;
	if(mLeft)		force.x() -= 1.0f;
	if(mUp)			force.y() += 1.0f;
	if(mDown)		force.y() -= 1.0f;

	if (force.Length() > 1.0f)
		force.Normalize();

	shared_ptr<Body> body = shared_static_cast<Body>(make_shared(GetParent()));

	if (body.get() != NULL)
	{
		// constrain angles
		if(mVAngle>88.0f)
			mVAngle = 88.0f;
		else
			if(mVAngle<-88.0f)
				mVAngle = -88.0f;

		Matrix matrix;
		matrix.RotationY(gDegToRad(-mHAngle));
		matrix.RotateX(gDegToRad(-mVAngle));

		dMatrix3 ODEMatrix;
		dRSetIdentity(ODEMatrix);
		ODEMatrix[0]=matrix.m[0];
		ODEMatrix[4]=matrix.m[1];
		ODEMatrix[8]=matrix.m[2];
		ODEMatrix[1]=matrix.m[4];
		ODEMatrix[5]=matrix.m[5];
		ODEMatrix[9]=matrix.m[6];
		ODEMatrix[2]=matrix.m[8];
		ODEMatrix[6]=matrix.m[9];
		ODEMatrix[10]=matrix.m[10];
		dBodySetRotation(body->GetODEBody(), ODEMatrix);

		force *= 20.0f/deltaTime;
		force = matrix.Rotate(force);
		dBodyAddForce(body->GetODEBody(), force.x(), force.y(), force.z());

		dMass m;
		const float* vel;

		//force = - drag_coefficient * mass * velocity
		dBodyGetMass(body->GetODEBody(), &m);
		vel = dBodyGetLinearVel(body->GetODEBody());
		dBodyAddForce(body->GetODEBody(), -vel[0]*m.mass/deltaTime, -vel[1]*m.mass/deltaTime, -vel[2]*m.mass/deltaTime);
	}
}
