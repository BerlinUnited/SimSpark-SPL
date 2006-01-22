/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Thu Jan 4 2006
   Copyright (C) 2006 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef UNIVERSALJOINTACTION_H
#define UNIVERSALJOINTACTION_H

#include <oxygen/gamecontrolserver/actionobject.h>

class UniversalJointAction : public oxygen::ActionObject
{
public:
    UniversalJointAction(const std::string& predicate, float velocity1, float velocity2)
        : ActionObject(predicate), mVelocityAxis1(velocity1), mVelocityAxis2(velocity2) 
    {};

    virtual ~UniversalJointAction() {}
    
    float GetMotorVelocity(oxygen::Joint::EAxisIndex idx) 
    { 
        if (idx == oxygen::Joint::AI_FIRST) 
            return mVelocityAxis1; 
        else 
            return mVelocityAxis2;
    }

protected:
    float mVelocityAxis1;
    float mVelocityAxis2;
};

#endif // UNIVERSALJOINTACTION_H
