/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D

   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#ifndef SPLRULE_H
#define SPLRULE_H

#include <soccerruleaspect/soccerruleaspect.h>
#include <soccertypes.h>
#include <ballstateaspect/ballstateaspect.h>
#include <splstate/splstate.h>

class AgentState;

namespace salt
{
    class AABB2;
}

namespace oxygen
{
    class RigidBody;
    class AgentAspect;
    class Transform;
}

class SPLRule : public SoccerRuleAspect
{
public:
    SPLRule();
    virtual ~SPLRule();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.
    */
    virtual void Update(float deltaTime);

    virtual void OnLink();

    virtual void OnUnlink();

    void UpdateInitialKickOff();

    void UpdateReady();

    void UpdateSet();

    void UpdatePlaying();

    salt::Vector3f getBallPositionAfterOutsideField(salt::Vector3f ballPos);

    bool checkIfGoal(salt::Vector3f ballPos);

    void CheckIllegalPosition(TTeamIndex idx);

    void CheckIllegalDefender(TTeamIndex idx);

    void manualPlacement(TTeamIndex idx);

    void CheckTime();

    void RemoveRobot(boost::shared_ptr<AgentState> robot) const;

    void Test(TTeamIndex idx);

    void MoveBall(salt::Vector3f toPosition);

    void HideBall();

    //TOOLS

    float clamp(float val, const float min, const float max);

protected:


    /** reference to the GameStateAspect */
    CachedPath<SPLState> mState;

    float mReadyDuration;
    float mSetDuration;
};

DECLARE_CLASS(SPLRule);


#endif // SPLRULE_H
