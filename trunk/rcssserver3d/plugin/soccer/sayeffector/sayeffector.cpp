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
#include "sayaction.h"
#include "sayeffector.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <agentstate/agentstate.h>
#include <soccerbase/soccerbase.h>
#include <soccerruleaspect/soccerruleaspect.h>
#include <boost/regex.hpp>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace std;

SayEffector::SayEffector()
    : oxygen::Effector()
{
}

SayEffector::~SayEffector()
{
}

bool
SayEffector::Realize(boost::shared_ptr<ActionObject> action)
{
    if (mSoccerRule.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SayEffector) Got no soccer rule aspect\n";
        return false;
    }

    if (mAgent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SayEffector) parent node is not derived from "
            << "BaseNode\n";
        return false;
    }

    if (mAgentState.get() == 0)
    {
        return false;
    }

    boost::shared_ptr<SayAction> sayAction =
        dynamic_pointer_cast<SayAction>(action);

    if (sayAction.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SayEffector) cannot realize an unknown ActionObject\n";
        return false;
    }

    sayAction->GetMessage(mMessage);
    ifText=true;

    // If characters outside allowed value range or " ", "(" or ")" are in mMessage, return false
    boost::regex allowedCharacterRange("[\x20-\x7E]*");
    if (!boost::regex_match(mMessage, allowedCharacterRange)
        || mMessage.find_first_of("() ") != std::string::npos)
    {
        std::string teamName = mAgentState->GetTeamIndex() == TI_RIGHT ? "Right" : "Left";

        // get the GameStateAspect
        boost::shared_ptr<GameStateAspect> mGameState = dynamic_pointer_cast<GameStateAspect>
            (SoccerBase::GetControlAspect(*this, "GameStateAspect"));
        if (mGameState.get() != 0)
        {
            teamName = mGameState->GetTeamName(mAgentState->GetTeamIndex());
        }
        GetLog()->Error()
            << "(SayEffector) found illegal character. Ignoring message ["
            << mMessage << "] from " << teamName << " " 
            << mAgentState->GetUniformNumber() << "\n";

        ifText=false;

        return false;
    }

    mSoccerRule->Broadcast(mMessage, mAgent->GetWorldTransform().Pos(),
        mAgentState->GetUniformNumber(), mAgentState->GetTeamIndex());

    return true;
}

string
SayEffector::GetText()
{
    ifText=false;
    return mMessage;
}

bool
SayEffector::IfText()const
{
    return ifText;
}

boost::shared_ptr<ActionObject>
SayEffector::GetActionObject(const Predicate& predicate)
{
    if (predicate.name != GetPredicate())
    {
        GetLog()->Error() << "ERROR: (SayEffector) invalid predicate"
                          << predicate.name << "\n";

        // some error happened
        return boost::shared_ptr<ActionObject>();
    }

    Predicate::Iterator iter = predicate.begin();

    std::string message;
    if (! predicate.AdvanceValue(iter, message))
    {
        GetLog()->Error()
            << "ERROR: (SayEffector) said message expected\n";

        // some error happened
        return boost::shared_ptr<ActionObject>();
    }

    // construct the SayAction object
    return boost::shared_ptr<SayAction>(new SayAction(GetPredicate(), message));

}

void
SayEffector::OnLink()
{
    SoccerBase::GetAgentState(*this,mAgentState);
    SoccerBase::GetSoccerRuleAspect(*this,mSoccerRule);

    mAgent = dynamic_pointer_cast<AgentAspect>(GetParent().lock());

    if (mAgent.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (SayEffector) parent node is not derived from "
            << "AgentAspect\n";
        return;
    }
}

void
SayEffector::OnUnlink()
{
    mAgent.reset();
    mAgentState.reset();
    mSoccerRule.reset();
}
