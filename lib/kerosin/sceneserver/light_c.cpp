#include "light.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(setRadius)
{
        if (in.size() == 1)
        {
                Light *l = static_cast<Light*>(obj);
                l->SetRadius(any_cast<float>(in[0]));
        }
}

FUNCTION(setDiffuseColor)
{
        if (in.size() == 3)
        {
                Light *l = static_cast<Light*>(obj);
                l->SetDiffuseColor(any_cast<float>(in[0]), any_cast<float>(in[1]), any_cast<float>(in[2]));
        }
}

void CLASS(Light)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/BaseNode);
        DEFINE_FUNCTION(setRadius);
        DEFINE_FUNCTION(setDiffuseColor);
}
