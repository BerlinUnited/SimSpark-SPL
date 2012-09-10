/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Thu Nov 8 2005
   Copyright (C) 2005 Koblenz University

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
#include "leggedspherebehavior.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace salt;

LeggedSphereBehavior::LeggedSphereBehavior() : mZG("." PACKAGE_NAME)
{
}

void LeggedSphereBehavior::SetupJointIDMap()
{
    mJointIDMap.clear();
    mJointIDMap["lefthip"]    = JID_HIP_LEFT;
    mJointIDMap["righthip"]   = JID_HIP_RIGHT;
    mJointIDMap["leftknee"]   = JID_KNEE_LEFT;
    mJointIDMap["rightknee"]  = JID_KNEE_RIGHT;
    mJointIDMap["leftankle"]  = JID_ANKLE_LEFT;
    mJointIDMap["rightankle"] = JID_ANKLE_RIGHT;
}

string LeggedSphereBehavior::Init()
{
    mZG.GetCore()->ImportBundle("sexpparser");

    mParser = shared_static_cast<BaseParser>
        (mZG.GetCore()->New("SexpParser"));

    if (mParser.get() == 0)
    {
        cerr << "unable to create SexpParser instance." << endl;
    }

    SetupJointIDMap();

    // use the scene effector to build the agent and beam to a
    // position near the center of the playing field
    return
        "(scene rsg/agent/leggedsphere.rsg)";
}

void LeggedSphereBehavior::ParseHingeJointInfo(const oxygen::Predicate& predicate)
{
    //cout << "(LeggedSphereBehavior) parsing HJ info" << endl;

    // read the object name
    string name;
    Predicate::Iterator iter(predicate);

    if (! predicate.GetValue(iter, "name", name))
    {
        return;
    }

    // try to lookup the joint id
    TJointIDMap::iterator idIter = mJointIDMap.find(name);
    if (idIter == mJointIDMap.end())
    {
        cerr << "(LeggedSphereBehavior) unknown joint id!" << endl;
        return;
    }

    JointID jid = (*idIter).second;

    // read the angle value
    HingeJointSense sense;
    if (! predicate.GetValue(iter,"axis", sense.angle))
    {
        return;
    }

    // update the map
    mHingeJointSenseMap[jid] = sense;
}

void LeggedSphereBehavior::ParseUniversalJointInfo(const oxygen::Predicate& predicate)
{
    // read the object name
    string name;
    Predicate::Iterator iter(predicate);

    if (! predicate.GetValue(iter, "name", name))
    {
        return;
    }

    // try to lookup the joint id
    TJointIDMap::iterator idIter = mJointIDMap.find(name);
    if (idIter == mJointIDMap.end())
    {
        cerr << "(LeggedSphereBehavior) unknown joint id!" << endl;
        return;
    }

    JointID jid = (*idIter).second;

    // record the angle and rate values
    UniversalJointSense sense;

    // try to read axis1 angle
    if (! predicate.GetValue(iter,"axis1", sense.angle1))
    {
        cerr << "(LeggedSphereBehavior) could not parse universal joint angle1!" << endl;
        return;
    }
    // try to read axis1 rate
    if (! predicate.GetValue(iter,"rate1", sense.rate1))
    {
        cerr << "(LeggedSphereBehavior) could not parse universal joint rate1!" << endl;
        return;
    }   
    // try to read axis2 angle
    if (! predicate.GetValue(iter,"axis2", sense.angle2))
    {
        cerr << "(LeggedSphereBehavior) could not parse universal joint angle2!" << endl;
        return;
    }   
    // try to read axis2 rate
    if (! predicate.GetValue(iter,"rate2", sense.rate2))
    {
        cerr << "(LeggedSphereBehavior) could not parse universal joint rate2!" << endl;
        return;
    }

    //cout << "(ParseUniversalJointInfo) got angles " << sense.angle1 
    //     << " and " << sense.angle2 << endl;

    // update the map
    mUniversalJointSenseMap[jid] = sense;
}

void LeggedSphereBehavior::ParseAMotorInfo(const oxygen::Predicate& predicate)
{

}

string LeggedSphereBehavior::Think(const std::string& message)
{
    static float gain = 0.1;
    static float i = 0.0;

    // parse message and extract joint angles
    //cout << "(LeggedSphereBehavior) received message " << message << endl;

	boost::shared_ptr<PredicateList> predList =
        mParser->Parse(message);

    if (predList.get() != 0)
    {
        PredicateList& list = *predList;

        for (
             PredicateList::TList::const_iterator iter = list.begin();
             iter != list.end();
             ++iter
             )
        {
            const Predicate& predicate = (*iter);

            // check for a joint percept
            switch(predicate.name[0])
            { 
            case 'H': // hinge joint (HJ)
                ParseHingeJointInfo(predicate);
                break;
            case 'U': // universal joint (UJ)
                ParseUniversalJointInfo(predicate);
                break;
            case 'A': // angular motor (AM)
                ParseAMotorInfo(predicate);
                break;
            default:
                break;
            }
        }
    }

    // reset i after 360 degrees
    if (i == 360.0) i = 0.0;

    const float & currentLHAngle1 = mUniversalJointSenseMap[JID_HIP_LEFT].angle1; 
    const float & currentRHAngle1 = mUniversalJointSenseMap[JID_HIP_RIGHT].angle1; 

    // move left and right leg with a phase difference of 90 degrees and
    // with a maximum angle of 60 degrees
    const float desiredLHAngle1 = abs(sin(gDegToRad(i)) * 60.0);
    const float desiredRHAngle1 = abs(cos(gDegToRad(i)) * 60.0);

    i += 0.5;

    // simulating a servo motor
    const float desiredLHVel1 = gain * (desiredLHAngle1 - currentLHAngle1);
    const float desiredRHVel1 = gain * (desiredRHAngle1 - currentRHAngle1);

//     cout << "+++" << endl;
//     cout << "current angles: " << currentLHAngle1 << " " << currentRHAngle1 << endl;
//     cout << "desired angles: " << desiredLHAngle1 << " " << desiredRHAngle1 << endl;

    // send commands to server
    stringstream ss;

    ss << "(lhe "  << desiredLHVel1 << " 0.0)(rhe " << desiredRHVel1 << " 0.0)";

//     cout << "(Behavior) sending string " << ss.str() << " to server" << endl;
//     cout << "---" << endl;

    return ss.str();
}
