/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef SOCCERCONTROLASPECT_H
#define SOCCERCONTROLASPECT_H

#include <oxygen/controlaspect/controlaspect.h>
#include <soccertypes.h>

class GameStateAspect;
class BallStateAspect;
class Ball;

namespace oxygen
{
    class RecorderHandler;
    class RigidBody;
}

/** \class SoccerControlAspect is the base class for all
    ControlAspects implemented in the soccer bundle. It serves as a
    place to collect utility functions and type definitions common to
    all ControlAspects in the soccer simulation
 */
class SoccerControlAspect : public oxygen::ControlAspect
{
public:
    SoccerControlAspect();
    virtual ~SoccerControlAspect();

    /** returns a reference to the RecorderHandler registered to the
        Ball node */
    boost::shared_ptr<oxygen::RecorderHandler> GetBallRecorder();

    /** returns a reference to the RecorderHandler registered to the
        left goal box */
    boost::shared_ptr<oxygen::RecorderHandler> GetLeftGoalRecorder();

    /** returns a reference to the RecorderHandler registered to the
        left goal box */
    boost::shared_ptr<oxygen::RecorderHandler> GetRightGoalRecorder();

protected:
    /** queries the SceneServer for the currently active scene */
    virtual void OnLink();

protected:
    /** cached scene path */
    std::string mScenePath;
};

DECLARE_ABSTRACTCLASS(SoccerControlAspect);

#endif // SOCCERCONTROLASPECT_H
