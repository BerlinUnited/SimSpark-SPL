#include "staticmesh.h"

using namespace boost;
using namespace kerosin;

FUNCTION(load)
{
        if (in.size() == 1)
        {
                StaticMesh *sm = static_cast<StaticMesh*>(obj);
                sm->Load(any_cast<char*>(in[0]));
        }
}

void CLASS(StaticMesh)::DefineClass()
{
        DEFINE_BASECLASS(oxygen/BaseNode);
        DEFINE_FUNCTION(load);
}
