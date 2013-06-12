#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>
#include <oxygen/oxygen.h>
#include <zeitgeist/fileserver/fileserver.h>

using namespace boost;
using namespace kerosin;
using namespace oxygen;
using namespace salt;
using namespace std;
using namespace zeitgeist;

//! the Zeitgeist context we are using
boost::shared_ptr<CoreContext> gContext;

//
// Input constant mappings
//
static const int        gCmdQuit        = 0x01;
static const int        gCmdTimer       = 0x02;

// mouse controls
static const int        gCmdMouseX              = 0x10;
static const int        gCmdMouseY              = 0x11;
static const int        gCmdMouseLeft   = 0x12;
static const int        gCmdLeft                = 0x20;
static const int        gCmdRight               = 0x22;
static const int        gCmdForward             = 0x24;
static const int        gCmdBackward    = 0x26;

//! font to display name entered
// "Font" is ambiguously declared (also in X somewhere), so use namespace
boost::shared_ptr<kerosin::Font>        gFont;

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
        // run initialization scripts
        boost::shared_ptr<ScriptServer> scriptServer = static_pointer_cast<ScriptServer>(gContext->Get("/sys/server/script"));
        scriptServer->Run("script/init.rb");
        // publish our commands to the scripts
        scriptServer->CreateVariable("Command.Quit",            gCmdQuit);
        scriptServer->CreateVariable("Command.Timer",           gCmdTimer);
        scriptServer->CreateVariable("Command.MouseX",          gCmdMouseX);
        scriptServer->CreateVariable("Command.MouseY",          gCmdMouseY);
        scriptServer->CreateVariable("Command.MouseLeft",       gCmdMouseLeft);
        scriptServer->CreateVariable("Command.Left",            gCmdLeft);
        scriptServer->CreateVariable("Command.Right",           gCmdRight);
        scriptServer->CreateVariable("Command.Forward",         gCmdForward);
        scriptServer->CreateVariable("Command.Backward",        gCmdBackward);

        scriptServer->CreateVariable("Material.Diffuse",        "");
        scriptServer->CreateVariable("Material.Normal",         "");
        scriptServer->CreateVariable("Material.Specular",       "");

        scriptServer->Run("script/scenetest.rb");

        // load font
        boost::shared_ptr<FontServer> fontServer = static_pointer_cast<FontServer>(gContext->Get("/sys/server/font"));
        gFont = fontServer->GetFont("font/andalemo.ttf");

        return true;
}

/*!     Here we process the incoming input events.
*/
static float processInput()
{
        float deltaTime = 0.0f;

        // retrieve the input server
        boost::shared_ptr<InputServer> inputServer = static_pointer_cast<InputServer>(gContext->Get("/sys/server/input"));
        // retrieve the script server
        boost::shared_ptr<ScriptServer> scriptServer = static_pointer_cast<ScriptServer>(gContext->Get("/sys/server/script"));

        boost::shared_ptr<FPSController> fpsController = static_pointer_cast<FPSController>(gContext->Get("/usr/scene1/camera0/_body/fps"));
    // Process incoming input
        Input input;
        while (inputServer->GetInput(input))
        {
                switch (input.mId)
                {
                        case gCmdQuit:
                                {
                                        boost::shared_ptr<OpenGLServer> openglServer = static_pointer_cast<OpenGLServer>(gContext->Get("/sys/server/opengl"));
                                        openglServer->Quit();
                                }
                                break;
                        case gCmdTimer:
                                deltaTime = (float) input.mData.l/1000.0f;
                                break;
                        case gCmdMouseX:
                                if (fpsController.get())
                                {
                                        fpsController->AdjustHAngle(0.3f*(float)input.mData.l);
                                }
                                break;
                        case gCmdMouseY:
                                if (fpsController.get())
                                {
                                        fpsController->AdjustVAngle(0.3f*(float)input.mData.l);
                                }
                                break;
                        case gCmdMouseLeft:
                                break;
                        case gCmdLeft:
                                if (fpsController.get())
                                {
                                        fpsController->StrafeLeft(input.mData.l!=0);
                                }
                                break;
                        case gCmdRight:
                                if (fpsController.get())
                                {
                                        fpsController->StrafeRight(input.mData.l!=0);
                                }
                                break;
                        case gCmdForward:
                                if (fpsController.get())
                                {
                                        fpsController->Forward(input.mData.l!=0);
                                }
                                break;
                        case gCmdBackward:
                                if (fpsController.get())
                                {
                                        fpsController->Backward(input.mData.l!=0);
                                }
                                break;
                }
        }

        return deltaTime;
}

/*!     Draw the scene
*/
static void renderFrame(float deltaTime)
{
        static float t = 0.0f;

#if 0 // as it used to be before the split
        shared_ptr<SceneServer> sceneServer = static_pointer_cast<SceneServer>(gContext->Get("/sys/server/scene"));
        if (sceneServer.get() != NULL)
        {
                sceneServer->Render();
        }
#else
    boost::shared_ptr<RenderServer> renderServer =
        static_pointer_cast<RenderServer>(gContext->Get("/sys/server/scene"));
    if (renderServer)
    {
        renderServer->Render(true);
    }
#endif
}

float gTime = 0.0f;
int   gNumFrames = 0;
/*!     Update

        Processes input, updates windowserver, current score, entities, etc..
*/
static void update()
{
        // process the input events, which have occured
        float deltaTime = processInput();

        boost::shared_ptr<SceneServer> sceneServer = static_pointer_cast<SceneServer>(gContext->Get("/sys/server/scene"));
        if (sceneServer.get() != NULL)
        {
                sceneServer->Update(deltaTime);
        }
        renderFrame(deltaTime);

        gTime += deltaTime;
        gNumFrames++;
}

/*!     Standard issue main procedure. We initialize ourselves with a few scripts
        and then enter the run-loop.
*/
int main(int argc, char **argv)
{
        // initialize Zeitgeist (object hierarchy, scripting, files, plugins)
        Zeitgeist       myZeitgeist("." PACKAGE_NAME);
        // initialize Kerosin (input, windowing)
        Kerosin         myKerosin(myZeitgeist);
        // create our global context (object hierarchy position container)
        gContext = myZeitgeist.CreateContext();
        // application specific initialization
        if (init() == false)
        {
                gContext->GetCore()->GetLogServer()->Error() << "ERROR: Init failed..." << endl;
        }

        // retrieve shared ptr to the OpenGL Server ... this represents the OpenGL
        // context the application runs in, as well as the window
        boost::shared_ptr<OpenGLServer> openglServer = static_pointer_cast<OpenGLServer>(gContext->Get("/sys/server/opengl"));
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
        boost::shared_ptr<InputServer> inputServer = static_pointer_cast<InputServer>(gContext->Get("/sys/server/input"));
        inputServer->Unlink();

        // average FPS
        printf("Average FPS: %.2f\n", gNumFrames/gTime);

        return 0;
}
