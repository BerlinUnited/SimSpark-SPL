#include "controlaspect.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

void ControlAspect::PerformKills()
{
	while (mKillList.size())
	{
		// get the transform the agent to kill was attached to and unlink it
		mKillList.back()->Unlink();
		// lose our reference to it ... bye, bye
		mKillList.pop_back();
	}
}

shared_ptr<Effector> ControlAspect::RequestEffector(shared_ptr<AgentAspect>& agent, const std::string& effectorName)
{
	// try internal function to see if we allow the creation of the effector
	shared_ptr<Effector> effector = RequestEffectorInternal(agent, effectorName);

	if (effector)
	{
		// we were allowed to create the effector, now we attach it to the agent aspect
		agent->AddChildReference(effector);
	}
	else
	{
		printf("EKill\n");
		// the agent tried to create an effector, which is illegal, we will remember its
		// name, so we can unlink it (which destroys it!)
		mKillList.push_back(make_shared(agent->GetParent()));
	}

	return effector;
}

shared_ptr<Perceptor> ControlAspect::RequestPerceptor(shared_ptr<AgentAspect>& agent, const std::string& perceptorName)
{
	// try internal function to see if we allow the creation of the effector
	shared_ptr<Perceptor> perceptor = RequestPerceptorInternal(agent, perceptorName);

	if (perceptor)
	{
		// we were allowed to create the effector, now we attach it to the agent aspect
		agent->AddChildReference(perceptor);
	}
	else
	{
		// the agent tried to create an effector, which is illegal, we will remember its
		// name, so we can unlink it (which destroys it!)
		mKillList.push_back(make_shared(agent->GetParent()));
	}

	return perceptor;
}

shared_ptr<Effector> ControlAspect::CreateEffector(const std::string& effectorName)
{
	// try to create an instance
	shared_ptr<Object> instance = GetCore()->New(effectorName);

	// check if we got a valid instance
	if (instance && instance->GetClass()->Supports("Effector"))
	{
		return shared_static_cast<Effector>(instance);
	}
	else
	{
		return shared_ptr<Effector>();
	}
}

shared_ptr<Perceptor> ControlAspect::CreatePerceptor(const std::string& perceptorName)
{
	// try to create an instance
	shared_ptr<Object> instance = GetCore()->New(perceptorName);

	// check if we got a valid instance
	if (instance && instance->GetClass()->Supports("Perceptor"))
	{
		return shared_static_cast<Perceptor>(instance);
	}
	else
	{
		return shared_ptr<Perceptor>();
	}
}
