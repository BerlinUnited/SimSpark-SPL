#ifndef FORCEEFFECTOR_H__
#define FORCEEFFECTOR_H__

#include "effector.h"

namespace kerosin
{

class ForceEffector : public Effector
{
	//
	// functions
	//
public:
	ForceEffector();

	//! apply the force to the physics aspect attached to base, true on success, false if no body was found
	virtual bool Perform(boost::shared_ptr<BaseNode> &base, float deltaTime);

	//! accumulates a force to be applied to a physics aspect
	void AddForce(const salt::Vector3f& force);

	void SetMaxForce(float maxForce)	{	mMaxForce = maxForce;	}
	//
	// members
	//
private:
	// the accumulated force this effector applies
	salt::Vector3f		mForce;
	//! maximum amount of force, this effector can apply
	float				mMaxForce;
};

DECLARE_CLASS(ForceEffector);

} // namespace kerosin

#endif //FORCEEFFECTOR_H__
