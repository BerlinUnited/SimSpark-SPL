/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: kicknrun.cpp,v 1.1.2.2 2004/02/08 14:01:46 rollmark Exp $

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
#include "kicknrun.h"
#include <strstream>

using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace oxygen;
using namespace salt;

KickNRun::KickNRun() : Soccer()
{
}

KickNRun::~KickNRun()
{
}

void KickNRun::BehaveBeforeKickOff()
{
    // do nothing
}

void KickNRun::BehavePlayOn()
{
    WorldModel::VisionSense ballSense =
        mWM->GetVisionSense(WorldModel::VO_BALL);

    // agentradius + ballradius + maxKickDist
    if (ballSense.distance <= 0.22 + 0.111 + 0.04)
        {
            // kick the ball
            Kick(20,80);
        } else
            {
                // dash towards the ball
                Vector3f dashVec(mWM->GetDashVec(ballSense));
                dashVec *= 15;

                Dash(dashVec);
            }
}

void KickNRun::Behave()
{
    switch (mWM->GetPlayMode())
        {
        case PM_BeforeKickOff :
            BehaveBeforeKickOff();
            break;

        case PM_PlayOn :
            BehavePlayOn();
            break;

        default:
            break;
        }
}




