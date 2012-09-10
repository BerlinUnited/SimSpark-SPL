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
#ifndef GAMESTATEITEM_H
#define GAMESTATEITEM_H

#include <oxygen/monitorserver/monitoritem.h>
#include <soccerbase/soccerbase.h>

class GameStateAspect;

class GameStateItem : public oxygen::MonitorItem
{
public:
    GameStateItem();
    virtual ~GameStateItem();

    /** This function is called once for every MonitorSystem each time
     *  a new client connects. It should append predicates to a list
     *  that is sent using the active monitor
     */
    virtual void GetInitialPredicates(oxygen::PredicateList& pList);

    /** This function will be called periodically to append predicates
        to a list that is sent using the active monitor
     */
    virtual void GetPredicates(oxygen::PredicateList& pList);

    /** sets the reference to the GameStateAspect */
    virtual void OnLink();

    /** resets the reference to the GameStateAspect */
    virtual void OnUnlink();

protected:
    void ResetSentFlags();

    /** helper method that queries the ScriptServer for the float
        variable name and appends a predicate describing it to pList
    */
    void PutFloatParam(const std::string& name, oxygen::PredicateList& pList);

protected:
    boost::shared_ptr<GameStateAspect> mGameState;

    // flags for sent information

    //! flag if we already sent the left teamname
    bool mSentLeftTeamname;
    //! flag if we already sent the left teamname
    bool mSentRightTeamname;
    //! the last half sent out to monitors
    TGameHalf mLastHalf;
    //! the last left score sent out to monitors
    int mLastLeftScore;
    //! the last right score sent out to monitors
    int mLastRightScore;
    //! the last playmode sent out to monitors
    TPlayMode mLastPlayMode;
    //! flag if the monitors received field flags information
    bool mSentFlags;

};

DECLARE_CLASS(GameStateItem)

#endif // GAMESTATEITEM_H

