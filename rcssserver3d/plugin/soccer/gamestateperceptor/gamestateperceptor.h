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
#ifndef GAMESTATEPERCEPTOR_H
#define GAMESTATEPERCEPTOR_H

#include <oxygen/agentaspect/perceptor.h>
#include <soccertypes.h>

class GameStateAspect;
class AgentState;

namespace oxygen
{
    class Predicate;
}

class GameStatePerceptor : public oxygen::Perceptor
{
public:
    GameStatePerceptor();
    virtual ~GameStatePerceptor();

    //! \return true, if valid data is available and false otherwise.
    virtual bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

protected:
    /** sets up the reference to the GameStateAspect */
    virtual void OnLink();

    /** resets the reference to the GameStateAspect */
    virtual void OnUnlink();

    /** inserts predicate parameters the agent receives once after a
        successful init command into the predicate. These are it's
        uniform number and team as well as a subset of the soccer
        variables
     */
    void InsertInitialPercept(oxygen::Predicate& predicate);

    /** inserts a soccer variable and its current value into the
        predicate
    */
    void InsertSoccerParam(oxygen::Predicate& predicate,
                           const std::string& name);

    /**
     * @brief Returns the side name of the given team index
     * @param ti    the team index
     * @return      the side of the team (left, right, none)
     */
    std::string GetTeamSide(TTeamIndex ti);

    void GetTeamInfo(zeitgeist::ParameterList& info, TTeamIndex ti);
protected:
    //! a reference to the game state
    boost::shared_ptr<GameStateAspect> mGameState;

    //! a reference to the agentstate
    boost::shared_ptr<AgentState> mAgentState;

    /** true until Percept() is called the first time after the agent
        is assigned to a team with a successful init command
    */
    bool mFirstPercept;

    /** report game score to agent */
    bool mReportScore;
};

DECLARE_CLASS(GameStatePerceptor);

#endif // GAMESTATEPERCEPTOR_H
