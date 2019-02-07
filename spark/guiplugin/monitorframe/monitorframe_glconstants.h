/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
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

#ifndef MONITORFRAME_GLCONSTANTS_H
#define MONITORFRAME_GLCONSTANTS_H

/**
 * \file monitorframe_glconstants.h
 * \brief Contains a set of constants for OpenGL rendering in the Monitorframe.
 */

#include <QString>

namespace MonitorFrameUtil
{

/** default path to the fps controller in the zeitgeist tree*/
extern const QString MAINFRAME_FPS_CONTROLLER;

/** minimal axis width uses to render selected BaseNodes*/
extern const float GLRENDER_MIN_AXIS_WIDTH;

} //namespace MonitorFrameUtil

#endif //MONITORFRAME_GLCONSTANTS_H
