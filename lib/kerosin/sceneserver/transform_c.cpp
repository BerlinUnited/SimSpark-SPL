#include "transform.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(setLocalPos)
{
        if (in.size() == 3)
        {
                Transform *trans = static_cast<Transform*>(obj);
                trans->SetLocalPos(any_cast<float>(in[0]), any_cast<float>(in[1]), any_cast<float>(in[2]));
        }
}

void CLASS(Transform)::DefineClass()
{
        DEFINE_BASECLASS(kerosin/BaseNode);
        DEFINE_FUNCTION(setLocalPos);
}
