/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentstate.h,v 1.1.2.5 2004/02/08 22:10:03 fruit Exp $

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
#ifndef AGENTSTATE_H
#define AGENTSTATE_H

#include <soccer/objectstate/objectstate.h>
#include <soccer/soccertypes.h>

namespace oxygen
{
    class Body;
}

class AgentState : public ObjectState
{
    //
    // functions
    //
public:
    AgentState();
    virtual ~AgentState();

    /** set the team index */
    void SetTeamIndex(TTeamIndex idx);

    /** returns the team index */
    TTeamIndex GetTeamIndex() const;

    /** Set the uniform number.
     *
     * This sets both the uniform number as well as the object id
     * (only the representation is different).
     */
    void SetUniformNumber(int number);

    /** returns the uniform number as integer */
    int GetUniformNumber() const;

    /** Set the object id for perceptors.
     *
     * This method is the same as SetUniformNumber for AgentState.
     * If id is not an integer, the object ID will not be changed.
     *
     * \param id a new ID, an integer represented as std::string.
     * \param pt the percept type for which the ID is set
     */
    virtual void SetID(const std::string& id, TPerceptType pt = PT_Default);

    /** Get the battery state */
    float GetBattery() const;

    /** Get motor force that can be applied (flip vector if necessary) and reduce battery */
    salt::Vector3f ApplyMotorForce(salt::Vector3f force);

protected:
    /** team index */
    TTeamIndex mTeamIndex;

    /** uniform number */
    int mUniformNumber;

    /** motor temperature */
    float mTemperature;

    /** battery state */
    double mBattery;

    /** battery decay for dashing one second with an unit length dash vector */
    double mBatteryDecay;
};

DECLARE_CLASS(AgentState);

#endif // AGENTSTATE_H
