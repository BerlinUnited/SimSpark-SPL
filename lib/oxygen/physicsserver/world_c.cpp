#include "world.h"

using namespace boost;
using namespace kerosin;

FUNCTION(setGravity)
{
	if (in.size() == 3)
	{
		World *w = static_cast<World*>(obj);
		w->SetGravity(any_cast<float>(in[0]), any_cast<float>(in[1]), any_cast<float>(in[2]));
	}
}

FUNCTION(setERP)
{
	if (in.size() == 1)
	{
		World *w = static_cast<World*>(obj);
		w->SetERP(any_cast<float>(in[0]));
	}
}

FUNCTION(setCFM)
{
	if (in.size() == 1)
	{
		World *w = static_cast<World*>(obj);
		w->SetCFM(any_cast<float>(in[0]));
	}
}

FUNCTION(step)
{
	if (in.size() == 1)
	{
		World *w = static_cast<World*>(obj);
		w->Step(any_cast<float>(in[0]));
	}
}

void CLASS(World)::DefineClass()
{
	DEFINE_BASECLASS(kerosin/ODEObject);
	DEFINE_FUNCTION(setGravity);
	DEFINE_FUNCTION(setERP);
	DEFINE_FUNCTION(setCFM);
	DEFINE_FUNCTION(step);
}
