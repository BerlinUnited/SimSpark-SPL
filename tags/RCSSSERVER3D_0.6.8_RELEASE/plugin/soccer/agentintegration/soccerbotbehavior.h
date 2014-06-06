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
#ifndef SOCCERBOTBEHAVIOR_H
#define SOCCERBOTBEHAVIOR_H

#include <oxygen/gamecontrolserver/behavior.h>
#include <oxygen/gamecontrolserver/predicate.h>

class SoccerbotBehavior : public oxygen::Behavior
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
        JID_HEAD_1   = 0,
        JID_HEAD_2   = 1,
        JID_LLEG_1   = 2,
        JID_RLEG_1   = 3,
        JID_LLEG_2_3 = 4,
        JID_RLEG_2_3 = 5,
        JID_LLEG_4   = 6,
        JID_RLEG_4   = 7,
        JID_LLEG_5_6 = 8,
        JID_RLEG_5_6 = 9,
        JID_LARM_1_2 = 10,
        JID_RARM_1_2 = 11,
        JID_LARM_3   = 12,
        JID_RARM_3   = 13,
        JID_LARM_4   = 14,
        JID_RARM_4   = 15,
        JID_LARM_5   = 16,
        JID_RARM_5   = 17
    };

    enum BehaviorState
    {
        ARM_UP = 0,
        ARM_ROTATE = 1,
        ARM_WAVE_1 = 2,
        ARM_WAVE_2 = 3,
        ARM_WAIT   = 4
    };

public:
    SoccerbotBehavior();

    ~SoccerbotBehavior();

    virtual std::string Init();

    virtual void Think(boost::shared_ptr<oxygen::PredicateList> senselist);

protected:
    void OnLink();
    void OnUnlink();

    void SetupJointIDMap();
    void ParseHingeJointInfo(const oxygen::Predicate& predicate);
    void ParseUniversalJointInfo(const oxygen::Predicate& predicate);
    void ParseHearInfo(const oxygen::Predicate& predicate);

protected:
    // mapping from joint id to joint hinge sense object
    typedef std::map<JointID, HingeJointSense> THingeJointSenseMap;
    THingeJointSenseMap mHingeJointSenseMap;

    // mapping from joint id to joint hinge sense object
    typedef std::map<JointID, UniversalJointSense> TUniversalJointSenseMap;
    TUniversalJointSenseMap mUniversalJointSenseMap;

    // mapping from object name to joint id
    typedef std::map<std::string, JointID> TJointIDMap;
    TJointIDMap mJointIDMap;

    // simulation cycle
    int mCycle;
};

DECLARE_CLASS(SoccerbotBehavior);

#endif // SOCCERBOTBEHAVIOR_H
