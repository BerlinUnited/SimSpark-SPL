/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: joint.cpp,v 1.10 2008/05/17 13:38:09 fengxue Exp $

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
#include <zeitgeist/logserver/logserver.h>
#include "joint.h"
#include "body.h"

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

Joint::Joint() : 
ODEObject(), mODEJoint(0), 
mJointMaxSpeed1(0), mJointMaxSpeed2(0), 
mIsLimitJointMaxSpeed1(false), mIsLimitJointMaxSpeed2(false)
{
}

Joint::~Joint()
{
}

void Joint::OnLink()
{
    if (mODEJoint == 0)
        {
            return;
        }

    dJointSetData(mODEJoint, this);
}

shared_ptr<Joint> Joint::GetJoint(dJointID id)
{
    if (id == 0)
        {
            return shared_ptr<Joint>();
        }

    Joint* jointPtr =
        static_cast<Joint*>(dJointGetData(id));

    if (jointPtr == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Joint) no joint found for dJointID "
                 << id << "\n";
            return shared_ptr<Joint>();
        }

    shared_ptr<Joint> joint = shared_static_cast<Joint>
        (make_shared(jointPtr->GetSelf()));

    if (joint.get() == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Joint) got no shared_ptr for dJointID "
                 << id << "\n";
        }

    return joint;
}

void Joint::Attach(shared_ptr<Body> body1, shared_ptr<Body> body2)
{
    if (mODEJoint == 0)
        {
            GetLog()->Error()
                << "(Joint) ERROR: Attach called with uninitialized ODE joint\n";
            return;
        }

    string path1,path2;
    dBodyID id1,id2;

    static const char strStaticEnv[] = "<static environment>";

    if (body1.get() == 0)
        {
            id1   = 0;
            path1 = strStaticEnv;
        } else
        {
            id1   = body1->GetODEBody();
            path1 = body1->GetFullPath();
        }

    if (body2.get() == 0)
        {
            id2   = 0;
            path2 = strStaticEnv;
        } else
        {
            id2   = body2->GetODEBody();
            path2 = body2->GetFullPath();
        }

    GetLog()->Debug() << "(Joint) Attaching '" << path1 << "' to '"
                      << path2 << '\n';

    dJointAttach(mODEJoint, id1, id2);
}

shared_ptr<Body> Joint::GetBody(const std::string& path)
{
    if (path.empty())
        {
            return shared_ptr<Body>();
        }

    shared_ptr<Leaf> mySelf = shared_static_cast<Leaf>
        (make_shared(GetSelf()));

    shared_ptr<Leaf> leaf = GetCore()->Get(path,mySelf);

    if (leaf.get() == 0)
        {
            GetLog()->Error()
                << "(Joint) ERROR: cannot find node '"
                << path << "'\n";
            return shared_ptr<Body>();
        }

    shared_ptr<Body> body = shared_dynamic_cast<Body>(leaf);

    if (body.get() == 0)
        {
            GetLog()->Error()
                << "(Joint) ERROR: node '"
                << path << "' is not a Body node \n";
        }

    return body;
}

void Joint::Attach(const std::string& path1, const std::string& path2)
{
    shared_ptr<Body> body1 = GetBody(path1);
    shared_ptr<Body> body2 = GetBody(path2);

    Attach(body1,body2);
}

int Joint::GetType() const
{
    return dJointGetType(mODEJoint);
}

boost::shared_ptr<Body> Joint::GetBody(EBodyIndex idx)
{
    return Body::GetBody(dJointGetBody(mODEJoint, idx));
}

bool Joint::AreConnected (shared_ptr<Body> body1, shared_ptr<Body> body2)
{
    if (
        (body1.get() == 0) ||
        (body2.get() == 0)
        )
        {
            return false;
        }

    const bool connected =
        (dAreConnected(body1->GetODEBody(),body2->GetODEBody())
         == 1);

    return connected;
}

bool Joint::AreConnectedExcluding (shared_ptr<Body> body1,
                                   shared_ptr<Body> body2,
                                   int joint_type)
{
    if (
        (body1.get() == 0) ||
        (body2.get() == 0)
        )
        {
            return false;
        }

    const bool connected =
        (dAreConnectedExcluding(body1->GetODEBody(),
                                body2->GetODEBody(),
                                joint_type
                                )
         == 1);

    return connected;
}

void Joint::EnableFeedback(bool enable)
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

bool Joint::FeedBackEnabled() const
{
    return (dJointGetFeedback(mODEJoint) != 0);
}

Vector3f Joint::GetFeedbackForce(EBodyIndex idx) const
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

Vector3f Joint::GetFeedbackTorque(EBodyIndex idx) const
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
    
void Joint::SetFudgeFactor(EAxisIndex idx, float fudge_factor)
{
    SetParameter(dParamFudgeFactor + (idx * dParamGroup), fudge_factor);
}

float Joint::GetFudgeFactor(EAxisIndex idx) const
{
    return GetParameter(dParamFudgeFactor + (idx * dParamGroup));
}


void Joint::SetBounce(EAxisIndex idx, float bounce)
{
    SetParameter(dParamBounce + (idx * dParamGroup),bounce);
}

float Joint::GetBounce(EAxisIndex idx) const
{
    return GetParameter(dParamBounce + (idx * dParamGroup));
}

void Joint::SetLowStopPos(EAxisIndex idx, float pos)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), pos);
}

float Joint::GetLowStopPos(EAxisIndex idx) const
{
    return GetParameter(dParamLoStop + (idx * dParamGroup));
}

void Joint::SetHighStopPos(EAxisIndex idx, float pos)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), pos);
}

float Joint::GetHighStopPos(EAxisIndex idx) const
{
    return GetParameter(dParamHiStop + (idx * dParamGroup));
}

void Joint::SetLowStopDeg(EAxisIndex idx, float deg)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), gDegToRad(deg));
}

float Joint::GetLowStopDeg(EAxisIndex idx) const
{
    return gRadToDeg(GetParameter(dParamLoStop + (idx * dParamGroup)));
}

void Joint::SetHighStopDeg(EAxisIndex idx, float deg)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), gDegToRad(deg));
}

float Joint::GetHighStopDeg(EAxisIndex idx) const
{
    return gRadToDeg(GetParameter(dParamHiStop + (idx * dParamGroup)));
}

void Joint::SetCFM(EAxisIndex idx, float cfm)
{
    SetParameter(dParamCFM + (idx * dParamGroup), cfm);
}

float Joint::GetCFM(EAxisIndex idx) const
{
    return GetParameter(dParamCFM + (idx * dParamGroup));
}

void Joint::SetStopCFM(EAxisIndex idx, float cfm)
{
    SetParameter(dParamStopCFM + (idx * dParamGroup), cfm);
}

float Joint::GetStopCFM(EAxisIndex idx) const
{
    return GetParameter(dParamStopCFM + (idx * dParamGroup));
}

void Joint::SetStopERP(EAxisIndex idx, float erp)
{
    SetParameter(dParamStopERP + (idx * dParamGroup), erp);
}

float Joint::GetStopERP(EAxisIndex idx) const
{
    return GetParameter(dParamStopERP + (idx * dParamGroup));
}

void Joint::SetSuspensionERP(EAxisIndex idx, float erp)
{
    SetParameter(dParamSuspensionERP + (idx * dParamGroup), erp);
}

float Joint::GetSuspensionERP(EAxisIndex idx) const
{
    return GetParameter(dParamSuspensionERP + (idx * dParamGroup));
}

void Joint::SetSuspensionCFM(EAxisIndex idx, float cfm)
{
    SetParameter(dParamSuspensionCFM + (idx * dParamGroup), cfm);
}

float Joint::GetSuspensionCFM(EAxisIndex idx) const
{
    return GetParameter(dParamSuspensionCFM + (idx * dParamGroup));
}

void Joint::SetLinearMotorVelocity(EAxisIndex idx, float vel)
{
    SetParameter(dParamVel + (idx * dParamGroup), vel);
}

float Joint::GetLinearMotorVelocity(EAxisIndex idx) const
{
    return GetParameter(dParamVel + (idx * dParamGroup));
}

void Joint::SetAngularMotorVelocity(EAxisIndex idx, float deg)
{
    SetParameter(dParamVel + (idx * dParamGroup), gDegToRad(deg));
}

float Joint::GetAngularMotorVelocity(EAxisIndex idx) const
{
    return gRadToDeg(GetParameter(dParamVel + (idx * dParamGroup)));
}

void Joint::SetMaxMotorForce(EAxisIndex idx, float f)
{
    SetParameter(dParamFMax + (idx * dParamGroup), f);
}

float Joint::GetMaxMotorForce(EAxisIndex idx) const
{
    return GetParameter(dParamFMax + (idx * dParamGroup));
}

void Joint::DestroyODEObject()
{
    if (! mODEJoint)
        {
            return;
        }

    EnableFeedback(false);
    dJointDestroy(mODEJoint);
    mODEJoint = 0;
}

void Joint::SetJointMaxSpeed1(float rad)
{
    mJointMaxSpeed1 = fabs(rad);
    mIsLimitJointMaxSpeed1 = true;
}

void Joint::SetJointMaxSpeed2(float rad)
{
    mJointMaxSpeed2 = fabs(rad);
    mIsLimitJointMaxSpeed2 = true;
}

float Joint::GetJointMaxSpeed1() const
{
    return mJointMaxSpeed1;
}

float Joint::GetJointMaxSpeed2() const
{
    return mJointMaxSpeed2;
}

bool Joint::IsLimitJointMaxSpeed1() const
{
    return mIsLimitJointMaxSpeed1;
}

bool Joint::IsLimitJointMaxSpeed2() const
{
    return mIsLimitJointMaxSpeed2;
}
