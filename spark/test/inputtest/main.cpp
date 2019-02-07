#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>
#ifndef WIN32
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

using namespace boost;
using namespace kerosin;
using namespace std;
using namespace zeitgeist;

//! the Zeitgeist context we are using
boost::shared_ptr<CoreContext> gContext;

boost::shared_ptr<LogServer> gLog;

static const int gCmdQuit = 1;

static void update()
{
        // retrieve the input server
        boost::shared_ptr<InputServer> inputServer = static_pointer_cast<InputServer>(gContext->Get("/sys/server/input"));

        if (!inputServer) return;

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
                }
        }
}

static void shutdown()
{
        // we have to make sure, the inputServer is shut down before the opengl server,
        // as the opengl server shuts down SDL ... this will conflict with the input
        // server
        boost::shared_ptr<InputServer> inputServer = static_pointer_cast<InputServer>(gContext->Get("/sys/server/input"));

        if (inputServer)
                inputServer->Unlink();
}

int main(int argc, char **argv)
{
        Zeitgeist       zg("." PACKAGE_NAME);

        // create our global context (object hierarchy position container)
        gContext = zg.CreateContext();
        Kerosin                                 kCore(zg);

        gLog = static_pointer_cast<LogServer>(gContext->Get("/sys/server/log"));

        boost::shared_ptr<ScriptServer> scriptServer = static_pointer_cast<ScriptServer>(gContext->Get("/sys/server/script"));
        scriptServer->Run("init.rb");
        scriptServer->Run("inputtest.rb");

        unsigned int prevTime = SDL_GetTicks();
        int frames = 0;
        boost::shared_ptr<InputServer> inputServer = static_pointer_cast<InputServer>(gContext->Get("/sys/server/input"));

        inputServer->BindCommand("escape", gCmdQuit);

        // retrieve shared ptr to the OpenGL Server ... this represents the OpenGL
        // context the application runs in, as well as the window
        boost::shared_ptr<OpenGLServer> openglServer = static_pointer_cast<OpenGLServer>(gContext->Get("/sys/server/opengl"));
        if (openglServer.get() == NULL)
        {
                gContext->GetCore()->GetLogServer()->Error() << "ERROR: Can't locate OpenGLServer ..." << endl;
                shutdown();
                return 1;
        }

        // the runloop
        while(!openglServer->WantsToQuit())
        {
                frames ++;
                // update the window (pumps event loop, etc..)
                openglServer->Update();
                // update all the other components
                update();

                openglServer->SwapBuffers();
        }

        unsigned int time = SDL_GetTicks() - prevTime;

        gLog->Normal() << "Average FPS: " << 1000.0f*frames / (float)time << endl;

        gLog.reset();

        shutdown();

        return 0;
}
