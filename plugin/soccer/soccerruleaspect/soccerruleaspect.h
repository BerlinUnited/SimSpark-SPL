/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerruleaspect.h,v 1.1.2.6 2004/02/10 20:45:06 rollmark Exp $

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

#include <soccer/soccercontrolaspect/soccercontrolaspect.h>

class GameStateAspect;
class BallStateAspect;

namespace oxygen
{
    class Body;
}

class SoccerRuleAspect : public SoccerControlAspect
{
public:
    SoccerRuleAspect();
    virtual ~SoccerRuleAspect();

    /** called during the update of the GameControlServer to allow the
        ControlAspect to perform any necessary checks.
    */
    virtual void Update(float deltaTime);

protected:
   /** set up the reference to the ball and field collider */
    virtual void OnLink();

    /** reset the reference to the ball and field recorder */
    virtual void OnUnlink();

    /** checks if the time for one game half has passed */
    void CheckTime();

    /** updates the RuleAspect during BeforeKickOff mode */
    void UpdateBeforeKickOff();

    /** updates the RuleAspect during KickOff mode */
    void UpdateKickOff();

    /** updates the RuleAspect during KickIn mode */
    void UpdateKickIn();

    /** update the RuleAspect during PlayOn mode */
    void UpdatePlayOn();

    /** update the RuleAspect during Goal mode */
    void UpdateGoal();

    /** checks if the ball is not on the playing field and puts it
        back to its last valid position. returns true if the ball left
        the field
    */
    bool CheckBallLeftField();

    /** checks if the ball is in one of the two goals and scores the
        appropriate team. returns true if the ball was in one of the
        goals
    */
    bool CheckGoal();

    /** moves the ball to pos setting its linear and angular velocity
        to 0 */
    void MoveBall(const salt::Vector3f& pos);

protected:
    /** reference to the body node of the Ball */
    boost::shared_ptr<oxygen::Body> mBallBody;

    /** reference to the GameStateAspect */
    boost::shared_ptr<GameStateAspect> mGameState;

    /** reference to the BallStateAspect */
    boost::shared_ptr<BallStateAspect> mBallState;

    /** the radius of the Ball */
    float mBallRadius;

    /** the length of the pause after a goal */
    float mGoalPauseTime;

    /** the length of the pause after the ball left the field */
    float mKickInPauseTime;

    /** the length of one game half */
    float mHalfTime;

    /** the point above the ground, where the ball left the field */
    salt::Vector3f mLastValidBallPos;
};

DECLARE_CLASS(SoccerRuleAspect);


#endif // SOCCERRULEASPECT_H
