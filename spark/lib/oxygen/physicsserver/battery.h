/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Copyright (C) 2011 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#ifndef OXYGEN_BATTERY_H
#define OXYGEN_BATTERY_H

#include <oxygen/physicsserver/physicsobject.h>

namespace oxygen
{

class OXYGEN_API Battery : public PhysicsObject
{
public:
    Battery();
    virtual ~Battery();
    virtual void OnLink();

    bool Consume(float v);

    void SetCapacity(float v) {mCapacity = v;}

    void SetPower(float v) { mPower = salt::gMin(mCapacity, v); }

    bool IsEmpty() const { return mPower <= 0; }

    float State() const { return mPower / mCapacity; }

protected:
    float mCapacity;
    float mPower;
};

DECLARE_CLASS(Battery);

} //namespace oxygen

#endif //OXYGEN_BATTERY_H
