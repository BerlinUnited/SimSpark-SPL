/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: ballstateaspect.h,v 1.1.2.1 2004/01/25 12:00:48 rollmark Exp $

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
#ifndef BALLLSTATEASPECT_H
#define BALLLSTATEASPECT_H

#include <oxygen/controlaspect/controlaspect.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/physicsserver/recorderhandler.h>

/** BallStateAspect is a ControlAspect that holds information about
    the current state of the ball in the simulation.
 */

class BallStateAspect : public oxygen::ControlAspect
{
public:
    BallStateAspect();
    virtual ~BallStateAspect();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.
    */
    virtual void Update(float deltaTime);

    /** set up the reference to the Ball collision recorder */
    virtual void OnLink();

    /** reset the reference to the Ball collision recorder */
    virtual void OnUnlink();

    /** returns the last agent that collided with the ball */
    boost::shared_ptr<oxygen::AgentAspect> GetLastCollidingAgent();

protected:
    /** reference to the Ball collision recorder */
    boost::shared_ptr<oxygen::RecorderHandler> mBallRecorder;

    /** holds a reference to the last agent that collided with the
        ball */
    boost::shared_ptr<oxygen::AgentAspect> mLastCollidingAgent;
};

DECLARE_CLASS(BallStateAspect);

#endif // BALLLSTATEASPECT_H
