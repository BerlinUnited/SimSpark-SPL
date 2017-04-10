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
#ifndef HMDPEFFECTOR_H
#define HMDPEFFECTOR_H

#include <oxygen/agentaspect/effector.h>
#include <oxygen/physicsserver/rigidbody.h>
#include "naospecific.h"

//! make it possible to have a pointer to the perceptor
class HMDPPerceptor;

namespace HMDP
{
extern "C"
{
//!Link to Micro-controller software headers
#include "hmdp_c/base.h"
#include "hmdp_c/hmdp_c.h"

}
}

/***********************************************************************************
 !Class  for receiving commands from the client
 ************************************************************************************/

class HMDPEffector: public oxygen::Effector
{
    TLeafList jointList;
    long iter;
    //
    // functions
    //
public:
    HMDPEffector();
    virtual ~HMDPEffector();

    /** returns the name of the predicate this effector implements. */
    virtual std::string GetPredicate()
    {
        return "HMDP";
    } // client uses "HMDP" as predicate, server uses "hmdp"

    /**! constructs an Actionobject, describing a predicate */
    virtual boost::shared_ptr<oxygen::ActionObject>
            GetActionObject(const oxygen::Predicate& predicate);

    /** setup the reference to the agents body node */
    virtual void OnLink();

    /** remove the reference to the agents body node */
    virtual void OnUnlink();
    //! messages coming in from the client -- should be protected but there is now something with the c code
    void sendMessage(std::string message);

    //!
    std::string inMessage;
    void searchForNextLinestartInMessage(); // refers to inMessage;

    std::vector<float> servo_target_pos; // vector of target positions of a vector
    std::vector<float> servo_gain; // gains
    std::vector<float> servo_angle;
    bool ifIRQ; // IRQ flag (for micro controller emulation)
    bool ifActive;
    NaoSpecificPluginFunctions nao; // all functions we should get rid of in the plugin
    bool checkIfServoIDExists(int id);

protected:
    /** realizes the action described by the ActionObject */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    /** reads joints from zeitgeist tree */
    void ReadOutJointList();

    /** */
    void controlPosServo();

    /** */
    void InitHMDP();

    /** */
    void prepareUsage();

    /** */
    float zeroPosServo(int id);

    HMDPPerceptor *perceptor;

    void mainLoop();
    HMDP::Hmdl local_hmdl;
    HMDP::Base_data local_base;

protected:
    /** the reference to the parents body node */
    boost::shared_ptr<oxygen::RigidBody> mBody;
};

DECLARE_CLASS(HMDPEffector)

#endif // HMDPEFFECTOR_H
