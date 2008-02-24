/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: constants.cpp,v 1.1 2008/02/24 16:20:21 rollmark Exp $

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
#include "constants.h"

//! default simulation stepping [sec]
const float SIM_SIMSTEP = 0.02f;

// the default height of the log window pane [pixel]
const int LOGWND_DEFAULT_HEIGHT = 150;

// the update interval of the log window [milliseconds}
const int LOGWND_UPDATE_INTERVAL = 100;

// the update interval of fps controller while the simulation is paused */
const int FPS_UPDATE_INTERVAL = 40;

// the default width of the tree window pane [pixel]
const int TREEWND_DEFAULT_WIDTH = 270;

// the default path to the fps controller
const wxString MAINFRAME_FPS_CONTROLLER = wxT("/usr/scene/camera/physics/controller");

// minimal axis width uses to render selected BaseNodes
const float GLRENDER_MIN_AXIS_WIDTH = 2.0;


