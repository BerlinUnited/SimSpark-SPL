/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitorlib.cpp,v 1.1 2004/03/12 17:22:02 rollmark Exp $

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
#include "monitorlib.h"
#include "commserver.h"
#include "monitorparser.h"

using namespace zeitgeist;

MonitorLib::MonitorLib(Zeitgeist& zg)
{
  // register classes to zeitgeist
  zg.GetCore()->RegisterClassObject(new CLASS(CommServer), "rcssmonitor3d/");
  zg.GetCore()->RegisterClassObject(new CLASS(MonitorParser), "rcssmonitor3d/");
}

MonitorLib::~MonitorLib()
{
}
