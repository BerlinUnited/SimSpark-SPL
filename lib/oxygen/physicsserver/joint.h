/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: joint.h,v 1.1 2004/04/07 11:31:57 rollmark Exp $

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
#ifndef OXYGEN_JOINT_H
#define OXYGEN_JOINT_H

#include "odeobject.h"

namespace oxygen
{
class Body;

class Joint : public ODEObject
{
public:
    enum EBodyIndex
        {
            BI_FIRST = 0,
            BI_SECOND = 1
        };

    Joint();
    virtual ~Joint();

    /** attaches the joint to some new bodies. If the joint is already
        attached, it will be detached from the old bodies first. To
        attach this joint to only one body, set body1 or body2 to
        null.  A null body refers to the static environment. Setting
        both bodies to zero puts the joint into "limbo", i.e. it will
        have no effect on the simulation.
    */
    void Attach(boost::shared_ptr<Body> body1, boost::shared_ptr<Body> body2);

    /** returns one of the bodies that this joint connects, according
        to the given EBodyIndex
    */
    boost::shared_ptr<Body> GetBody(EBodyIndex idx);

    /** returns the Joint node corresponding to the given ODE joint */
    static boost::shared_ptr<Joint> GetJoint(dJointID id);

    /** returns the type of the managed ODE joint, possible return
        values are dJointTypeNone, dJointTypeBall, dJointTypeHinge,
        dJointTypeSlider, dJointTypeContact, dJointTypeUniversal,
        dJointTypeHinge2, dJointTypeFixed or dJointTypeAMotor.
     */
    int GetType();

    /** returns true if the two given bodies are connected by a
        joint */
    static bool AreConnected (boost::shared_ptr<Body> body1,
                              boost::shared_ptr<Body> body2);

    /** returns true if the two given bodies are connected together by
        a joint that does not have the given joint type. For possible
        joint type constants see GetType()
     */
    static bool AreConnectedExcluding (boost::shared_ptr<Body> body1,
                                       boost::shared_ptr<Body> body2,
                                       int joint_type);

    /** during the world time step, the forces that are applied by
        each joint are added directly to the joined bodies, and the
        user normally has no way of telling which joint contributed
        how much force. If this information is desired the joint can
        be enabled to collect feedback information. By default a joint
        does not collect any feedback information.
     */
    void EnableFeedback(bool enable);

    /** returns true if the joint is set to collect feedback
        information */
    bool FeedBackEnabled();

    /** queries the force that the joint applied to one body attached
        to it during the last timestep.
    */
    salt::Vector3f GetFeedbackForce(EBodyIndex idx);

    /** queries the torque that the joint applied to one body attached
        to it during the last timestep.
    */
    salt::Vector3f GetFeedbackTorque(EBodyIndex idx);

protected:
    /** associated the created ODE joint with this node */
    virtual void OnLink();

protected:
    /** the managed ODE joint */
    dJointID mODEJoint;

    /** the allocated joint feedback structure */
    boost::shared_ptr<dJointFeedback> mFeedback;
};

DECLARE_CLASS(Joint);

} // namespace oxygen

#endif // OXYGEN_JOINT_H
