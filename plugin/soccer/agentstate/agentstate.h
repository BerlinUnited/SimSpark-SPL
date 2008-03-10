/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentstate.h,v 1.6 2008/03/10 23:57:07 sgvandijk Exp $

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

#include <soccer/soccertypes.h>
#include <soccer/objectstate/objectstate.h>

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

    /** Set the battery state */
    void SetBattery(float battery);

    /** Get the motor temperature */
    float GetTemperature() const;

    /** Set the temperature */
    void SetTemperature(float temperature);

    /** reduce battery by the given amout.
     *  \param consumption the amount by which the battery is reduced if possible
     *  \return true if the battery if good enough for the given consumption
     */
    bool ReduceBattery(float consumption);

    /** Add a new message to the list */
    void AddMessage(const std::string& msg, float direction, bool teamMate);
    void AddSelfMessage(const std::string& msg);

    /** Get the first message from the list */
    bool GetMessage(std::string& msg, float& direction, bool teamMate);
    bool GetSelfMessage(std::string& msg);

protected:
    /** team index */
    TTeamIndex mTeamIndex;

    /** uniform number */
    int mUniformNumber;

    /** motor temperature */
    float mTemperature;

    /** battery state */
    float mBattery;

    /** self message */
    std::string mSelfMsg;

    /** team-mate's message */
    std::string mMateMsg;
    float mMateMsgDir;

    /** opponent's message */
    std::string mOppMsg;
    float mOppMsgDir;

    /** max hear capacity units */
    int mHearMax;
    /** hear capacity increase units when it's silent */
    int mHearInc;
    /** hear capacity decrease units when player hears a message */
    int mHearDecay;

    /** hear capacity for his team */
    int mHearMateCap;
    /** hear capacity for opponent team */
    int mHearOppCap;

    /** is there any message from myself */
    bool mIfSelfMsg;
    /** is there any message from teammate */
    bool mIfMateMsg;
    /** is there any message from oponnent */
    bool mIfOppMsg;

protected:
    virtual void OnUnlink();

};

DECLARE_CLASS(AgentState);

#endif // AGENTSTATE_H
