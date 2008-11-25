/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef SAYEFFECTOR_H
#define SAYEFFECTOR_H

#include <oxygen/agentaspect/effector.h>

namespace oxygen
{
       class AgentAspect;
}

class SoccerRuleAspect;

class AgentState;

class SayEffector : public oxygen::Effector
{
public:
    SayEffector();
    virtual ~SayEffector();

    /** realizes the action described by the ActionObject */
    virtual bool Realize(boost::shared_ptr<oxygen::ActionObject> action);

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate() { return "say"; }

    /** constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
    GetActionObject(const oxygen::Predicate& predicate);

    /** setup the reference to the ball body node */
    virtual void OnLink();

    /** remove the reference to the ball body node */
    virtual void OnUnlink();

    std::string GetText();

    bool IfText()const;

protected:
    /** reference to the agent aspect */
    boost::shared_ptr<oxygen::AgentAspect> mAgent;

    /** reference to the agentstate */
    boost::shared_ptr<AgentState> mAgentState;

    /** reference to the soccer rule aspect */
    boost::shared_ptr<SoccerRuleAspect> mSoccerRule;

private:
    /** said message */
    std::string mMessage;

    bool ifText;
};

DECLARE_CLASS(SayEffector);

#endif // SAYEFFECTOR_H
