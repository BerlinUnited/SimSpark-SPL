/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamestateaspect.h,v 1.2 2004/02/12 14:07:26 fruit Exp $

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
#ifndef GAMESTATEASPECT_H
#define GAMESTATEASPECT_H

#include <soccer/soccercontrolaspect/soccercontrolaspect.h>
#include <set>

class AgentState;

class GameStateAspect : public SoccerControlAspect
{
protected:
    typedef std::set<int> TUnumSet;

public:
    GameStateAspect();
    virtual ~GameStateAspect();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.
    */
    virtual void Update(float deltaTime);

    /** returns the current play mode */
    TPlayMode GetPlayMode() const;

    /** sets the current play mode */
    void SetPlayMode(TPlayMode mode);

    /** set the current play mode randomly to PM_KickOff_Left or
        PM_KickOff_Right
    */
    void KickOff();

    /** returns the current game time */
    TTime GetTime() const;

    /** returns the time passed since the last playmode change */
    TTime GetModeTime();

    /** returns the time of the last playmode change */
    TTime GetLastModeChange();

    /** sets the current game half */
    void SetGameHalf(TGameHalf half);

    /** returns the current game half */
    TGameHalf GetGameHalf();

    /** sets the name of a team */
    void SetTeamName(TTeamIndex idx, std::string name);

    /** returns the name of a team */
    std::string GetTeamName(TTeamIndex idx);

    /** called from the InitEffector to request a uniformn number and
        teamname */
    bool RequestUniform(boost::shared_ptr<AgentState> agentState,
                        std::string teamName, unsigned int unum);

    /** returns the next uniform number not taken for the given team */
    int RequestUniformNumber(const TTeamIndex ti);

    /** called from the InitEffector to request an initial position
        for an agent */
    salt::Vector3f RequestInitPosition(const TTeamIndex ti);

    /** increments the score of a team */
    void ScoreTeam(TTeamIndex idx);

    /** returns the score of a team */
    int GetScore(TTeamIndex idx);

protected:
    /** setup the init positions for the agents */
    virtual void OnLink();

    /** advances the game time */
    void UpdateTime(float deltaTime);

    /** checks if the set of uniform numbers of given team already
        contains a uniform number unum and inserts it. */
    bool GameStateAspect::InsertUnum(TTeamIndex idx, int unum);

    /** returns the team index corresponding to the given teamName. If
        the teamname does not exist and less than two teams are
        registered, the given team name is registered.
    */
    TTeamIndex GetTeamIndex(const std::string& teamName);

protected:
    /** the current play mode */
    TPlayMode mPlayMode;

    /** the last time the play mode changed */
    TTime mLastModeChange;

    /** the current game time */
    TTime mTime;

    /** the current half of the game */
    TGameHalf mGameHalf;

    /** the team that had the last KickOff */
    TTeamIndex mLastKickOff;

    /** the names of the two teams */
    std::string mTeamName[2];

    /** the set of uniform number for each team */
    TUnumSet mUnumSet[2];

    /** the maximal uniform number handed out for each team */
    int mMaxUnum[2];

    /** the scores of two teams */
    int mScore[2];

    /** the position where the last left player was put initially */
    salt::Vector3f mLeftInit;

    /** the position where the last right player was put initially */
    salt::Vector3f mRightInit;

    /** the radius of an agent */
    float mAgentRadius;
};

DECLARE_CLASS(GameStateAspect);

#endif // GAMESTATEASPECT_H
