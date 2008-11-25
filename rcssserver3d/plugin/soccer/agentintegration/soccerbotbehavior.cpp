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
#include "soccerbotbehavior.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace std;
using namespace boost;
using namespace salt;
using namespace zeitgeist;
using namespace oxygen;

SoccerbotBehavior::SoccerbotBehavior() : mCycle(0)
{
}

SoccerbotBehavior::~SoccerbotBehavior()
{
}

void SoccerbotBehavior::OnLink()
{
    Behavior::OnLink();

    SetupJointIDMap();
}

void SoccerbotBehavior::OnUnlink()
{
    Behavior::OnUnlink();
}

void SoccerbotBehavior::SetupJointIDMap()
{
    mJointIDMap.clear();
    //mJointIDMap["head_joint_2"]   = JID_HEAD_2;
    //mJointIDMap["head_joint_1"]   = JID_HEAD_1;
    mJointIDMap["llj1"]   = JID_LLEG_1;
    mJointIDMap["rlj1"]   = JID_RLEG_1;
    mJointIDMap["llj2_3"] = JID_LLEG_2_3;
    mJointIDMap["rlj2_3"] = JID_RLEG_2_3;
    mJointIDMap["llj4"]   = JID_LLEG_4;
    mJointIDMap["rlj4"]   = JID_RLEG_4;
    mJointIDMap["llj5_6"] = JID_LLEG_5_6;
    mJointIDMap["rlj5_6"] = JID_RLEG_5_6;
    mJointIDMap["laj1_2"] = JID_LARM_1_2;
    mJointIDMap["raj1_2"] = JID_RARM_1_2;
    mJointIDMap["laj3"]   = JID_LARM_3;
    mJointIDMap["raj3"]   = JID_RARM_3;
    mJointIDMap["laj4"]   = JID_LARM_4;
    mJointIDMap["raj4"]   = JID_RARM_4;
    mJointIDMap["laj5"]   = JID_LARM_5;
    mJointIDMap["raj5"]   = JID_RARM_5;
}

string SoccerbotBehavior::Init()
{
    // use the scene effector to build the agent and beam to a
    // position near the center of the playing field
    return
        "(scene rsg/agent/soccerbot058/soccerbot.rsg)";
}

void SoccerbotBehavior::ParseHingeJointInfo(const oxygen::Predicate& predicate)
{
    //cout << "(SoccerbotBehavior) parsing HJ info" << endl;

    // read the object name
    string name;
    Predicate::Iterator iter(predicate);

    if (! predicate.GetValue(iter, "n", name))
    {
        return;
    }

    // try to lookup the joint id
    TJointIDMap::iterator idIter = mJointIDMap.find(name);
    if (idIter == mJointIDMap.end())
    {
        cerr << "(SoccerbotBehavior) unknown joint id!" << endl;
        return;
    }

    JointID jid = (*idIter).second;

    // read the angle value
    HingeJointSense sense;
    if (! predicate.GetValue(iter,"ax", sense.angle))
    {
        return;
    }

    // update the map
    mHingeJointSenseMap[jid] = sense;
}

void SoccerbotBehavior::ParseUniversalJointInfo(const oxygen::Predicate& predicate)
{
    // read the object name
    string name;
    Predicate::Iterator iter(predicate);

    if (! predicate.GetValue(iter, "n", name))
    {
        return;
    }

    // try to lookup the joint id
    TJointIDMap::iterator idIter = mJointIDMap.find(name);
    if (idIter == mJointIDMap.end())
    {
        cerr << "(SoccerbotBehavior) unknown joint id!" << endl;
        return;
    }

    JointID jid = (*idIter).second;

    // record the angle and rate values
    UniversalJointSense sense;

    // try to read axis1 angle
    if (! predicate.GetValue(iter,"ax1", sense.angle1))
    {
        cerr << "(SoccerbotBehavior) could not parse universal joint angle1!" << endl;
        return;
    }
    // try to read axis2 angle
    if (! predicate.GetValue(iter,"ax2", sense.angle2))
    {
        cerr << "(SoccerbotBehavior) could not parse universal joint angle2!" << endl;
        return;
    }
    // try to read axis2 rate

    // update the map
    mUniversalJointSenseMap[jid] = sense;
}

// This is just a demo implementation. Message is printed and discarded
void SoccerbotBehavior::ParseHearInfo(const oxygen::Predicate& predicate)
{
    double heartime;
    string sender;
    string message;

    Predicate::Iterator iter(predicate);

    if (! predicate.AdvanceValue(iter, heartime))
    {
        cerr << "could not get hear time \n";
        return;
    }

    if (! predicate.AdvanceValue(iter, sender))
    {
        cerr << "could not get sender \n";
        return;
    }


    if (! predicate.GetValue(iter, message))
    {
        cerr << "could not get message \n";
        return;
    }

    if (sender == "self")
    {
        cout << "I said " << message << " at " << heartime << endl;

    } else {
        cout << "Someone "
             << (abs(atof(sender.c_str()))<90?"in front of":"behind")
             << " me said " << message << " at " << heartime << endl;
    }
    return;
}

void SoccerbotBehavior::Think(boost::shared_ptr<oxygen::PredicateList> senselist)
{
    mCommand.clear();
    mCycle++;
    
    if ( 1 == mCycle)
    {
        mCommand = Init();
        return;
    }

    if ( 2 == mCycle ){
        mCommand = "(init (unum 0)(teamname IntegratedAgent))";
        return;
    }

    shared_ptr<PredicateList> predList = senselist;

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
            case 'h': // hear
                ParseHearInfo(predicate);
                break;
            default:
                break;
            }
        }
    }

    stringstream ss("");

    static const float gain = 0.1;
    static BehaviorState leftstate = ARM_UP;
    static BehaviorState rightstate = ARM_UP;

    float curAngle = 0;
    float newAngle = 0;

#if 1
    switch(rightstate)
    {
    case ARM_UP:
        curAngle = mUniversalJointSenseMap[JID_RARM_1_2].angle2;
        if (curAngle > -90.0)
        {
            newAngle = gain * (-90.1 - curAngle);
            ss << "(rae1_2 0.0 " << newAngle << ")";
        }
        else
        {
            rightstate = ARM_ROTATE;
        }
        break;
    case ARM_ROTATE:
        curAngle = mHingeJointSenseMap[JID_RARM_3].angle;
        //cout << curAngle << endl;

        if (curAngle > -90.0)
        {
            newAngle = gain * (-90.1 - curAngle);
            ss << "(rae3 " << newAngle << ")";
        }
        else
        {
            rightstate = ARM_WAVE_1;
        }
        break;
    case ARM_WAVE_1:
        curAngle = mHingeJointSenseMap[JID_RARM_4].angle;
        if (curAngle < 90.0)
        {
            newAngle = gain * (90.1 - curAngle);
            ss << "(rae4 " << newAngle << ")"; 
        }
        else
        {
            ss << "(say woohoo)";
            rightstate = ARM_WAVE_2;
        }
        break;
    case ARM_WAVE_2:
        curAngle = mHingeJointSenseMap[JID_RARM_4].angle;
        if (curAngle > 45.0 || curAngle <= 44.5)
        {
            newAngle = gain * (44.9 - curAngle);
            ss << "(rae4 " << newAngle << ")";
        }
        else
        {
            rightstate = ARM_WAVE_1;
        }
        break;
    default:
        break;
    }
#endif

#if 1
    switch(leftstate)
    {
    case ARM_UP:
        curAngle = mUniversalJointSenseMap[JID_LARM_1_2].angle2;
        if (curAngle < 90.0)
        {
            newAngle = gain * (90.1 - curAngle);
            ss << "(lae1_2 0.0 " << newAngle << ")";
            //cout << "curang = " << curAngle << " / newang = " << newAngle << "\n";

        }
        else
        {
            leftstate = ARM_ROTATE;
        }
        break;
    case ARM_ROTATE:
        curAngle = mHingeJointSenseMap[JID_LARM_3].angle;
        //cout << curAngle << endl;

        if (curAngle < 90.0)
        {
            newAngle = gain * (90.1 - curAngle);
            ss << "(lae3 " << newAngle << ")";
        }
        else
        {
            leftstate = ARM_WAVE_1;
        }
        break;
    case ARM_WAVE_1:
        curAngle = mHingeJointSenseMap[JID_LARM_4].angle;
        if (curAngle < 90.0)
        {
            newAngle = gain * (90.1 - curAngle);
            ss << "(lae4 " << newAngle << ")"; 
        }
        else
        {
            leftstate = ARM_WAVE_2;
        }
        break;
    case ARM_WAVE_2:
        curAngle = mHingeJointSenseMap[JID_LARM_4].angle;
        if (curAngle > 45.0 || curAngle <= 44.5)
        {
            newAngle = gain * (44.9 - curAngle);
            ss << "(lae4 " << newAngle << ")";
        }
        else
        {
            leftstate = ARM_WAVE_1;
        }
        break;
    default:
        break;
    }

#endif

    //    cout << "+++" << endl;
    //    cout << "current angle: " << curAngle  << endl;
    //    cout << "desired angle: " << newAngle  << endl;
    //    cout << "(Behavior) sending string " << ss.str() << " to server" << endl;
    //    cout << "State is " << state << endl;
    //    cout << "---" << endl;

    mCommand = ss.str();
}
