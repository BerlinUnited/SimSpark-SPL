/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: netcontrol_c.cpp,v 1.1 2004/04/25 16:46:03 rollmark Exp $

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
#include "netcontrol.h"

using namespace oxygen;
using namespace std;

FUNCTION(NetControl, setServerTypeTCP)
{
    obj->SetServerType(NetControl::ST_TCP);
    return true;
}

FUNCTION(NetControl, setServerTypeUDP)
{
    obj->SetServerType(NetControl::ST_UDP);
    return true;
}

FUNCTION(NetControl, setServerPort)
{
    unsigned int inPort;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(), inPort))
        )
        {
            return false;
        }

    obj->SetServerPort(inPort);
    return true;
}

FUNCTION(NetControl, getServerPort)
{
    return obj->GetServerPort();
}

void CLASS(NetControl)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/SimControlNode);
    DEFINE_FUNCTION(setServerTypeTCP);
    DEFINE_FUNCTION(setServerTypeUDP);
    DEFINE_FUNCTION(setServerPort);
    DEFINE_FUNCTION(getServerPort);
}
