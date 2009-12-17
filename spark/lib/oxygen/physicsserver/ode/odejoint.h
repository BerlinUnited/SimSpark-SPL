/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeobject.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#ifndef OXYGEN_ODEJOINT_H
#define OXYGEN_ODEJOINT_H

#include <oxygen/physicsserver/ode/odephysicsobject.h>
#include <oxygen/physicsserver/int/jointint.h>

namespace oxygen
{

class OXYGEN_API ODEJoint : public JointInt, public ODEPhysicsObject
{
public:
    ODEJoint();
    
    virtual void DestroyJoint();
    virtual void Attach(boost::shared_ptr<RigidBody> body1,
                        boost::shared_ptr<RigidBody> body2);
    void Attach(const std::string& path1, const std::string& path2);
    int GetType() const;
    boost::shared_ptr<RigidBody> GetBody(EBodyIndex idx);
    void EnableFeedback(bool enable);
    bool FeedbackEnabled() const;
    salt::Vector3f GetFeedbackForce(EBodyIndex idx) const;
    salt::Vector3f GetFeedbackTorque(EBodyIndex idx) const;
    void SetFudgeFactor(EAxisIndex idx, float fudge_factor);
    float GetFudgeFactor(EAxisIndex idx) const;
    void SetBounce(EAxisIndex idx, float bounce);
    float GetBounce(EAxisIndex idx) const;
    void SetLowStopDeg(EAxisIndex idx, float deg);
    float GetLowStopDeg(EAxisIndex idx) const;
    void SetHighStopDeg(EAxisIndex idx, float deg);
    float GetHighStopDeg(EAxisIndex idx) const;
    void SetLowStopPos(EAxisIndex idx, float deg);
    float GetLowStopPos(EAxisIndex idx) const;
    void SetHighStopPos(EAxisIndex idx, float deg);
    float GetHighStopPos(EAxisIndex idx) const;
    void SetCFM(EAxisIndex idx, float cfm);
    float GetCFM(EAxisIndex idx) const;
    void SetStopCFM(EAxisIndex idx, float cfm);
    float GetStopCFM(EAxisIndex idx) const;
    void SetStopERP(EAxisIndex idx, float erp);
    float GetStopERP(EAxisIndex idx) const;
    void SetSuspensionERP(EAxisIndex idx, float erp);
    float GetSuspensionERP(EAxisIndex idx) const;
    void SetSuspensionCFM(EAxisIndex idx, float cfm);
    float GetSuspensionCFM(EAxisIndex idx) const;
    void SetLinearMotorVelocity(EAxisIndex idx, float vel);
    float GetLinearMotorVelocity(EAxisIndex idx) const;
    void SetAngularMotorVelocity(EAxisIndex idx, float deg);
    float GetAngularMotorVelocity(EAxisIndex idx) const;
    void SetMaxMotorForce(EAxisIndex idx, float f);
    float GetMaxMotorForce(EAxisIndex idx) const;
    virtual void SetParameter(int parameter, float value){};
    virtual float GetParameter(int parameter) const;
    virtual void SetJointMaxSpeed1(float rad);
    virtual void SetJointMaxSpeed2(float rad);
    virtual float GetJointMaxSpeed1() const;
    virtual float GetJointMaxSpeed2() const;
    bool IsLimitJointMaxSpeed1() const;
    bool IsLimitJointMaxSpeed2() const;
    void OnLink();
    
protected:
    dJointID mODEJoint;
};

} //namespace oxygen

#endif //OXYGEN_ODEJOINT_H
