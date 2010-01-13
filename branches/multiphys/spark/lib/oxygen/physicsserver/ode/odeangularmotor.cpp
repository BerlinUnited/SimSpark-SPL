/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: angularmotor.cpp 127 2009-12-22 08:21:04Z a-held $

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

#include <oxygen/physicsserver/ode/odeangularmotor.h>

using namespace oxygen;
using namespace salt;

ODEAngularMotor::ODEAngularMotor() : ODEJoint(){

}

long ODEAngularMotor::CreateAngularMotor(long worldID){
    dWorldID ODEWorld = (dWorldID) worldID;
    dJointID ODEJoint = dJointCreateAMotor(ODEWorld, 0);
    return (long) ODEJoint;
}

void ODEAngularMotor::SetModeUserMode(long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetAMotorMode(ODEJoint, dAMotorUser);
}

void ODEAngularMotor::SetModeEulerMode(long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetAMotorMode(ODEJoint, dAMotorEuler);
}

int ODEAngularMotor::GetMode(long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    int motorMode = dJointGetAMotorMode(ODEJoint);
    if (motorMode = dAMotorUser) 
        return 0;
    //motor is in Euler mode
    return 1; 
}

void ODEAngularMotor::SetNumAxes(int num, long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetAMotorNumAxes(ODEJoint, num);
}

int ODEAngularMotor::GetNumAxes(long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    return dJointGetAMotorNumAxes(ODEJoint);
}

void ODEAngularMotor::SetMotorAxis(int idx, int anchor, Vector3f axis, long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    
    dJointSetAMotorAxis (ODEJoint, idx, anchor,
                        axis[0], axis[1], axis[2]);
}

int ODEAngularMotor::GetAxisAnchor(int idx, long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    return dJointGetAMotorAxisRel(ODEJoint, idx);
}

Vector3f ODEAngularMotor::GetMotorAxis(int idx, long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    dVector3 dAxis;
    dJointGetAMotorAxis(ODEJoint,idx,dAxis);
    return Vector3f(dAxis[0],dAxis[1],dAxis[2]);
}

void ODEAngularMotor::SetAxisAngle(int idx, float degAngle, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetAMotorAngle(ODEJoint, idx, gDegToRad(degAngle));
}

float ODEAngularMotor::GetAxisAngle(int idx, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    return gRadToDeg(dJointGetAMotorAngle(ODEJoint, idx));
}

float ODEAngularMotor::GetAxisAngleRate(int idx, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    return gRadToDeg(dJointGetAMotorAngleRate(ODEJoint,idx));
}
