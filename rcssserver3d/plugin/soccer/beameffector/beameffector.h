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
#ifndef BEAMEFFECTOR_H
#define BEAMEFFECTOR_H

#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <gamestateaspect/gamestateaspect.h>
#include <salt/random.h>

class BeamEffector : public oxygen::Effector
{
protected:
    typedef boost::shared_ptr<salt::UniformRNG<> > UniformRngPtr;

public:
    BeamEffector();
    virtual ~BeamEffector();

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "beam"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

protected:
    /** setup the reference to the agents body node */
    virtual void OnLink();

    /** remove the reference to the agents body node */
    virtual void OnUnlink();

    /** realizes the action described by the ActionObject */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

protected:
    /** the reference to the parents body node */
    boost::shared_ptr<oxygen::RigidBody> mBody;

    /** the reference to the GameState */
    boost::shared_ptr<GameStateAspect> mGameState;

    /** a reference to the agent state */
    boost::shared_ptr<AgentState> mAgentState;

    /** the cached field length */
    float mFieldLength;

    /** the cached field width */
    float mFieldWidth;

    /** the cached agent radius */
    float mAgentRadius;

    /** random number generator for noise */
    UniformRngPtr mNoiseRng;

    /** amount of noise added to beam X and Y values */
    float mBeamNoiseXY;

    /** amount of noise added to beam angle value */
    float mBeamNoiseAngle;

    /** Allow starting at any field position including on opponent's
        side of the field */
    bool mStartAnyFieldPosition;
};

DECLARE_CLASS(BeamEffector);

#endif // BEAMEFFECTOR_H
