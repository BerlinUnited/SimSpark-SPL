/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: types.h,v 1.4 2004/04/20 11:46:35 fruit Exp $

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
#ifndef _TYPES_H
#define _TYPES_H

// communication params
#define MAX_MSG_LEN 4096

#define DEFAULT_PORT 12001
#define DEFAULT_HOST "localhost"

// mouse params
#define DEFAULT_MOUSE_POSX 0
#define DEFAULT_MOUSE_POSY 0

//soccer field params
#define DEFAULT_WIDTH  700
#define DEFAULT_HEIGHT 700

#define DEFAULT_FIELD_LENGTH 105.0
#define DEFAULT_FIELD_WIDTH  68.0
#define DEFAULT_FIELD_HEIGHT 20.0

#define DEFAULT_BORDER_SIZE  4.0
#define DEFAULT_LINE_WIDTH 0.08

#define DEFAULT_GOAL_WIDTH   7.32
#define DEFAULT_GOAL_DEPTH   2.0
#define DEFAULT_GOAL_HEIGHT  2.44

// agent params
#define DEFAULT_AGENT_MASS 75.0
#define DEFAULT_AGENT_RADIUS 0.22
#define DEFAULT_AGENT_MAXSPEED 10.0

// ball params
#define DEFAULT_BALL_RADIUS 0.111
#define DEFAULT_BALL_MASS 0.41

// camera mode
#define DEFAULT_AUTO_CAM 0

// default draw mode
#define DEFAULT_DRAW_UNUMS true

#endif // _TYPES_H
