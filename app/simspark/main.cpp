/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: main.cpp,v 1.2 2004/04/14 18:34:43 rollmark Exp $

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
    SimSpark(const std::string& relPathPrefix) :
        Spark(relPathPrefix)
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
};

void SimSpark::PrintGreeting()
{
    GetLog()->Normal()
        << "simspark, a monolithic simulator 0.1\n"
        << "Copyright (C) 2004 Markus Rollmann, \n"
        << "Universität Koblenz.\n"
        << "Copyright (C) 2004, "
        << "The RoboCup Soccer Server Maintenance Group.\n"
        << "\nType '--help' for further information\n\n";
}

void SimSpark::PrintHelp()
{
    GetLog()->Normal()
        << "\nusage: simspark [options]\n"
         << "\noptions:\n"
         << " --help\t print this message.\n"
         << "\n";
}

bool SimSpark::ProcessCmdLine(int argc, char* argv[])
{
  for( int i = 0; i < argc; i++)
    {
      if( strcmp( argv[i], "--help" ) == 0 )
        {
          PrintHelp();
          return false;
        }
    }

  return true;
}

void SimSpark::UpdatePreRenderFrame()
{
}

bool SimSpark::InitApp(int argc, char** argv)
{
    SetSimStep(0.02);

    PrintGreeting();

    // process command line
    if (! ProcessCmdLine(argc, argv))
        {
            return false;
        }

    // run initialization scripts
    GetScriptServer()->Run("simspark.rb");
    GetScriptServer()->Run("bindings.rb");

    // tell spark the loaction of our camera
    if (! SetFPSController("/usr/scene/camera/physics/controller"))
        {
            return false;
        }

    return true;
}

void SimSpark::ProcessInput(kerosin::InputServer::Input& /*input*/)
{
}

int main(int argc, char** argv)
{
    // the spark app framework instance
    SimSpark spark("../../");

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
