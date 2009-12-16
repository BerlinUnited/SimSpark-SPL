/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: space.h 102 2009-11-18 07:24:29Z a-held $

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

#ifndef OXYGEN_JOINTINT_H
#define OXYGEN_JOINTINT_H

#include <oxygen/physicsserver/ode/odewrapper.h>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>
#include <string>

namespace oxygen
{
class RigidBody;
class Joint;

class OXYGEN_API JointInt
{
public:
    enum EBodyIndex
        {
            BI_FIRST = 0,
            BI_SECOND = 1
        };

    enum EAxisIndex
        {
            AI_FIRST = 0,
            AI_SECOND = 1,
            AI_THIRD = 2
        };
        
    static Joint* GetJoint(long jointID){
    
    }
    
    static bool AreConnected(boost::shared_ptr<RigidBody> body1,
                             boost::shared_ptr<RigidBody> body2){
                            
    }
    
    static bool AreConnectedExcluding(boost::shared_ptr<RigidBody> body1,
                                      boost::shared_ptr<RigidBody> body2,
                                      int joint_type){
                                      
    }
    
    virtual void DestroyJoint() = 0;
    virtual void Attach(boost::shared_ptr<RigidBody> body1,
                        boost::shared_ptr<RigidBody> body2) = 0;
    virtual void Attach(const std::string& path1, const std::string& path2) = 0;
    virtual int GetType() const = 0;
    virtual boost::shared_ptr<RigidBody> GetBody(EBodyIndex idx) = 0;
    virtual void EnableFeedback(bool enable) = 0;
    virtual bool FeedbackEnabled() const = 0;
    virtual salt::Vector3f GetFeedbackForce(EBodyIndex idx) const = 0;
    virtual salt::Vector3f GetFeedbackTorque(EBodyIndex idx) const = 0;
    virtual void SetFudgeFactor(EAxisIndex idx, float fudge_factor) = 0;
    virtual float GetFudgeFactor(EAxisIndex idx) const = 0;
    virtual void SetBounce(EAxisIndex idx, float bounce) = 0;
    virtual float GetBounce(EAxisIndex idx) const = 0;
    virtual void SetLowStopDeg(EAxisIndex idx, float deg) = 0;
    virtual float GetLowStopDeg(EAxisIndex idx) const = 0;
    virtual void SetHighStopDeg(EAxisIndex idx, float deg) = 0;
    virtual float GetHighStopDeg(EAxisIndex idx) const = 0;
    virtual void SetLowStopPos(EAxisIndex idx, float deg) = 0;
    virtual float GetLowStopPos(EAxisIndex idx) const = 0;
    virtual void SetHighStopPos(EAxisIndex idx, float deg) = 0;
    virtual float GetHighStopPos(EAxisIndex idx) const = 0;
    virtual void SetCFM(EAxisIndex idx, float cfm) = 0;
    virtual float GetCFM(EAxisIndex idx) const = 0;
    virtual void SetStopCFM(EAxisIndex idx, float cfm) = 0;
    virtual float GetStopCFM(EAxisIndex idx) const = 0;
    virtual void SetStopERP(EAxisIndex idx, float erp) = 0;
    virtual float GetStopERP(EAxisIndex idx) const = 0;
    virtual void SetSuspensionERP(EAxisIndex idx, float erp) = 0;
    virtual float GetSuspensionERP(EAxisIndex idx) const = 0;
    virtual void SetSuspensionCFM(EAxisIndex idx, float cfm) = 0;
    virtual float GetSuspensionCFM(EAxisIndex idx) const = 0;
    virtual void SetLinearMotorVelocity(EAxisIndex idx, float vel) = 0;
    virtual float GetLinearMotorVelocity(EAxisIndex idx) const = 0;
    virtual void SetAngularMotorVelocity(EAxisIndex idx, float deg) = 0;
    virtual float GetAngularMotorVelocity(EAxisIndex idx) const = 0;
    virtual void SetMaxMotorForce(EAxisIndex idx, float f) = 0;
    virtual float GetMaxMotorForce(EAxisIndex idx) const = 0;
    virtual void SetParameter(int parameter, float value) = 0;
    virtual float GetParameter(int parameter) const = 0;
    virtual void SetJointMaxSpeed1(float rad) = 0;
    virtual void SetJointMaxSpeed2(float rad) = 0;
    virtual float GetJointMaxSpeed1() const = 0;
    virtual float GetJointMaxSpeed2() const = 0;
    virtual bool IsLimitJointMaxSpeed1() const = 0;
    virtual bool IsLimitJointMaxSpeed2() const = 0;
    virtual void OnLink() = 0;
    
protected:
    dJointID mODEJoint;
    boost::shared_ptr<dJointFeedback> mFeedback;
    float mJointMaxSpeed1;
    bool mIsLimitJointMaxSpeed1;
    float mJointMaxSpeed2;
    bool mIsLimitJointMaxSpeed2;
};

} //namespace oxygen

#endif //OXYGEN_JOINTINT_H
