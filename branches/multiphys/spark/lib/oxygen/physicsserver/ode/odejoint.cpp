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
#include <oxygen/physicsserver/joint.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/ode/odejoint.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

ODEJoint::ODEJoint() : ODEPhysicsObject(), mODEJoint(0) 
{
    mJointMaxSpeed1 = mJointMaxSpeed2 = 0.0;
    mIsLimitJointMaxSpeed1 = mIsLimitJointMaxSpeed2 = false;
}

void ODEJoint::OnLink()
{
    if (mODEJoint == 0)
        {
            return;
        }

    dJointSetData(mODEJoint, this);
}

void ODEJoint::Attach(shared_ptr<RigidBody> body1, shared_ptr<RigidBody> body2)
{
    string path1,path2;
    long id1,id2;

    static const char strStaticEnv[] = "<static environment>";

    if (body1.get() == 0)
        {
            id1   = 0;
            path1 = strStaticEnv;
        } else
        {
            id1   = body1->GetBodyID();
            path1 = body1->GetFullPath();
        }

    if (body2.get() == 0)
        {
            id2   = 0;
            path2 = strStaticEnv;
        } else
        {
            id2   = body2->GetBodyID();
            path2 = body2->GetFullPath();
        }

    dJointAttach(mODEJoint, (dBodyID) id1, (dBodyID) id2);
}

void ODEJoint::Attach(const std::string& path1, const std::string& path2)
{
    shared_ptr<RigidBody> body1;
    shared_ptr<RigidBody> body2;
    Attach(body1,body2);
}

int ODEJoint::GetType() const
{
    return dJointGetType(mODEJoint);
}

boost::shared_ptr<RigidBody> ODEJoint::GetBody(EBodyIndex idx)
{
    long bodyID = (long) dJointGetBody(mODEJoint, idx);
    return RigidBody::GetBody(bodyID);
}

void ODEJoint::EnableFeedback(bool enable)
{
    if (enable)
        {
            if (mFeedback.get() == 0)
                {
                    mFeedback = shared_ptr<dJointFeedback>(new dJointFeedback());
                    memset(mFeedback.get(),0,sizeof(dJointFeedback));
                }
        } else
            {
                if (mFeedback.get() != 0)
                    {
                        mFeedback.reset();
                    }
            }

    dJointSetFeedback(mODEJoint,mFeedback.get());
}

bool ODEJoint::FeedbackEnabled() const
{
    return (dJointGetFeedback(mODEJoint) != 0);
}

Vector3f ODEJoint::GetFeedbackForce(EBodyIndex idx) const
{
    dJointFeedback* fb = mFeedback.get();
    if (fb == 0)
        {
            return Vector3f(0,0,0);
        }

    switch (idx)
        {
        case BI_FIRST :
            return Vector3f(
                            fb->f1[0],
                            fb->f1[1],
                            fb->f1[2]
                            );

        case BI_SECOND :
            return Vector3f(
                            fb->f2[0],
                            fb->f2[1],
                            fb->f2[2]
                            );

        default:
            return Vector3f(0,0,0);
        }
}

Vector3f ODEJoint::GetFeedbackTorque(EBodyIndex idx) const
{
    dJointFeedback* fb = mFeedback.get();
    if (fb == 0)
        {
            return Vector3f(0,0,0);
        }

    switch (idx)
        {
        case BI_FIRST :
            return Vector3f(
                            fb->t1[0],
                            fb->t1[1],
                            fb->t1[2]
                            );

        case BI_SECOND :
            return Vector3f(
                            fb->t2[0],
                            fb->t2[1],
                            fb->t2[2]
                            );

        default:
            return Vector3f(0,0,0);
        }
}
    
void ODEJoint::SetFudgeFactor(EAxisIndex idx, float fudge_factor)
{
    SetParameter(dParamFudgeFactor + (idx * dParamGroup), fudge_factor);
}

float ODEJoint::GetFudgeFactor(EAxisIndex idx) const
{
    return GetParameter(dParamFudgeFactor + (idx * dParamGroup));
}


void ODEJoint::SetBounce(EAxisIndex idx, float bounce)
{
    SetParameter(dParamBounce + (idx * dParamGroup),bounce);
}

float ODEJoint::GetBounce(EAxisIndex idx) const
{
    return GetParameter(dParamBounce + (idx * dParamGroup));
}

void ODEJoint::SetLowStopPos(EAxisIndex idx, float pos)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), pos);
}

float ODEJoint::GetLowStopPos(EAxisIndex idx) const
{
    return GetParameter(dParamLoStop + (idx * dParamGroup));
}

void ODEJoint::SetHighStopPos(EAxisIndex idx, float pos)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), pos);
}

float ODEJoint::GetHighStopPos(EAxisIndex idx) const
{
    return GetParameter(dParamHiStop + (idx * dParamGroup));
}

void ODEJoint::SetLowStopDeg(EAxisIndex idx, float deg)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), gDegToRad(deg));
}

float ODEJoint::GetLowStopDeg(EAxisIndex idx) const
{
    return gRadToDeg(GetParameter(dParamLoStop + (idx * dParamGroup)));
}

void ODEJoint::SetHighStopDeg(EAxisIndex idx, float deg)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), gDegToRad(deg));
}

float ODEJoint::GetHighStopDeg(EAxisIndex idx) const
{
    return gRadToDeg(GetParameter(dParamHiStop + (idx * dParamGroup)));
}

void ODEJoint::SetCFM(EAxisIndex idx, float cfm)
{
    SetParameter(dParamCFM + (idx * dParamGroup), cfm);
}

float ODEJoint::GetCFM(EAxisIndex idx) const
{
    return GetParameter(dParamCFM + (idx * dParamGroup));
}

void ODEJoint::SetStopCFM(EAxisIndex idx, float cfm)
{
    SetParameter(dParamStopCFM + (idx * dParamGroup), cfm);
}

float ODEJoint::GetStopCFM(EAxisIndex idx) const
{
    return GetParameter(dParamStopCFM + (idx * dParamGroup));
}

void ODEJoint::SetStopERP(EAxisIndex idx, float erp)
{
    SetParameter(dParamStopERP + (idx * dParamGroup), erp);
}

float ODEJoint::GetStopERP(EAxisIndex idx) const
{
    return GetParameter(dParamStopERP + (idx * dParamGroup));
}

void ODEJoint::SetSuspensionERP(EAxisIndex idx, float erp)
{
    SetParameter(dParamSuspensionERP + (idx * dParamGroup), erp);
}

float ODEJoint::GetSuspensionERP(EAxisIndex idx) const
{
    return GetParameter(dParamSuspensionERP + (idx * dParamGroup));
}

void ODEJoint::SetSuspensionCFM(EAxisIndex idx, float cfm)
{
    SetParameter(dParamSuspensionCFM + (idx * dParamGroup), cfm);
}

float ODEJoint::GetSuspensionCFM(EAxisIndex idx) const
{
    return GetParameter(dParamSuspensionCFM + (idx * dParamGroup));
}

void ODEJoint::SetLinearMotorVelocity(EAxisIndex idx, float vel)
{
    SetParameter(dParamVel + (idx * dParamGroup), vel);
}

float ODEJoint::GetLinearMotorVelocity(EAxisIndex idx) const
{
    return GetParameter(dParamVel + (idx * dParamGroup));
}

void ODEJoint::SetAngularMotorVelocity(EAxisIndex idx, float deg)
{
    SetParameter(dParamVel + (idx * dParamGroup), gDegToRad(deg));
}

float ODEJoint::GetAngularMotorVelocity(EAxisIndex idx) const
{
    return gRadToDeg(GetParameter(dParamVel + (idx * dParamGroup)));
}

void ODEJoint::SetMaxMotorForce(EAxisIndex idx, float f)
{
    SetParameter(dParamFMax + (idx * dParamGroup), f);
}

float ODEJoint::GetMaxMotorForce(EAxisIndex idx) const
{
    return GetParameter(dParamFMax + (idx * dParamGroup));
}

void ODEJoint::DestroyJoint()
{
    if (! mODEJoint)
        {
            return;
        }

    EnableFeedback(false);
    dJointDestroy(mODEJoint);
    mODEJoint = 0;
}

void ODEJoint::SetJointMaxSpeed1(float rad)
{
    mJointMaxSpeed1 = fabs(rad);
    mIsLimitJointMaxSpeed1 = true;
}

void ODEJoint::SetJointMaxSpeed2(float rad)
{
    mJointMaxSpeed2 = fabs(rad);
    mIsLimitJointMaxSpeed2 = true;
}

float ODEJoint::GetJointMaxSpeed1() const
{
    return mJointMaxSpeed1;
}

float ODEJoint::GetJointMaxSpeed2() const
{
    return mJointMaxSpeed2;
}

bool ODEJoint::IsLimitJointMaxSpeed1() const
{
    return mIsLimitJointMaxSpeed1;
}

bool ODEJoint::IsLimitJointMaxSpeed2() const
{
    return mIsLimitJointMaxSpeed2;
}

float ODEJoint::GetParameter(int parameter) const
{
    return 0.0;
} 
