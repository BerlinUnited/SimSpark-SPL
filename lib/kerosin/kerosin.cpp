#include "kerosin.h"
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace kerosin;
using namespace zeitgeist;

Kerosin::Kerosin(zeitgeist::Zeitgeist &zg)
{
        zg.GetCore()->RegisterClassObject(new CLASS(SoundServer), "kerosin/");

        zg.GetCore()->RegisterClassObject(new CLASS(InputServer), "kerosin/");

        zg.GetCore()->RegisterClassObject(new CLASS(ImageServer), "kerosin/");

        zg.GetCore()->RegisterClassObject(new CLASS(FontServer), "kerosin/");

        zg.GetCore()->RegisterClassObject(new CLASS(OpenGLServer), "kerosin/");

        zg.GetCore()->RegisterClassObject(new CLASS(RenderServer), "kerosin/");

        zg.GetCore()->RegisterClassObject(new CLASS(TextureServer), "kerosin/");

        zg.GetCore()->RegisterClassObject(new CLASS(MaterialServer), "kerosin/");

        // scene graph

        zg.GetCore()->RegisterClassObject(new CLASS(Axis), "kerosin/");
        zg.GetCore()->RegisterClassObject(new CLASS(Light), "kerosin/");
        zg.GetCore()->RegisterClassObject(new CLASS(StaticMesh), "kerosin/");
        zg.GetCore()->RegisterClassObject(new CLASS(Transform), "kerosin/");

        // load default setting
        zg.GetCore()->GetRoot()->GetScript()->Run("sys/script/default.rb");
}
