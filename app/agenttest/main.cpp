/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: main.cpp,v 1.10 2004/04/07 13:37:26 fruit Exp $

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
#include <sstream>
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

// the playes uniform number, if left to zero the server will allocate
// the next freee unum
int teamUnum = 0;

// set to 1 to write debug information to stdout
#define ENABLE_LOGGING 1

#ifdef ENABLE_LOGGING
void
Log(const char* out)
{
    printf(out);
    fflush(0);
}
#else
#define Log(x)
#endif

void
printHelp()
{
    cout << "\nusage: agenttest [options]" << endl;
    cout << "\noptions:" << endl;
    cout << " --help      prints this message." << endl;
    cout << " --teamname  sets the team name. " << endl;
    cout << " --unum      sets the uniform number." << endl;
    cout << "\n";
}

void
ReadOptions(int argc, char* argv[])
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
        } else if (strcmp( argv[i], "--unum" ) == 0 )
        {
            teamUnum = atoi(argv[i+1]);
            ++i;
            stringstream ss;
            ss << "setting uniform number to " << teamUnum << "\n";
            Log(ss.str().c_str());
        }
    }
}

int
Init(int argc, char* argv[], Zeitgeist& zg)
{
    // register agent classes
    zg.GetCore()->RegisterClassObject(new CLASS(CommServer), "");
    zg.GetCore()->RegisterClassObject(new CLASS(WorldModel), "");
    zg.GetCore()->RegisterClassObject(new CLASS(KickNRun), "");

    // run init script
    zg.GetCore()->GetScriptServer()->RunInitScript("agenttest.rb", "app/agenttest");

    // read command line options
    ReadOptions(argc,argv);

    // register and create the CommServer
    comm = shared_dynamic_cast<CommServer>
        (zg.GetCore()->Get("/sys/CommServer"));

    if (comm.get() == 0)
    {
        Log("cannot find CommServer\n");
        return 1;
    }

    // register and create the WorldModel
    wm = shared_dynamic_cast<WorldModel>
        (zg.GetCore()->Get("/sys/WorldModel"));

    if (wm.get() == 0)
    {
        Log("cannot find WorldModel\n");
        return 1;
    }

    // get the installed behavior
    behave = shared_dynamic_cast<Soccer>
        (zg.GetCore()->Get("sys/Behavior"));

    if (behave.get() == 0)
    {
        Log("cannot find Behavior\n");
        return 1;
    }

    behave->SetTeamName(teamName);
    behave->SetTeamUnum(teamUnum);

    return 0;
}

void
Run()
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

int
main(int argc, char* argv[])
{
    //init zeitgeist and oxygen
    Zeitgeist zg("." PACKAGE_NAME);
    Oxygen oyxgen(zg);

    int ret = Init(argc, argv, zg);

    if (ret > 0)
    {
        return ret;
    }

    Run();
}
