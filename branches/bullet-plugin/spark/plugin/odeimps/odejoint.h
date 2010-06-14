/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef ODEJOINT_H
#define ODEJOINT_H

#include "odephysicsobject.h"
#include <oxygen/physicsserver/genericphysicsobjects.h>
#include <oxygen/physicsserver/int/jointint.h>

class JointImp : public oxygen::JointInt, public PhysicsObjectImp
{
    /** See physicsserver/int/jointint.h for documentation */

public:
    JointImp();

    oxygen::Joint* GetJoint(long jointID);
    bool AreConnected(long bodyID1, long bodyID2);
    bool AreConnectedExcluding(long bodyID1, long bodyID2, int joint_type);
    virtual void DestroyJoint(long jointID,
                              boost::shared_ptr<oxygen::GenericJointFeedback> feedback);
    virtual void Attach(long bodyID1, long bodyID2, long jointID);
    int GetType(long jointID) const;
    long GetBodyID(int idx, long jointID);
    void EnableFeedback(bool enable, long jointID, 
                        boost::shared_ptr<oxygen::GenericJointFeedback> feedback);
    bool FeedbackEnabled(long jointID) const;
    salt::Vector3f GetFeedbackForce(int idx,
                                    boost::shared_ptr<oxygen::GenericJointFeedback> feedback) const;
    salt::Vector3f GetFeedbackTorque(int idx,
                                     boost::shared_ptr<oxygen::GenericJointFeedback> feedback) const;
    void SetFudgeFactor(int idx, float fudge_factor, long jointID);
    float GetFudgeFactor(int idx, long jointID) const;
    void SetBounce(int idx, float bounce, long jointID);
    float GetBounce(int idx, long jointID) const;
    void SetLowStopDeg(int idx, float deg, long jointID);
    float GetLowStopDeg(int idx, long jointID) const;
    void SetHighStopDeg(int idx, float deg, long jointID);
    float GetHighStopDeg(int idx, long jointID) const;
    void SetLowStopPos(int idx, float deg, long jointID);
    float GetLowStopPos(int idx, long jointID) const;
    void SetHighStopPos(int idx, float deg, long jointID);
    float GetHighStopPos(int idx, long jointID) const;
    void SetCFM(int idx, float cfm, long jointID);
    float GetCFM(int idx, long jointID) const;
    void SetStopCFM(int idx, float cfm, long jointID);
    float GetStopCFM(int idx, long jointID) const;
    void SetStopERP(int idx, float erp, long jointID);
    float GetStopERP(int idx, long jointID) const;
    void SetSuspensionERP(int idx, float erp, long jointID);
    float GetSuspensionERP(int idx, long jointID) const;
    void SetSuspensionCFM(int idx, float cfm, long jointID);
    float GetSuspensionCFM(int idx, long jointID) const;
    void SetLinearMotorVelocity(int idx, float vel, long jointID);
    float GetLinearMotorVelocity(int idx, long jointID) const;
    void SetAngularMotorVelocity(int idx, float deg, long jointID);
    float GetAngularMotorVelocity(int idx, long jointID) const;
    void SetMaxMotorForce(int idx, float f, long jointID);
    float GetMaxMotorForce(int idx, long jointID) const;
    void SetParameter(int parameter, float value, long jointID);
    float GetParameter(int parameter, long jointID) const;
    void OnLink(long jointID, oxygen::Joint* joint);
};

DECLARE_CLASS(JointImp);

#endif //ODEJOINT_H
