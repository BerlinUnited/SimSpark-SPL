/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: joint.h,v 1.5 2004/04/20 14:16:07 rollmark Exp $

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

/** \class Joint encapsulates an ODE joint object. A joint is a
    relationship (a constraint) that is enforced between two bodies so
    that they can only have certain positions and orientations
    relative to each other.

    Note that the joint geometry parameter setting functions should
    only be called after the joint has been attached to bodies, and
    those bodies have been correctly positioned, otherwise the joint
    may not be initialized correctly. If the joint is not already
    attached, these functions will do nothing.

    The default anchor for all joints is global (0,0,0). The default
    axis for all joints is global (1,0,0).

    There are no functions to set joint angles or positions (or their
    rates) directly, instead you must set the corresponding body
    positions and velocities.
 */
class Joint : public ODEObject
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

    Joint();
    virtual ~Joint();

    /** attaches the joint to some new bodies. If the joint is already
        attached, it will be detached from the old bodies first. To
        attach this joint to only one body, set body1 or body2 to
        null.  A null body refers to the static environment. Setting
        both bodies to zero puts the joint into "limbo", i.e. it will
        have no effect on the simulation.
    */
    virtual void Attach(boost::shared_ptr<Body> body1,
                        boost::shared_ptr<Body> body2);

    /** attaches the joint to some new bodies, that are given as path
        expressions. These path expressions are allowed to be relative
        to this joint node.
     */
    void Attach(const std::string& path1, const std::string& path2);

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

    /** get the node at 'path' and tries a cast to Body */
    boost::shared_ptr<Body> GetBody(const std::string& path);

protected:
    /** the managed ODE joint */
    dJointID mODEJoint;

    /** the allocated joint feedback structure */
    boost::shared_ptr<dJointFeedback> mFeedback;
};

DECLARE_CLASS(Joint);

} // namespace oxygen

#endif // OXYGEN_JOINT_H
