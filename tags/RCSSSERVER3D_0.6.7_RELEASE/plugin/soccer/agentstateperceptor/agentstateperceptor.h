/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef AGENTSTATEPERCEPTOR_H
#define AGENTSTATEPERCEPTOR_H

#include <oxygen/agentaspect/perceptor.h>
#include <soccertypes.h>

class AgentState;

namespace oxygen
{
    class Predicate;
}

class AgentStatePerceptor : public oxygen::Perceptor
{
public:
    AgentStatePerceptor();
    virtual ~AgentStatePerceptor();

    //! \return true, if valid data is available and false otherwise.
    virtual bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

protected:
    /** sets up the reference to the AgentState */
    virtual void OnLink();

    /** resets the reference to the AgentState */
    virtual void OnUnlink();

protected:
    //! a reference to the agentstate
    boost::shared_ptr<AgentState> mAgentState;

    //! the percept percept rate is the rate SenseIntervals divided by number of percepts
    int mPerceptRate;

    //! sense interval counter for subsequent percepts
    int mSenses;
};

DECLARE_CLASS(AgentStatePerceptor);

#endif // AGENTSTATEPERCEPTOR_H
