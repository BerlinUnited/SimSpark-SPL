/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: commserver.cpp,v 1.5 2004/02/16 18:34:27 patstg Exp $

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
#include <netinet/in.h>
#include "commserver.h"

using namespace std;
using namespace boost;
using namespace oxygen;

CommServer::CommServer()
{
}

bool
CommServer::Init(std::string parser, std::string host, int port)
{
    mParser = shared_dynamic_cast<oxygen::BaseParser>(GetCore()->New(parser));

    if (mParser.get() == 0)
        {
            GetLog()->Error() <<
                "ERROR: (CommServer) failed to create parser " << parser << endl;
            return false;
        }

    return mCommUnit.OpenConnection(host,port);
}


bool
CommServer::GetMessage()
{
    string line = mCommUnit.GetMessage();
    if (line == "")
        {
            return false;
        }

    Parse(line);
    return true;
}


void
CommServer::Parse(std::string msg)
{
    if (mParser.get() == 0)
        {
            mPredicates = shared_ptr<Predicate::TList>();
            return;
        }

    mPredicates = mParser->Parse(msg);
}

shared_ptr<Predicate::TList>
CommServer::GetPredicates()
{
  return mPredicates;
}

void CommServer::SendMessage(const std::string& message)
{
    unsigned long len = htonl(message.size());
    std::string s((const char*)&len,sizeof(len));
    s += message;
    mCommUnit.PutMessage(s);
}

void CommServer::SendPauseCmd()
{
    SendMessage("P");
}

void CommServer::SendRunCmd()
{
    SendMessage("R");
}

void CommServer::SendDisconnectCmd()
{
    SendMessage("D");
}

void CommServer::SendToWorldModel(const std::string& msg)
{
    SendMessage("W"+msg);
}

void CommServer::SendKickOffCmd()
{
    SendToWorldModel("(kickoff)");
}


