#ifndef AGENTASPECT_H__
#define AGENTASPECT_H__

#include "../sceneserver/basenode.h"
#include "effector.h"
#include "perceptor.h"

namespace kerosin
{

class AgentAspect : public BaseNode
{
public:
	
	//! this method must be implemented for the agents 'think' behavior
	virtual void Think(float deltaTime) = 0;

protected:
	salt::Vector3f GetVelocity() const;
private:
	//! update internal state before physics calculation ... class Think()
	void PrePhysicsUpdateInternal(float deltaTime);
};

DECLARE_ABSTRACTCLASS(AgentAspect);

} // namespace kerosin

#endif //AGENTASPECT_H__
