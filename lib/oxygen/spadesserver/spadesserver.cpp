/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesserver.cpp,v 1.1.2.2 2003/11/17 10:10:53 fruit Exp $

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
#include "spadesserver.h"
#include "paramreader.h"

using namespace boost;
using namespace oxygen;
using namespace spades;

#include <zeitgeist/corecontext.h>
#include <zeitgeist/logserver/logserver.h>

SpadesServer::SpadesServer() :
    zeitgeist::Leaf(), spades::WorldModel(), mSimEngine(0)
{
}

SpadesServer::~SpadesServer()
{
}

// methods for our framework

bool
SpadesServer::Init(const std::string& paramReaderName)
{
    // create the parameter reader
    mParamReader =
        shared_static_cast<ParamReader>(GetCore()->New(paramReaderName));

    if (!mParamReader)
    {
        // could not create parameter reader
        GetLog()->Error() << "ERROR: Unable to create "
                          << paramReaderName << "\n";
        return false;
    }

    AddChildReference(mParamReader);

    // initialize the parameter reader
    if (!mParamReader->Init())
    {
        // initializing the parameter reader failed
        GetLog()->Error() << "ERROR: Could not init "
                          << paramReaderName << "\n";
        return false;
    }

    return true;
}

// SPADES interface methods

EngineParam*
SpadesServer::parseParameters(int argc, const char *const *argv)
{
    // SimulationEngineMain uses the pointer we have to return to get the
    // command line options. It doesn't delete the ParamReader, so we can
    // return a simple pointer (otherwise our shared_ptr would become invalid)
    ParamReader* pr = mParamReader.get();

    // uncommenting the next line of code gives the following error:

    // spadesserver/spadesserver.cpp: In member function `virtual spades::EngineParam*
    // oxygen::SpadesServer::parseParameters(int, const char* const*)':
    // spadesserver/spadesserver.cpp:81: error: `spades::EngineParam' is an
    // inaccessible base of `oxygen::ParamReader'

    // EngineParam* ep = dynamic_cast<EngineParam*>(pr);

    return 0;
}

bool
SpadesServer::initialize(SimEngine* pSE)
{
    mSimEngine = pSE;
    return true;
}

bool
SpadesServer::finalize()
{
    return true;
}

SimTime
SpadesServer::simToTime(SimTime time_curr, SimTime time_desired)
{
    return time_desired;
}

DataArray
SpadesServer::getMonitorHeaderInfo()
{
}

DataArray
SpadesServer::getMonitorInfo(SimTime time)
{
}

void
SpadesServer::parseMonitorMessage (const char* data, unsigned datalen)
{
}

SimTime
SpadesServer::getMinActionLatency() const
{
}

SimTime
SpadesServer::getMinSenseLatency() const
{
}

ActEvent*
SpadesServer::parseAct(SimTime t, AgentID a, const char* data, unsigned datalen) const
{
}

void
SpadesServer::pauseModeCallback()
{
}

bool
SpadesServer::agentConnect(AgentID agent, AgentTypeDB::AgentTypeConstIterator at)
{
}

bool
SpadesServer::agentDisappear(AgentID agent, AgentLostReason reason)
{
}

void
SpadesServer::notifyCommserverConnect(ServerID s)
{
}

void
SpadesServer::notifyCommserverDisconnect(ServerID s)
{
}

