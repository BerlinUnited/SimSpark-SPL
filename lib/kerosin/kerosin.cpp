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

	zg.GetCore()->RegisterClassObject(new CLASS(TextureServer), "kerosin/");

	zg.GetCore()->RegisterClassObject(new CLASS(MaterialServer), "kerosin/");

	// scene graph
	zg.GetCore()->RegisterClassObject(new CLASS(SceneServer), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Axis), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(BaseNode), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Camera), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(FPSController), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Light), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Scene), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Sphere), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(StaticMesh), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Transform), "kerosin/");

	// physics
	zg.GetCore()->RegisterClassObject(new CLASS(PhysicsServer), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Body), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(ODEObject), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Space), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(World), "kerosin/");
	// collider
	zg.GetCore()->RegisterClassObject(new CLASS(Collider), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(PlaneCollider), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(SphereCollider), "kerosin/");

	// agentaspect
	zg.GetCore()->RegisterClassObject(new CLASS(AgentAspect), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Effector), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(ForceEffector), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(Perceptor), "kerosin/");
	zg.GetCore()->RegisterClassObject(new CLASS(VisionPerceptor), "kerosin/");

	// controlaspect
	zg.GetCore()->RegisterClassObject(new CLASS(ControlAspect), "kerosin/");

	// load default setting
	zg.GetCore()->GetRoot()->GetScript()->Run("sys/script/default.rb");
}
