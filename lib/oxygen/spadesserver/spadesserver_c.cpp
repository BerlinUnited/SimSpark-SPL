/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesserver_c.cpp,v 1.2.2.1 2003/12/26 16:28:39 fruit Exp $

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

#include "spadesserver.h"

using namespace oxygen;

FUNCTION(queueAgents)
{
    SpadesServer* ss = static_cast<SpadesServer*>(obj);
    switch (in.size())
    {
    case 0:
        ss->QueueAgents();
        break;
    case 1:
        ss->QueueAgents(boost::any_cast<char*>(in[0]));
        break;
    case 2:
        ss->QueueAgents(boost::any_cast<char*>(in[0]), boost::any_cast<int>(in[1]));
        break;
    default:
        ;
    }
}

void CLASS(SpadesServer)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Node);
    DEFINE_FUNCTION(queueAgents);
}
