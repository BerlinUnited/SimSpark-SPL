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

//! font to display name entered
shared_ptr<Font>	gFont;

/*!
	The main initialization function. It performs the following tasks:
		- Register application specific classes
		- Setup the object hierarchy by running a few init scripts
		- Load font
		- Check if all necessary OpenGL extensions are supported

	returns true on success
*/
static bool init()
{
	// register all the application specific classes
	//gContext->GetCore()->RegisterClassObject(new CLASS(MD5Mesh), "/");

	// run initialization scripts
	shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(gContext->Get("/sys/server/script"));
	scriptServer->Run("script/init.rb");
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
	
	scriptServer->CreateVariable("Material.Diffuse",	"");
	scriptServer->CreateVariable("Material.Normal",		"");
	scriptServer->CreateVariable("Material.Specular",	"");
	
	scriptServer->Run("script/scenetest.rb");

	// load font
	shared_ptr<FontServer> fontServer = shared_static_cast<FontServer>(gContext->Get("/sys/server/font"));
	gFont = fontServer->GetFont("font/andalemo.ttf");

	return true;
}

/*!	Here we process the incoming input events.
*/
static float processInput()
{
	float deltaTime = 0.0f;

	// retrieve the input server
	shared_ptr<InputServer> inputServer = shared_static_cast<InputServer>(gContext->Get("/sys/server/input"));
	// retrieve the script server
	shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(gContext->Get("/sys/server/script"));

	shared_ptr<FPSController> fpsController = shared_static_cast<FPSController>(gContext->Get("/usr/scene1/camera0/_body/fps"));
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
			case gCmdTimer:
				deltaTime = (float) input.data.l/1000.0f;
				break;
			case gCmdMouseX:
				if (fpsController.get())
				{
					fpsController->AdjustHAngle(0.3f*(float)input.data.l);
				}
				break;
			case gCmdMouseY:
				if (fpsController.get())
				{
					fpsController->AdjustVAngle(0.3f*(float)input.data.l);
				}
				break;
			case gCmdMouseLeft:
				break;
			case gCmdLeft:
				if (fpsController.get())
				{
					fpsController->StrafeLeft(input.data.l!=0);
				}
				break;
			case gCmdRight:
				if (fpsController.get())
				{
					fpsController->StrafeRight(input.data.l!=0);
				}
				break;
			case gCmdForward:
				if (fpsController.get())
				{
					fpsController->Forward(input.data.l!=0);
				}
				break;
			case gCmdBackward:
				if (fpsController.get())
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
	static float t = 0.0f;

	shared_ptr<SceneServer> sceneServer = shared_static_cast<SceneServer>(gContext->Get("/sys/server/scene"));
	if (sceneServer.get() != NULL)
	{
		sceneServer->Render();
	}
}

float gTime = 0.0f;
int   gNumFrames = 0;
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

	gTime += deltaTime;
	gNumFrames++;
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

	// average FPS
	printf("Average FPS: %.2f\n", gNumFrames/gTime);

	return 0;
}
