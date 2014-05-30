/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef HEARPERCEPTOR_H
#define HEARPERCEPTOR_H

#include <oxygen/agentaspect/perceptor.h>

class AgentState;
class GameStateAspect;

namespace oxygen
{
    class Predicate;
}

class HearPerceptor : public oxygen::Perceptor
{
public:
    HearPerceptor();
    virtual ~HearPerceptor();

    //! \return true, if valid data is available and false otherwise.
    virtual bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

protected:
    /** sets up the reference to the AgentState */
    virtual void OnLink();

    /** resets the reference to the AgentState */
    virtual void OnUnlink();

protected:
    /** reference to the agentstate */
    boost::shared_ptr<AgentState> mAgentState;
    
    /** reference to the GameStateAspect */
    boost::shared_ptr<GameStateAspect> mGameState;

    /** label agent messages with the team of the sender */
    bool mLabelMessages;

    
};

DECLARE_CLASS(HearPerceptor);

#endif // HEARPERCEPTOR_H
