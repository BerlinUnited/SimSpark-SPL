#ifndef SURVIVALCONTROLASPECT_H__
#define SURVIVALCONTROLASPECT_H__

#include <kerosin/controlaspect/controlaspect.h>
#include <kerosin/fontserver/font.h>
#include "survivalagentaspect.h"
#include <list>

class SurvivalControlAspect : public kerosin::ControlAspect
{
protected:
	//! function used by agents to request effectors
	boost::shared_ptr<kerosin::Effector> RequestEffectorInternal(boost::shared_ptr<kerosin::AgentAspect>& agent, const std::string& effectorName);
	//! function used by agents to request perceptors
	boost::shared_ptr<kerosin::Perceptor> RequestPerceptorInternal(boost::shared_ptr<kerosin::AgentAspect>& agent, const std::string& perceptorName);
private:
	//! update internal state before physics calculation
	virtual void PrePhysicsUpdateInternal(float deltaTime);

	//! init state of simulation
	virtual void OnLink();
	
	//! displays current health and score
	virtual void RenderInternal();

	//! register an agent to this control aspect
	bool RegisterAgent(boost::shared_ptr<kerosin::AgentAspect>& agent);

private:
	struct AgentControlState
	{
		boost::shared_ptr<SurvivalAgentAspect>	agent;

		float	healthInit;	// the amount of health this agent 
							// receives with each live

		float	health;		// the current health
		int		death;		// number of its deaths

		bool	hasFood;	// (internal) true, if the agent 
							// touched the food in the current cycle
	};

	typedef std::list<AgentControlState> ControlledAgentList;

private:
	boost::shared_ptr<kerosin::Font>	mFont;
	ControlledAgentList					mControlledAgents;

	//! rate at which health decreses (units/second)
	float	mSickRate;

	//! minimum distance for an agent to get the food
	float	mFoodMinDist;

	//! the health increase an agent gets if it touches the food.
	//! this value is divided among the agents touching the food
	//! in simulatinously in one cycle
	float	mFoodValue;
};

DECLARE_CLASS(SurvivalControlAspect);

#endif //SURVIVALCONTROLASPECT_H__
