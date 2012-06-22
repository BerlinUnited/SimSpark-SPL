/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: netclient_c.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "netclient.h"

using namespace std;
using namespace oxygen;

FUNCTION(NetClient, getServer)
{
    return obj->GetServer();
}

FUNCTION(NetClient, setServer)
{
    string inServer;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inServer))
        )
        {
            return false;
        }

    obj->SetServer(inServer);
    return true;
}

FUNCTION(NetClient, setClientTypeTCP)
{
    obj->SetClientType(NetControl::ST_TCP);
    return true;
}

FUNCTION(NetClient, setClientTypeUDP)
{
    obj->SetClientType(NetControl::ST_UDP);
    return true;
}


FUNCTION(NetClient, getPort)
{
    return obj->GetPort();
}

FUNCTION(NetClient, setPort)
{
    int inPort;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inPort))
        )
        {
            return false;
        }

    obj->SetPort(inPort);
    return true;
}

void
CLASS(NetClient)::DefineClass()
{
    DEFINE_BASECLASS(oxygen/SimControlNode);
    DEFINE_FUNCTION(getServer);
    DEFINE_FUNCTION(setServer);
    DEFINE_FUNCTION(getPort);
    DEFINE_FUNCTION(setPort);
    DEFINE_FUNCTION(setClientTypeTCP);
    DEFINE_FUNCTION(setClientTypeUDP);
}
