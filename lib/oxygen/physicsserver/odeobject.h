#ifndef ODEOBJECT_H__
#define ODEOBJECT_H__

#include "../sceneserver/basenode.h"
#include <ode/ode.h>

namespace kerosin
{

class PhysicsServer;

class ODEObject : public BaseNode
{
	//
	// Functions
	//
protected:
	virtual bool ConstructInternal();

	//
	// Members
	//
protected:
	boost::shared_ptr<PhysicsServer>	mPhysicsServer;
};

DECLARE_CLASS(ODEObject);

} //namespace kerosin

#endif //ODEOBJECT_H__

