/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: joint.cpp 113 2009-12-03 09:04:19Z a-held $

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

#include "odejoint.h"

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

ODEJoint::ODEJoint() : ODEPhysicsObject() 
{
}

Joint* ODEJoint::GetJoint(long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    return static_cast<Joint*>(dJointGetData(ODEJoint));
}

bool ODEJoint::AreConnected(long bodyID1, long bodyID2){
    dBodyID ODEBody1 = (dBodyID) bodyID1;
    dBodyID ODEBody2 = (dBodyID) bodyID2;
    return dAreConnected(ODEBody1, ODEBody2) == 1;
}

bool ODEJoint::AreConnectedExcluding(long bodyID1, long bodyID2, int joint_type){
    dBodyID ODEBody1 = (dBodyID) bodyID1;
    dBodyID ODEBody2 = (dBodyID) bodyID2;
    return dAreConnectedExcluding(ODEBody1, ODEBody2, joint_type) == 1;
}

void ODEJoint::OnLink(long jointID, Joint* joint)
{
    dJointID ODEJoint = (dJointID) jointID;
    dJointSetData(ODEJoint, joint);
}

void ODEJoint::Attach(long bodyID1, long bodyID2, long jointID)
{
    dBodyID ODEBody1 = (dBodyID) bodyID1;
    dBodyID ODEBody2 = (dBodyID) bodyID2;
    dJointID ODEJoint = (dJointID) jointID;
    dJointAttach(ODEJoint, ODEBody1, ODEBody2);
}

int ODEJoint::GetType(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    return dJointGetType(ODEJoint);
}

long ODEJoint::GetBodyID(int idx, long jointID)
{
    dJointID ODEJoint = (dJointID) jointID;
    dBodyID ODEBodyID = dJointGetBody(ODEJoint, idx);
    return (long) ODEBodyID;
}

void ODEJoint::EnableFeedback(bool enable, long jointID, 
                              shared_ptr<GenericJointFeedback> feedback)
{
    dJointID ODEJoint = (dJointID) jointID;
    
    if (enable)
        {
            if (feedback.get() == 0)
                {
                    feedback = shared_ptr<GenericJointFeedback>(
                        (GenericJointFeedback*) new dJointFeedback());
                    memset(feedback.get(),0,sizeof(dJointFeedback));
                }
        } else
            {
                if (feedback.get() != 0)
                    {
                        feedback.reset();
                    }
            }

    cout << "bla";
    std::cin;
    dJointFeedback* ODEFeedback = (dJointFeedback*) feedback.get();

    dJointSetFeedback(ODEJoint,ODEFeedback);
}

bool ODEJoint::FeedbackEnabled(long jointID) const
{
    dJointID ODEJoint = (dJointID) jointID;
    return (dJointGetFeedback(ODEJoint) != 0);
}

Vector3f ODEJoint::GetFeedbackForce(int idx, 
                                    shared_ptr<GenericJointFeedback> feedback) const
{
    dJointFeedback* fb = (dJointFeedback*) feedback.get();
    if (fb == 0)
        {
            return Vector3f(0,0,0);
        }

    switch (idx)
        {
        case 0 :
            return Vector3f(
                            fb->f1[0],
                            fb->f1[1],
                            fb->f1[2]
                            );

        case 1 :
            return Vector3f(
                            fb->f2[0],
                            fb->f2[1],
                            fb->f2[2]
                            );

        default:
            return Vector3f(0,0,0);
        }
}

Vector3f ODEJoint::GetFeedbackTorque(int idx,
                                     shared_ptr<GenericJointFeedback> feedback) const
{
    dJointFeedback* fb = (dJointFeedback*) feedback.get();
    if (fb == 0)
        {
            return Vector3f(0,0,0);
        }

    switch (idx)
        {
        case 0 :
            return Vector3f(
                            fb->t1[0],
                            fb->t1[1],
                            fb->t1[2]
                            );

        case 1 :
            return Vector3f(
                            fb->t2[0],
                            fb->t2[1],
                            fb->t2[2]
                            );

        default:
            return Vector3f(0,0,0);
        }
}
    
void ODEJoint::SetFudgeFactor(int idx, float fudge_factor, long jointID)
{
    SetParameter(dParamFudgeFactor + (idx * dParamGroup), fudge_factor, jointID);
}

float ODEJoint::GetFudgeFactor(int idx, long jointID) const
{
    return GetParameter(dParamFudgeFactor + (idx * dParamGroup), jointID);
}

void ODEJoint::SetBounce(int idx, float bounce, long jointID)
{
    SetParameter(dParamBounce + (idx * dParamGroup),bounce, jointID);
}

float ODEJoint::GetBounce(int idx, long jointID) const
{
    return GetParameter(dParamBounce + (idx * dParamGroup), jointID);
}

void ODEJoint::SetLowStopPos(int idx, float pos, long jointID)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), pos, jointID);
}

float ODEJoint::GetLowStopPos(int idx, long jointID) const
{
    return GetParameter(dParamLoStop + (idx * dParamGroup), jointID);
}

void ODEJoint::SetHighStopPos(int idx, float pos, long jointID)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), pos, jointID);
}

float ODEJoint::GetHighStopPos(int idx, long jointID) const
{
    return GetParameter(dParamHiStop + (idx * dParamGroup), jointID);
}

void ODEJoint::SetLowStopDeg(int idx, float deg, long jointID)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), gDegToRad(deg), jointID);
}

float ODEJoint::GetLowStopDeg(int idx, long jointID) const
{
    return gRadToDeg(GetParameter(dParamLoStop + (idx * dParamGroup), jointID));
}

void ODEJoint::SetHighStopDeg(int idx, float deg, long jointID)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), gDegToRad(deg), jointID);
}

float ODEJoint::GetHighStopDeg(int idx, long jointID) const
{
    return gRadToDeg(GetParameter(dParamHiStop + (idx * dParamGroup), jointID));
}

void ODEJoint::SetCFM(int idx, float cfm, long jointID)
{
    SetParameter(dParamCFM + (idx * dParamGroup), cfm, jointID);
}

float ODEJoint::GetCFM(int idx, long jointID) const
{
    return GetParameter(dParamCFM + (idx * dParamGroup), jointID);
}

void ODEJoint::SetStopCFM(int idx, float cfm, long jointID)
{
    SetParameter(dParamStopCFM + (idx * dParamGroup), cfm, jointID);
}

float ODEJoint::GetStopCFM(int idx, long jointID) const
{
    return GetParameter(dParamStopCFM + (idx * dParamGroup), jointID);
}

void ODEJoint::SetStopERP(int idx, float erp, long jointID)
{
    SetParameter(dParamStopERP + (idx * dParamGroup), erp, jointID);
}

float ODEJoint::GetStopERP(int idx, long jointID) const
{
    return GetParameter(dParamStopERP + (idx * dParamGroup), jointID);
}

void ODEJoint::SetSuspensionERP(int idx, float erp, long jointID)
{
    SetParameter(dParamSuspensionERP + (idx * dParamGroup), erp, jointID);
}

float ODEJoint::GetSuspensionERP(int idx, long jointID) const
{
    return GetParameter(dParamSuspensionERP + (idx * dParamGroup), jointID);
}

void ODEJoint::SetSuspensionCFM(int idx, float cfm, long jointID)
{
    SetParameter(dParamSuspensionCFM + (idx * dParamGroup), cfm, jointID);
}

float ODEJoint::GetSuspensionCFM(int idx, long jointID) const
{
    return GetParameter(dParamSuspensionCFM + (idx * dParamGroup), jointID);
}

void ODEJoint::SetLinearMotorVelocity(int idx, float vel, long jointID)
{
    SetParameter(dParamVel + (idx * dParamGroup), vel, jointID);
}

float ODEJoint::GetLinearMotorVelocity(int idx, long jointID) const
{
    return GetParameter(dParamVel + (idx * dParamGroup), jointID);
}

void ODEJoint::SetAngularMotorVelocity(int idx, float deg, long jointID)
{
    SetParameter(dParamVel + (idx * dParamGroup), gDegToRad(deg), jointID);
}

float ODEJoint::GetAngularMotorVelocity(int idx, long jointID) const
{
    return gRadToDeg(GetParameter(dParamVel + (idx * dParamGroup), jointID));
}

void ODEJoint::SetMaxMotorForce(int idx, float f, long jointID)
{
    SetParameter(dParamFMax + (idx * dParamGroup), f, jointID);
}

float ODEJoint::GetMaxMotorForce(int idx, long jointID) const
{
    return GetParameter(dParamFMax + (idx * dParamGroup), jointID);
}

void ODEJoint::DestroyJoint(long jointID, 
                            shared_ptr<GenericJointFeedback> feedback)
{
    dJointID ODEJoint = (dJointID) jointID;
    EnableFeedback(false, jointID, feedback);
    dJointDestroy(ODEJoint);
}

void ODEJoint::SetParameter(int parameter, float value, long jointID){
    dJointID ODEJoint = (dJointID) jointID;
    int jointType = dJointGetType(ODEJoint);
    switch (jointType){
        case dJointTypeHinge: dJointSetHingeParam(ODEJoint, parameter, value);
                              break;
        case dJointTypeHinge2: dJointSetHinge2Param(ODEJoint, parameter, value);
                               break;
        case dJointTypeSlider: dJointSetSliderParam(ODEJoint, parameter, value);
                               break;
        case dJointTypeUniversal: dJointSetUniversalParam(ODEJoint, parameter, value);
                                  break;
        case dJointTypeAMotor: dJointSetAMotorParam(ODEJoint, parameter, value);
                               break; 
        default: return;
    }
}

float ODEJoint::GetParameter(int parameter, long jointID) const{
    dJointID ODEJoint = (dJointID) jointID;
    int jointType = dJointGetType(ODEJoint);
    switch (jointType){
        case dJointTypeHinge: return dJointGetHingeParam(ODEJoint, parameter);
        case dJointTypeHinge2: return dJointGetHinge2Param(ODEJoint, parameter);
        case dJointTypeSlider: return dJointGetSliderParam(ODEJoint, parameter);
        case dJointTypeUniversal: return dJointGetUniversalParam(ODEJoint, parameter);
        case dJointTypeAMotor: return dJointGetAMotorParam(ODEJoint, parameter);
        default: return 0;
    }
}
