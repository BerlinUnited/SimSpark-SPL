#include "sphere.h"

using namespace boost;
using namespace kerosin;

FUNCTION(setRadius)
{
        if (in.size() == 1)
        {
                Sphere *s = static_cast<Sphere*>(obj);
                s->SetRadius(any_cast<float>(in[0]));
        }
}

void CLASS(Sphere)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/BaseNode);
        DEFINE_FUNCTION(setRadius);
}
