/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: simulationserver_c.cpp,v 1.2.6.1 2007/05/31 14:17:03 jboedeck Exp $

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

#include "simulationserver.h"

using namespace oxygen;
using namespace std;

FUNCTION(SimulationServer,quit)
{
    obj->Quit();
    return true;
}

FUNCTION(SimulationServer,initControlNode)
{
    string inClassName;
    string inName;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inClassName)) ||
        (! in.GetValue(in[1],inName))
        )
        {
            return false;
        }

    return obj->InitControlNode(inClassName,inName);
}


FUNCTION(SimulationServer, getTime)
{
    return obj->GetTime();
}

FUNCTION(SimulationServer, setSimStep)
{
    float inDeltaTime;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inDeltaTime))
        )
        {
            return false;
        }

    obj->SetSimStep(inDeltaTime);
    return true;

}

FUNCTION(SimulationServer, getSimStep)
{
    return obj->GetSimStep();
}

FUNCTION(SimulationServer, setAutoTimeMode)
{
    bool inSet;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inSet))
        )
        {
            return false;
        }

    obj->SetAutoTimeMode(inSet);
    return true;
}

FUNCTION(SimulationServer, getAutoTimeMode)
{
    return obj->GetAutoTimeMode();
}

FUNCTION(SimulationServer, setMultiThreads)
{
    bool inSet;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0], inSet))
        )
        {
            return false;
        }

    obj->SetMultiThreads(inSet);
    return true;
}

void CLASS(SimulationServer)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Node);
    DEFINE_FUNCTION(quit);
    DEFINE_FUNCTION(initControlNode);
    DEFINE_FUNCTION(getTime);
    DEFINE_FUNCTION(setSimStep);
    DEFINE_FUNCTION(getSimStep);
    DEFINE_FUNCTION(setAutoTimeMode);
    DEFINE_FUNCTION(getAutoTimeMode);
    DEFINE_FUNCTION(setMultiThreads);
}
