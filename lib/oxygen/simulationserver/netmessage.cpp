/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: netmessage.cpp,v 1.3 2007/02/27 03:44:50 jboedeck Exp $

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
#include "netmessage.h"
#include "netbuffer.h"
#include <netinet/in.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace std;
using namespace boost;

NetMessage::NetMessage() : Leaf()
{
}

NetMessage::~NetMessage()
{
}

void NetMessage::PrepareToSend(std::string& msg)
{
    // prefix the message with it's payload length
    unsigned int len = htonl(msg.size());
    string prefix((const char*)&len,sizeof(unsigned int));
    msg = prefix + msg;
}

bool NetMessage::Extract(shared_ptr<NetBuffer> buffer, std::string& msg)
{
    // a message is prefixed with it's payload length
    const unsigned int preSz = sizeof(unsigned int);

    if (buffer.get() == 0)
    {
        return false;
    }

    string& data = buffer->GetData();

    if (data.size() < preSz)
        {
            return false;
        }

    unsigned int msgLen = ntohl((*(unsigned int*)data.data()));

    if (data.size() < (msgLen + preSz))
        {
            // incomplete message
            return false;
        }

    // copy to msg and cut from buffer
    msg = string(data.c_str() + preSz, msgLen);
    data.erase(0, preSz + msgLen);

    // zero terminate received data
    msg += '\0';

    return true;
}
