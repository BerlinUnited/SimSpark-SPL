#include "odeobject.h"
#include "physicsserver.h"

using namespace boost;
using namespace kerosin;

bool ODEObject::ConstructInternal()
{
	if (BaseNode::ConstructInternal() == false) return false;
	
	mPhysicsServer = shared_static_cast<PhysicsServer>(GetCore()->Get("/sys/server/physics"));

	if (mPhysicsServer.get() == NULL)
	{
		return false;
	}

	return true;
}