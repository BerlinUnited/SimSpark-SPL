/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: joint.cpp,v 1.4 2004/05/02 11:44:41 rollmark Exp $

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

Joint::Joint() : ODEObject(), mODEJoint(0)
{
}

Joint::~Joint()
{
    EnableFeedback(false);
    if (mODEJoint)
        {
            dJointDestroy(mODEJoint);
            mODEJoint = 0;
        }
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

    const dBodyID id1 = (body1.get() == 0) ? 0 : body1->GetODEBody();
    const dBodyID id2 = (body2.get() == 0) ? 0 : body2->GetODEBody();

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

int Joint::GetType()
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

    return dAreConnected(body1->GetODEBody(),body2->GetODEBody());
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

    return dAreConnectedExcluding(body1->GetODEBody(),body2->GetODEBody(),
                                  joint_type);
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

bool Joint::FeedBackEnabled()
{
    return (dJointGetFeedback(mODEJoint) != 0);
}

Vector3f Joint::GetFeedbackForce(EBodyIndex idx)
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

Vector3f Joint::GetFeedbackTorque(EBodyIndex idx)
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

void Joint::SetBounce(EAxisIndex idx, float bounce)
{
    SetParameter(dParamBounce + (idx * dParamGroup),bounce);
}

float Joint::GetBounce(EAxisIndex idx)
{
    return GetParameter(dParamBounce + (idx * dParamGroup));
}

void Joint::SetLowStopPos(EAxisIndex idx, float pos)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), pos);
}

float Joint::GetLowStopPos(EAxisIndex idx)
{
    return GetParameter(dParamLoStop + (idx * dParamGroup));
}

void Joint::SetHighStopPos(EAxisIndex idx, float pos)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), pos);
}

float Joint::GetHighStopPos(EAxisIndex idx)
{
    return GetParameter(dParamHiStop + (idx * dParamGroup));
}

void Joint::SetLowStopDeg(EAxisIndex idx, float deg)
{
    SetParameter(dParamLoStop + (idx * dParamGroup), gDegToRad(deg));
}

float Joint::GetLowStopDeg(EAxisIndex idx)
{
    return gRadToDeg(GetParameter(dParamLoStop + (idx * dParamGroup)));
}

void Joint::SetHighStopDeg(EAxisIndex idx, float deg)
{
    SetParameter(dParamHiStop + (idx * dParamGroup), gDegToRad(deg));
}

float Joint::GetHighStopDeg(EAxisIndex idx)
{
    return gRadToDeg(GetParameter(dParamHiStop + (idx * dParamGroup)));
}

void Joint::SetCFM(EAxisIndex idx, float cfm)
{
    SetParameter(dParamCFM + (idx * dParamGroup), cfm);
}

float Joint::GetCFM(EAxisIndex idx)
{
    return GetParameter(dParamCFM + (idx * dParamGroup));
}

void Joint::SetStopCFM(EAxisIndex idx, float cfm)
{
    SetParameter(dParamStopCFM + (idx * dParamGroup), cfm);
}

float Joint::GetStopCFM(EAxisIndex idx)
{
    return GetParameter(dParamStopCFM + (idx * dParamGroup));
}

void Joint::SetStopERP(EAxisIndex idx, float erp)
{
    SetParameter(dParamStopERP + (idx * dParamGroup), erp);
}

float Joint::GetStopERP(EAxisIndex idx)
{
    return GetParameter(dParamStopERP + (idx * dParamGroup));
}

void Joint::SetSuspensionERP(EAxisIndex idx, float erp)
{
    SetParameter(dParamSuspensionERP + (idx * dParamGroup), erp);
}

float Joint::GetSuspensionERP(EAxisIndex idx)
{
    return GetParameter(dParamSuspensionERP + (idx * dParamGroup));
}

void Joint::SetSuspensionCFM(EAxisIndex idx, float cfm)
{
    SetParameter(dParamSuspensionCFM + (idx * dParamGroup), cfm);
}

float Joint::GetSuspensionCFM(EAxisIndex idx)
{
    return GetParameter(dParamSuspensionCFM + (idx * dParamGroup));
}

void Joint::SetLinearMotorVelocity(EAxisIndex idx, float vel)
{
    SetParameter(dParamVel + (idx * dParamGroup), vel);
}

float Joint::GetLinearMotorVelocity(EAxisIndex idx)
{
    return GetParameter(dParamVel + (idx * dParamGroup));
}

void Joint::SetAngularMotorVelocity(EAxisIndex idx, float deg)
{
    SetParameter(dParamVel + (idx * dParamGroup), gDegToRad(deg));
}

float Joint::GetAngularMotorVelocity(EAxisIndex idx)
{
    return gRadToDeg(GetParameter(dParamVel + (idx * dParamGroup)));
}

void Joint::SetMaxMotorForce(EAxisIndex idx, float f)
{
    SetParameter(dParamFMax + (idx * dParamGroup), f);
}

float Joint::GetMaxMotorForce(EAxisIndex idx)
{
    return GetParameter(dParamFMax + (idx * dParamGroup));
}





