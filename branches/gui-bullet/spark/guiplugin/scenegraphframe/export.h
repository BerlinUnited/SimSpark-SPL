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

#ifndef EXPORT_LIB
#define EXPORT_LIB
/**
 * \file scenegraphframe/export.h
 * \brief Exports the SceneGraphFrame class as a plugin from a shared library.
 */

#include "scenegraphframe.h"
#include <carbon.h>

PLUGINS_EXPORT_BEGIN()
PLUGIN_EXPORT(SceneGraphFrame)
PLUGINS_EXPORT_END()

#endif