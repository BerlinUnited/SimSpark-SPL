#include "boxcollider.h"

using namespace boost;
using namespace kerosin;

FUNCTION(setBoxLengths)
{
        if (in.size() == 3)
        {
                BoxCollider *bc = static_cast<BoxCollider*>(obj);
                bc->SetBoxLengths(any_cast<float>(in[0]),any_cast<float>(in[1]),any_cast<float>(in[2]));
        }
}

void CLASS(BoxCollider)::DefineClass()
{
        DEFINE_BASECLASS(kerosin/Collider);
        DEFINE_FUNCTION(setBoxLengths);
}
