#include "scene.h"

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;

Scene::Scene() :
BaseNode()
{
}

Scene::~Scene()
{
}

const salt::Matrix& Scene::GetWorldTransform() const
{
	return mIdentityMatrix;
}
