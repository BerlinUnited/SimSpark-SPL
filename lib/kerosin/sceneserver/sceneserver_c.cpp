#include "sceneserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(createScene)
{
	if (in.size() == 1)
	{
		SceneServer *ss = static_cast<SceneServer*>(obj);
		ss->CreateScene(any_cast<char*>(in[0]));
	}
}

FUNCTION(setActiveScene)
{
	if (in.size() == 1)
	{
		SceneServer *ss = static_cast<SceneServer*>(obj);
		ss->SetActiveScene(any_cast<char*>(in[0]));
	}
}

void CLASS(SceneServer)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Leaf);
	DEFINE_FUNCTION(createScene);
	DEFINE_FUNCTION(setActiveScene);
}