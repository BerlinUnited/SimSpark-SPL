/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Sat April 12 2008
   Copyright (C) 2008 RoboCup Soccer Server 3D Maintenance Group
   $Id: behavior.cpp,v 1.2 2008/05/05 11:56:17 benpwd Exp $

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
#include "behavior.h"
#include <zeitgeist/logserver/logserver.h>

using namespace std;
using namespace boost;
using namespace zeitgeist;
using namespace oxygen;

void Behavior::OnLink()
{
    RegisterCachedPath(mTrainControl, "/sys/server/simulation/TrainControl");
    if (mTrainControl.expired())
    {
        GetLog()->Error()
            << "(Behavior) ERROR: (OnLink) TrainControl not found"
            << endl;
        return ;
    }

    mTrainControl->ClientConnect(shared_ptr<Behavior>(this));
}

void Behavior::OnUnlink()
{
    if (mTrainControl.get() == 0)
    {
        GetLog()->Error()
            << "(Behavior) ERROR: (OnUnlink) TrainControl not found"
            << endl;
        return ;
    }

    mTrainControl->ClientDisconnect(shared_ptr<Behavior>(this));
    mTrainControl.reset();
}

void Behavior::SetCommand(const string& command)
{
    mCommand = command;
}

string Behavior::GetCommand() const
{
    return mCommand;
}
