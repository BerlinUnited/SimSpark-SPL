#include "planecollider.h"

using namespace boost;
using namespace kerosin;

FUNCTION(setParams)
{
	if (in.size() == 4)
	{
		PlaneCollider *pc = static_cast<PlaneCollider*>(obj);
		pc->SetParams(any_cast<float>(in[0]), any_cast<float>(in[1]), any_cast<float>(in[2]), any_cast<float>(in[3]));
	}
}

void CLASS(PlaneCollider)::DefineClass()
{
	DEFINE_BASECLASS(kerosin/Collider);
	DEFINE_FUNCTION(setParams);
}