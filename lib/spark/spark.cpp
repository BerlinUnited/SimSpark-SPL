/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: spark.cpp,v 1.1 2004/04/11 11:17:53 rollmark Exp $

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

#include "spark.h"
#include <zeitgeist/zeitgeist.h>

//#include <SDL/SDL.h>
using namespace spark;
using namespace kerosin;
using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;


Spark::Spark(const string& relPathPrefix) :
    mZeitgeist("." PACKAGE_NAME, relPathPrefix),
    mOxygen(mZeitgeist),
    mKerosin(mZeitgeist),
    mTime(0.0f),
    mFramesRendered(0),
    mHorSens(0.3),
    mVertSens(0.3)
{
}

Spark::~Spark()
{
    // we have to make sure, the inputServer is shut down before the
    // opengl server, as the opengl server shuts down SDL ... this
    // will conflict with the input server
    mInputServer->Unlink();
    mInputServer.reset();
}

bool Spark::Init(int argc, char** argv)
{
    mLogServer = mZeitgeist.GetCore()->GetLogServer();
    if (mLogServer.get() == 0)
        {
            cout << "(SPARK) ERROR: LogServer not found\n";
            return false;
        }

    mScriptServer = mZeitgeist.GetCore()->GetScriptServer();
    if (mScriptServer.get() == 0)
        {
            mLogServer->Error()
                << "(SPARK) ERROR: ScriptServer not found\n";
            return false;
        }

    // publish common command constants to the scripts
    mScriptServer->CreateVariable("Command.Quit",     CmdQuit);
    mScriptServer->CreateVariable("Command.Timer",    CmdTimer);
    mScriptServer->CreateVariable("Command.MouseX",   CmdMouseX);
    mScriptServer->CreateVariable("Command.MouseY",   CmdMouseY);
    mScriptServer->CreateVariable("Command.Left",     CmdLeft);
    mScriptServer->CreateVariable("Command.Right",    CmdRight);
    mScriptServer->CreateVariable("Command.Forward",  CmdForward);
    mScriptServer->CreateVariable("Command.Backward", CmdBackward);
    mScriptServer->CreateVariable("Command.Up",       CmdUp);
    mScriptServer->CreateVariable("Command.Down",     CmdDown);

    // run the spark init script
    mZeitgeist.GetCore()->GetRoot()->GetScript()->RunInitScript
        (
         "spark.rb",
         "lib/spark",
         ScriptServer::IS_COMMON
         );

    mSceneServer = shared_dynamic_cast<SceneServer>
        (mZeitgeist.GetCore()->Get("/sys/server/scene"));

    if (mSceneServer.get() == 0)
        {
            mLogServer->Error() << "(SPARK) ERROR: SceneServer not found\n";
            return false;
        }

    mOpenGLServer =  shared_dynamic_cast<OpenGLServer>
        (mZeitgeist.GetCore()->Get("/sys/server/opengl"));

    if (mOpenGLServer.get() == 0)
        {
            mLogServer->Error() << "(SPARK) ERROR: OpenGLServer not found\n";
            return false;
        }

    mInputServer = shared_dynamic_cast<InputServer>
        (mZeitgeist.GetCore()->Get("/sys/server/input"));

    if (mInputServer.get() == 0)
        {
            mLogServer->Error() << "(SPARK) ERROR: InputServer not found\n";
            return false;
        }


    mRenderServer = shared_dynamic_cast<RenderServer>
        (mZeitgeist.GetCore()->Get("/sys/server/render"));

    if (mRenderServer.get() == 0)
        {
            mLogServer->Error() << "(SPARK) ERROR: RenderServer not found\n";
            return false;
        }

    // run the app defined init
    return InitApp(argc,argv);
}

bool Spark::SetFPSController(const std::string& path)
{
    if (path.empty())
        {
            mFPSController.reset();
            return true;
        }

    shared_ptr<Leaf> leaf = mZeitgeist.GetCore()->Get(path);

    if (leaf.get() == 0)
        {
            mLogServer->Error()
                << "(SPARK::SetFPSController) ERROR: invalid path "
                << path << "'\n";
            return false;
        }

    mFPSController = shared_dynamic_cast<FPSController>
        (mZeitgeist.GetCore()->Get(path));

    if (mFPSController.get() == 0)
        {
            // the path is valid but doesn't point to an FPSController;
            // for convenience search below for a controller
            mFPSController  =
                leaf->FindChildSupportingClass<FPSController>(true);
        }

    if (mFPSController.get() == 0)
        {
            mLogServer->Error()
                << "(SPARK) ERROR: no FPSController found at '"
                << path << "'\n";
            return false;
        }

    return true;
}

void Spark::ProcessInput(kerosin::InputServer::Input& /*input*/)
{
}

void Spark::UpdatePreRenderFrame()
{
}

bool Spark::InitApp(int /*argc*/, char** /*argv*/)
{
    return true;
}

void Spark::SetFPSController(shared_ptr<FPSController> controller)
{
    mFPSController = controller;
}

void Spark::SetHorizontalSensitivity(float s)
{
    mHorSens = s;
}

void Spark::SetVerticalSensitivity(float s)
{
    mVertSens = s;
}

float Spark::GetHorizontalSensitivity()
{
    return mHorSens;
}

float Spark::GetVerticalSensitivity()
{
    return mVertSens;
}

float Spark::GetDeltaTime()
{
    return mDeltaTime;
}

float Spark::GetTime()
{
    return mTime;
}

int Spark::GetFramesRendered()
{
    return mFramesRendered;
}

Zeitgeist& Spark::GetZeitgeist()
{
    return mZeitgeist;
}

shared_ptr<Core> Spark::GetCore()
{
    return mZeitgeist.GetCore();
}

shared_ptr<zeitgeist::LogServer> Spark::GetLog()
{
    return mZeitgeist.GetCore()->GetLogServer();
}

shared_ptr<SceneServer> Spark::GetSceneServer()
{
    return mSceneServer;
}

shared_ptr<ScriptServer> Spark::GetScriptServer()
{
    return mScriptServer;
}

shared_ptr<Scene> Spark::GetActiveScene()
{
    shared_ptr<Scene> scene = mSceneServer->GetActiveScene();

    if (scene.get() == 0)
        {
            mLogServer->Warning()
                << "(SPARK) Warning: no active scene registered\n";
        }

    return scene;
}

void Spark::ReadInput()
{
    // Process incoming input
    mDeltaTime = 0.0f;
    static InputServer::Input input;

    while (mInputServer->GetInput(input))
        {
            switch (input.id)
                {
                case CmdQuit:
                    mOpenGLServer->Quit();
                    break;

                case CmdTimer:
                    mDeltaTime = (float) input.data.l/1000.0f;
                    break;

                case CmdMouseX:
                    mFPSController->AdjustHAngle(mHorSens*(float)input.data.l);
                    break;

                case CmdMouseY:
                    mFPSController->AdjustVAngle(mVertSens*(float)input.data.l);
                    break;

                case CmdUp:
                    mFPSController->Up(input.data.l!=0);
                    break;

                case CmdDown:
                    mFPSController->Down(input.data.l!=0);
                    break;

                case CmdLeft:
                    mFPSController->StrafeLeft(input.data.l!=0);
                    break;

                case CmdRight:
                    mFPSController->StrafeRight(input.data.l!=0);
                    break;

                case CmdForward:
                    mFPSController->Forward(input.data.l!=0);
                    break;

                case CmdBackward:
                    mFPSController->Backward(input.data.l!=0);
                    break;

                default:
                    // process a user defined command
                    ProcessInput(input);
                    break;
                }
        }
}

void Spark::Run()
{
    while(! mOpenGLServer->WantsToQuit())
        {
            // read updates from the InputServer
            ReadInput();

            // take user defined action
            UpdatePreRenderFrame();

            // update the scene graph and do physics
            mSceneServer->Update(mDeltaTime);

            // update the window (pumps event loop, etc..) and render
            // the current frame
            mOpenGLServer->Update();
            mRenderServer->Render();
            mOpenGLServer->SwapBuffers();

            mTime += mDeltaTime;
            ++mFramesRendered;
        }
}
