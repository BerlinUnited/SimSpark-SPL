/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: main.cpp,v 1.4 2004/03/20 13:17:51 rollmark Exp $

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

#include <sstream>
#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>
#include <oxygen/oxygen.h>
#include <SDL/SDL.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <monitorlib.h>
#include <commserver.h>
#include <monitorparser.h>

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
        CmdKickOff
    };

// initialize Zeitgeist (object hierarchy, scripting, files, plugins)
Zeitgeist gZg("." PACKAGE_NAME, "../../../");

// initialize Oxygen (simulation and physics)
Oxygen gOx(gZg);

// initialize Kerosin (input, windowing)
Kerosin gks(gZg);

// initialize monitor lib
MonitorLib ml(gZg);

// game state data
GameState gGameState;

// game parameter data
GameParam gGameParam;

shared_ptr<LogServer> gLogServer;
shared_ptr<ScriptServer> gScriptServer;
shared_ptr<SceneServer> gSceneServer;
shared_ptr<OpenGLServer> gOpenGLServer;
shared_ptr<InputServer> gInputServer;
shared_ptr<FPSController> gFPSController;
shared_ptr<RenderServer> gRenderServer;
shared_ptr<CommServer> gCommServer;
shared_ptr<MonitorParser> gMonitorParser;

// total time passed
float gTime = 0.0f;

// total frames rendered
int   gNumFrames = 0;

// monitor port
int gPort           = DEFAULT_PORT;

// server machine
string gSoccerServer = DEFAULT_HOST;

// color for player of the different teams
const float gTeamLColor[4] = {1.0f, 0.2f, 0.2f, 1.0f};
const float gTeamRColor[4] = {0.2f, 0.2f, 1.0f, 1.0f};

bool init()
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
    gScriptServer->CreateVariable("Command.KickOff",  CmdKickOff);

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

    // get MonitorLib classes

    gCommServer = shared_dynamic_cast<CommServer>
        (gZg.GetCore()->Get("/sys/server/comm"));

    if (gCommServer.get() == 0)
        {
            gLogServer->Error() << "ERROR: CommServer not found\n";
            return false;
      }

  gMonitorParser = shared_dynamic_cast<MonitorParser>
      (gZg.GetCore()->Get("/sys/server/parser"));

  if (gMonitorParser.get() == 0)
      {
          gLogServer->Error() << "ERROR: MonitorParser not found\n";
          return false;
      }

    return true;
}

float processInput()
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

                case CmdKickOff:
                    // kick off
                    gLogServer->Normal() <<"--- Kick Off\n";
                    gCommServer->SendKickOffCmd();
                    break;
                }
        }

    return deltaTime;
}

/*! Update Processes input, updates windowserver, current score,
  entities, etc..
*/
float update()
{
    // process the input events, which have occured
    float deltaTime = processInput();

    if (deltaTime == 0.0f)
        {
            gLogServer->Error()
                << "(Update) ERROR: deltaTime==0\n";
        }

    // update the scene
    gSceneServer->Update(deltaTime);

    return deltaTime;
}

void printHelp()
{
    cout << "\nusage: rcsserver3D-kerosin [options]\n"
         << "\noptions:\n"
         << " --help\t print this message.\n"
         << " --port\t sets the port number\n"
         << " --server\t sets the server name\n"
         << "\n";
}

bool processInput(int argc, char* argv[])
{
  for( int i = 0; i < argc; i++)
    {
      if( strcmp( argv[i], "--server" ) == 0 )
        {
          if( i+1  < argc)
            {
              gSoccerServer = argv[i+1];
              ++i;
              cout << "server set to "
                   << gSoccerServer << "\n";
            }
        }
      else if( strcmp( argv[i], "--port" ) == 0 )
        {
          if( i+1 < argc )
            {
              gPort = atoi( argv[i+1] );
              ++i;
              cout << "port set to " << gPort << "\n";
            }
        }
      else if( strcmp( argv[i], "--help" ) == 0 )
        {
          printHelp();
          return false;
        }
    }

  return true;
}

void printGreeting()
{
    cout
        << "rcssmonitor3D-kerosin version 0.2\n"
        << "Copyright (C) 2004 Markus Rollmann, \n"
        << "Universität Koblenz.\n"
        << "Copyright (C) 2004, "
        << "The RoboCup Soccer Server Maintenance Group.\n"
        << "\nType '--help' for further information\n\n";
}

bool mangleExpr(MonitorParser::Expression& expr, string& name)
{
    switch (expr.etype)
        {
        default:
            return false;

        case MonitorParser::ET_BALL:
            name = "ball";
            break;

        case MonitorParser::ET_FLAG:
            {
                // flags are differing only in the pos
                stringstream ss;
                ss << "flag"
                   << expr.pos[0]
                   << expr.pos[1]
                   << expr.pos[2];
                name = ss.str();
                break;
            }

        case MonitorParser::ET_AGENT:
            {
                stringstream ss;
                ss << "agent";

                switch (expr.team)
                    {
                    case TI_LEFT:
                        ss << "L";
                        break;

                    case TI_RIGHT:
                        ss << "R";
                        break;

                    default:
                    case TI_NONE :
                        ss << "N";
                        break;
                    }

                ss << expr.unum;
                name = ss.str();
            }
        }

    return true;
}

shared_ptr<Transform> getSphere(MonitorParser::Expression& expr)
{
    string name;
    if (! mangleExpr(expr, name))
    {
        return shared_ptr<Transform>();
    }

    shared_ptr<Scene> scene = gSceneServer->GetActiveScene();
    if (scene.get() == 0)
        {
            gLogServer->Error()
                << "ERROR: got no active scene from SceneServer\n";
            return shared_ptr<Transform>();
        }

    // try to find the transform node
    shared_ptr<Transform> node = shared_dynamic_cast<Transform>
        (scene->GetChild(name));

    if (node.get() != 0)
        {
            return node;
        }

    string fktName;

    // create a new visual
    switch (expr.etype)
        {
        default:
            return shared_ptr<Transform>();

        case MonitorParser::ET_BALL:
            fktName = "addBall";
            break;

        case MonitorParser::ET_AGENT:
            {
                stringstream ss;
                ss << "addAgent";

                switch (expr.team)
                    {
                    case TI_NONE:
                        ss << "N";
                        break;

                    case TI_LEFT:
                        ss << "L";
                        break;

                    case TI_RIGHT:
                        ss << "R";
                        break;
                    }

                fktName = ss.str();
                break;
            }

        case MonitorParser::ET_FLAG:
            fktName = "addFlag";
            break;
        }

    stringstream ss;
    ss << fktName << "('" << name << "')";
    gScriptServer->Eval(ss.str());

    return shared_dynamic_cast<Transform>
        (scene->GetChild(name));
}

void processUpdates()
{
    static bool initialUpdate = true;

    if (! gCommServer->GetMessage())
        {
            return;
        }

    boost::shared_ptr<oxygen::Predicate::TList> predicates =
        gCommServer->GetPredicates();

    MonitorParser::TExprList exprList;

    if (
        (predicates.get() != 0) &&
        (predicates->size() > 0)
        )
        {
            // parse the received expressions
            gMonitorParser->ParsePredicates(*predicates,gGameState,
                                    gGameParam,exprList);
        }


    if (initialUpdate)
        {
            // publish the soccer default values to the scripts
            gScriptServer->CreateVariable("Soccer.FieldLength", gGameParam.GetFieldLength());
            gScriptServer->CreateVariable("Soccer.FieldWidth",  gGameParam.GetFieldWidth());
            gScriptServer->CreateVariable("Soccer.FieldHeight", gGameParam.GetFieldHeight());
            gScriptServer->CreateVariable("Soccer.BorderSize",  gGameParam.GetBorderSize());
            gScriptServer->CreateVariable("Soccer.LineWidth",   gGameParam.GetLineWidth());
            gScriptServer->CreateVariable("Soccer.GoalWidth",   gGameParam.GetGoalWidth());
            gScriptServer->CreateVariable("Soccer.GoalDepth",   gGameParam.GetGoalDepth());
            gScriptServer->CreateVariable("Soccer.GoalHeight",  gGameParam.GetGoalHeight());
            gScriptServer->CreateVariable("Soccer.AgentMass",  gGameParam.GetAgentMass());
            gScriptServer->CreateVariable("Soccer.AgentRadius",  gGameParam.GetAgentRadius());
            gScriptServer->CreateVariable("Soccer.AgentMaxSpeed",  gGameParam.GetAgentMaxSpeed());
            gScriptServer->CreateVariable("Soccer.BallRadius", gGameParam.GetBallRadius());
            gScriptServer->CreateVariable("Soccer.BallMass", gGameParam.GetBallMass());

            initialUpdate = false;
        }

    for (
         MonitorParser::TExprList::iterator iter = exprList.begin();
         iter != exprList.end();
         ++iter)
        {
            MonitorParser::Expression& expr = (*iter);

            shared_ptr<Transform> node = getSphere(expr);

            if (node.get() == 0)
                {
                    continue;
                }

            Vector3f pos(expr.pos[0],expr.pos[2],expr.pos[1]);
            node->SetLocalPos(pos);
        }
}

int main(int argc, char** argv)
{
    printGreeting();

    // process command line
    if (! processInput(argc, argv))
        {
            return 1;
        }

    // application specific initialization
    if (init() == false)
        {
            gLogServer->Error() << "ERROR: Init failed" << endl;
            return 1;
        }

    // setup the CommServer
    gCommServer->Init("SexpParser",gSoccerServer,gPort);

    // the runloop
    while(! gOpenGLServer->WantsToQuit())
        {
            // read and parser updates from the server
            processUpdates();

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
