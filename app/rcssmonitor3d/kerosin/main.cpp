/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: main.cpp,v 1.11.2.1 2004/05/10 11:51:19 fruit Exp $

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
#include <types.h>
#include <commserver.h>
#include "monitorinputcontrol.h"
#include "monitorclientcontrol.h"

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
    MonitorSpark(const std::string& relPathPrefix) :
        Spark(relPathPrefix),
        mMl(GetZeitgeist()),
        mPort(DEFAULT_PORT),
        mSoccerServer(DEFAULT_HOST)
    {};

    /** called once after Spark finished it's init */
    virtual bool InitApp(int argc, char** argv);

    /** print command line options */
    void PrintHelp();

    /** print a greeting */
    void PrintGreeting();

    /** process command line options */
    bool ProcessCmdLine(int argc, char* argv[]);

public:
    // the monitor lib
    MonitorLib mMl;

    // monitor port
    int mPort;

    // server machine
    string mSoccerServer;
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

bool MonitorSpark::InitApp(int argc, char** argv)
{
    PrintGreeting();

    // process command line
    if (! ProcessCmdLine(argc, argv))
        {
            return false;
        }

    // register SimControl nodes
    GetZeitgeist().GetCore()->RegisterClassObject
        (new CLASS(MonitorInputControl), "");

    GetZeitgeist().GetCore()->RegisterClassObject
        (new CLASS(MonitorClientControl), "");

    // run initialization scripts
    GetScriptServer()->RunInitScript("rcssmonitor3D-kerosin.rb",
                                     "app/rcssmonitor3d/kerosin/");
    GetScriptServer()->RunInitScript("bindings.rb",
                                     "app/rcssmonitor3d/kerosin/",
                                     ScriptServer::IS_COMMON);

    // tell the inputControl node the loaction of our camera
    shared_ptr<MonitorInputControl> inputCtr =
        shared_dynamic_cast<MonitorInputControl>
        (GetSimulationServer()->GetControlNode("MonitorInputControl"));

    if (inputCtr.get() != 0)
        {
            inputCtr->SetFPSController("/usr/scene/camera/physics/controller");
        } else
            {
                GetLog()->Error() << "ERROR: MonitorInputControl node not found\n";
                return false;
            }

    // get MonitorLib classes
    shared_ptr<CommServer> commServer = shared_dynamic_cast<CommServer>
        (GetCore()->Get("/sys/server/comm"));

    if (commServer.get() == 0)
        {
            GetLog()->Error() << "ERROR: CommServer not found\n";
            return false;
        }

    // setup the CommServer
    commServer->Init("SexpParser",mSoccerServer,mPort);

    return true;
}

int main(int argc, char** argv)
{
    // the spark app framework instance
    MonitorSpark spark("../../../");

    if (! spark.Init(argc, argv))
    {
        return 1;
    }

    spark.GetSimulationServer()->Run(argc,argv);

    shared_ptr<RenderControl> renderCtr = spark.GetRenderControl();
    if (renderCtr.get() != 0)
        {
            spark.GetLog()->Normal()
                << "Average FPS: "
                << renderCtr->GetFramesRendered() /
                   spark.GetSimulationServer()->GetTime()
                << "\n";
        }

    return 0;
}
