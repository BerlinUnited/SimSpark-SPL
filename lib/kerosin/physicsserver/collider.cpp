#include "collider.h"
#include "../sceneserver/scene.h"
#include "space.h"
#include "body.h"
#include "world.h"

using namespace boost;
using namespace kerosin;

Collider::Collider() :
ODEObject(), mODEGeom(0)
{
}

Collider::~Collider()
{
	//printf("~Collider '%s'\n", GetClass()->GetName().c_str());

	if (mODEGeom)
	{
		dGeomDestroy(mODEGeom);
		mODEGeom = 0;
	}
}

void Collider::OnLink()
{
	ODEObject::OnLink();

	shared_ptr<Scene> scene = GetScene();
	
	if (scene.get() != NULL)
	{
		mWorld = shared_static_cast<World>(scene->GetChildOfClass("World"));
		mSpace = shared_static_cast<Space>(scene->GetChildOfClass("Space"));
		dSpaceID space = mSpace->GetODESpace();

		// if we have a space and an object, add it to the space
		if (mODEGeom && space && !dSpaceQuery(space, mODEGeom))
		{
			dSpaceAdd(space, mODEGeom);
		}
		
		// if there is a Body hanging on our parent, link to it
		shared_ptr<Body> body = shared_static_cast<Body>(make_shared(GetParent())->GetChildOfClass("Body"));
		if (body.get() != NULL)
		{
			dGeomSetBody (mODEGeom, body->GetODEBody());
		}
	}
}

void Collider::OnUnlink()
{
	ODEObject::OnUnlink();

	dSpaceID space = mSpace->GetODESpace();

	// remove collision geometry from space
	if (mODEGeom && space && dSpaceQuery(space, mODEGeom))
	{
		dSpaceRemove(space, mODEGeom);
	}

	mSpace.reset();
}
