#ifndef WORLD_H__
#define WORLD_H__

#include "odeobject.h"

namespace kerosin
{

class World : public ODEObject
{
	//
	// Functions
	//
public:
	World();
	~World();

	dWorldID	GetODEWorld() const;

	// gravity
	void	SetGravity(float x, float y, float z);
	void	SetGravity(const salt::Vector3f &v);

	// simulation control
	void	SetERP(float erp);
	float	GetERP() const;
	void	SetCFM(float cfm);
	float	GetCFM() const;

	//! perform phyisics simulation
	void	Step(float deltaTime);
	
protected:
	//! creates ode dynamics world ... makes sure that mODEWorld is valid
	virtual bool ConstructInternal();

	//
	// Members
	//
private:
	//! the dynamics world represented by this object (usually attached to a scene node)
	dWorldID	mODEWorld;
};

DECLARE_CLASS(World);

};

#endif //WORLD_H__