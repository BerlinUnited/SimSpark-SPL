#include "survivalagentaspect.h"

using namespace boost;

FUNCTION(setWanderingRate)
{
	if (in.size() == 1)
	{
		SurvivalAgentAspect *saa = static_cast<SurvivalAgentAspect*>(obj);
		saa->SetWanderingRate(any_cast<float>(in[0]));
	}
}

FUNCTION(setWanderingStrength)
{
	if (in.size() == 1)
	{
		SurvivalAgentAspect *saa = static_cast<SurvivalAgentAspect*>(obj);
		saa->SetWanderingStrength(any_cast<float>(in[0]));
	}
}

void CLASS(SurvivalAgentAspect)::DefineClass()
{
	DEFINE_BASECLASS(kerosin/AgentAspect);
	DEFINE_FUNCTION(setWanderingRate);
	DEFINE_FUNCTION(setWanderingStrength);
}
