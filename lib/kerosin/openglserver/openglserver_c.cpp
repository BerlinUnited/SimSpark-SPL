#include "openglserver.h"

using namespace std;
using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(OpenGLServer,init)
{
    string openGLSysName;

    return (
            (in.GetSize() == 1) &&
            (in.GetValue(in.begin(), openGLSysName)) &&
            (obj->Init(openGLSysName))
            );
}

void CLASS(OpenGLServer)::DefineClass()
{
  DEFINE_BASECLASS(zeitgeist/Leaf);
  DEFINE_FUNCTION(init);
}
