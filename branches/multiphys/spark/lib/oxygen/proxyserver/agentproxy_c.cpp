/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Thu Dec 31 2009
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004-2009 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentproxy_c.cpp 132 2010-01-01 22:01:05Z hedayat $

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
#include "agentproxy.h"

using namespace oxygen;
using namespace std;

void CLASS(AgentProxy)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Node);
}
