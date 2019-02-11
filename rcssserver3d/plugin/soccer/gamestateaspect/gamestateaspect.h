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
#ifndef GAMESTATEASPECT_H
#define GAMESTATEASPECT_H

#include <soccercontrolaspect/soccercontrolaspect.h>
#include <set>
#include <vector>

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

    /** Used by the SexpMonitor to check if the terminating message should be sent.
        \return true if the match is over
    */
    bool IsFinished() const { return mFinished; }

    /** finish the simulation */
    void Finish() { mFinished = true; }

    /** returns the current play mode */
    TPlayMode GetPlayMode() const;

    virtual std::string GetPlayModeStr() const;

    TTeamIndex GetKickoffTeam() const;

    /** sets the current play mode */
    void SetPlayMode(TPlayMode mode);

    /** set the current play mode randomly to PM_KickOff_Left or PM_KickOff_Right.
        \param ti Team which should do the kick off (if TI_NONE, then do a coin toss)
    */
    virtual void KickOff(TTeamIndex ti = TI_NONE);

    /** returns the current game time */
    TTime GetTime() const;

    /** returns the time passed since the last playmode change */
    TTime GetModeTime() const;

    /** returns the time of the last playmode change */
    TTime GetLastModeChange() const;

    /** sets the current game half */
    void SetGameHalf(TGameHalf half);

    /** returns the current game half */
    TGameHalf GetGameHalf() const;

    /** sets the name of a team */
    void SetTeamName(TTeamIndex idx, const std::string& name);

    /** returns the name of a team */
    std::string GetTeamName(TTeamIndex idx) const;

    /** called from the InitEffector to request a uniformn number and
        teamname */
    bool RequestUniform(boost::shared_ptr<AgentState> agentState,
                        std::string teamName, unsigned int unum);

    /** notifies that a uniform number is free again */
    bool ReturnUniform(TTeamIndex ti, unsigned int unum, int type);

    /** returns the next uniform number not taken for the given team */
    int RequestUniformNumber(TTeamIndex ti) const;

    /** called from the InitEffector to request an initial position
        for an agent */
    salt::Vector3f RequestInitPosition(const TTeamIndex ti);

    /** called from the InitEffector to request an initial orientation
        for an agent */
    float RequestInitOrientation(TTeamIndex ti) const;

    /** increments the score of a team */
    void ScoreTeam(TTeamIndex idx);

    /** returns the score of a team */
    int GetScore(TTeamIndex idx) const;

    /** sets the current game time. useful if you start a game in the middle */
    void SetTime(float time);

    /** sets the current game scores. useful if you start a game in the middle */
    void SetScores(int scoreLeft, int scoreRight);

    /** returns if the game is paused */
    bool IsPaused() const;

    /** sets the game running state (paused or not) */
    void SetPaused(bool paused);

    /** swaps the team index (side) of current teams */
    void SwapTeamIndexes();

protected:
    /** setup the init positions for the agents */
    virtual void OnLink();

    /** advances the game time */
    void UpdateTime(float deltaTime);

    /** checks if the set of uniform numbers of given team already
        contains a uniform number unum and inserts it. */
    bool InsertUnum(TTeamIndex idx, int unum);

    /** checks if the set of uniform numbers of given team does
        contain a uniform number unum and erases it. */
    bool EraseUnum(TTeamIndex idx, int unum);

    /**
     * Adds a robot of the given type to the given team if permitted.
     * @return true if permitted
     */
    bool InsertRobotType(TTeamIndex idx, int type);

    /**
     * Removes a robot of the given type from the given team if exists.
     * @return true on success
     */
    bool EraseRobotType(TTeamIndex idx, int type);

    /** returns the team index corresponding to the given teamName. If
        the teamname does not exist and less than two teams are
        registered, the given team name is registered.
    */
    TTeamIndex GetTeamIndex(const std::string& teamName);

    /**
     * @param[in] idx the team index
     * @return the internal index used for indexing variables storing data
     * about teams. If idx is TI_NONE, it will return -1
     */
    int GetInternalIndex(TTeamIndex idx) const;

protected:
    /** the current play mode */
    TPlayMode mPlayMode;

    /** the last time the play mode changed */
    TTime mLastModeChange;

    /** the current game time */
    TTime mTime;

    /** the current time before the match started */
    TTime mLeadTime;

    /** the time in game state Game Over */
    TTime mFupTime;

    /** the current half of the game */
    TGameHalf mGameHalf;

    /** the team that had the last KickOff */
    TGameHalf mLastKickOffGameHalf;

    /** the team that has to start the next half */
    TTeamIndex mNextHalfKickOff;

    /** the internal index for a team, since its TTeamIndex might change in
     * the second half */
    int mInternalIndex[3];

    /** the names of the two teams */
    std::string mTeamName[2];

    /** the set of uniform number for each team */
    TUnumSet mUnumSet[2];

    /** the array of robot type counts for each team */
    std::vector<int> mRobotTypeCount[2];

    /** the scores of two teams */
    int mScore[2];

    /** the position where the last left player was put initially */
    salt::Vector3f mLeftInit;

    /** the position where the last right player was put initially */
    salt::Vector3f mRightInit;

    /** the radius of an agent */
    float mAgentRadius;

    /** flag if the simulation should be stopped */
    bool mFinished;

    /** flag if the game is running or paused (e.g. in goal_left/right state) */
    bool mGamePaused;

    /** the maximum number of players of a single type per team */
    int mMaxRobotTypeCount;

    /** the minimum number of different robot types per team */
    int mMinRobotTypesCount;

    /** the maximum sum of robots for any two robot types */
    int mMaxSumTwoRobotTypes;
};

DECLARE_CLASS(GameStateAspect);

#endif // GAMESTATEASPECT_H
