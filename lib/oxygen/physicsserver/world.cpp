#include "world.h"

using namespace kerosin;

World::World() :
mODEWorld(0)
{
}

World::~World()
{
	//printf("~World '%s'\n", GetClass()->GetName().c_str());

	// release ode world
	if (mODEWorld)
	{
		dWorldDestroy(mODEWorld);
		mODEWorld = 0;
	}
}

dWorldID World::GetODEWorld() const
{
	return mODEWorld;
}

void World::SetGravity(float x, float y, float z)
{
	dWorldSetGravity(mODEWorld, x, y, z);
}

void World::SetGravity(const salt::Vector3f &v)
{
	SetGravity(v.x(), v.y(), v.z());
}

void World::SetERP(float erp)
{
	dWorldSetERP(mODEWorld, erp);
}

float World::GetERP() const
{
	return dWorldGetERP(mODEWorld);
}

void World::SetCFM(float cfm)
{
	dWorldSetCFM(mODEWorld, cfm);
}

float World::GetCFM() const
{
	return dWorldGetCFM(mODEWorld);
}

void World::Step(float deltaTime)
{
	dWorldStep(mODEWorld, deltaTime);
}

bool World::ConstructInternal()
{
	if (!ODEObject::ConstructInternal()) return false;

	// create an ode world
	mODEWorld = dWorldCreate();

	return (mODEWorld != 0);
}
