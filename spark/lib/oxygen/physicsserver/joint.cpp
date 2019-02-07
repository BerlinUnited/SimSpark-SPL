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
#include <oxygen/physicsserver/joint.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/int/jointint.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace std;
using namespace salt;

boost::shared_ptr<JointInt> Joint::mJointImp;

Joint::Joint() : 
PhysicsObject(), mJointID(0), 
mJointMaxSpeed1(0), mJointMaxSpeed2(0), 
mIsLimitJointMaxSpeed1(false), mIsLimitJointMaxSpeed2(false)
{

}

Joint::~Joint()
{
}

void Joint::OnLink()
{
    if (mJointImp.get() == 0)
        mJointImp = dynamic_pointer_cast<JointInt>
            (GetCore()->New("JointImp"));

    if (mJointID == 0)
        {
            return;
        }

    mJointImp->OnLink(mJointID, this);
}

boost::shared_ptr<Joint> Joint::GetJoint(long jointID)
{
    if (jointID == 0)
        {
            return boost::shared_ptr<Joint>();
        }

    Joint* jointPtr = mJointImp->GetJoint(jointID);

    if (jointPtr == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Joint) no joint found for dJointID "
                 << jointID << "\n";
            return boost::shared_ptr<Joint>();
        }

    boost::shared_ptr<Joint> joint = static_pointer_cast<Joint>
        (jointPtr->GetSelf().lock());

    if (joint.get() == 0)
        {
            // we cannot use the logserver here
            cerr << "ERROR: (Joint) got no boost::shared_ptr for dJointID "
                 << jointID << "\n";
        }

    return joint;
}

void Joint::Attach(boost::shared_ptr<RigidBody> body1, boost::shared_ptr<RigidBody> body2)
{
    if (mJointID == 0)
        {
            GetLog()->Error()
                << "(Joint) ERROR: Attach called with uninitialized ODE joint\n";
            return;
        }

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

    GetLog()->Debug() << "(Joint) Attaching '" << path1 << "' to '"
                      << path2 << '\n';

    mJointImp->Attach(id1, id2, mJointID);
}

boost::shared_ptr<RigidBody> Joint::GetBody(const std::string& path)
{
    if (path.empty())
        {
            return boost::shared_ptr<RigidBody>();
        }

    boost::shared_ptr<Leaf> mySelf = static_pointer_cast<Leaf>
        (GetSelf().lock());

    boost::shared_ptr<Leaf> leaf = GetCore()->Get(path,mySelf);

    if (leaf.get() == 0)
        {
            GetLog()->Error()
                << "(Joint) ERROR: cannot find node '"
                << path << "'\n";
            return boost::shared_ptr<RigidBody>();
        }

    boost::shared_ptr<RigidBody> body = dynamic_pointer_cast<RigidBody>(leaf);

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
    boost::shared_ptr<RigidBody> body1 = GetBody(path1);
    boost::shared_ptr<RigidBody> body2 = GetBody(path2);

    Attach(body1,body2);
}

int Joint::GetType() const
{
    return mJointImp->GetType(mJointID);
}

boost::shared_ptr<RigidBody> Joint::GetBody(EBodyIndex idx)
{
    long bodyID = mJointImp->GetBodyID(idx, mJointID);
    return RigidBody::GetBody(bodyID);
}

bool Joint::AreConnected(boost::shared_ptr<RigidBody> body1, boost::shared_ptr<RigidBody> body2)
{
    if (
        (body1.get() == 0) ||
        (body2.get() == 0)
        )
        {
            return false;
        }

    const bool connected = mJointImp->AreConnected(body1->GetBodyID(), body2->GetBodyID());

    return connected;
}

bool Joint::AreConnectedExcluding (boost::shared_ptr<RigidBody> body1,
                                   boost::shared_ptr<RigidBody> body2,
                                   int joint_type)
{
    if (
        (body1.get() == 0) ||
        (body2.get() == 0)
        )
        {
            return false;
        }

    const bool connected = mJointImp->AreConnectedExcluding(body1->GetBodyID(),
                                                            body2->GetBodyID(),
                                                            joint_type);

    return connected;
}

void Joint::EnableFeedback(bool enable)
{
    mJointImp->EnableFeedback(enable, mJointID, mFeedback);
}

bool Joint::FeedBackEnabled() const
{
    return mJointImp->FeedbackEnabled(mJointID);
}

Vector3f Joint::GetFeedbackForce(EBodyIndex idx) const
{
    return mJointImp->GetFeedbackForce(idx, mFeedback);
}

Vector3f Joint::GetFeedbackTorque(EBodyIndex idx) const
{
    return mJointImp->GetFeedbackTorque(idx, mFeedback);
}
    
void Joint::SetFudgeFactor(EAxisIndex idx, float fudge_factor)
{
    mJointImp->SetFudgeFactor(idx, fudge_factor, mJointID);
}

float Joint::GetFudgeFactor(EAxisIndex idx) const
{
    return mJointImp->GetFudgeFactor(idx, mJointID);
}

void Joint::SetBounce(EAxisIndex idx, float bounce)
{
    mJointImp->SetBounce(idx, bounce, mJointID);
}

float Joint::GetBounce(EAxisIndex idx) const
{
    return mJointImp->GetBounce(idx, mJointID);
}

void Joint::SetLowStopPos(EAxisIndex idx, float pos)
{
    mJointImp->SetLowStopPos(idx, pos, mJointID);
}

float Joint::GetLowStopPos(EAxisIndex idx) const
{
    return mJointImp->GetLowStopPos(idx, mJointID);
}

void Joint::SetHighStopPos(EAxisIndex idx, float pos)
{
    mJointImp->SetHighStopPos(idx, pos, mJointID);
}

float Joint::GetHighStopPos(EAxisIndex idx) const
{
    return mJointImp->GetHighStopPos(idx, mJointID);
}

void Joint::SetLowStopDeg(EAxisIndex idx, float deg)
{
    mJointImp->SetLowStopDeg(idx, deg, mJointID);
}

float Joint::GetLowStopDeg(EAxisIndex idx) const
{
    return mJointImp->GetLowStopDeg(idx, mJointID);
}

void Joint::SetHighStopDeg(EAxisIndex idx, float deg)
{
    mJointImp->SetHighStopDeg(idx, deg, mJointID);
}

float Joint::GetHighStopDeg(EAxisIndex idx) const
{
    return mJointImp->GetHighStopDeg(idx, mJointID);
}

void Joint::SetCFM(EAxisIndex idx, float cfm)
{
    mJointImp->SetCFM(idx, cfm, mJointID);
}

float Joint::GetCFM(EAxisIndex idx) const
{
    return mJointImp->GetCFM(idx, mJointID);
}

void Joint::SetStopCFM(EAxisIndex idx, float cfm)
{
    mJointImp->SetStopCFM(idx, cfm, mJointID);
}

float Joint::GetStopCFM(EAxisIndex idx) const
{
    return mJointImp->GetStopCFM(idx, mJointID);
}

void Joint::SetStopERP(EAxisIndex idx, float erp)
{
    mJointImp->SetStopERP(idx, erp, mJointID);
}

float Joint::GetStopERP(EAxisIndex idx) const
{
    return mJointImp->GetStopERP(idx, mJointID);
}

void Joint::SetSuspensionERP(EAxisIndex idx, float erp)
{
    mJointImp->SetSuspensionERP(idx, erp, mJointID);
}

float Joint::GetSuspensionERP(EAxisIndex idx) const
{
    return mJointImp->GetSuspensionERP(idx, mJointID);
}

void Joint::SetSuspensionCFM(EAxisIndex idx, float cfm)
{
    mJointImp->SetSuspensionCFM(idx, cfm, mJointID);
}

float Joint::GetSuspensionCFM(EAxisIndex idx) const
{
    return mJointImp->GetSuspensionCFM(idx, mJointID);
}

void Joint::SetLinearMotorVelocity(EAxisIndex idx, float vel)
{
    mJointImp->SetLinearMotorVelocity(idx, vel, mJointID);
}

float Joint::GetLinearMotorVelocity(EAxisIndex idx) const
{
    return mJointImp->GetLinearMotorVelocity(idx, mJointID);
}

void Joint::SetAngularMotorVelocity(EAxisIndex idx, float deg)
{
    mJointImp->SetAngularMotorVelocity(idx, deg, mJointID);
}

float Joint::GetAngularMotorVelocity(EAxisIndex idx) const
{
    return mJointImp->GetAngularMotorVelocity(idx, mJointID);
}

void Joint::SetMaxMotorForce(EAxisIndex idx, float f)
{
    mJointImp->SetMaxMotorForce(idx, f, mJointID);
}

float Joint::GetMaxMotorForce(EAxisIndex idx) const
{
    return mJointImp->GetMaxMotorForce(idx, mJointID);
}

void Joint::DestroyPhysicsObject()
{
    if (!mJointID)
        {
            return;
        }

    EnableFeedback(false);
    mJointImp->DestroyJoint(mJointID, mFeedback);
    mJointID = 0;
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
