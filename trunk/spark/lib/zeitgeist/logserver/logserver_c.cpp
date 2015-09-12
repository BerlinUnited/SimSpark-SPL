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
    bool inSync = false;

    if (
        (in.GetSize() < 2) ||
        (in.GetSize() > 3) ||
        (! in.GetValue(in[0],inName)) ||
        (! in.GetValue(in[1],inPriority)) ||
        ((in.GetSize() == 3) && (! in.GetValue(in[2],inSync))) //optional parameter 3
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
    bool found = false;

    if (inPriority.find("eAll") != -1)
    {
        pLevel = LogServer::eAll;
        found = true;
    }
    else
    {
        if (inPriority.find("eNone") != -1)
        {
            found = true;
        }

        if (inPriority.find("eDebug") != -1)
        {
            pLevel |= LogServer::eDebug;
            found = true;
        }

        if (inPriority.find("eNormal") != -1)
        {
            pLevel |= LogServer::eNormal;
            found = true;
        }

        if (inPriority.find("eWarning") != -1)
        {
            pLevel |= LogServer::eWarning;
            found = true;
        }

        if (inPriority.find("eError") != -1)
        {
            pLevel |= LogServer::eError;
            found = true;
        }
    }

    // no match
    if (!found)
        return false;

    obj->AddStream(stream, pLevel, inSync);
    return true;
}

FUNCTION(LogServer,removeAllStreams)
{
    obj->RemoveAllStreams();
    return true;
}

FUNCTION(LogServer,debug)
{
    std::string inMessage;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inMessage))
        )
    {
        return false;
    }

    obj->Debug() << inMessage;
    return true;
}

FUNCTION(LogServer,normal)
{
    std::string inMessage;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inMessage))
        )
    {
        return false;
    }

    obj->Normal() << inMessage;
    return true;
}

FUNCTION(LogServer,warning)
{
    std::string inMessage;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inMessage))
        )
    {
        return false;
    }

    obj->Warning() << inMessage;
    return true;
}

FUNCTION(LogServer,error)
{
    std::string inMessage;
    if (
        (in.GetSize() != 1) ||
        (! in.GetValue(in[0],inMessage))
        )
    {
        return false;
    }

    obj->Error() << inMessage;
    return true;
}

FUNCTION(LogServer,message)
{
    std::string inMessage;
    std::string inLevel;
    if (
        (in.GetSize() != 2) ||
        (! in.GetValue(in[0],inMessage)) ||
        (! in.GetValue(in[1],inLevel))
        )
    {
        return false;
    }

    if (inLevel == "eDebug")
        obj->Debug() << inMessage;
    else if (inLevel == "eNormal")
        obj->Normal() << inMessage;
    else if (inLevel == "eWarning")
        obj->Warning() << inMessage;
    else if (inLevel == "eError")
        obj->Error() << inMessage;
    else return false;
        
    return true;
}

void CLASS(LogServer)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Node);
    DEFINE_FUNCTION(addStream);
    DEFINE_FUNCTION(removeAllStreams);
    DEFINE_FUNCTION(debug);
    DEFINE_FUNCTION(normal);
    DEFINE_FUNCTION(warning);
    DEFINE_FUNCTION(error);
    DEFINE_FUNCTION(message);
}
