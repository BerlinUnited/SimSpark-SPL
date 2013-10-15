/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: inputserver_c.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "inputserver.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;
using namespace std;

FUNCTION(InputServer,init)
{
    string inInputSysName;

    return (
            (in.GetSize() == 1) &&
            (in.GetValue(in.begin(), inInputSysName)) &&
            (obj->Init(inInputSysName))
            );
}

FUNCTION(InputServer,createDevice)
{
    string inDeviceName;

    return (
            (in.GetSize() == 1) &&
            (in.GetValue(in.begin(), inDeviceName)) &&
            (obj->CreateDevice(inDeviceName))
            );
}

FUNCTION(InputServer,reset)
{
    obj->Reset();
    return true;
}

FUNCTION(InputServer,bindCommand)
{
    string inDesc;
    int inCmd;

    return(
           (in.GetSize() == 2) &&
           (in.GetValue(in[0], inDesc)) &&
           (in.GetValue(in[1], inCmd)) &&
           (obj->BindCommand(inDesc,inCmd))
           );
}

FUNCTION(InputServer,setScanCodeMapping)
{
    string inName;

    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in.begin(),inName))
        )
        {
            return false;
        }

    obj->SetScanCodeMapping(inName);
    return true;
}

FUNCTION(InputServer,addCode)
{
    int inIc;
    string inName;
    int inNoMod;
    int inShiftMod;
    int inAltMod;

    if (
        (in.GetSize() != 5) ||
        (! in.GetValue(in[0],inIc)) ||
        (! in.GetValue(in[1],inName)) ||
        (! in.GetValue(in[2],inNoMod)) ||
        (! in.GetValue(in[3],inShiftMod)) ||
        (! in.GetValue(in[4],inAltMod))
        )
        {
            return  false;
        }

    obj->AddCode(inIc,inName,inNoMod,inShiftMod,inAltMod);
    return true;
}

void CLASS(InputServer)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Node);
    DEFINE_FUNCTION(init);
    DEFINE_FUNCTION(createDevice);
    DEFINE_FUNCTION(reset);
    DEFINE_FUNCTION(bindCommand);
    DEFINE_FUNCTION(setScanCodeMapping);
    DEFINE_FUNCTION(addCode);
}
