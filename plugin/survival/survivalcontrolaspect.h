#ifndef SURVIVALCONTROLASPECT_H__
#define SURVIVALCONTROLASPECT_H__

#include <kerosin/controlaspect/controlaspect.h>
#include <kerosin/fontserver/font.h>

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

private:
	boost::shared_ptr<kerosin::Font>	mFont;

	//! simulation state
	float	mHealthSlow;
	int		mDeathSlow;
	float	mHealthFast;
	int		mDeathFast;

	//! rate at which health decreses (units/second)
	float	mSickRate;
};

DECLARE_CLASS(SurvivalControlAspect);

#endif //SURVIVALCONTROLASPECT_H__
