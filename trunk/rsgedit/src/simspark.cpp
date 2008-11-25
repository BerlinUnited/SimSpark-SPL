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
#include "simspark.h"
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/simulationserver/simulationserver.h>

using namespace std;
using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace spark;

std::ostringstream* SimSpark::mLogStream = 0;

SimSpark::SimSpark(const string& relPathPrefix) :
    Spark(relPathPrefix)
{
    mState = S_PAUSED;
}

SimSpark::~SimSpark()
{
    // the stream is owned by the logserver
    mLogStream = 0;
}

bool SimSpark::InitApp(int argc, char** argv)
{
    // process command line
    if (! ProcessCmdLine(argc, argv))
        {
            return false;
        }

    // run initialization scripts
    GetScriptServer()->Run("rsgedit.rb");

    mLogStream = new ostringstream;
    GetLog()->AddStream
        (mLogStream, LogServer::eNormal + LogServer::eWarning + LogServer::eError);
    PrintGreeting();

    return true;
}

unsigned int SimSpark::GetLogPriority()
{
    return GetLog()->GetPriorityMask(mLogStream);
}

void SimSpark::SetLogPriority(unsigned int mask)
{
    if (mLogStream == 0)
        {
            assert(false);
            return;
        }

    GetLog()->SetPriorityMask(mLogStream, mask);
}

void SimSpark::ResetSimulation()
{
    // recreate a default empty scene with a camera node
    GetScriptServer()->Eval("rsgeditResetScene()");

    // update stale references
    GetScriptServer()->UpdateCachedAllNodes();
}

void SimSpark::PrintHelp()
{
}

void SimSpark::PrintGreeting()
{
}

bool SimSpark::ProcessCmdLine(int /*argc*/, char* /*argv*/[])
{
    return true;
}

ESimState SimSpark::GetSimState() const
{
    return mState;
}

void SimSpark::SetSimState(ESimState state)
{
    mState = state;
}

wxString SimSpark::GetLogBuffer()
{
    if (mLogStream == 0)
        {
            assert(false);
            return wxString();
        }

    return wxString(mLogStream->str().c_str(), wxConvUTF8);
}

void SimSpark::ClearLogBuffer()
{
    if (mLogStream == 0)
        {
            assert(false);
            return;
        }

    mLogStream->str(string());
}

bool SimSpark::HasLogContents()
{
    if (mLogStream == 0)
        {
            assert(false);
            return false;
        }

    return (! mLogStream->str().empty());
}
