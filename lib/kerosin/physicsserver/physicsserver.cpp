#include "physicsserver.h"
#include <ode/ode.h>

using namespace kerosin;

PhysicsServer::PhysicsServer()
{
}

PhysicsServer::~PhysicsServer()
{
	// release memory associated with ode
	dCloseODE();
}