#include "agentaspect.h"
#include "../physicsserver/body.h"

using namespace boost;
using namespace kerosin;
using namespace salt;

salt::Vector3f AgentAspect::GetVelocity() const
{
	shared_ptr<Body> body = shared_static_cast<Body>(make_shared(GetParent())->GetChildOfClass("Body"));
	if (body)
		return body->GetVelocity();
	else
		return Vector3f(0,0,0);
}

void AgentAspect::PrePhysicsUpdateInternal(float deltaTime)
{
	Think(deltaTime);
}
