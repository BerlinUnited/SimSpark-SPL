/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: worldmodel.cpp,v 1.1.2.2 2004/02/08 14:01:46 rollmark Exp $

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
    mPlayModeMap[STR_PM_KickOff]       = PM_KickOff;
    mPlayModeMap[STR_PM_PlayOn]        = PM_PlayOn;
    mPlayModeMap[STR_PM_KickOff_Left]  = PM_KickOff_Left;
    mPlayModeMap[STR_PM_KickOff_Right] = PM_KickOff_Right;
    mPlayModeMap[STR_PM_FirstHalfOver] = PM_FirstHalfOver;
}


bool
WorldModel::ConstructInternal()
{
    // init the s-expression parser
    if (! GetCore()->ImportBundle("sexpparser"))
        {
            GetLog()->Error()
                << "ERROR: (WorldModel) cannot import bundle sexpparser\n";
            return false;
        }

    mParser = shared_dynamic_cast<BaseParser>(GetCore()->New("SexpParser"));
    if (mParser.get() == 0)
        {
            GetLog()->Error()
                << "ERROR: (WorldModel) create sexpparser parser instance\n.";

            return false;
        }

    SetupGameStateMap();

    return true;
}

void
WorldModel::ParseTeamIndex(const Predicate& predicate,
                           Predicate::Iterator& objIter)
{
    // parse team index
    string team;
    if (predicate.GetValue(objIter,team))
        {
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
}

void WorldModel::ParsePlayMode(const Predicate& predicate,
                               Predicate::Iterator& objIter)
{
    string mode;
    // parse play mode
    if (predicate.GetValue(objIter,mode))
        {
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
}


void WorldModel::ParseGameState(const Predicate& predicate)
{
    GetGameStateParam(predicate, "FieldLength", mFieldLength);
    GetGameStateParam(predicate, "FieldWidth",  mFieldWidth);
    GetGameStateParam(predicate, "FieldHeigth", mFieldHeight);
    GetGameStateParam(predicate, "GoalWidth",   mGoalWidth);
    GetGameStateParam(predicate, "GoalDepth",   mGoalDepth);
    GetGameStateParam(predicate, "GoalHeight",  mGoalHeight);
    GetGameStateParam(predicate, "BorderSize",  mBorderSize);
    GetGameStateParam(predicate, "time",  mTime);
    GetGameStateParam(predicate, "playmode",mPlayMode);
    GetGameStateParam(predicate, "unum",  mTeamUnum);
    GetGameStateParam(predicate, "team",  mTeamIndex);
}

bool WorldModel::GetVision(const Predicate& predicate, const string& name,
                           const VisionObject obj)
{
    // find the PerfectVision data about the object
    Predicate::Iterator iter(predicate);

    // advance to the section about object 'name'
    if (! predicate.FindParameter(iter,name))
        {
            return false;
        }

    // advance to the 'pos' entry in the object's section
    if (! predicate.FindParameter(iter,"pol"))
        {
            return false;
        }

    // read the position vector
    VisionSense sense;
    if (
        (! predicate.GetValue(iter,sense.distance)) ||
        (! predicate.GetValue(iter,sense.theta)) ||
        (! predicate.GetValue(iter,sense.phi))
        )
        {
            return false;
        }

    mVisionMap[obj] = sense;

    return true;
}

void
WorldModel::ParseVision(const Predicate& predicate)
{
    GetVision(predicate,"Ball",VO_BALL);
    GetVision(predicate,"Flag_1_l",VO_FLAG1L);
    GetVision(predicate,"Flag_1_r",VO_FLAG1R);
    GetVision(predicate,"Flag_2_l",VO_FLAG2L);
    GetVision(predicate,"Flag_2_r",VO_FLAG2R);

    GetVision(predicate,"Goal_1_l",VO_GOAL1L);
    GetVision(predicate,"Goal_1_r",VO_GOAL1R);
    GetVision(predicate,"Goal_2_l",VO_GOAL2L);
    GetVision(predicate,"Goal_2_r",VO_GOAL2R);
}

void WorldModel::Parse(const string& message)
{
    shared_ptr<Predicate::TList> predicates =
        mParser->Parse(message);

    for (
         Predicate::TList::const_iterator iter = predicates->begin();
         iter != predicates->end();
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

TPlayMode WorldModel::GetPlayMode()
{
    return mPlayMode;
}

Vector3f WorldModel::GetDashVec(const WorldModel::VisionSense& vision)
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

salt::Vector3f GetMyPosition()
{
    return Vector3f(0,0,0);
}





