#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>
#include <SDL/SDL.h>
#include <zeitgeist/fileserver/fileserver.h>

//#include "md5mesh.h"

using namespace boost;
using namespace kerosin;
using namespace salt;
using namespace std;
using namespace zeitgeist;

//! the Zeitgeist context we are using
shared_ptr<CoreContext>	gContext;

//
// Input constant mappings
//
const static int	gCmdQuit	= 0x01;
const static int	gCmdTimer	= 0x02;

// mouse controls
const static int	gCmdMouseX		= 0x10;
const static int	gCmdMouseY		= 0x11;
const static int	gCmdMouseLeft	= 0x12;
const static int	gCmdLeft		= 0x20;
const static int	gCmdRight		= 0x22;
const static int	gCmdForward		= 0x24;
const static int	gCmdBackward	= 0x26;

// lighting
const static int	gCmdLighting	= 0x30;


/*!
	The main initialization function. It performs the following tasks:
		- Register application specific classes
		- Setup the object hierarchy by running a few init scripts

	returns true on success
*/
static bool init()
{
	// run initialization scripts
	shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(gContext->Get("/sys/server/script"));

	if (scriptServer.get() != NULL)
	{
		// the following script loads the plugins used by the simulator and sets
		// up the file server
		scriptServer->Run("preinit.rb");

		// publish our commands to the scripts
		scriptServer->CreateVariable("Command.Quit",		gCmdQuit);
		scriptServer->CreateVariable("Command.Timer",		gCmdTimer);
		scriptServer->CreateVariable("Command.MouseX",		gCmdMouseX);
		scriptServer->CreateVariable("Command.MouseY",		gCmdMouseY);
		scriptServer->CreateVariable("Command.MouseLeft",	gCmdMouseLeft);
		scriptServer->CreateVariable("Command.Left",		gCmdLeft);
		scriptServer->CreateVariable("Command.Right",		gCmdRight);
		scriptServer->CreateVariable("Command.Forward",		gCmdForward);
		scriptServer->CreateVariable("Command.Backward",	gCmdBackward);
		scriptServer->CreateVariable("Command.Lighting",	gCmdLighting);

		// the next script will create all the necessary servers
		scriptServer->Run("script/init.rb");

		printf("initialized...\n");
		// loads the simulation
		scriptServer->Run("survival/survival.rb");
	}

	return true;
}

/*!	Here we process the incoming input events.
*/
static float processInput()
{
	float deltaTime = 0.0f;

	// retrieve the input server
	shared_ptr<InputServer> inputServer = shared_static_cast<InputServer>(gContext->Get("/sys/server/input"));

	if (!inputServer)
	{
		// we don't have an input server
		return 0.0f;
	}
	
	// get the controller, which we will reroute our input to
	shared_ptr<FPSController> fpsController = shared_static_cast<FPSController>(gContext->GetCore()->GetRoot()->GetChildOfClass("FPSController", true));
    // Process incoming input
	InputServer::Input input;
	while (inputServer->GetInput(input))
	{
		switch (input.id)
		{
			case gCmdQuit:
				{
					shared_ptr<OpenGLServer> openglServer = shared_static_cast<OpenGLServer>(gContext->Get("/sys/server/opengl"));
					openglServer->Quit();
				}
				break;
			case gCmdLighting:
				{
					shared_ptr<OpenGLServer> openglServer = shared_static_cast<OpenGLServer>(gContext->Get("/sys/server/opengl"));
					openglServer->ToggleFancyLighting();
				}
				break;
			case gCmdTimer:
				deltaTime = (float) input.data.l/1000.0f;
				break;
			case gCmdMouseX:
				if (fpsController)
				{
					fpsController->AdjustHAngle(0.3f*(float)input.data.l);
				}
				break;
			case gCmdMouseY:
				if (fpsController)
				{
					fpsController->AdjustVAngle(0.3f*(float)input.data.l);
				}
				break;
			case gCmdMouseLeft:
				break;
			case gCmdLeft:
				if (fpsController)
				{
					fpsController->StrafeLeft(input.data.l!=0);
				}
				break;
			case gCmdRight:
				if (fpsController)
				{
					fpsController->StrafeRight(input.data.l!=0);
				}
				break;
			case gCmdForward:
				if (fpsController)
				{
					fpsController->Forward(input.data.l!=0);
				}
				break;
			case gCmdBackward:
				if (fpsController)
				{
					fpsController->Backward(input.data.l!=0);
				}
				break;
		}
	}

	return deltaTime;
}

/*!	Draw the scene
*/
static void renderFrame(float deltaTime)
{
	shared_ptr<SceneServer> sceneServer = shared_static_cast<SceneServer>(gContext->Get("/sys/server/scene"));
	if (sceneServer)
	{
		sceneServer->Render();
	}
}

/*!	Update 

	Processes input, updates windowserver, current score, entities, etc..
*/
static void update()
{
	// process the input events, which have occured
	float deltaTime = processInput();

	shared_ptr<SceneServer> sceneServer = shared_static_cast<SceneServer>(gContext->Get("/sys/server/scene"));
	if (sceneServer.get() != NULL)
	{
		sceneServer->Update(deltaTime);
	}
	renderFrame(deltaTime);
}

/*!	Standard issue main procedure. We initialize ourselves with a few scripts
	and then enter the run-loop.
*/
int main(int argc, char **argv)
{
	// initialize Zeitgeist (object hierarchy, scripting, files, plugins)
	Zeitgeist	myZeitgeist;
	// initialize Kerosin (input, windowing)
	Kerosin		myKerosin(myZeitgeist);
	// create our global context (object hierarchy position container)
	gContext = myZeitgeist.CreateContext();
	// application specific initialization
	if (init() == false)
	{
		gContext->GetCore()->GetLogServer()->Error() << "ERROR: Init failed..." << endl;
		return 1;
	}

	// retrieve shared ptr to the OpenGL Server ... this represents the OpenGL
	// context the application runs in, as well as the window
	shared_ptr<OpenGLServer> openglServer = shared_static_cast<OpenGLServer>(gContext->Get("/sys/server/opengl"));
	if (openglServer.get() == NULL)
	{
		gContext->GetCore()->GetLogServer()->Error() << "ERROR: Can't locate OpenGLServer ..." << endl;
		return 1;
	}

	// the runloop
	while(!openglServer->WantsToQuit())
	{
		// update the window (pumps event loop, etc..)
		openglServer->Update();
		// update all the other components
		update();

		openglServer->SwapBuffers();
	}

	// we have to make sure, the inputServer is shut down before the opengl server,
	// as the opengl server shuts down SDL ... this will conflict with the input
	// server
	shared_ptr<InputServer> inputServer = shared_static_cast<InputServer>(gContext->Get("/sys/server/input"));
	inputServer->Unlink();

	return 0;
}
