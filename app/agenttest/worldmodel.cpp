/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: worldmodel.cpp,v 1.8 2004/04/08 14:26:26 markelic Exp $

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
#include "worldmodel.h"

using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace oxygen;
using namespace salt;

WorldModel::WorldModel()
{
    mFieldLength = 105.0;
    mFieldWidth = 68.0;
    mFieldHeight = 20.0;
    mGoalWidth = 7.32;
    mGoalDepth = 2.0;
    mGoalHeight = 2.44;
    mBorderSize = 4.0f;

    // agent parameter
    mAgentMass = 75;
    mAgentRadius = 0.22;
    mAgentMaxSpeed = 10;

    // ball parameter
    mBallRadius = 0.111;
    mBallMass = 0.4;

    // team information
    mTeamIndex = TI_NONE;
    mTeamUnum = 0;
}

WorldModel::~WorldModel()
{
}

void
WorldModel::SetupGameStateMap()
{
    mPlayModeMap.clear();

    mPlayModeMap[STR_PM_BeforeKickOff] = PM_BeforeKickOff;
    mPlayModeMap[STR_PM_KickOff_Left]  = PM_KickOff_Left;
    mPlayModeMap[STR_PM_KickOff_Right] = PM_KickOff_Right;
    mPlayModeMap[STR_PM_PlayOn]        = PM_PlayOn;
    mPlayModeMap[STR_PM_KickIn_Left]   = PM_KickIn_Left;
    mPlayModeMap[STR_PM_KickIn_Right]  = PM_KickIn_Right;
    mPlayModeMap[STR_PM_Goal_Left]     = PM_Goal_Left;
    mPlayModeMap[STR_PM_Goal_Right]    = PM_Goal_Right;
    mPlayModeMap[STR_PM_GameOver]      = PM_GameOver;
}

void
WorldModel::SetupVisionObjectMap()
{
    mVisionObjectMap.clear();

    // <ObjectName>+<Id> -> VisionObject
    mVisionObjectMap["Flag1_l"] = VO_FLAG1L;
    mVisionObjectMap["Flag1_r"] = VO_FLAG1R;
    mVisionObjectMap["Flag2_l"] = VO_FLAG2L;
    mVisionObjectMap["Flag2_r"] = VO_FLAG2R;
    mVisionObjectMap["Goal1_l"] = VO_GOAL1L;
    mVisionObjectMap["Goal1_r"] = VO_GOAL1R;
    mVisionObjectMap["Goal2_l"] = VO_GOAL2L;
    mVisionObjectMap["Goal2_r"] = VO_GOAL2R;
    mVisionObjectMap["Ball"]    = VO_BALL;
}

bool
WorldModel::ConstructInternal()
{
    mParser = shared_dynamic_cast<BaseParser>(GetCore()->New("SexpParser"));
    if (mParser.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (WorldModel) create sexpparser parser instance\n.";

            return false;
        }

    SetupGameStateMap();
    SetupVisionObjectMap();

    return true;
}

void
WorldModel::ParseTeamIndex(const Predicate& predicate)
{
    string team;
    if (! predicate.GetValue(predicate.begin(),"team",team))
        {
            return;
        }

    // parse team index
    if (team == "left")
        mTeamIndex = TI_LEFT;
    else if (team == "right")
        mTeamIndex = TI_RIGHT;
    else
        {
            mTeamIndex = TI_NONE;
            GetLog()->Debug()
                << "(WorldModel) received TeamIndex TI_NONE\n";
        }
}

void WorldModel::ParsePlayMode(const Predicate& predicate)
{
    string mode;
    if (! predicate.GetValue(predicate.begin(),"playmode", mode))
        {
            return;
        }

    // parse play mode
    TPlayModeMap::iterator iter = mPlayModeMap.find(mode);
    if (iter != mPlayModeMap.end())
        {
            mPlayMode = (TPlayMode)(*iter).second;
        } else
            {
                GetLog()->Debug()
                    << "ERROR: (WorldModel) could not parse playmode "
                    << mode << "\n";
            }
}

void WorldModel::ParseGameState(const Predicate& predicate)
{
    ParseTeamIndex(predicate);
    ParsePlayMode(predicate);
    predicate.GetValue(predicate.begin(),"FieldLength",   mFieldLength);
    predicate.GetValue(predicate.begin(),"FieldWidth",    mFieldWidth);
    predicate.GetValue(predicate.begin(),"FieldHeigth",   mFieldHeight);
    predicate.GetValue(predicate.begin(),"GoalWidth",     mGoalWidth);
    predicate.GetValue(predicate.begin(),"GoalDepth",     mGoalDepth);
    predicate.GetValue(predicate.begin(),"GoalHeight",    mGoalHeight);
    predicate.GetValue(predicate.begin(),"BorderSize",    mBorderSize);
    predicate.GetValue(predicate.begin(),"time",          mTime);
    predicate.GetValue(predicate.begin(),"unum",          mTeamUnum);
    predicate.GetValue(predicate.begin(),"AgentRadius",   mAgentRadius);
    predicate.GetValue(predicate.begin(),"AgentMass",     mAgentMass);
    predicate.GetValue(predicate.begin(),"AgentMaxSpeed", mAgentMaxSpeed);
    predicate.GetValue(predicate.begin(),"BallRadius",    mBallRadius);
    predicate.GetValue(predicate.begin(),"BallMass",      mBallMass);
}

void WorldModel::ParseObjectVision(const Predicate& predicate)
{
    for (
         Predicate::Iterator iter(predicate);
         iter != iter.end();
         ++iter
         )
        {
            // extract the element as a parameter list
            Predicate::Iterator paramIter = iter;
            if (! predicate.DescentList(paramIter))
                {
                    continue;
                }

            // read the object name
            string name;
            if (! predicate.GetValue(paramIter,name))
            {
                continue;
            }

            // try read the 'id' section
            string strId;
            if (predicate.GetValue(paramIter,"id", strId))
                {
                    name += strId;
                }

            // try to lookup the VisionObject
            TVisionObjectMap::iterator iter = mVisionObjectMap.find(name);
            if (iter == mVisionObjectMap.end())
                {
                    continue;
                }

            VisionObject vo = (*iter).second;

            // find  to the 'pol' entry in the object's section
            Predicate::Iterator polIter = paramIter;
            if (! predicate.FindParameter(polIter,"pol"))
                {
                    continue;
                }

            // read the position vector
            VisionSense sense;
            if (
                (! predicate.AdvanceValue(polIter,sense.distance)) ||
                (! predicate.AdvanceValue(polIter,sense.theta)) ||
                (! predicate.AdvanceValue(polIter,sense.phi))
                )
            {
                continue;
            }

            // update the vision map
            mVisionMap[vo] = sense;
        }
}

void
WorldModel::ParseVision(const Predicate& predicate)
{
    ParseObjectVision(predicate);

    //
    // get our position. The current implementation of the worldmodel
    // relies on the VisionPerceptor that deliverd our position. In
    // future releases this should be replaced by a self localization
    // algorithm using the relative positions of some flags.
    //

    // find the PerfectVision data about the object
    Predicate::Iterator iter(predicate);

    // advance to the section about object 'name'
    if (! predicate.FindParameter(iter,"mypos"))
        {
            return;
        }

    // read my position
    VisionSense sense;

    predicate.GetValue(iter,mMyPos);
}

void WorldModel::Parse(const string& message)
{
    shared_ptr<PredicateList> predList =
        mParser->Parse(message);

    if (predList.get() == 0)
    {
            return;
    }

    PredicateList& list = *predList;

    for (
         PredicateList::TList::const_iterator iter = list.begin();
         iter != list.end();
         ++iter
         )
        {
            const Predicate& predicate = (*iter);

            if (predicate.name == "GameState")
                {
                    ParseGameState(predicate);
                    continue;
                }

            // check for the Vision perceptor
            if (predicate.name == "Vision")
                {
                    ParseVision(predicate);
                    continue;
                }

            GetLog()->Debug()
                << "(WorldModel) skipping unknown predicate "
                << predicate.name << "\n";
        }

    // update our position
    CalcPlayerPosition();
}

WorldModel::VisionSense WorldModel::GetVisionSense(VisionObject obj)
{
    TVisionMap::iterator iter = mVisionMap.find(obj);

    if (iter == mVisionMap.end())
        {
            GetLog()->Error()
                << "ERROR: (WorldMode::GetVisionSense) unknown VisionObject "
                << obj << "\n";

            return VisionSense();
        }

    return (*iter).second;
}

Vector3f WorldModel::GetPosition(VisionSense sense)
{
    return mMyPos + GetDriveVec(sense) * sense.distance;
}

Vector3f WorldModel::GetObjectPosition(VisionObject obj)
{
    return GetPosition(GetVisionSense(obj));
}


TPlayMode WorldModel::GetPlayMode()
{
    return mPlayMode;
}

Vector3f WorldModel::GetDriveVec(const WorldModel::VisionSense& vision)
{
    return Vector3f
        (
         vision.distance * gCos(gDegToRad(vision.theta)) *
         gSin(gDegToRad(90.0 - vision.phi)),

         vision.distance * gSin(gDegToRad(vision.theta)) *
         gSin(gDegToRad(90.0 - vision.phi)),

         vision.distance * gCos(gDegToRad(90.0 - vision.phi))
         );
}

void WorldModel::CalcPlayerPosition()
{
}

salt::Vector3f WorldModel::GetMyPosition()
{
    return mMyPos;
}

TTeamIndex WorldModel::GetMyTeam()
{
    return mTeamIndex;
}

float WorldModel::GetMinimalKickDistance()
{
    return mAgentRadius + mBallRadius + 0.04;
}

int WorldModel::GetTeamUnum()
{
    return mTeamUnum;
}


void WorldModel::SetTeamName(std::string name){
    mTeamName =   name;
}

string WorldModel::GetTeamName(){
    return mTeamName;
}

void WorldModel:: SetOppTeamName(std::string name)
{
    mOppTeamName = name;
}

string WorldModel::GetOppTeamName()
{
       return mOppTeamName;    
}





