/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: hinge2action.h 3 2008-11-21 02:38:08Z hedayat $

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
#ifndef HINGE2ACTION_H
#define HINGE2ACTION_H

#include <oxygen/gamecontrolserver/actionobject.h>

class Hinge2Action : public oxygen::ActionObject
{
public:
    Hinge2Action(const std::string& predicate, float velocity)
        : ActionObject(predicate), mVelocity(velocity) {};

    virtual ~Hinge2Action() {}
    float GetMotorVelocity() { return mVelocity; }

protected:
    float mVelocity;
};

#endif // HINGE2ACTION_H
