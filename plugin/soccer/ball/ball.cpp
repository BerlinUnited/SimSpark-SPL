#include <oxygen/physicsserver/body.h>
#include "ball.h"

using namespace boost;
using namespace oxygen;

Ball::Ball() : Transform(), mForceTTL(0)
{
}

void
Ball::SetAcceleration(int steps, const salt::Vector3f& force,
                      const salt::Vector3f& torque)
{
    mForceTTL = steps;
    mGamma = mInc = 1.0/steps;

    mForce = force;
    mTorque = torque;

    if (mBody.get() == 0)
    {
        mBody = shared_dynamic_cast<Body>(GetChildOfClass("Body"));
    }
}

void
Ball::PrePhysicsUpdateInternal(float deltaTime)
{
    Transform::PrePhysicsUpdateInternal(deltaTime);
    if (mBody.get() == 0 || mForceTTL <= 0) return;

    mBody->AddForce(mForce * mGamma);
    mBody->AddTorque(mTorque * mGamma);

    --mForceTTL;
    if (mGamma < 1.0) mGamma += mInc;
}
