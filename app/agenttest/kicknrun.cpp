/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: kicknrun.cpp,v 1.1.2.6 2004/02/09 14:19:54 fruit Exp $

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

void KickNRun::BehaveKickOff()
{
    BehavePlayOn();
}

void KickNRun::BehavePlayOn()
{
    WorldModel::VisionSense ballSense =
        mWM->GetVisionSense(WorldModel::VO_BALL);

    // calculate kick position behind the ball, looking towards the
    // opponent goal
    Vector3f goal(
                  mWM->GetFieldLength(),
                  mWM->GetFieldWidth()/2.0,
                  0
                  );

    GetLog()->Debug() << "GOAL :" << goal[0] << " " << goal[1] << " " << goal[2] << "\n";

    Vector3f ballPos = mWM->GetPosition(ballSense);

    GetLog()->Debug() << "BALL :" << ballPos[0] << " " << ballPos[1] << " " << ballPos[2] << "\n";

    Vector3f goalDriveLine(goal - ballPos);
    goalDriveLine.Normalize();

    float minKickDist = mWM->GetMinimalKickDistance();
    goalDriveLine *= minKickDist;

    Vector3f kickPos = ballPos + goalDriveLine;

    GetLog()->Debug() << "KICK :" << kickPos[0] << " " << kickPos[1] << " " << kickPos[2] << "\n";

    // if we are far away from the kickPos drive towards it
    Vector3f myPos = mWM->GetMyPosition();
    GetLog()->Debug() << "MYPOS :" << myPos[0] << " " << myPos[1] << " " << myPos[2] << "\n";

    Vector3f kickDriveLine = kickPos - myPos;
    float kickPosDist = kickDriveLine.Length();
    kickDriveLine.Normalize();
    if (kickPosDist > 10)
        {
            // we are far away, go full power
            kickDriveLine *= 100;
            Drive(kickDriveLine);
        } else if (kickPosDist > minKickDist)
        {
            // we're getting closer, go slower
            kickDriveLine *= 20;
            Drive(kickDriveLine);
        } else
            {
                // we are close enough to kick the ball
                Kick(20,100);
            }
}

void KickNRun::Behave()
{
    switch (mWM->GetPlayMode())
        {
        case PM_BeforeKickOff :
            BehaveBeforeKickOff();
            break;

        case PM_KickOff :
            BehaveKickOff();
            break;

        case PM_PlayOn :
            BehavePlayOn();
            break;

        default:
            break;
        }
}




