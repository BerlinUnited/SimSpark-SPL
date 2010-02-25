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

#ifndef ODEANGULARMOTOR_H
#define ODEANGULARMOTOR_H

#include <oxygen/physicsserver/int/angularmotorint.h>
#include "odejoint.h"

class AngularMotorImp : public oxygen::AngularMotorInt, public JointImp{

/** An angular motor allows the relative angular velocities of two
    bodies to be controlled. The angular velocity can be controlled on
    up to three axes, allowing torque motors and stops to be set for
    rotation about those axes.
    
    See physicsserver/int/angularmotorint for a documentation.
*/

public:
    AngularMotorImp();
    long CreateAngularMotor(long worldID);
    void SetModeUserMode(long jointID);
    void SetModeEulerMode(long jointID);
    int GetMode(long jointID);
    void SetNumAxes(int num, long jointID);
    int GetNumAxes(long jointID);
    void SetMotorAxis(int idx, int anchor, salt::Vector3f axis, long jointID);
    int GetAxisAnchor(int idx, long jointID);
    salt::Vector3f GetMotorAxis(int idx, long jointID);
    void SetAxisAngle(int idx, float degAngle, long jointID);
    float GetAxisAngle(int idx, long jointID);
    float GetAxisAngleRate(int idx, long jointID);
};

DECLARE_CLASS(AngularMotorImp);

#endif //ODEANGULARMOTOR_H
