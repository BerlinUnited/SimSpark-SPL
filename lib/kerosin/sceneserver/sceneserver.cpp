#include "sceneserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/corecontext.h>
#include <salt/vector.h>
#include <salt/frustum.h>
#include "camera.h"
#include "scene.h"
#include "../physicsserver/world.h"
#include "../physicsserver/space.h"
#include "../openglserver/openglserver.h"
#include "../controlaspect/controlaspect.h"
#include "light.h"

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace zeitgeist;

SceneServer::SceneServer() :
Leaf()
{
}

SceneServer::~SceneServer()
{
}

boost::shared_ptr<Scene> SceneServer::CreateScene(const std::string &location)
{
	shared_ptr<CoreContext> context = GetCore()->CreateContext();
	shared_ptr<Scene> scene = shared_static_cast<Scene>(context->New("kerosin/Scene", location));

	if (scene.get() != NULL)
	{
		mActiveScene = scene;
	}

	return scene;
}

bool SceneServer::SetActiveScene(const std::string &location)
{
	shared_ptr<Scene> scene = shared_static_cast<Scene>(GetCore()->Get(location));

	if (scene.get() != NULL)
	{
		mActiveScene = scene;
		return true;
	}

	return false;
}

/*!
	Update the scene graph. I do not make the distinction between 'controllers'
	and 'scene graph nodes' ... this forces me to perform several update passes
	through the hierarchy. Nevertheless, the more homogenuous structure is worth
	it.

	Example Hierarchy:

	scene
		|-   transform
					|-   body
					|		|-  quakecontroller
					|
					|-   camera
	
	The following passes are performed:

	1st pass:	PrePhysicsUpdate(deltaTime)
		The first update pass is responsible for updating the internal state of
		an object. For example, the quakecontroller has received input from
		the main application in terms of state changes (StrafeLeft, Forward, etc..).
		As the controller is physics driven, it has to translate these commands
		to actual forces, which act on the body. This is done during this pass.

	[Physics + Collision called]

	2nd pass:	PostPhysicsUpdate()
		This pass can be used to affect other nodes. For example, 'body' has
		undergone physics simulation and collision detection. It has reached its
		final 'simulated' position, which has to be	passed to the parent (transform).

	3rd pass:	UpdateHierarchy()
		As the second pass affects the position of objects, we have to recalculate
		the transformation hierarchy before we can display the scene.
*/

void SceneServer::Update(float deltaTime)
{
	if (deltaTime > 0.0f && mActiveScene)
	{
		// update control aspects
		TLeafList controlAspects;
		TLeafList::iterator i;

		mActiveScene->GetChildrenSupportingClass("ControlAspect", controlAspects);

		for (i = controlAspects.begin(); i != controlAspects.end(); ++i)
		{
			shared_static_cast<ControlAspect>(*i)->PerformKills();
		}


		mActiveScene->PrePhysicsUpdate(deltaTime);

		// determine collisions
		shared_ptr<Space> space = shared_static_cast<Space>(mActiveScene->GetChildOfClass("Space"));
		if (space.get() != NULL)
		{
			space->Collide();
		}

		// do physics
		shared_ptr<World> world = shared_static_cast<World>(mActiveScene->GetChildOfClass("World"));
		if (world.get() != NULL)
		{
			world->Step(deltaTime);
		}

		mActiveScene->PostPhysicsUpdate();

		mActiveScene->UpdateHierarchy();
	}
}

void SceneServer::Render()
{
	if (mActiveScene.get() != NULL)
	{
		// get a camera to render with
		shared_ptr<Camera> camera = shared_static_cast<Camera>(mActiveScene->GetChildOfClass("Camera", true));
		// get opengl server
		shared_ptr<OpenGLServer> openglServer = shared_static_cast<OpenGLServer>(GetCore()->Get("/sys/server/opengl"));

		if (camera.get() != NULL && openglServer.get() != NULL)
		{
			glClearColor(0,0,0,0);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glColor3f(1,1,1);

			// set the view transformation
			camera->Bind();
			Frustum frustum;
			camera->DescribeFrustum(frustum);

			// cull lights and geometries against the frustum
			TLeafList myLights;
			TLeafList allMeshes;
			TLeafList visibleMeshes;
			TLeafList::iterator i;

			mActiveScene->GetChildrenSupportingClass("Light", myLights, true);
			mActiveScene->GetChildrenSupportingClass("StaticMesh", allMeshes, true);

			for (i = allMeshes.begin(); i != allMeshes.end(); ++i)
			{
				// try to cull meshes, which are outside the viewing frustum
				if (frustum.Intersects(shared_static_cast<StaticMesh>(*i)->GetWorldBoundingBox())!=Frustum::FS_OUTSIDE)
				{
					visibleMeshes.push_back(*i);
				}
			}

			// actual rendering 

			// ambient pass
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);

			if (openglServer->SupportsFancyLighting())
			{
				glEnable(GL_VERTEX_PROGRAM_ARB);
				glBindProgramARB(GL_VERTEX_PROGRAM_ARB, mAmbientVP);

				glColor3f(0.1f,0.1f,0.1f);
				mActiveScene->RenderAmbient();

				// render lights
				glBlendFunc(GL_ONE, GL_ONE);
				glEnable(GL_BLEND);
				//glEnable(GL_ALPHA_TEST);
				//glAlphaFunc(GL_GREATER, 0.0f);
				glDepthMask(0);
				glDepthFunc(GL_EQUAL);

				for (TLeafList::iterator i=myLights.begin(); i != myLights.end(); ++i)
				{
					shared_ptr<Light> light = shared_static_cast<Light>(*i);

					// only render the light if it is visible
					if (frustum.Intersects(light->GetWorldBoundingBox())!=Frustum::FS_OUTSIDE)
					{
						for (TLeafList::iterator j=visibleMeshes.begin(); j != visibleMeshes.end(); ++j)
						{
							shared_ptr<StaticMesh> mesh = shared_static_cast<StaticMesh>(*j);

							// we only have to render meshes, whose bounding volume intersects the light volume
							if (light->GetWorldBoundingBox().Intersects(mesh->GetWorldBoundingBox()))
							{

								Matrix   toObjectSpace;
								toObjectSpace.Identity();
								toObjectSpace = mesh->GetWorldTransform();
								toObjectSpace.InvertRotationMatrix();
								//light->GetWorldTransform().Pos().Dump();
								light->Prepare();
								Vector3f lightPos = toObjectSpace.Transform(light->GetWorldTransform().Pos());
								Vector3f viewPos = toObjectSpace.Transform(camera->GetWorldTransform().Pos());
								glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 0, lightPos.x(), lightPos.y(), lightPos.z(), 1.0f);
								glProgramLocalParameter4fARB(GL_VERTEX_PROGRAM_ARB, 1, viewPos.x(), viewPos.y(), viewPos.z(), 1.0f);
								light->RenderLitMesh(shared_static_cast<StaticMesh>(*j));
							}
						}
					}
				}

				glDisable(GL_BLEND);
				glDepthMask(1);

				glActiveTextureARB(GL_TEXTURE0_ARB);
				glDisable(GL_TEXTURE_2D);
				glActiveTextureARB(GL_TEXTURE1_ARB);
				glDisable(GL_TEXTURE_2D);
				glActiveTextureARB(GL_TEXTURE2_ARB);
				glDisable(GL_TEXTURE_2D);
			}
			else
			{
				// ambient lighting only
				glColor3f(1, 1, 1);
				mActiveScene->RenderAmbient();
			}

			if (openglServer->SupportsFancyLighting())
			{
				glEnable(GL_VERTEX_PROGRAM_ARB);
				glBindProgramARB(GL_VERTEX_PROGRAM_ARB, mAmbientVP);
			}
			// standard rendering
			mActiveScene->Render();
			if (openglServer->SupportsFancyLighting())
			{
				glDisable(GL_VERTEX_PROGRAM_ARB);
			}
		}
	}
}

bool SceneServer::ConstructInternal()
{
	shared_ptr<OpenGLServer> openglServer = shared_static_cast<OpenGLServer>(GetCore()->Get("/sys/server/opengl"));

	mAmbientVP = openglServer->LoadARBVertexProgram("sys/program/ambient.vp");
	if (mAmbientVP == 0)
	{
		GetLog()->Error() << "ERROR: Could not load vertex program..." << std::endl;
	}

	return true;
}