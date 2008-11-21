/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#include "logserver.h"
#include <fstream>

using namespace boost;
using namespace zeitgeist;

FUNCTION(LogServer,addStream)
{
    std::string inName;
    std::string inPriority;

    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inName)) ||
        (! in.GetValue(in[1],inPriority))
        )
    {
        return false;
    }

    std::ostream *stream = NULL;
    if (inName == ":cout")
    {
        stream = &std::cout;
    } else if (inName == ":cerr")
    {
        stream = &std::cerr;
    } else
    {
        stream = new std::ofstream(inName.c_str());
    }

    unsigned int pLevel = LogServer::eNone;

    do
    {
        if (inPriority == "eNone")
        {
            pLevel = LogServer::eNone;
            break;
        }

        if (inPriority == "eDebug")
        {
            pLevel = LogServer::eDebug;
            break;
        }

        if (inPriority == "eNormal")
        {
            pLevel = LogServer::eNormal;
            break;
        }

        if (inPriority == "eWarning")
        {
            pLevel = LogServer::eWarning;
            break;
        }

        if (inPriority == "eError")
        {
            pLevel = LogServer::eError;
            break;
        }

        if (inPriority == "eAll")
        {
            pLevel = LogServer::eAll;
            break;
        }

        // no match
        return false;

    } while(true);

    obj->AddStream(stream, pLevel);
    return true;
}

FUNCTION(LogServer,removeAllStreams)
{
    obj->RemoveAllStreams();
    return true;
}

void CLASS(LogServer)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Node);
    DEFINE_FUNCTION(addStream);
    DEFINE_FUNCTION(removeAllStreams);
}
