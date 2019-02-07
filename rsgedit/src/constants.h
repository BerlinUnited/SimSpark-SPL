/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef RSGEDIT_CONSTANTS_H
#define RSGEDIT_CONSTANTS_H

#include <wx/defs.h>
#include <wx/string.h>

enum ESimState
  {
      S_PAUSED,
      S_RUNNING
  };

enum ECommandIds
    {
        ID_SIM_START = (wxID_HIGHEST+10),
        ID_SIM_PAUSE = (wxID_HIGHEST+11),
        ID_SIM_STEP = (wxID_HIGHEST+12),

        ID_VIEW_LOG  = (wxID_HIGHEST+20),
        ID_VIEW_TREE = (wxID_HIGHEST+21),
        ID_VIEW_TOOLBAR = (wxID_HIGHEST+22),

        ID_FILE_OPEN = (wxID_HIGHEST+23),
        ID_FILE_RELOAD = (wxID_HIGHEST+24),

        ID_HELP_ABOUT = (wxID_HIGHEST+25),

        ID_AGENT_OPEN = (wxID_HIGHEST+26),
        ID_AGENT_START = (wxID_HIGHEST+27),
        ID_AGENT_KILL = (wxID_HIGHEST+28),

        ID_SPARK_CONTEXT = (wxID_HIGHEST+29),

        ID_CONTEXT_PROPERTIES = (wxID_HIGHEST+30),
        ID_CONTEXT_EDIT_SOURCE = (wxID_HIGHEST+31),
        ID_CONTEXT_COPY_FILENAME = (wxID_HIGHEST+32),
        ID_CONTEXT_COPY_PATH = (wxID_HIGHEST+33),

        ID_LOG_CLEAR = (wxID_HIGHEST+34),
        ID_LOG_COPY = (wxID_HIGHEST+35),
        ID_LOG_CHANNEL_DEBUG = (wxID_HIGHEST+36),
        ID_LOG_CHANNEL_NORMAL = (wxID_HIGHEST+37),
        ID_LOG_CHANNEL_WARNING = (wxID_HIGHEST+38),
        ID_LOG_CHANNEL_ERROR = (wxID_HIGHEST+39),

        ID_FILE_SAVE = (wxID_HIGHEST+40),
        ID_CONTEXT_KINEMATIC = (wxID_HIGHEST+41),
        ID_FILE_SAVEAS = (wxID_HIGHEST+42),
        ID_FILE_NEW = (wxID_HIGHEST+43),

        ID_TAB_CLOSE = (wxID_HIGHEST+50),
        ID_TAB_CLOSE_OTHER = (wxID_HIGHEST+51),
        ID_TAB_STARTSCRIPT = (wxID_HIGHEST+52)
    };

//! default simulation stepping [sec]
extern const float SIM_SIMSTEP;

// the default height of the log window pane [pixel]
extern const int LOGWND_DEFAULT_HEIGHT;

// the update interval of the log window [milliseconds}
extern const int LOGWND_UPDATE_INTERVAL;

// the update interval of fps controller while the simulation is paused */
extern const int FPS_UPDATE_INTERVAL;

// the default width of the tree window pane [pixel]
extern const int TREEWND_DEFAULT_WIDTH;

// the default path to the fps controller
extern const wxString MAINFRAME_FPS_CONTROLLER;

// minimal axis width uses to render selected BaseNodes
extern const float GLRENDER_MIN_AXIS_WIDTH;

#endif // RSGEDIT_CONSTANTS_H
