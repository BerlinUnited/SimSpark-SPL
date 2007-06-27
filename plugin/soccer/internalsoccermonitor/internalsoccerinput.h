/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: internalsoccerinput.h,v 1.2 2007/06/27 22:29:52 jamu Exp $

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
#ifndef INTERNALSOCCERINPUT_H
#define INTERNALSOCCERINPUT_H

#include <kerosin/inputserver/inputitem.h>
#include <kerosin/inputserver/inputcontrol.h>
#include <soccer/soccertypes.h>

namespace oxygen
{
    class FPSController;
    class Body;
}

class GameStateAspect;
class SoccerRuleAspect;

class InternalSoccerInput : public kerosin::InputItem
{
public:
    enum ECmds
        {
            CmdUser              = kerosin::InputControl::CmdUser,
            CmdKickOff           = CmdUser + 1,
            CmdMoveAgent         = CmdKickOff + 1,
            CmdDropBall          = CmdMoveAgent + 1,
            CmdShootBall         = CmdDropBall + 1,
            CmdMoveBall          = CmdShootBall + 1,
            CmdCameraLeftGoal    = CmdMoveBall + 1,
            CmdCameraLeftCorner  = CmdCameraLeftGoal + 1,
            CmdCameraMiddleLeft  = CmdCameraLeftCorner + 1,
            CmdCameraMiddleRight = CmdCameraMiddleLeft + 1,
            CmdCameraMiddle      = CmdCameraMiddleRight + 1,
            CmdCameraRightCorner = CmdCameraMiddle + 1,
            CmdCameraRightGoal   = CmdCameraRightCorner + 1,
            //JAN
            CmdFreeKickLeft      = CmdCameraRightGoal + 1,
            CmdFreeKickRight   = CmdFreeKickLeft + 1   
        };

public:
    InternalSoccerInput();
    virtual ~InternalSoccerInput();

    /** this method is called every time an input unknown to the
        parent InputControl node is received
    */
    virtual void ProcessInput(const kerosin::InputServer::Input& input);

    /** registers the soccermonitor command constants and cache
        references
    */
    virtual void OnLink();

    /** reset cached references */
    virtual void OnUnlink();

protected:
    /** cached reference to the monitor client */
    boost::shared_ptr<GameStateAspect> mGameState;
    
    /** cached reference to the soccer rule aspect */
    boost::shared_ptr<SoccerRuleAspect> mSoccerRule; 

    /** cached reference to the camera body */
    boost::shared_ptr<oxygen::Body> mCameraBody;

    /** cached reference to the camera body */
    boost::shared_ptr<oxygen::FPSController> mFPS;
    
    TTeamIndex mLastKickOff;
};

DECLARE_CLASS(InternalSoccerInput);

#endif // INTERNALSOCCERINPUT_H
