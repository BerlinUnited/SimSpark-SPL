/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: monitorclientcontrol.h,v 1.2 2004/05/10 14:10:45 fruit Exp $

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

#include <oxygen/simulationserver/simcontrolnode.h>
#include <gamestate.h>

namespace oxygen
{
    class Transform;
}

class CommServer;

/** \class MonitorClientControl is a SimControlNode that handles the
    communication with the soccer server using the MonitorLib and
    updates the scene graph at the begin of each simulation cycle
 */
class MonitorClientControl : public oxygen::SimControlNode
{
public:
    /** called when a new simulation cycle starts, before the
        simulation is stepped */
    virtual void StartCycle();

protected:
    /** constructs the corresponding ball or returns an already existing instance */
    boost::shared_ptr<oxygen::Transform> GetBall();

    /** constructs the corresponding agent or returns an already existing instance */
    boost::shared_ptr<oxygen::Transform> GetAgent(TTeamIndex side, int unum);

    /** constructs the corresponding flag or returns an already existing instance */
    boost::shared_ptr<oxygen::Transform> GetFlag(GameState::EFlagType i);

    virtual void OnLink();

protected:
    /** game state data */
    GameState mGameState;

    /** the cached CommServer reference */
    boost::shared_ptr<CommServer> mCommServer;

};

DECLARE_CLASS(MonitorClientControl);
