/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: soccer.cpp,v 1.4 2006/03/14 08:23:00 fruit Exp $

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
#include "soccer.h"
#include <sstream>

using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace oxygen;
using namespace salt;

Soccer::Soccer() : Behavior()
{
    mTeamName = "unnamed";
    mSensationCount = 0;
    mDriveVec = Vector3f(0,0,0);
}

Soccer::~Soccer()
{
}

void Soccer::InitAgent()
{
    GetLog()->Debug() << "sending init command\n";

    stringstream ss;
    ss << "A(init (unum " << mTeamUnum << ") (teamname " << mTeamName << "))";

    mComm->PutOutput(ss.str().c_str());
}

void Soccer::CreateAgent()
{
    // use the create effector to setup the Sensors and Effectors
    GetLog()->Debug() << "creating agent\n";

    mComm->PutOutput("A(create)");
}

void Soccer::Beam(const salt::Vector3f& position)
{
    GetLog()->Debug()
        << "beaming ["
        << position[0]
        << " " << position[1]
        << " " << position[2]
        << "]\n";

    stringstream ss;
    ss << "A(beam "
       << " " << position[0]
       << " " << position[1]
       << "  " << position[2]
       << ")\n";

    mComm->PutOutput(ss.str());
}

void Soccer::Kick(const float angle, const float power)
{
    GetLog()->Debug()
        << "kicking Ball"
        << " angle=" << angle
        << " power=" << power
        << "\n";

    stringstream ss;
    ss << "A(kick " << angle << " " << power << ")";

    mComm->PutOutput(ss.str());
}

void Soccer::Drive(const salt::Vector3f& driveVec)
{
    GetLog()->Debug()
        << "driving ["
        << driveVec[0] << " "
        << driveVec[1] << " "
        << driveVec[2]
        << "]\n";

    if (driveVec == mDriveVec)
    {
        // no explicit change needed
        GetLog()->Debug()
            << "not changing drive vector\n";
        return;
    }

    stringstream ss;
    ss << "A(drive"
       << " " << driveVec[0]
       << " " << driveVec[1]
       << "  " << driveVec[2]
       << ")";

    mComm->PutOutput(ss.str());
}

void
Soccer::PanTilt(float pan, float tilt)
{
    GetLog()->Debug() << "panning+tilting ["
                      << pan << " " << tilt << "]\n";

    if (pan == 0 && tilt == 0)
    {
        // no explicit change needed
        GetLog()->Debug() << "not changing camera\n";
        return;
    }

    stringstream ss;
    ss << "A(pantilt"  << " " << pan << " " << tilt << ")";

    mComm->PutOutput(ss.str());
}

void Soccer::Think()
{
    GetLog()->Debug()
        << "received sensation " << mSensationCount << "\n";

    switch (mSensationCount)
    {
    case 0:
        CreateAgent();
        break;

    case 1:
    case 2:
    case 4:
    case 5:
        break;

    case 3:
        InitAgent();
        break;

    default:
        Behave();
    }

    ++mSensationCount;

    mComm->PutOutput(DoneThinkingMessage());
}
