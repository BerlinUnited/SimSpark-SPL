#include "ccylindercollider.h"

using namespace boost;
using namespace kerosin;

FUNCTION(setParams)
{
        if (in.size() == 2)
        {
                CCylinderCollider *cc = static_cast<CCylinderCollider*>(obj);
                cc->SetParams(any_cast<float>(in[0]),any_cast<float>(in[1]));
        }
}

void CLASS(CCylinderCollider)::DefineClass()
{
        DEFINE_BASECLASS(kerosin/Collider);
        DEFINE_FUNCTION(setParams);
}
