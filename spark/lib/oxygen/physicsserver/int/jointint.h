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

#include <oxygen/physicsserver/genericphysicsobjects.h>
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
    
    virtual void DestroyJoint(long jointID,
                              boost::shared_ptr<GenericJointFeedback> feedback) = 0;
    virtual void Attach(long bodyID1, long bodiID2, long jointID) = 0;
    virtual int GetType(long jointID) const = 0;
    virtual long GetBodyID(int idx, long jointID) = 0;
    virtual void EnableFeedback(bool enable, long jointID,
                                boost::shared_ptr<GenericJointFeedback> feedback) = 0;
    virtual bool FeedbackEnabled(long jointID) const = 0;
    virtual salt::Vector3f GetFeedbackForce(int idx,
                                            boost::shared_ptr<GenericJointFeedback> feedback) const = 0;
    virtual salt::Vector3f GetFeedbackTorque(int idx,
                                            boost::shared_ptr<GenericJointFeedback> feedback) const = 0;
    virtual void SetFudgeFactor(int idx, float fudge_factor, long jointID) = 0;
    virtual float GetFudgeFactor(int idx, long jointID) const = 0;
    virtual void SetBounce(int idx, float bounce, long jointID) = 0;
    virtual float GetBounce(int idx, long jointID) const = 0;
    virtual void SetLowStopDeg(int idx, float deg, long jointID) = 0;
    virtual float GetLowStopDeg(int idx, long jointID) const = 0;
    virtual void SetHighStopDeg(int idx, float deg, long jointID) = 0;
    virtual float GetHighStopDeg(int idx, long jointID) const = 0;
    virtual void SetLowStopPos(int idx, float deg, long jointID) = 0;
    virtual float GetLowStopPos(int idx, long jointID) const = 0;
    virtual void SetHighStopPos(int idx, float deg, long jointID) = 0;
    virtual float GetHighStopPos(int idx, long jointID) const = 0;
    virtual void SetCFM(int idx, float cfm, long jointID) = 0;
    virtual float GetCFM(int idx, long jointID) const = 0;
    virtual void SetStopCFM(int idx, float cfm, long jointID) = 0;
    virtual float GetStopCFM(int idx, long jointID) const = 0;
    virtual void SetStopERP(int idx, float erp, long jointID) = 0;
    virtual float GetStopERP(int idx, long jointID) const = 0;
    virtual void SetSuspensionERP(int idx, float erp, long jointID) = 0;
    virtual float GetSuspensionERP(int idx, long jointID) const = 0;
    virtual void SetSuspensionCFM(int idx, float cfm, long jointID) = 0;
    virtual float GetSuspensionCFM(int idx, long jointID) const = 0;
    virtual void SetLinearMotorVelocity(int idx, float vel, long jointID) = 0;
    virtual float GetLinearMotorVelocity(int idx, long jointID) const = 0;
    virtual void SetAngularMotorVelocity(int idx, float deg, long jointID) = 0;
    virtual float GetAngularMotorVelocity(int idx, long jointID) const = 0;
    virtual void SetMaxMotorForce(int idx, float f, long jointID) = 0;
    virtual float GetMaxMotorForce(int idx, long jointID) const = 0;
    virtual void SetParameter(int parameter, float value, long jointID) = 0;
    virtual float GetParameter(int parameter, long jointID) const = 0;
    virtual void OnLink(long jointID, Joint* joint) = 0;
};

} //namespace oxygen

#endif //OXYGEN_JOINTINT_H
