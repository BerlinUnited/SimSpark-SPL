/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccermonitor.h,v 1.2 2004/12/30 15:56:22 rollmark Exp $

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
#ifndef SPARK_SOCCERMONITOR_H
#define SPARK_SOCCERMONITOR_H

#include <oxygen/monitorserver/custommonitor.h>
#include <soccer/soccertypes.h>
#include <map>
#include <string>

class SoccerMonitor : public oxygen::CustomMonitor
{
public:
    enum EPredicate
        {
            P_PLAYMODES, //! list of play mode strings
            P_TIME,      //! game time update
            P_PLAYMODE,  //! play mode update
            P_HALF       //! game half update
        };

    typedef std::map<std::string, EPredicate> TPredicateMap;

public:
    SoccerMonitor();
    virtual ~SoccerMonitor();

    /** This function will be called periodically to do any monitor
        updates. Custom predicates received via the SparkMonitorClient
        are passed as a PredicateList
     */
    virtual void ParseCustomPredicates(const oxygen::PredicateList& pList);

    TTime GetTime() const;

    TPlayMode GetPlayMode() const;
    std::string GetPlayModeString() const;

    TGameHalf GetGameHalf() const;
    std::string GetGameHalfString() const;

protected:
    void ParsePredicates(const oxygen::PredicateList& pList);
    void ParsePlayModes(const oxygen::Predicate& pred);

protected:
    void SetupPredicateMap();

protected:
    TPredicateMap mPredMap;

    float mTime;
    int mPlayMode;
    int mHalf;

    std::vector<std::string> mPlayModes;
};

DECLARE_CLASS(SoccerMonitor);

#endif // SPARKMONITORCLIENT_H
