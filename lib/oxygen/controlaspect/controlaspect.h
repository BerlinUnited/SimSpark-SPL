#ifndef CONTROLASPECT_H__
#define CONTROLASPECT_H__

#include "../sceneserver/basenode.h"
#include "../agentaspect/effector.h"
#include "../agentaspect/perceptor.h"
#include "../agentaspect/agentaspect.h"

namespace kerosin
{

class ControlAspect : public BaseNode
{
public:
	//! called by scene server, before the world is updated
	void	PerformKills();

	//! function used by agents to request effectors
	boost::shared_ptr<Effector> RequestEffector(boost::shared_ptr<AgentAspect>& agent, const std::string& effectorName);
	//! function used by agents to request perceptors
	boost::shared_ptr<Perceptor> RequestPerceptor(boost::shared_ptr<AgentAspect>& agent, const std::string& perceptorName);
protected:
	//! function used by agents to request effectors (this must be implemented to provide custom behavior)
	virtual boost::shared_ptr<Effector> RequestEffectorInternal(boost::shared_ptr<AgentAspect>& agent, const std::string& effectorName) = 0;
	//! function used by agents to request perceptors (this must be implemented to provide custom behavior)
	virtual boost::shared_ptr<Perceptor> RequestPerceptorInternal(boost::shared_ptr<AgentAspect>& agent, const std::string& perceptorName) = 0;

	//! creates an instance of an effector class (used in RequestEffector)
	boost::shared_ptr<Effector> CreateEffector(const std::string& effectorName);
	//! creates an instance of a perceptor class (used in RequestPerceptor)
	boost::shared_ptr<Perceptor> CreatePerceptor(const std::string& perceptorName);

private:
	// this list holds references to agents, which need to be killed in the next frame
	TLeafList	mKillList;
};

DECLARE_ABSTRACTCLASS(ControlAspect);

} // namespace kerosin

#endif //CONTROLASPECT_H__
