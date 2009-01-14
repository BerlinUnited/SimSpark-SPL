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
#ifndef SOCCERRULEASPECT_H
#define SOCCERRULEASPECT_H

#include <soccercontrolaspect/soccercontrolaspect.h>
#include <soccertypes.h>
#include <ballstateaspect/ballstateaspect.h>
#include <gamestateaspect/gamestateaspect.h>

class AgentState;

namespace salt
{
    class AABB2;
}

namespace oxygen
{
    class Body;
    class AgentAspect;
    class Transform;
}

class SoccerRuleAspect : public SoccerControlAspect
{
public:
    typedef std::list<boost::shared_ptr<AgentState> > TAgentStateList;

public:
    SoccerRuleAspect();
    virtual ~SoccerRuleAspect();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.
    */
    virtual void Update(float deltaTime);

    /** Drop ball at its current position and move all players away
        by the free kick radius.
    */
    void DropBall();
    /** Drop ball at a given position and move all players away from that
        position by the free kick radius.
        \param pos position where the ball should be dropped-
    */
    void DropBall(salt::Vector3f pos);

    /** broadcast a said message to all players
        \param message said message-
        \param pos positon of the player-
        \param num uniform number-
        \param idx team index-
    */
    void Broadcast(const std::string& message, const salt::Vector3f& pos,
                   int number, TTeamIndex idx);

    /** Move all the players from a team which are too close to pos away
        from pos, with exception of the specified agent.
        \param pos the center of the area to be checked
        \param radius the radius of the area to be checked
        \param min_dist the minimum distance players will be moved away from pos
        \param idx the team which should be checked.

        If idx is TI_NONE, nothing will happen. Players are moved towards their
        side of the field. If they would leave the playing field, they are moved
        towards the X axis (the line from the left goal to the right goal).
    */
    void ClearPlayersWithException(const salt::Vector3f& pos, float radius, float min_dist,
                      TTeamIndex idx, boost::shared_ptr<AgentState> agentState);

    /** 
     * get the size of field, i.e. length and width
     * 
     * 
     * @return the length and width
     */
    salt::Vector2f GetFieldSize() const;
    
protected:
    /** rereads the current soccer script values */
    virtual void UpdateCachedInternal();

    /** set up the reference to the ball and field collider */
    virtual void OnLink();

    /** reset the reference to the ball and field recorder */
    virtual void OnUnlink();

    /** checks if the time for one game half has passed */
    void CheckTime();

    /** updates the RuleAspect during BeforeKickOff mode */
    void UpdateBeforeKickOff();

    /** updates the RuleAspect during KickOff mode */
    void UpdateKickOff(TTeamIndex idx = TI_NONE);

    /** updates the RuleAspect during KickIn mode */
    void UpdateKickIn(TTeamIndex ti = TI_NONE);

    /** updates the RuleAspect during FreeKick mode */
    void UpdateFreeKick(TTeamIndex ti = TI_NONE);

    /** updates the RuleAspect during Goal Kick mode */
    void UpdateGoalKick(TTeamIndex ti = TI_NONE);

    /** updates the RuleAspect during Corner Kick mode */
    void UpdateCornerKick(TTeamIndex ti = TI_NONE);

    /** update the RuleAspect during PlayOn mode */
    void UpdatePlayOn();

    /** update the RuleAspect during Goal mode */
    void UpdateGoal();

    /** update the RuleAspect during Offside mode */
    void UpdateOffside(TTeamIndex ti = TI_NONE);

    /** update the RuleAspect when the match is over */
    void UpdateGameOver();

    /** checks if the ball is not on the playing field and puts it
        back to its last valid position. returns true if the ball left
        the field
    */
    bool CheckBallLeftField();

    /** Checks if the ball is in one of the two goals and scores the
        appropriate team.
        \return true if the ball was in one of the goals.
    */
    bool CheckGoal();

    /** checks if the assistant referee should raise the flag for offside */
    bool CheckOffside();

    /** moves the ball to pos setting its linear and angular velocity to 0 */
    void MoveBall(const salt::Vector3f& pos);

    /** Move all the players from a team which are too close to pos away
        from pos.
        \param pos the center of the area to be checked
        \param radius the radius of the area to be checked
        \param min_dist the minimum distance players will be moved away from pos
        \param idx the team which should be checked.

        If idx is TI_NONE, nothing will happen. Players are moved towards their
        side of the field. If they would leave the playing field, they are moved
        towards the X axis (the line from the left goal to the right goal).
    */
    void ClearPlayers(const salt::Vector3f& pos, float radius,
                      float min_dist, TTeamIndex idx);
    /** Move all the players from a team inside an rectangular area away
        from that area.
        \param box the rectangular area to be checked
        \param min_dist the minimum distance players will be moved away from box
        \param idx the team which should be checked.

        If idx is TI_NONE, nothing will happen. Players are moved towards their
        side of the field.
    */
    void ClearPlayers(const salt::AABB2& box, float min_dist, TTeamIndex idx);

    /** 
     * clear the player before kick off, if the team is the kick off
     * side, the robots can be on his own half and the center circle,
     * otherwise the robots can only be on his own half except the
     * center circle
     *
     * @param idx the team which kick off
     */
    void ClearPlayersBeforeKickOff(TTeamIndex idx);

protected:
    /** reference to the body node of the Ball */
    boost::shared_ptr<oxygen::Body> mBallBody;

    /** reference to the GameStateAspect */
    CachedPath<GameStateAspect> mGameState;

    /** reference to the BallStateAspect */
    CachedPath<BallStateAspect> mBallState;

    /** the radius of the Ball */
    float mBallRadius;

    /** the length of the pause after a goal */
    float mGoalPauseTime;

    /** the length of the pause after the ball left the field */
    float mKickInPauseTime;

    /** the length of one game half */
    float mHalfTime;

    /** the time we wait before dropping the ball in play modes where only
        one team can touch the ball
    */
    float mDropBallTime;

    /** the point above the ground, where the ball left the field */
    salt::Vector3f mLastValidBallPos;
    /** the field length (in meters) */
    float mFieldLength;
    /** the field width (in meters) */
    float mFieldWidth;
    /** the goal width (in meters) */
    float mGoalWidth;
    /** the point on the field where we do the kick in, free kick etc. */
    salt::Vector3f mFreeKickPos;
    /** the distance opponents have to keep during free kicks, kick ins etc. */
    float mFreeKickDist;
    /** the (least) distance opponents will be moved away if they are to close
        during a free kick, kick in and situations like that. */
    float mFreeKickMoveDist;
    /** the distance from the back line the ball should be placed at for a goal kick */
    float mGoalKickDist;
    /** flag if the simulator should do the kick off automatically after the agent */
    bool mAutomaticKickOff;
    /** time to wait until we kick off automatically */
    float mWaitBeforeKickOff;
    /** flag if we want to play only one half of the match */
    bool mSingleHalfTime;
    // areas where opponents are not allowed in certain play modes
    /** bounding box for the right half of the field */
    salt::AABB2 mRightHalf;
    /** bounding box for the left half of the field */
    salt::AABB2 mLeftHalf;
    /** bounding box for the right penalty area */
    salt::AABB2 mRightPenaltyArea;
    /** bounding box for the left penalty area */
    salt::AABB2 mLeftPenaltyArea;

    /** say message size max limit */
    int mSayMsgSize;
    /** max distance that player can hear a message */
    float mAudioCutDist;

    /** list of players that are in offside position */
    std::vector<int> mInOffsideLeftPlayers;
    std::vector<int> mInOffsideRightPlayers;
    boost::shared_ptr<oxygen::AgentAspect> mPreLastCollidingAgent;

    /** first colliding player in playon mode */
    bool mFirstCollidingAgent;

    /** if agents don't pass don't worry about offside */
    bool mNotOffside;

    /** if last mode was not playon, clear offside lists*/
    bool mLastModeWasPlayOn;

    /** use offside law */
    bool mUseOffside;
};

DECLARE_CLASS(SoccerRuleAspect);


#endif // SOCCERRULEASPECT_H
