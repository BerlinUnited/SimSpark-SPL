#include "md5mesh.h"

using namespace boost;

FUNCTION(load)
{
        if (in.size() == 1)
        {
                MD5Mesh *m = static_cast<MD5Mesh*>(obj);
                m->Load(any_cast<char*>(in[0]));
        }
}

FUNCTION(loadAnimation)
{
        if (in.size() == 1)
        {
                MD5Mesh *m = static_cast<MD5Mesh*>(obj);
                m->LoadAnimation(any_cast<char*>(in[0]));
        }
}

void CLASS(MD5Mesh)::DefineClass()
{
        DEFINE_BASECLASS(kerosin/BaseNode);
        DEFINE_FUNCTION(load);
        DEFINE_FUNCTION(loadAnimation);
}
