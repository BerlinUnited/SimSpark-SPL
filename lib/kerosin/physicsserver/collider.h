#ifndef COLLIDER_H__
#define COLLIDER_H__

/*!	\class Collider

	Base class of all colliders. It has a function to receive the space the
	object is associated with.
*/

#include "odeobject.h"

namespace kerosin
{

class Space;
class World;

class Collider : public ODEObject
{
	//
	// Functions
	//
public:
	Collider();
	virtual ~Collider();

protected:
	virtual void OnLink();
	virtual void OnUnlink();

	//
	// Members
	//
protected:
	//! the world this collider is associated with
	boost::shared_ptr<World>	mWorld;
	//! the space this collider is associated with
	boost::shared_ptr<Space>	mSpace;
	//! the ode collision geometry
	dGeomID						mODEGeom;	
};

DECLARE_CLASS(Collider);

} //namespace kerosin

#endif //COLLIDER_H__
