#ifndef BODY_H__
#define BODY_H__

#include "odeobject.h"

namespace kerosin
{

class World;

class Body : public ODEObject
{
	//
	// Functions
	//
public:
	Body();
	virtual ~Body();

	dBodyID GetODEBody() const;

	// simulation
	void Enable();
	void Disable();
	bool IsEnabled() const;
	
	void UseGravity(bool f);
	bool UsesGravity() const;

	// mass and density distribution
	void SetMass(float mass);
	void SetSphere(float density, float radius);

	// velocity
	void SetMaxSpeed(float speed)	{	mMaxSpeed = speed;	}
	salt::Vector3f GetVelocity() const;

protected:
	virtual void OnLink();
	virtual void OnUnlink();

private:
	//! update internal state after physics calculation
	virtual void PostPhysicsUpdateInternal();
	
	//
	// Members
	//
protected:
	//! the world this body is associated with
	boost::shared_ptr<World>	mWorld;
	//! the ode collision geometry
	dBodyID						mODEBody;	
	//! linear drag (default 0.3)
	float						mLinearDrag;
	//! maximum velocity, this body can attain
	float						mMaxSpeed;
	//! maximum force which can be applied to this body
	float						mMaxForce;
};

DECLARE_CLASS(Body);

} //namespace kerosin

#endif //BODY_H__
