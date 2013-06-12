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
#include "naobehavior.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace salt;

const double PI = 3.1415926;

NaoBehavior::NaoBehavior() : mZG("." PACKAGE_NAME), mInit(false)
{
    mState = S_Wait;
    mCounter = 0;
}

void NaoBehavior::SetupJointIDMap()
{
    mJointIDMap.clear();
    mJointIDMap["hj1"]      = JID_HEAD_1;
    mJointIDMap["hj2"]      = JID_HEAD_2;

    mJointIDMap["llj1"]     = JID_LLEG_1;
    mJointIDMap["rlj1"]     = JID_RLEG_1;
    mJointIDMap["llj2"]     = JID_LLEG_2;
    mJointIDMap["rlj2"]     = JID_RLEG_2;
    mJointIDMap["llj3"]     = JID_LLEG_3;
    mJointIDMap["rlj3"]     = JID_RLEG_3;
    mJointIDMap["llj4"]     = JID_LLEG_4;
    mJointIDMap["rlj4"]     = JID_RLEG_4;
    mJointIDMap["llj5"]     = JID_LLEG_5;
    mJointIDMap["rlj5"]     = JID_RLEG_5;
    mJointIDMap["llj6"]     = JID_LLEG_6;
    mJointIDMap["rlj6"]     = JID_RLEG_6;

    mJointIDMap["laj1"]     = JID_LARM_1;
    mJointIDMap["raj1"]     = JID_RARM_1;
    mJointIDMap["laj2"]     = JID_LARM_2;
    mJointIDMap["raj2"]     = JID_RARM_2;
    mJointIDMap["laj3"]     = JID_LARM_3;
    mJointIDMap["raj3"]     = JID_RARM_3;
    mJointIDMap["laj4"]     = JID_LARM_4;
    mJointIDMap["raj4"]     = JID_RARM_4;
}

std::string gJointEff_2_Str[] =
{
"he1",
"he2",
"lle1",
"rle1",
"lle2",
"rle2",
"lle3",
"rle3",
"lle4",
"rle4",
"lle5",
"rle5",
"lle6",
"rle6",
"lae1",
"rae1",
"lae2",
"rae2",
"lae3",
"rae3",
"lae4",
"rae4",
};

string NaoBehavior::Init()
{
    mZG.GetCore()->ImportBundle("sexpparser");

    mParser = static_pointer_cast<BaseParser>
        (mZG.GetCore()->New("SexpParser"));

    if (mParser.get() == 0)
    {
        cerr << "unable to create SexpParser instance." << endl;
    }

    SetupJointIDMap();
    // use the scene effector to build the agent and beam to a
    // position near the center of the playing field
    return
        "(scene rsg/agent/nao/nao_hetero.rsg 0)";
}

void NaoBehavior::ParseHingeJointInfo(const oxygen::Predicate& predicate)
{
    //cout << "(NaoBehavior) parsing HJ info" << endl;

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
        cerr << "(NaoBehavior) unknown joint id!" << endl;
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

void NaoBehavior::ParseUniversalJointInfo(const oxygen::Predicate& predicate)
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
        cerr << "(NaoBehavior) unknown joint id!" << endl;
        return;
    }

    JointID jid = (*idIter).second;

    // record the angle and rate values
    UniversalJointSense sense;

    // try to read axis1 angle
    if (! predicate.GetValue(iter,"ax1", sense.angle1))
    {
        cerr << "(NaoBehavior) could not parse universal joint angle1!" << endl;
        return;
    }
    // try to read axis2 angle
    if (! predicate.GetValue(iter,"ax2", sense.angle2))
    {
        cerr << "(NaoBehavior) could not parse universal joint angle2!" << endl;
        return;
    }
    // try to read axis2 rate

    // update the map
    mUniversalJointSenseMap[jid] = sense;
}


// This is just a demo implementation. Message is printed and discarded

void NaoBehavior::ParseHearInfo(const oxygen::Predicate& predicate)
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


double NaoBehavior::GetJointAngleDeg(JointID jid)
{
    return mHingeJointSenseMap[jid].angle;
}


double NaoBehavior::GetJointAngleRad(JointID jid)
{
    return GetJointAngleDeg(jid) * PI / 180.0;
}


string NaoBehavior::Think(const std::string& message)
{
    if (! mInit)
    {
        mInit = true;
        return "(init (unum 0)(teamname NaoRobot))";
    }

    // parse message and extract joint angles
    boost::shared_ptr<PredicateList> predList = mParser->Parse(message);

    if (predList.get() != 0)
    {
        PredicateList& list = *predList;

        for (PredicateList::TList::const_iterator iter = list.begin();
             iter != list.end();
             ++iter)
        {
            const Predicate& predicate = (*iter);

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

    //Action
    stringstream ss;
    switch (mState)
    {
    case S_Wait:
        if (mCounter > 50) { mCounter = 0; mState = S_Sit; break; }
        break;

    case S_Sit:
        if (mCounter > 40)
        {
            mCounter = 0;
            mState = S_SwingLeft;
            ss << "(lae1 0)"
               << "(rae1 0)"
               << "(lae2 0)"
               << "(rae2 0)"
               << "(lae3 0)"
               << "(rae3 0)"
               << "(lae4 0)"
               << "(rae4 0)"
               << "(lle1 0)"
               << "(rle1 0)"
               << "(lle2 0)"
               << "(rle2 0)"
               << "(lle3 0)"
               << "(rle3 0)"
               << "(lle4 0)"
               << "(rle4 0)"
               << "(lle5 0)"
               << "(rle5 0)"
               << "(lle6 0)"
               << "(rle6 0)";
            break;
        }
        ss << "(lae1 -1.5)"
           << "(rae1 -1.5)"
           << "(lae2 0.9)"
           << "(rae2 -0.9)"
           << "(lae3 0)"
           << "(rae3 0)"
           << "(lae4 -0.8)"
           << "(rae4 0.8)"
           << "(lle3 1)"
           << "(rle3 1)"
           << "(lle4 -2)"
           << "(rle4 -2)"
           << "(lle5 1)"
           << "(rle5 1)"
           << "(lle6 -0)"
           << "(rle6 0)";
        break;

    case S_SwingLeft:
        if (mCounter > 50) { mCounter = 0; mState = S_SwingRight; break; }
        ss << "(he1 -1)";
        break;

    case S_SwingRight:
        if (mCounter > 50) { mCounter = 0; mState = S_SwingLeft;  break; }
        ss << "(he1 1)";
        break;
    }

    mCounter++;

    if (mCounter % 10 == 0) ss << "(say ComeOn!)";

    //std::cout << "Sent: " << ss.str() << "\n";
    return ss.str();
}
