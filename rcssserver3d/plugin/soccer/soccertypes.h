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
#ifndef SOCCERTYPES_H
#define SOCCERTYPES_H

/** this file contains types common to all plugins in the soccer
    simulation
*/

#include <string>

enum TPlayMode
{
    // the order of the first 3 play modes should not be changed.
    PM_BeforeKickOff = 0,      /*!< before_kick_off:   before the match  */
    PM_KickOff_Left = 1,       /*!< kick_off_left:     kick off for the left team  */
    PM_KickOff_Right = 2,      /*!< kick_off_right:    kick off for the right team */
    PM_PlayOn,                 /*!< play_on:           regular game play */
    PM_KickIn_Left,
    PM_KickIn_Right,
    PM_CORNER_KICK_LEFT,       /*!< corner_kick_l:     corner kick left team   */
    PM_CORNER_KICK_RIGHT,      /*!< corner_kick_r:     corner kick right team  */
    PM_GOAL_KICK_LEFT,         /*!< goal_kick_l:       goal kick for left team */
    PM_GOAL_KICK_RIGHT,        /*!< goal_kick_r:       goal kick for right team*/
    PM_OFFSIDE_LEFT,           /*!< offside_l:         offside for left team   */
    PM_OFFSIDE_RIGHT,          /*!< offside_r:         offside for right team  */
    PM_GameOver,
    PM_Goal_Left,
    PM_Goal_Right,
    PM_FREE_KICK_LEFT,         /*!< free_kick_l:       free kick for left team */
    PM_FREE_KICK_RIGHT,        /*!< free_kick_r:       free kick for right team*/
    PM_DIRECT_FREE_KICK_LEFT,  /*!< direct_free_kick_l:       direct free kick for left team */
    PM_DIRECT_FREE_KICK_RIGHT, /*!< direct_free_kick_r:       direct free kick for right team*/
    PM_PASS_LEFT,
    PM_PASS_RIGHT,
    PM_NONE                    /*!< no play mode, this must be the last entry */
};

/** mapping from TPlayMode to string constants */
#define STR_PM_BeforeKickOff "BeforeKickOff"
#define STR_PM_KickOff_Left "KickOff_Left"
#define STR_PM_KickOff_Right "KickOff_Right"
#define STR_PM_PlayOn "PlayOn"
#define STR_PM_KickIn_Left "KickIn_Left"
#define STR_PM_KickIn_Right "KickIn_Right"
#define STR_PM_CORNER_KICK_LEFT "corner_kick_left"
#define STR_PM_CORNER_KICK_RIGHT "corner_kick_right"
#define STR_PM_GOAL_KICK_LEFT "goal_kick_left"
#define STR_PM_GOAL_KICK_RIGHT "goal_kick_right"
#define STR_PM_OFFSIDE_LEFT "offside_left"
#define STR_PM_OFFSIDE_RIGHT "offside_right"
#define STR_PM_GameOver "GameOver"
#define STR_PM_Goal_Left "Goal_Left"
#define STR_PM_Goal_Right "Goal_Right"
#define STR_PM_FREE_KICK_LEFT "free_kick_left"
#define STR_PM_FREE_KICK_RIGHT "free_kick_right"
#define STR_PM_DIRECT_FREE_KICK_LEFT "direct_free_kick_left"
#define STR_PM_DIRECT_FREE_KICK_RIGHT "direct_free_kick_right"
#define STR_PM_PASS_LEFT "pass_left"
#define STR_PM_PASS_RIGHT "pass_right"
#define STR_PM_Unknown "unknown"

enum TTeamIndex
{
    TI_NONE = 0,
    TI_LEFT = 1,
    TI_RIGHT = 2
};

typedef float TTime;

enum TGameHalf
{
    GH_NONE = 0,
    GH_FIRST = 1,
    GH_SECOND = 2
};

namespace spl
{
enum TSPLState
{
  Initial,
  Ready,
  Set,
  Playing,
  Finished,
  Penalized,
  Unknown
};

enum TSPLPenalty
{
    penalty_none,
    penalty_illegal_ball_contact,
    penalty_player_pushing,
    penalty_illegal_motion_in_set,
    penalty_inactive_player,
    penalty_illegal_defender,
    penalty_leaving_the_field,
    penalty_kick_off_goal,
    penalty_request_for_pickup,
    penalty_local_game_stuck,
    penalty_illegal_positioning,
    penalty_substitute,
    penalty_manual
};
}

#define STR_SPL_STATE_INITIAL "initial"
#define STR_SPL_STATE_READY "ready"
#define STR_SPL_STATE_SET "set"
#define STR_SPL_STATE_PLAYING "playing"
#define STR_SPL_STATE_FINISHED "finished"
#define STR_SPL_STATE_PENALIZED "penalized"
#define STR_SPL_STATE_UNKNOWN "unknown"

#define STR_SPL_PENALTY_NONE "penalty_none"
#define STR_SPL_PENALTY_ILLEGAL_BALL_CONTACT "illegal_ball_contact"
#define STR_SPL_PENALTY_PLAYER_PUSHING "player_pushing"
#define STR_SPL_PENALTY_ILLEGAL_MOTION_IN_SET "illegal_motion_in_set"
#define STR_SPL_PENALTY_INACTIVE_PLAYER "inactive_player"
#define STR_SPL_PENALTY_ILLEGAL_DEFENDER "illegal_defender"
#define STR_SPL_PENALTY_LEAVING_THE_FIELD "leaving_the_field"
#define STR_SPL_PENALTY_KICK_OFF_GOAL "kick_off_goal"
#define STR_SPL_PENALTY_REQUEST_FOR_PICKUP "request_for_pickup"
#define STR_SPL_PENALTY_LOCAL_GAME_STUCK "local_game_stuck"
#define STR_SPL_PENALTY_ILLEGAL_POSITIONING "illegal_positioning"
#define STR_SPL_PENALTY_SUBSTITUTE "substitute"
#define STR_SPL_PENALTY_MANUAL "manual"

#endif // SOCCERTYPES_H
