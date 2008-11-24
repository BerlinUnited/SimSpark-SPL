/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: objectstate.cpp,v 1.5 2008/02/22 16:48:20 hedayat Exp $

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
#include "objectstate.h"

using namespace oxygen;

ObjectState::ObjectState() : SoccerNode()
{
}

ObjectState::~ObjectState()
{
}

void
ObjectState::SetPerceptName(const std::string& name, TPerceptType pt)
{
    mPerceptNames[pt] = name;
}

void
ObjectState::SetPerceptName(const std::string& name, TPerceptType pt1, TPerceptType pt2)
{
//    mPerceptNames[pt1] = "Player";
    mPerceptNames[pt1] = "P";
    mPerceptNames[pt2] = name;

}

std::string
ObjectState::GetPerceptName(TPerceptType pt) const
{
    TPerceptStringMap::const_iterator i = mPerceptNames.find(pt);
    if (i == mPerceptNames.end())
        return std::string();

    return i->second;
}

void
ObjectState::SetID(const std::string& id, TPerceptType pt)
{
    mIDs[pt] = id;
}

std::string
ObjectState::GetID(TPerceptType pt) const
{
    TPerceptStringMap::const_iterator i = mIDs.find(pt);
    if (i == mIDs.end())
        return std::string();

    return i->second;
}
