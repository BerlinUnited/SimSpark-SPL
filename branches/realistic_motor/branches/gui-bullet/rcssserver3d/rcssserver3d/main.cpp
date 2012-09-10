/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: main.cpp 292 2012-02-14 15:51:26Z sgvandijk $

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
#include <zeitgeist/zeitgeist.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <oxygen/simulationserver/simulationserver.h>
#include <kerosin/renderserver/rendercontrol.h>
#include <kerosin/inputserver/inputcontrol.h>

#ifdef HAVE_CONFIG_H
#undef PACKAGE_NAME
#include <rcssserver3d_config.h>
#endif

#if __APPLE__
#include <SDL.h>
#endif

using namespace spark;
using namespace kerosin;
using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;

class SimSpark : public Spark
{
public:
    SimSpark() :
        Spark(),
        mScriptPath("rcssserver3d.rb")
    {};

    /** called once after Spark finished it's init */
    virtual bool InitApp(int argc, char** argv);

    /** print command line options */
    void PrintHelp();

    /** print a greeting */
    void PrintGreeting();

    /** process command line options */
    bool ProcessCmdLine(int argc, char* argv[]);

private:
    std::string     mScriptPath;
};

void SimSpark::PrintGreeting()
{
    GetLog()->Normal()
        << "rcssserver3d (formerly simspark), a monolithic simulator "
            RCSS_VERSION"\n"
        << "Copyright (C) 2004 Markus Rollmann, \n"
        << "Universität Koblenz.\n"
        << "Copyright (C) 2004-2009, "
        << "The RoboCup Soccer Server Maintenance Group.\n"
        << "\nType '--help' for further information\n\n";
}

void SimSpark::PrintHelp()
{
    GetLog()->Normal()
        << "\nusage: rcssserver3d [options] [script]\n"
        << "\noptions:\n"
        << " --help\t\t\t\t print this message.\n"
        << " --script-path PATH\t\t set the script path (rcssserver3d.rb path).\n"
        << " --init-script-prefix PATH\t path prefix for init scripts (spark.rb, oxygen.rb, etc.).\n"
        << " --agent-port PORTNUM\t\t port for agents to connect to.\n"
        << " --server-port PORTNUM\t\t port for monitors to connect to.\n"
        << "\n";
}

bool SimSpark::ProcessCmdLine(int argc, char* argv[])
{
    for( int i = 1; i < argc; i++)
      {
        if(strcmp( argv[i], "--help" ) == 0)
        {
          PrintHelp();
          return false;
        }
        else if(strcmp(argv[i], "--script-path") == 0)
        {
          i++;
          if (i < argc)
            mScriptPath = argv[i];
          else
            {
               PrintHelp();
               return false;
            }
        }
        else if (strcmp(argv[i], "--agent-port") == 0)
        {
          i++;
          if (i < argc)
            GetScriptServer()->Eval(string("$agentPort = ") + argv[i]);
          else
            {
               PrintHelp();
               return false;
            }
        }
        else if (strcmp(argv[i], "--server-port") == 0)
        {
          i++;
          if (i < argc)
            GetScriptServer()->Eval(string("$serverPort = ") + argv[i]);
          else
            {
               PrintHelp();
               return false;
            }
        }

      }

    return true;
}

bool SimSpark::InitApp(int argc, char** argv)
{
    GetCore()->GetFileServer()->AddResourceLocation(RCSS_BUNDLE_PATH);
    GetSimulationServer()->SetSimStep(0.02f);
    PrintGreeting();

    // process command line
    if (! ProcessCmdLine(argc, argv))
    {
        return false;
    }

    // run initialization scripts
    GetScriptServer()->Run(mScriptPath);

    // tell the inputControl node the loaction of our camera
    boost::shared_ptr<InputControl> inputCtr = GetInputControl();
    if (inputCtr.get() != 0)
    {
        inputCtr->SetFPSController("/usr/scene/camera/physics/controller");
    }

    return true;
}

int main(int argc, char** argv)
{
    // the spark app framework instance
    SimSpark spark;

    if (! spark.Init(argc, argv))
    {
        return 1;
    }

    spark.GetSimulationServer()->Run(argc,argv);

    boost::shared_ptr<RenderControl> renderCtr = spark.GetRenderControl();
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
