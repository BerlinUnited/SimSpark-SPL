/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: logfileserver.cpp,v 1.4 2006/05/19 07:52:12 jamu Exp $

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
#include "logfileserver.h"

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;

LogfileServer::LogfileServer() : mPaused(false)
{
}

bool
LogfileServer::Init(const std::string& parser, const std::string& filename, int /* dummy */)
{
    mParser = shared_dynamic_cast<oxygen::BaseParser>(GetCore()->New(parser));

    if (mParser.get() == 0)
    {
        GetLog()->Error() <<
            "ERROR: (LogfileServer) failed to create parser " << parser << std::endl;
        return false;
    }

    mLog.open(filename.c_str(), std::ifstream::in);
    return true;
}

void
LogfileServer::Reset()
{
    mLog.seekg(0);
}

void
LogfileServer::GoBack(int lineCount)
{
    if (linePositions.size() < 4)
        return;
    for(unsigned i = 0; i<lineCount; i++)
        linePositions.pop();
    mLog.seekg( linePositions.top() );
}

bool
LogfileServer::ReadMessage()
{
    std::string line;
    return ReadMessage(line);
}

bool
LogfileServer::ReadMessage(std::string& msg)
{
    if (!mLog.good() || mPaused) return false;

		linePositions.push( mLog.tellg() );
    std::getline(mLog,msg);
    Parse(msg);
    return true;
}

void
LogfileServer::Parse(const std::string& message)
{
    if (mParser.get() == 0)
    {
        mPredicates = shared_ptr<PredicateList>();
        return;
    }

    mPredicates = mParser->Parse(message);
}

shared_ptr<PredicateList>
LogfileServer::GetPredicates() const
{
    return mPredicates;
}

void
LogfileServer::SendPauseCmd()
{
    mPaused = true;
}

void
LogfileServer::SendRunCmd()
{
    mPaused = false;
}



