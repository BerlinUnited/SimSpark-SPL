/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: worldmodel.h,v 1.4 2004/03/22 18:12:00 fruit Exp $

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

#ifndef WORLDMODEL_H
#define WORLDMODEL_H

#include <string>
#include <map>
#include <zeitgeist/leaf.h>
#include <soccertypes.h>
#include <oxygen/gamecontrolserver/predicate.h>
#include <oxygen/gamecontrolserver/baseparser.h>

class WorldModel : public zeitgeist::Leaf
{
public:
    struct VisionSense
    {
        /** distance perceptor to object */
        float distance;

        /** theta is the angle in the X-Y (horizontal) plane */
        float theta;

        /** phi is the latitude angle */
        float phi;

        VisionSense() : distance(0), theta(0), phi(0) {};
    };

    enum VisionObject
        {
            VO_BALL   = 0,
            VO_FLAG1L = 1,
            VO_FLAG1R = 2,
            VO_FLAG2L = 3,
            VO_FLAG2R = 4,
            VO_GOAL1L = 5,
            VO_GOAL1R = 6,
            VO_GOAL2L = 7,
            VO_GOAL2R = 8,
        };

public:
    WorldModel();
    virtual ~WorldModel();

    /** parses a message received from the server */
    void Parse(const std::string& message);

    /** returns the polar coordinates of the requested object */
    VisionSense GetVisionSense(VisionObject obj);

    /** returns the global cartesian coordinates of the sensed
        object */
    salt::Vector3f GetPosition(VisionSense sense);

    /** returns the coordinates of the requested object */
    salt::Vector3f GetObjectPosition(VisionObject obj);

    /** returns the current PlayMode */
    TPlayMode GetPlayMode();

    /** given an object position in polar coordinates and distance,
        CalcDriveVec returns a cartesian normalized vector that can be
        passed to the drive effector to reach this position.
    */
    salt::Vector3f GetDriveVec(const WorldModel::VisionSense& vision);

    /** returns the players calculated position */
    salt::Vector3f GetMyPosition();

    /** returns the players team index */
    TTeamIndex GetMyTeam();

    float GetFieldWidth() { return mFieldWidth; }
    float GetFieldLength() { return mFieldLength; }

    float GetAgentRadius() { return mAgentRadius; }
    float GetBallRadius() { return mBallRadius; }

    float GetMinimalKickDistance();
    int GetTeamUnum();

protected:
    virtual bool ConstructInternal();

    void CalcPlayerPosition();

    void ParseObjectVision(const oxygen::Predicate& predicate);
    void ParseGameState(const oxygen::Predicate& predicate);
    void ParseVision(const oxygen::Predicate& predicate);

    void ParsePlayMode(const oxygen::Predicate& predicate,
                       oxygen::Predicate::Iterator& objIter);
    void ParseTeamIndex(const oxygen::Predicate& predicate,
                        oxygen::Predicate::Iterator& objIter);

    void SetupGameStateMap();
    void SetupVisionObjectMap();


    template<typename TYPE>
    void GetGameStateParam(const oxygen::Predicate& predicate,
                           const std::string& name, TYPE& value)
    {
        // find the GameStateData
        oxygen::Predicate::Iterator objIter(predicate);

        if (! predicate.FindParameter(objIter,name))
            {
                return;
            }

        const void* var = (void*)(&value);

        // parse special cases
        if (var == &mTeamIndex)
            {
                ParseTeamIndex(predicate,objIter);
            } else if (var == &mPlayMode)
                {
                    ParsePlayMode(predicate,objIter);
                } else
                    {
                        // parse generic cases
                        predicate.GetValue(objIter,value);
                    }
    }

protected:
    // mapping from gamestate string to TPlayMode
    typedef std::map<std::string, TPlayMode> TPlayModeMap;
    TPlayModeMap mPlayModeMap;

    // mapping from vision object to VisionSense
    typedef std::map<VisionObject, VisionSense> TVisionMap;
    TVisionMap mVisionMap;

    // mapping from <ObjectName>+<ID> to VisionObject
    typedef std::map<std::string, VisionObject> TVisionObjectMap;
    TVisionObjectMap mVisionObjectMap;

    // parser instance
    boost::shared_ptr<oxygen::BaseParser> mParser;

    // soccer variables
    float mFieldLength;
    float mFieldWidth;
    float mFieldHeight;
    float mGoalWidth;
    float mGoalDepth;
    float mGoalHeight;
    float mBorderSize;

    // agent parameter
    float mAgentMass;
    float mAgentRadius;
    float mAgentMaxSpeed;

    // ball parameter
    float mBallRadius;
    float mBallMass;

    // team information
    TTeamIndex mTeamIndex;
    unsigned int mTeamUnum;

    // game state information
    float mTime;
    TPlayMode mPlayMode;

    // the players position
    salt::Vector3f mMyPos;
};

DECLARE_CLASS(WorldModel);

#endif // WORLDMODEL_H
