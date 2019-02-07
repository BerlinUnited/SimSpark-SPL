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
#ifndef GAMETIMEPERCEPTOR_H
#define GAMETIMEPERCEPTOR_H

#include <oxygen/agentaspect/perceptor.h>
#include <soccertypes.h>

class GameStateAspect;
//class AgentState;

namespace oxygen
{
    class Predicate;
}

class GameTimePerceptor : public oxygen::Perceptor
{
public:
    GameTimePerceptor();
    virtual ~GameTimePerceptor();

    //! \return true, if valid data is available and false otherwise.
    virtual bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

protected:
    /** sets up the reference to the GameStateAspect */
    virtual void OnLink();

    /** resets the reference to the GameStateAspect */
    virtual void OnUnlink();

protected:
    //! a reference to the game state
    boost::shared_ptr<GameStateAspect> mGameState;
};

DECLARE_CLASS(GameTimePerceptor);

#endif // GAMESTATEPERCEPTOR_H
