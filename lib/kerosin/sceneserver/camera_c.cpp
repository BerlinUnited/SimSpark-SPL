#include "camera.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

FUNCTION(setViewport)
{
	if (in.size() == 4)
	{
		Camera *cam = static_cast<Camera*>(obj);
		cam->SetViewport(any_cast<int>(in[0]), any_cast<int>(in[1]), any_cast<int>(in[2]), any_cast<int>(in[3]));
	}
}

FUNCTION(setFOV)
{
	if (in.size() == 1)
	{
		Camera *cam = static_cast<Camera*>(obj);
		cam->SetFOV(any_cast<float>(in[0]));
	}
}

FUNCTION(setZNear)
{
	if (in.size() == 1)
	{
		Camera *cam = static_cast<Camera*>(obj);
		cam->SetZNear(any_cast<float>(in[0]));
	}
}

FUNCTION(setZFar)
{
	if (in.size() == 1)
	{
		Camera *cam = static_cast<Camera*>(obj);
		cam->SetZFar(any_cast<float>(in[0]));
	}
}

void CLASS(Camera)::DefineClass()
{
	DEFINE_BASECLASS(kerosin/BaseNode);
	DEFINE_FUNCTION(setViewport);
	DEFINE_FUNCTION(setFOV);
	DEFINE_FUNCTION(setZNear);
	DEFINE_FUNCTION(setZFar);
}