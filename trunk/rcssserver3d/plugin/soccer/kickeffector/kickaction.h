/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef KICKACTION_H
#define KICKACTION_H

#include <oxygen/gamecontrolserver/actionobject.h>
#include <salt/vector.h>

class KickAction : public oxygen::ActionObject
{
public:
    KickAction(const std::string& predicate, float phi, float power)
        : ActionObject(predicate), mPower(power), mKickAngle(phi) {}

    virtual ~KickAction() {}

    /** returns the stored kick power */
    float GetPower() const { return mPower; }

    /** returns the stored kick type */
    float GetAngle() { return mKickAngle; }

protected:
    /** the amount of force to be applied to be the ball */
    float mPower;

    /** the kick angle to be applied to the ball */
    float mKickAngle;
};

#endif // KICKACTION_H
