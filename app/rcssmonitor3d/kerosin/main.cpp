/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: main.cpp,v 1.1 2004/03/12 09:14:46 rollmark Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>
#include <oxygen/oxygen.h>
#include <SDL/SDL.h>
#include <zeitgeist/fileserver/fileserver.h>

using namespace boost;
using namespace kerosin;
using namespace oxygen;
using namespace salt;
using namespace std;
using namespace zeitgeist;

//
// Input constant mappings
//
enum ECmds
    {
        CmdQuit,
        CmdTimer,
        CmdMouseX,
        CmdMouseY,
        CmdUp,
        CmdDown,
        CmdLeft,
        CmdRight,
        CmdForward,
        CmdBackward,
    };

// initialize Zeitgeist (object hierarchy, scripting, files, plugins)
Zeitgeist gZg("." PACKAGE_NAME, "../../../");

// initialize Oxygen (simulation and physics)
Oxygen gOx(gZg);

// initialize Kerosin (input, windowing)
Kerosin gks(gZg);

shared_ptr<LogServer> gLogServer;
shared_ptr<ScriptServer> gScriptServer;
shared_ptr<SceneServer> gSceneServer;
shared_ptr<OpenGLServer> gOpenGLServer;
shared_ptr<InputServer> gInputServer;
shared_ptr<FPSController> gFPSController;
shared_ptr<RenderServer> gRenderServer;

// total time passed
float gTime = 0.0f;

// total frames rendered
int   gNumFrames = 0;

static bool init()
{
    gLogServer = gZg.GetCore()->GetLogServer();
    gScriptServer = gZg.GetCore()->GetScriptServer();

    // publish our commands to the scripts
    gScriptServer->CreateVariable("Command.Quit",     CmdQuit);
    gScriptServer->CreateVariable("Command.Timer",    CmdTimer);
    gScriptServer->CreateVariable("Command.MouseX",   CmdMouseX);
    gScriptServer->CreateVariable("Command.MouseY",   CmdMouseY);
    gScriptServer->CreateVariable("Command.Left",     CmdLeft);
    gScriptServer->CreateVariable("Command.Right",    CmdRight);
    gScriptServer->CreateVariable("Command.Forward",  CmdForward);
    gScriptServer->CreateVariable("Command.Backward", CmdBackward);
    gScriptServer->CreateVariable("Command.Up",       CmdUp);
    gScriptServer->CreateVariable("Command.Down",     CmdDown);

    // run initialization scripts
    gScriptServer->Run("rcssmonitor3D-kerosin.rb");
    gScriptServer->Run("german.scan.rb");
    gScriptServer->Run("bindings.rb");

    gSceneServer = shared_dynamic_cast<SceneServer>
        (gZg.GetCore()->Get("/sys/server/scene"));

    if (gSceneServer.get() == 0)
        {
            gLogServer->Error() << "ERROR: SceneServer not fount\n";
            return false;
        }

    gOpenGLServer =  shared_dynamic_cast<OpenGLServer>
        (gZg.GetCore()->Get("/sys/server/opengl"));

    if (gOpenGLServer.get() == 0)
        {
            gLogServer->Error() << "ERROR: OpenGLServer not found\n";
            return false;
        }

    gInputServer = shared_dynamic_cast<InputServer>
        (gZg.GetCore()->Get("/sys/server/input"));

    if (gInputServer.get() == 0)
        {
            gLogServer->Error() << "ERROR: InputServer not found\n";
            return false;
        }

    gFPSController = shared_dynamic_cast<FPSController>
        (gZg.GetCore()->Get("/usr/scene/camera0/physics/controller"));

    if (gFPSController.get() == 0)
        {
            gLogServer->Error() << "ERROR: FPSController not found\n";
            return false;
        }

    gRenderServer = shared_dynamic_cast<RenderServer>
        (gZg.GetCore()->Get("/sys/server/render"));

    if (gRenderServer.get() == 0)
        {
            gLogServer->Error() << "ERROR: RenderServer not found\n";
            return false;
        }
    return true;
}

static float processInput()
{
    float deltaTime = 0.0f;

    // Process incoming input
    InputServer::Input input;
    while (gInputServer->GetInput(input))
        {
            switch (input.id)
                {
                case CmdQuit:
                    gOpenGLServer->Quit();
                    break;

                case CmdTimer:
                    deltaTime = (float) input.data.l/1000.0f;
                    break;

                case CmdMouseX:
                    gFPSController->AdjustHAngle(0.3f*(float)input.data.l);
                    break;

                case CmdMouseY:
                    gFPSController->AdjustVAngle(0.3f*(float)input.data.l);
                    break;

                case CmdUp:
                    gFPSController->Up(input.data.l!=0);
                    break;

                case CmdDown:
                    gFPSController->Down(input.data.l!=0);
                    break;

                case CmdLeft:
                    gFPSController->StrafeLeft(input.data.l!=0);
                    break;

                case CmdRight:
                    gFPSController->StrafeRight(input.data.l!=0);
                    break;

                case CmdForward:
                    gFPSController->Forward(input.data.l!=0);
                    break;

                case CmdBackward:
                    gFPSController->Backward(input.data.l!=0);
                    break;
                }
        }

    return deltaTime;
}

/*! Update Processes input, updates windowserver, current score,
  entities, etc..
*/
static float update()
{
    // process the input events, which have occured
    float deltaTime = processInput();

    if (deltaTime == 0.0f)
        {
            gLogServer->Error()
                << "(Update) ERROR: deltaTime==0\n";
        }

    gSceneServer->Update(deltaTime);

    return deltaTime;
}

int main(int /*argc*/, char** /*argv*/)
{
    // application specific initialization
    if (init() == false)
        {
            gLogServer->Error() << "ERROR: Init failed" << endl;
            return 1;
        }

    // the runloop
    while(! gOpenGLServer->WantsToQuit())
        {
            // update the window (pumps event loop, etc..)
            gOpenGLServer->Update();

            // update all the other components
            float deltaTime = update();

            // render the current frame
            gRenderServer->Render();
            gTime += deltaTime;
            ++gNumFrames;

            gOpenGLServer->SwapBuffers();
        }

    // we have to make sure, the inputServer is shut down before the
    // opengl server, as the opengl server shuts down SDL ... this will
    // conflict with the input server
    gInputServer->Unlink();
    gInputServer.reset();

    // average FPS
    gLogServer->Normal() << "Average FPS: " << gNumFrames/gTime << "\n";

    return 0;
}
