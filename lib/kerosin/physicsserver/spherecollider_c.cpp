#include "spherecollider.h"

using namespace boost;
using namespace kerosin;

FUNCTION(setRadius)
{
	if (in.size() == 1)
	{
		SphereCollider *sc = static_cast<SphereCollider*>(obj);
		sc->SetRadius(any_cast<float>(in[0]));
	}
}

void CLASS(SphereCollider)::DefineClass()
{
	DEFINE_BASECLASS(kerosin/Collider);
	DEFINE_FUNCTION(setRadius);
}