#include "visionperceptor.h"
#include "../sceneserver/sceneserver.h"
#include "../sceneserver/scene.h"
#include "../sceneserver/staticmesh.h"
#include <salt/frustum.h>

using namespace boost;
using namespace kerosin;
using namespace salt;

bool VisionPerceptor::Percept(TDictionary &dictionary)
{
	// reset internal state
	TLeafList visibleObjects;

	shared_ptr<Scene> scene = shared_static_cast<Scene>(GetCore()->Get("/usr/scene/"));

	if (scene)
	{
		Matrix mat;
		mat.LookAt(GetWorldTransform().Pos(), mDir, Vector3f(0, 1, 0));
		Frustum frustum;
		frustum.Set(mat, 90.0f, 0.1f, 2000.0f);

		TLeafList allMeshes;
		TLeafList visibleMeshes;
		TLeafList::iterator i, j;

		scene->GetChildrenSupportingClass("StaticMesh", allMeshes, true);
		
		for (i = allMeshes.begin(); i != allMeshes.end(); ++i)
		{
			// try to cull meshes, which are outside the viewing frustum
			if (frustum.Intersects(shared_static_cast<StaticMesh>(*i)->GetWorldBoundingBox())!=Frustum::FS_OUTSIDE)
			{
				visibleMeshes.push_back(*i);
			}
		}

		// get visual aspect of the agent we belong to

		// perform the raycheck between our position and the 
		for (i = visibleMeshes.begin(); i != visibleMeshes.end(); ++i)
		{
			// (*i) is the target mesh

			bool isOccluded = false;
			// create on Opcode ray from our position to the target
			for (j = visibleMeshes.begin(); isOccluded == false && j != visibleMeshes.end(); ++j)
			{
				// (*j) are potential occluders

				// make sure (*j) is not the visual aspect of the same object
				// the agent aspect belongs to
				//if (ray intersects occluder)

				if (false)
				{
					printf("isOccluded");
					isOccluded = true;
				}
			}

			// if the target mesh is not occluded, we add it to the list of
			// visible objects
			if (!isOccluded)
			{
				visibleObjects.push_back(make_shared((*i)->GetParent()));
			}
		}
	}

	if (!visibleObjects.empty())
	{
		dictionary["visibleObjects"] = visibleObjects;
	}

	return (!visibleObjects.empty());
}
