#include "forceeffector.h"
#include "../physicsserver/body.h"

using namespace boost;
using namespace kerosin;
using namespace salt;

ForceEffector::ForceEffector()
{
	mForce.Set(0, 0, 0);
	mMaxForce = 5.0f;
}

bool ForceEffector::Perform(boost::shared_ptr<BaseNode> &base, float deltaTime)
{
	if (!base) return false;

	// base should be a transform, or some other node, which has a Body-child
	shared_ptr<Body> body = shared_static_cast<Body>(base->GetChildOfClass("Body"));

	if (!body) return false;

	if (mForce.Length() > mMaxForce) mForce = mMaxForce/mForce.Length()*mForce;

	dBodyAddForce(body->GetODEBody(), mForce.x(), mForce.y(), mForce.z());

	mForce.Set(0,0,0);
	return true;
}

void ForceEffector::AddForce(const salt::Vector3f& force)
{
	mForce += force;
}
