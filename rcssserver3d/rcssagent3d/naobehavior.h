/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Thu Mar 27 2006
   Copyright (C) 2006 RoboCup Simulation League Maintenance Committee

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
#ifndef NAOBEHAVIOR_H
#define NAOBEHAVIOR_H

#include "behavior.h"
#include <oxygen/gamecontrolserver/baseparser.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <zeitgeist/zeitgeist.h>

class NaoBehavior : public Behavior
{
public:
    struct HingeJointSense
    {
        /** joint angle */
        float angle;

        /** joint angle rate */
        float rate;

        HingeJointSense() : angle(0), rate(0) {};
    };

    struct UniversalJointSense
    {
        /** joint angle axis 1*/
        float angle1;

        /** joint angle axis 2*/
        float angle2;

        /** joint angle rate axis 1*/
        float rate1;

        /** joint angle rate axis 2*/
        float rate2;

        UniversalJointSense() : angle1(0), angle2(0), rate1(0), rate2(0) {};
    };

    enum JointID
    {
        JID_HEAD_1, 
        JID_HEAD_2,
     
        JID_LLEG_1,
        JID_RLEG_1,
        JID_LLEG_2,
        JID_RLEG_2,
        JID_LLEG_3,
        JID_RLEG_3,
        JID_LLEG_4,
        JID_RLEG_4,
        JID_LLEG_5,
        JID_RLEG_5,
        JID_LLEG_6,
        JID_RLEG_6,

        JID_LARM_1,
        JID_RARM_1,
        JID_LARM_2,
        JID_RARM_2,
        JID_LARM_3,
        JID_RARM_3,
        JID_LARM_4,
        JID_RARM_4,
    };

    enum BehaviorState
    {
    };

public:
    NaoBehavior();

    virtual std::string Init();
    virtual std::string Think(const std::string& message);

protected:
    void SetupJointIDMap();
    void ParseHingeJointInfo(const oxygen::Predicate& predicate);
    void ParseUniversalJointInfo(const oxygen::Predicate& predicate);
    void ParseHearInfo(const oxygen::Predicate& predicate);

protected:
    zeitgeist::Zeitgeist mZG;
    boost::shared_ptr<oxygen::BaseParser> mParser;

    // mapping from joint id to joint hinge sense object
    typedef std::map<JointID, HingeJointSense> THingeJointSenseMap;
    THingeJointSenseMap mHingeJointSenseMap;

    // mapping from joint id to joint hinge sense object
    typedef std::map<JointID, UniversalJointSense> TUniversalJointSenseMap;
    TUniversalJointSenseMap mUniversalJointSenseMap;

    // mapping from object name to joint id
    typedef std::map<std::string, JointID> TJointIDMap;
    TJointIDMap mJointIDMap;

    // flag whether we sent the init message yet
    bool mInit;

protected:
    enum eState
    {
        S_Wait,
        S_Sit,
        S_SwingLeft,
        S_SwingRight,
    }mState;

    int mCounter;

    double GetJointAngleDeg(JointID jid);
    double GetJointAngleRad(JointID jid);
    
};

#endif

