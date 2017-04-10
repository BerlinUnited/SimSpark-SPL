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
#ifndef BALLLSTATEASPECT_H
#define BALLLSTATEASPECT_H

#include <soccercontrolaspect/soccercontrolaspect.h>
#include <gamestateaspect/gamestateaspect.h>

/** BallStateAspect is a ControlAspect that holds information about
    the current state of the ball in the simulation.
 */
class Ball;

namespace oxygen
{
    class RecorderHandler;
    class AgentAspect;
}

class BallStateAspect : public SoccerControlAspect
{
public:
    BallStateAspect();
    virtual ~BallStateAspect();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.
    */
    virtual void Update(float deltaTime);

    /** returns list of agents colliding with the ball 
     */
    bool GetCollidingAgents
    (std::list<boost::shared_ptr<oxygen::AgentAspect> >& agents);

    /** returns the last agent that collided with the ball and the
        time when this happened*/
    bool GetLastCollidingAgent
    (boost::shared_ptr<oxygen::AgentAspect>& agent, TTime& time);

    /** returns the last agent that kicked the ball and the
        time when this happened*/
    bool GetLastKickingAgent
    (boost::shared_ptr<oxygen::AgentAspect>& agent, TTime& time);

    /** returns true if the ball over the playing field */
    bool GetBallOnField();

    /** returns the last valid position of the ball over the playing
        field */
    salt::Vector3f GetLastValidBallPosition();

    /** returns the goal in which the balls position is or TI_NONE
        otherwise */
    TTeamIndex GetGoalState();

    /** updates the reference to the last agent that collided with the
        ball
    */
    void UpdateLastCollidingAgent(boost::shared_ptr<oxygen::AgentAspect> agent);

    /** updates the reference to the last agent that kicked the ball */
    void UpdateLastKickingAgent(boost::shared_ptr<oxygen::AgentAspect> agent);

    /** returns if ball is currently colliding with an agent from the given
        team 
    */
    bool GetBallCollidingWithAgentTeam(TTeamIndex team);

protected:
    /** set up the reference to the ball and field collider */
    virtual void OnLink();

    /** reset the reference to the ball and field recorder */
    virtual void OnUnlink();

    /** updates the references to agents colliding with the ball */
    void UpdateCollidingAgents();

    /** checks if the ball is on the playing field an updates the
        mBallOnField flag
    */
    void UpdateBallOnField();

    /** checks if the ball is in one of the goals and updates the
        mGoalState member*/
    void UpdateGoalState();

    /** if the ball has a valid position, i.e. is on the field,
        remember it
     */
    void UpdateLastValidBallPos();

protected:
    /** reference to the Ball node */
    boost::shared_ptr<Ball> mBall;

    /** reference to the Ball collision recorder */
    boost::shared_ptr<oxygen::RecorderHandler> mBallRecorder;

    /** reference to the left goal recorder */
    boost::shared_ptr<oxygen::RecorderHandler> mLeftGoalRecorder;

    /** reference to the right goal recorder */
    boost::shared_ptr<oxygen::RecorderHandler> mRightGoalRecorder;

    /** list of references to agents currently colliding with the ball */
    std::list<boost::shared_ptr<oxygen::AgentAspect> > mCollidingAgents;

    /** holds a reference to the last agent that collided with the
        ball */
    boost::shared_ptr<oxygen::AgentAspect> mLastCollidingAgent;

    /** holds a reference to the last agent that kicked the ball */
    boost::shared_ptr<oxygen::AgentAspect> mLastKickingAgent;

    /** holds a reference to the GameStateAspect */
    CachedPath<GameStateAspect> mGameState;

    /** then time when the last agent collided with the ball */
    TTime mLastAgentCollisionTime;

    /** if ball is currently colliding with left team agent */
    bool mCollidingWithLeftTeamAgent;

    /** if ball is currently colliding with right team agent */
    bool mCollidingWithRightTeamAgent;

    /** then time when the last agent kicked the ball */
    TTime mLastAgentKickTime;

    /** true if the ball on the soccer field, i.e. not on the border
        surrounding the soccer field */
    bool mBallOnField;

    /** holds the last valid ball position */
    salt::Vector3f mLastValidBallPos;

    /** indicates in which goal the ball is, TI_NONE otherwise */
    TTeamIndex mGoalState;

    /** the cached ball radius */
    float mBallRadius;
    
    /** the cached field length */
    float mHalfFieldLength;

    /** the cached field width */
    float mHalfFieldWidth;
};

DECLARE_CLASS(BallStateAspect);

#endif // BALLLSTATEASPECT_H
