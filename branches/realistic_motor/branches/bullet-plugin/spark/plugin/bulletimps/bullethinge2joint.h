/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef BULLETHINGE2JOINT_H
#define BULLETHINGE2JOINT_H

#include "bulletgeneric6dofjoint.h"
#include <oxygen/physicsserver/int/hinge2jointint.h>

class Hinge2JointImp : public oxygen::Hinge2JointInt, public Generic6DOFJointImp
{
    /** See physicsserver/int/hinge2jointint.h for documentation. */
    
public:
    Hinge2JointImp();
    long CreateHinge2Joint(long worldID);
    void SetAnchor(const salt::Vector3f& gAnchor, 
                   const salt::Vector3f& up, 
                   const salt::Vector3f& right,
                   long jointID);
    salt::Vector3f GetAnchor1(long jointID);
    salt::Vector3f GetAnchor2(long jointID);
    float GetAngle(long jointID);
    float GetAngleRate1(long jointID);
    float GetAngleRate2(long jointID);
};

DECLARE_CLASS(Hinge2JointImp);

#endif //BULLETHINGE2JOINT_H
