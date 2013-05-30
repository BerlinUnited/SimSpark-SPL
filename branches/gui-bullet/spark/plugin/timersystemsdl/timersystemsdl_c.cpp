/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of rcssserver3D
   Thu Mar 24 2011
   Copyright (C) 2003-1011 RoboCup Soccer Server 3D Maintenance Group
   $Id: timersystemsdl_c.cpp 261 2011-03-25 00:54:16Z hedayat $

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
#include "timersystemsdl.h"

using namespace oxygen;

void CLASS(TimerSystemSDL)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/TimerSystem);
}
