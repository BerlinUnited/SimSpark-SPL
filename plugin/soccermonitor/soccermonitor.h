/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccermonitor.h,v 1.1 2004/12/22 16:15:19 rollmark Exp $

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
#include <map>
#include <string>

class SoccerMonitor : public oxygen::CustomMonitor
{
public:
    enum EPredicate
        {
            P_TIME,
            P_PLAYMODE
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

protected:
    void ParsePredicates(const oxygen::PredicateList& pList);

protected:
    void SetupPredicateMap();

protected:
    TPredicateMap mPredMap;

    float mTime;
    int mPlayMode;
};

DECLARE_CLASS(SoccerMonitor);

#endif // SPARKMONITORCLIENT_H
