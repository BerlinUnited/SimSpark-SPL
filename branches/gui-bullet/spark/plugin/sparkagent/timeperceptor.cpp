/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: timeperceptor.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "timeperceptor.h"
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

TimePerceptor::TimePerceptor() : Perceptor()
{
}

TimePerceptor::~TimePerceptor()
{
}

void TimePerceptor::OnLink()
{
    mSimulationServer = shared_dynamic_cast<SimulationServer>
        (GetCore()->Get("/sys/server/simulation"));

    if (mSimulationServer.get() == 0)
        {
            GetLog()->Error()
                << "(TimePerceptor) ERROR: SimulationServer not found\n";
        }
}

void TimePerceptor::OnUnlink()
{
    mSimulationServer.reset();
}

bool TimePerceptor::Percept(boost::shared_ptr<oxygen::PredicateList> predList)
{
    if (mSimulationServer.get() == 0)
        {
            return false;
        }

    Predicate& predicate = predList->AddPredicate();
    predicate.name = "time";
    predicate.parameter.Clear();

    ParameterList& nowElement = predicate.parameter.AddList();
    nowElement.AddValue(string("now"));
    nowElement.AddValue(mSimulationServer->GetTime());

//     ParameterList& stepElement = predicate.parameter.AddList();
//     stepElement.AddValue(string("st"));
//     stepElement.AddValue(mSimulationServer->GetSimStep());

    return true;
}


