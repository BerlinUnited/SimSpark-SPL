#ifndef SPACE_H__
#define SPACE_H__

#include "odeobject.h"

namespace kerosin
{

class Space : public ODEObject
{
	//
	// Functions
	//
public:
	Space();
	~Space();

	dSpaceID	GetODESpace() const;

	//! do collision
	void Collide();
	
	//! collide callback
	virtual void HandleCollide(dGeomID obj1, dGeomID obj2);

protected:
	virtual bool ConstructInternal();
	virtual void OnLink();
	virtual void OnUnlink();

private:
	virtual void PrePhysicsUpdateInternal(float deltaTime);

	//
	// Members
	//
private:
	//! the space that colliders live in
	dSpaceID		mODESpace;
	//! the container for all collision contacts
	dJointGroupID	mODEContactGroup;
	//! cached ODE world for the contacts ... always updated OnLink
	dWorldID		mWorld;
};

DECLARE_CLASS(Space);

} //namespace kerosin

#endif SPACE_H__
