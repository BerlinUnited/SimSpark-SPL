/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: main.cpp,v 1.3.2.12 2004/02/07 16:30:58 rollmark Exp $

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

   Main of the rcssmonitor3D application

   This is a very simple 3D monitor example which will hopefully be
   used for the first steps of the 3D SoccerServer
*/

/** This is a dummy agent created for testing with the SpadesTest. It
    reads length prefixed data (see spades manual 6.3) from fd 3 and
    writes to fd 4. A corresponding agent DB (agentdb.list, entry
    'default') exists in the spadestest directory.
*/
#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>
#include <soccertypes.h>
#include "worldmodel.h"
#include "commserver.h"
#include "behavior.h"
#include "soccer.h"
#include "kicknrun.h"

using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace oxygen;
using namespace salt;

// commserver instance
static shared_ptr<CommServer> comm;

// worldmodel instance
static shared_ptr<WorldModel> wm;

// behavior instance
static shared_ptr<Soccer> behave;

// the default team name
string teamName = "Robolog";

// set to 1 to write debug information to stdout
#define ENABLE_LOGGING 1

#ifdef ENABLE_LOGGING
void Log(const char* out)
{
   printf(out);
   fflush(0);
}
#else
 #define Log(x)
#endif

void printHelp()
{
  cout << "\nusage: agenttest [options]" << endl;
  cout << "\noptions:" << endl;
  cout << " --help      print this message." << endl;
  cout << " --teamname  sets the team name. " << endl;
  cout << "\n";
}

void ReadOptions(int argc, char* argv[])
{
  for( int i = 0; i < argc; i++)
    {
      if( strcmp( argv[i], "--teamname" ) == 0 )
        {
          if( i+1  < argc)
            {
              teamName = argv[i+1];
              ++i;
              Log("setting teamname to ");
              Log(teamName.c_str());
              Log("\n");
            }
        }
      else if( strcmp( argv[i], "--help" ) == 0 )
        {
          printHelp();
          exit(0);
        }
    }
}

int Init(int argc, char* argv[])
{
  //init zeitgeist and oxygen
  Zeitgeist zg("." PACKAGE_NAME);
  Oxygen oygen(zg);

  // read command line options
  ReadOptions(argc,argv);

  // register and create the CommServer
  zg.GetCore()->RegisterClassObject(new CLASS(CommServer), "");
  comm = shared_dynamic_cast<CommServer>(zg.GetCore()->New("CommServer"));
  if (comm.get() == 0)
      {
          Log("cannot create CommServer\n");
          return 1;
      }

  // register and create the WorldModel
  zg.GetCore()->RegisterClassObject(new CLASS(WorldModel), "");
  wm = shared_dynamic_cast<WorldModel>(zg.GetCore()->New("WorldModel"));
  if (wm.get() == 0)
    {
      Log("cannot create WorldModel\n");
      return 1;
    }

  // register and create the KickNRun class
  zg.GetCore()->RegisterClassObject(new CLASS(KickNRun), "");
  behave = shared_dynamic_cast<Soccer>(zg.GetCore()->New("KickNRun"));

  if (behave.get() == 0)
      {
          Log("cannot create Behavior\n");
          return 1;
      }

  behave->SetTeamName(teamName);
  behave->SetWorldModel(wm);
  behave->SetCommServer(comm);

  return 0;
}

void Run()
{
  Log("AgentTest started\n");

  while (true)
    {
      if (! comm->GetInput())
        {
          break;
        }

      wm->Parse(comm->GetMsg());

      switch(comm->GetMsg()[0])
        {
        case 'D' :
            behave->ProcessInitMessage();
            break;

        case 'K' :
            behave->ProcessThinkTimeMessage();
          break;

        case 'S' :
            behave->Think();
          break;

        default:
          Log("received unknown data: ");
          Log(comm->GetMsg());
          Log("\n");
          break;
        }
    }
}

int main(int argc, char* argv[])
{
  int ret = Init(argc, argv);

  if (ret > 0)
      {
          return ret;
      }

  Run();
}
