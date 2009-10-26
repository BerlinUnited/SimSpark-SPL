/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D

   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   Copyright (C) 2008 N. Michael Mayer, email: nmmayer@gmail.com

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

#include "hmdpeffector.h"
#include "hmdpperceptor.h"
#include "hmdpaction.h"
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/gamecontrolserver/actionobject.h>
#include <oxygen/agentaspect/jointeffector.h>
#include <oxygen/physicsserver/hingejoint.h>

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace HMDP;


//! Handler for HMDP Effector and Perceptor back and forward

HMDPPerceptor *hmdpPerceptorHandle = NULL;
HMDPEffector *hmdpEffectorHandle = NULL;


namespace HMDP
{
extern "C"
{
using namespace HMDP;

int lock;
extern Hmdl *hmdl;
extern Base_data *base_data;
extern int readChar;
void disableIRQ(); //! mimicks IRQ functionality
void enableIRQ();

}
}


HMDPEffector::HMDPEffector(): oxygen::Effector()
{
    ifActive = false;
}

HMDPEffector::~HMDPEffector()
{
    HMDP::lock = 0;
}

void HMDPEffector::PrePhysicsUpdateInternal(float deltaTime)
{
    if (iter == 0)
    {
        ReadOutJointList();
        InitHMDP();
    }
    iter++;

    if (!ifActive)
        GetLog()->Normal() << "MAIN LOOP NOT ACTIVE THOUGH!!!" << std::endl;
    mainLoop();

    if (mAction.get() == 0 || mBody.get() == 0)
    {
        return;
    }

    shared_ptr<HMDPAction> hMDPAction = shared_dynamic_cast<HMDPAction>(mAction);
    mAction.reset();
    if (hMDPAction.get() == 0)
    {
        GetLog()->Error()
                << "ERROR: (HMDPEffector) cannot realize an unknown ActionObject\n";
        return;
    }

}

shared_ptr<ActionObject> HMDPEffector::GetActionObject(const Predicate& predicate)
{
         //std::cout << " GetActionObject called " << std::endl;
    if (predicate.name != GetPredicate())
    {
        GetLog()->Error() << "ERROR: (HMDPEffector) invalid predicate"
                << predicate.name << "\n";
        return shared_ptr<ActionObject>();
    }

    std::string message;

    if (!predicate.GetValue(predicate.begin(), message))
    {
        GetLog()->Error()
                << "ERROR: (HMDPEffector) Some Problem while receiving the HMDP Message\n";
        return shared_ptr<ActionObject>();
    };

    inMessage = inMessage + message + "\r\n";

    //std::cout << inMessage << std::endl;
    return shared_ptr<ActionObject>(new HMDPAction(GetPredicate(), inMessage));
}

void HMDPEffector::OnLink()
{
    hmdpEffectorHandle = this;
    perceptor = hmdpPerceptorHandle;
    std::cout << "Perceptor points to " << perceptor << std::endl;
    std::cout << "in OnLink " << std::endl;
    ifActive = true;

    iter = 0; // just to do the right things on iter = 0 in pre physics routine
    shared_ptr<Node> parent = GetParent().lock();

    if (parent.get() == 0)
    {
        GetLog()->Error()
                << "ERROR: (HMDPEffector) parent node is not derived from BaseNode\n";
        return;
    }

    // parent should be a transform, or some other node, which has a
    // Body-child
    mBody = shared_dynamic_cast<Body>(parent->GetChildOfClass("Body"));

    if (mBody.get() == 0)
    {
        GetLog()->Error()
                << "ERROR: (HMDPEffector) parent node has no Body child;"
                    "cannot apply HMDP\n";
        return;
    }

    inMessage = ""; //! initialize the incoming messages stored in this string
}

void HMDPEffector::OnUnlink()
{
    mBody.reset();
    HMDP::lock = 0;
    ifActive = false;
}

/****************************************************************************************************
                 SPECIAL HMDP RELATED METHODS of the Effecotr Class
****************************************************************************************************/
float HMDPEffector::zeroPosServo(int id) //! this is to define a standard position for HMDP ( some kind of standing straight with arms diagonal down )
{
    return nao.zeroPosition(id); //! NAO related function inside the plugin
}

//! it is important for the parser to go the beginning of the new line -- no matter if the p
void HMDPEffector::searchForNextLinestartInMessage()
{
    int state = 0;
    while (state < 3)
    {
        char a = inMessage[0];
        if ((a == 13) && (state == 0))
            state = 1;
        if (((a != 13) || (a != 10)) && (state == 1))
            state = 2;
        inMessage = inMessage.substr(1, inMessage.size() - 1);
        if (state == 2)
            state = 3;
    }
}

void HMDPEffector::sendMessage(std::string message)
{
    //std::cout << "message in effector "<< message << std::endl;
    perceptor->sendMessage(message);
    // goes into perceptor part -- this is the only part where the perceptor is needed
}

void HMDPEffector::ReadOutJointList()
{

    shared_ptr<Node> parent = GetParent().lock();
    shared_ptr<Node> grandparent = parent->GetParent().lock();

    grandparent->ListChildrenSupportingClass<HingeJoint>(jointList, true);

    // This vectors describe the state of each joint and thus there need to be an entry for each joint
    servo_target_pos.resize(jointList.size());
    servo_gain.resize(jointList.size());
    servo_angle.resize(jointList.size());

    int i = 0;
    for (TLeafList::iterator j_it = jointList.begin(); j_it != jointList.end(); j_it++)
    { // set all to yzero (necessary ?)

        servo_target_pos[i] = 0.;
        servo_gain[i] = 0.05;
        servo_angle[i] = 0;

        shared_ptr<Leaf> join = *j_it;
        shared_ptr<BaseNode> jparent =
            shared_dynamic_cast<BaseNode>(join->GetParent().lock());
        std::cout << i << "    " << jparent->GetName() << std::endl;
        i++;

    }
}

bool HMDPEffector::checkIfServoIDExists(int ID) //! checks if a joint with ID ID exists
{
    if ((ID >= 0) && (ID < jointList.size()))
        return true;
    return false;
}

void HMDPEffector::mainLoop() // HMDP update method
{
    prepareUsage();

    int watchdog = 0;
    hmdpEffectorHandle = this; // just to make sure you have the right one set -- in the case of several agents

    while ((inMessage.size() > 0) && (watchdog < 100))
    {
        watchdog++;
        inMessage = inMessage;
        //if (inMessage.size()>10) std::cout << "GOT LARGE MESS " << this << std::endl;
        HMDP::parse_one_line(); // HMDP parser reads one line in one call

    }
    HMDP::lock = 0;

    if (ifIRQ)
        HMDP::inter_routine_base(); // real time functionality (!) now  in the loop
    controlPosServo(); // servo update loop

}

void HMDPEffector::controlPosServo() // for controlling servos
{
    int i = 0;
    for (TLeafList::iterator j_it = jointList.begin(); j_it != jointList.end(); j_it++)
    {

        shared_ptr<HingeJoint> joint = shared_static_cast<HingeJoint> (*j_it);
        servo_angle[i] = joint->GetAngle() - zeroPosServo(i);
        double tpos = servo_target_pos[i];
        float err = servo_gain[i] * (tpos - servo_angle[i]);

        joint->SetParameter(dParamVel, err);

        if (abs(err) > 0.00001)
        {
            shared_ptr<Body> body = joint->GetBody(Joint::BI_FIRST);
            if (body && !body->IsEnabled())
            {
                body->Enable();
            }
        }

        i++;
    }
}

void HMDPEffector::prepareUsage()
{
    while (lock)
    {
        //std::cout <<" waiting in lock "<<std::endl;
    };
    hmdl = &local_hmdl;
    base_data = &local_base;
    lock = 1;
}

void HMDPEffector::InitHMDP() //! startup of HMDP
{
    prepareUsage();
    HMDP::init_base(); //! boot the base part
    HMDP::init_hmdl(); //! boot the hmdp part
    HMDP::enableIRQ(); /* start the IRQ */

    for (int i = 0; i < 64; i++)
    {
        local_base.zero_pos_inits_feed[i] = 32* 64 ;

        if (checkIfServoIDExists(i))
        {
            std::cout << nao.getJointName(i) << std::endl;
            for (int j=0; j<6;j++) HMDP::jointnames[i][j] = nao.getJointName(i).c_str()[j];
            HMDP::jointnames[i][7]=0; //! stop signal for c-string
        }
    }
    local_base.zero_pos_inits = &(local_base.zero_pos_inits_feed[0]);
    HMDP::lock=0;
}
