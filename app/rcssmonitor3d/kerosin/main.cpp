/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: main.cpp,v 1.8 2004/04/12 13:49:42 rollmark Exp $

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

#include <spark/spark.h>
#include <sstream>
#include <monitorlib.h>
#include <commserver.h>
#include <monitorparser.h>

using namespace spark;
using namespace kerosin;
using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;

class MonitorSpark : public Spark
{
public:
    // user define command constants
    static const int CmdKickOff = CmdUser+1;

public:
    MonitorSpark(const std::string& relPathPrefix) :
        Spark(relPathPrefix),
        mMl(GetZeitgeist()),
        mPort(DEFAULT_PORT),
        mSoccerServer(DEFAULT_HOST)
    {};

    /** process user defined input constants */
    virtual void ProcessInput(kerosin::InputServer::Input& input);

    /** reads and parses messages from the soccer server */
    virtual void UpdatePreRenderFrame();

    /** called once after Spark finished it's init */
    virtual bool InitApp(int argc, char** argv);

    /** print command line options */
    void PrintHelp();

    /** print a greeting */
    void PrintGreeting();

    /** process command line options */
    bool ProcessCmdLine(int argc, char* argv[]);

    /** constructs a unique string used as the node name for the give
        expression */
    bool MangleExpr(MonitorParser::Expression& expr, string& name);

    /** mangles the given expression and construcs the corresponding
        sphere or returns an already existing instance
    */
    shared_ptr<Transform> GetSphere(MonitorParser::Expression& expr);

public:
    // the monitor lib
    MonitorLib mMl;

    // game state data
    GameState mGameState;

    // game parameter data
    GameParam mGameParam;

    // monitor port
    int mPort;

    // server machine
    string mSoccerServer;

    // the communication server instance
    shared_ptr<CommServer> mCommServer;

    // the parser instance
    shared_ptr<MonitorParser> mMonitorParser;
};

void MonitorSpark::PrintGreeting()
{
    GetLog()->Normal()
        << "rcssmonitor3D-kerosin version 0.2\n"
        << "Copyright (C) 2004 Markus Rollmann, \n"
        << "Universität Koblenz.\n"
        << "Copyright (C) 2004, "
        << "The RoboCup Soccer Server Maintenance Group.\n"
        << "\nType '--help' for further information\n\n";
}

void MonitorSpark::PrintHelp()
{
    GetLog()->Normal()
        << "\nusage: rcsserver3D-kerosin [options]\n"
         << "\noptions:\n"
         << " --help\t print this message.\n"
         << " --port\t sets the port number\n"
         << " --server\t sets the server name\n"
         << "\n";
}

bool MonitorSpark::ProcessCmdLine(int argc, char* argv[])
{
  for( int i = 0; i < argc; i++)
    {
      if( strcmp( argv[i], "--server" ) == 0 )
        {
          if( i+1  < argc)
            {
              mSoccerServer = argv[i+1];
              ++i;
              GetLog()->Normal()
                  << "server set to "
                  << mSoccerServer << "\n";
            }
        }
      else if( strcmp( argv[i], "--port" ) == 0 )
        {
          if( i+1 < argc )
            {
              mPort = atoi( argv[i+1] );
              ++i;
              GetLog()->Normal()
                  << "port set to " << mPort << "\n";
            }
        }
      else if( strcmp( argv[i], "--help" ) == 0 )
        {
          PrintHelp();
          return false;
        }
    }

  return true;
}

bool MonitorSpark::MangleExpr(MonitorParser::Expression& expr, string& name)
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

shared_ptr<Transform> MonitorSpark::GetSphere(MonitorParser::Expression& expr)
{
    string name;
    shared_ptr<Scene> scene = GetActiveScene();
    if (
        (scene.get() == 0) ||
        (! MangleExpr(expr, name))
        )
        {
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
    GetScriptServer()->Eval(ss.str());

    return shared_dynamic_cast<Transform>
        (scene->GetChild(name));
}

void MonitorSpark::UpdatePreRenderFrame()
{
    static bool initialUpdate = true;

    if (! mCommServer->GetMessage())
        {
            return;
        }

    boost::shared_ptr<oxygen::PredicateList> predicates =
        mCommServer->GetPredicates();

    MonitorParser::TExprList exprList;

    if (
        (predicates.get() != 0) &&
        (predicates->GetSize() > 0)
        )
        {
            // parse the received expressions
            mMonitorParser->ParsePredicates(*predicates,mGameState,
                                            mGameParam,exprList);
        }

    if (initialUpdate)
        {
            // publish the soccer default values to the scripts
            GetScriptServer()->CreateVariable("Soccer.FieldLength", mGameParam.GetFieldLength());
            GetScriptServer()->CreateVariable("Soccer.FieldWidth",  mGameParam.GetFieldWidth());
            GetScriptServer()->CreateVariable("Soccer.FieldHeight", mGameParam.GetFieldHeight());
            GetScriptServer()->CreateVariable("Soccer.BorderSize",  mGameParam.GetBorderSize());
            GetScriptServer()->CreateVariable("Soccer.LineWidth",   mGameParam.GetLineWidth());
            GetScriptServer()->CreateVariable("Soccer.GoalWidth",   mGameParam.GetGoalWidth());
            GetScriptServer()->CreateVariable("Soccer.GoalDepth",   mGameParam.GetGoalDepth());
            GetScriptServer()->CreateVariable("Soccer.GoalHeight",  mGameParam.GetGoalHeight());
            GetScriptServer()->CreateVariable("Soccer.AgentMass",   mGameParam.GetAgentMass());
            GetScriptServer()->CreateVariable("Soccer.AgentRadius",  mGameParam.GetAgentRadius());
            GetScriptServer()->CreateVariable("Soccer.AgentMaxSpeed",  mGameParam.GetAgentMaxSpeed());
            GetScriptServer()->CreateVariable("Soccer.BallRadius",  mGameParam.GetBallRadius());
            GetScriptServer()->CreateVariable("Soccer.BallMass",    mGameParam.GetBallMass());

            // create the playing field with the acutal dimensions
            GetScriptServer()->Eval("addField()");
            initialUpdate = false;
        }

    for (
         MonitorParser::TExprList::iterator iter = exprList.begin();
         iter != exprList.end();
         ++iter)
        {
            MonitorParser::Expression& expr = (*iter);
            shared_ptr<Transform> node = GetSphere(expr);

            if (node.get() == 0)
                {
                    continue;
                }

            Vector3f pos(expr.pos[0],expr.pos[2],expr.pos[1]);
            node->SetLocalPos(pos);
        }
}

bool MonitorSpark::InitApp(int argc, char** argv)
{
    PrintGreeting();

    // process command line
    if (! ProcessCmdLine(argc, argv))
        {
            return false;
        }

    // publish user defined command constants
    GetScriptServer()->CreateVariable("Command.KickOff", CmdKickOff);

    // run initialization scripts
    GetScriptServer()->Run("rcssmonitor3D-kerosin.rb");
    GetScriptServer()->Run("bindings.rb");

    // tell spark the loaction of our camera
    if (! SetFPSController("/usr/scene/camera/physics/controller"))
        {
            return false;
        }

    // get MonitorLib classes
    mCommServer = shared_dynamic_cast<CommServer>
        (GetCore()->Get("/sys/server/comm"));

    if (mCommServer.get() == 0)
        {
            GetLog()->Error() << "ERROR: CommServer not found\n";
            return false;
      }

    // setup the CommServer
    mCommServer->Init("SexpParser",mSoccerServer,mPort);

    mMonitorParser = shared_dynamic_cast<MonitorParser>
      (GetCore()->Get("/sys/server/parser"));

    if (mMonitorParser.get() == 0)
        {
          GetLog()->Error() << "ERROR: MonitorParser not found\n";
          return false;
        }

    SetSimStep(0.05);
    return true;
}

void MonitorSpark::ProcessInput(kerosin::InputServer::Input& input)
{
    switch (input.id)
        {
        case CmdKickOff:
            // kick off
            GetLog()->Normal() <<"--- Kick Off\n";
            mCommServer->SendKickOffCmd();
            break;
        }
}

int main(int argc, char** argv)
{
    // the spark app framework instance
    MonitorSpark spark("../../../");

    if (! spark.Init(argc, argv))
        {
            return 1;
        }

    spark.Run();

    spark.GetLog()->Normal()
        << "Average FPS: "
        << spark.GetFramesRendered()/spark.GetTime()
        << "\n";

    return 0;
}
