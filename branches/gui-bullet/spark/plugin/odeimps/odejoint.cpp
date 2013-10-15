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

#include "odejoint.h"

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

JointImp::JointImp() : PhysicsObjectImp()
{
}

Joint* JointImp::GetJoint(long jointID){
    dJointID JointImp = (dJointID) jointID;
    return static_cast<Joint*>(dJointGetData(JointImp));
}

bool JointImp::AreConnected(long bodyID1, long bodyID2){
    dBodyID ODEBody1 = (dBodyID) bodyID1;
    dBodyID ODEBody2 = (dBodyID) bodyID2;
    return dAreConnected(ODEBody1, ODEBody2) == 1;
}

bool JointImp::AreConnectedExcluding(long bodyID1, long bodyID2, int joint_type){
    dBodyID ODEBody1 = (dBodyID) bodyID1;
    dBodyID ODEBody2 = (dBodyID) bodyID2;
    return dAreConnectedExcluding(ODEBody1, ODEBody2, joint_type) == 1;
}

void JointImp::OnLink(long jointID, Joint* joint)
{
    dJointID JointImp = (dJointID) jointID;
    dJointSetData(JointImp, joint);
}

void JointImp::Attach(long bodyID1, long bodyID2, long jointID)
{
    dBodyID ODEBody1 = (dBodyID) bodyID1;
    dBodyID ODEBody2 = (dBodyID) bodyID2;
    dJointID JointImp = (dJointID) jointID;
    dJointAttach(JointImp, ODEBody1, ODEBody2);
}

int JointImp::GetType(long jointID) const
{
    dJointID JointImp = (dJointID) jointID;
    return dJointGetType(JointImp);
}

long JointImp::GetBodyID(int idx, long jointID)
{
    dJointID JointImp = (dJointID) jointID;
    dBodyID ODEBodyID = dJointGetBody(JointImp, idx);
    return (long) ODEBodyID;
}

void JointImp::EnableFeedback(bool enable, long jointID,
                              boost::shared_ptr<GenericJointFeedback>& feedback)
{
    dJointID JointImp = (dJointID) jointID;

    if (enable)
        {
            if (feedback.get() == 0)
                {
                    feedback = boost::shared_ptr<GenericJointFeedback>(
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

    dJointFeedback* ODEFeedback = (dJointFeedback*) feedback.get();

    dJointSetFeedback(JointImp,ODEFeedback);
}

bool JointImp::FeedbackEnabled(long jointID) const
{
    dJointID JointImp = (dJointID) jointID;
    return (dJointGetFeedback(JointImp) != 0);
}

Vector3f JointImp::GetFeedbackForce(int idx,
                                    boost::shared_ptr<GenericJointFeedback> feedback) const
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

Vector3f JointImp::GetFeedbackTorque(int idx,
                                     boost::shared_ptr<GenericJointFeedback> feedback) const
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

void JointImp::SetFudgeFactor(int idx, float fudge_factor, long jointID)
{
    SetParameter(dParamFudgeFactor + (idx * dParamGroup), fudge_factor, jointID);
}

float JointImp::GetFudgeFactor(int idx, long jointID) const
{
    return GetParameter(dParamFudgeFactor + (idx * dParamGroup), jointID);
}

void JointImp::SetBounce(int idx, float bounce, long jointID)
{
    SetParameter(dParamBounce + (idx * dParamGroup),bounce, jointID);
}

float JointImp::GetBounce(int idx, long jointID) const
{
    return GetParameter(dParamBounce + (idx * dParamGroup), jointID);
}

void JointImp::SetLowStopPos(int idx, float pos, long jointID)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), pos, jointID);
}

float JointImp::GetLowStopPos(int idx, long jointID) const
{
    return GetParameter(dParamLoStop + (idx * dParamGroup), jointID);
}

void JointImp::SetHighStopPos(int idx, float pos, long jointID)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), pos, jointID);
}

float JointImp::GetHighStopPos(int idx, long jointID) const
{
    return GetParameter(dParamHiStop + (idx * dParamGroup), jointID);
}

void JointImp::SetLowStopDeg(int idx, float deg, long jointID)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), gDegToRad(deg), jointID);
}

float JointImp::GetLowStopDeg(int idx, long jointID) const
{
    return gRadToDeg(GetParameter(dParamLoStop + (idx * dParamGroup), jointID));
}

void JointImp::SetHighStopDeg(int idx, float deg, long jointID)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), gDegToRad(deg), jointID);
}

float JointImp::GetHighStopDeg(int idx, long jointID) const
{
    return gRadToDeg(GetParameter(dParamHiStop + (idx * dParamGroup), jointID));
}

void JointImp::SetCFM(int idx, float cfm, long jointID)
{
    SetParameter(dParamCFM + (idx * dParamGroup), cfm, jointID);
}

float JointImp::GetCFM(int idx, long jointID) const
{
    return GetParameter(dParamCFM + (idx * dParamGroup), jointID);
}

void JointImp::SetStopCFM(int idx, float cfm, long jointID)
{
    SetParameter(dParamStopCFM + (idx * dParamGroup), cfm, jointID);
}

float JointImp::GetStopCFM(int idx, long jointID) const
{
    return GetParameter(dParamStopCFM + (idx * dParamGroup), jointID);
}

void JointImp::SetStopERP(int idx, float erp, long jointID)
{
    SetParameter(dParamStopERP + (idx * dParamGroup), erp, jointID);
}

float JointImp::GetStopERP(int idx, long jointID) const
{
    return GetParameter(dParamStopERP + (idx * dParamGroup), jointID);
}

void JointImp::SetSuspensionERP(int idx, float erp, long jointID)
{
    SetParameter(dParamSuspensionERP + (idx * dParamGroup), erp, jointID);
}

float JointImp::GetSuspensionERP(int idx, long jointID) const
{
    return GetParameter(dParamSuspensionERP + (idx * dParamGroup), jointID);
}

void JointImp::SetSuspensionCFM(int idx, float cfm, long jointID)
{
    SetParameter(dParamSuspensionCFM + (idx * dParamGroup), cfm, jointID);
}

float JointImp::GetSuspensionCFM(int idx, long jointID) const
{
    return GetParameter(dParamSuspensionCFM + (idx * dParamGroup), jointID);
}

void JointImp::SetLinearMotorVelocity(int idx, float vel, long jointID)
{
    SetParameter(dParamVel + (idx * dParamGroup), vel, jointID);
}

float JointImp::GetLinearMotorVelocity(int idx, long jointID) const
{
    return GetParameter(dParamVel + (idx * dParamGroup), jointID);
}

void JointImp::SetAngularMotorVelocity(int idx, float deg, long jointID)
{
    SetParameter(dParamVel + (idx * dParamGroup), gDegToRad(deg), jointID);
}

float JointImp::GetAngularMotorVelocity(int idx, long jointID) const
{
    return gRadToDeg(GetParameter(dParamVel + (idx * dParamGroup), jointID));
}

void JointImp::SetMaxMotorForce(int idx, float f, long jointID)
{
    SetParameter(dParamFMax + (idx * dParamGroup), f, jointID);
}

float JointImp::GetMaxMotorForce(int idx, long jointID) const
{
    return GetParameter(dParamFMax + (idx * dParamGroup), jointID);
}

void JointImp::DestroyJoint(long jointID,
                            boost::shared_ptr<GenericJointFeedback> feedback)
{
    dJointID JointImp = (dJointID) jointID;
    EnableFeedback(false, jointID, feedback);
    dJointDestroy(JointImp);
}

void JointImp::SetParameter(int parameter, float value, long jointID){
    dJointID JointImp = (dJointID) jointID;
    int jointType = dJointGetType(JointImp);
    switch (jointType){
        case dJointTypeHinge: dJointSetHingeParam(JointImp, parameter, value);
                              break;
        case dJointTypeHinge2: dJointSetHinge2Param(JointImp, parameter, value);
                               break;
        case dJointTypeSlider: dJointSetSliderParam(JointImp, parameter, value);
                               break;
        case dJointTypeUniversal: dJointSetUniversalParam(JointImp, parameter, value);
                                  break;
        case dJointTypeAMotor: dJointSetAMotorParam(JointImp, parameter, value);
                               break;
        default: return;
    }
}

float JointImp::GetParameter(int parameter, long jointID) const{
    dJointID JointImp = (dJointID) jointID;
    int jointType = dJointGetType(JointImp);
    switch (jointType){
        case dJointTypeHinge: return dJointGetHingeParam(JointImp, parameter);
        case dJointTypeHinge2: return dJointGetHinge2Param(JointImp, parameter);
        case dJointTypeSlider: return dJointGetSliderParam(JointImp, parameter);
        case dJointTypeUniversal: return dJointGetUniversalParam(JointImp, parameter);
        case dJointTypeAMotor: return dJointGetAMotorParam(JointImp, parameter);
        default: return 0;
    }
}
