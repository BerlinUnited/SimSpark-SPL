/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: kickaction.h,v 1.1.2.1 2004/01/25 12:14:14 rollmark Exp $

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
    enum EKickType
        {
            KT_HORIZONTAL,
            KT_STEEP
        };

public:
    KickAction(const std::string& predicate, EKickType type, float power)
        : ActionObject(predicate), mKickType(type),mPower(power) {}

    virtual ~KickAction() {}

    /** returns the stored kick power */
    const float GetPower() { return mPower; }

    /** returns the stored kick type */
    EKickType GetType() { return mKickType; }

protected:
    /** the amount of force to be applied to be the ball */
    float mPower;

    /** the kick tyep to be applied to the ball */
    EKickType mKickType;
};

#endif // KICKACTION_H
