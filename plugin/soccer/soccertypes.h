/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccertypes.h,v 1.1.2.5 2004/02/10 20:38:04 rollmark Exp $

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
    PM_BeforeKickOff,
    PM_KickOff_Left,
    PM_KickOff_Right,
    PM_PlayOn,
    PM_KickIn_Left,
    PM_KickIn_Right,
    PM_GameOver,
    PM_Goal_Left,
    PM_Goal_Right,
  };

/** mapping from TPlayMode to string constants */
#define STR_PM_BeforeKickOff "BeforeKickOff"
#define STR_PM_KickOff_Left "KickOff_Left"
#define STR_PM_KickOff_Right "KickOff_Right"
#define STR_PM_PlayOn "PlayOn"
#define STR_PM_KickIn_Left "KickIn_Left"
#define STR_PM_KickIn_Right "KickIn_Right"
#define STR_PM_GameOver "GameOver"
#define STR_PM_Goal_Left "Goal_Left"
#define STR_PM_Goal_Right "Goal_Right"
#define STR_PM_Unknown "unknown"

enum TTeamIndex
  {
    TI_NONE = -1,
    TI_LEFT = 0,
    TI_RIGHT = 1
  };

typedef float TTime;

enum TGameHalf
    {
        GH_FIRST = 0,
        GH_SECOND = 1
    };

#endif // SOCCERTYPES_H
