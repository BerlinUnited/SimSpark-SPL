/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: commserver.cpp,v 1.6 2004/06/09 12:12:07 fruit Exp $

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
#include "commserver.h"
#include <netinet/in.h>

using namespace zeitgeist;
using namespace std;
using namespace boost;
using namespace salt;

#include <zeitgeist/logserver/logserver.h>

CommServer::CommServer()
  : mReadFd(3), mWriteFd(4)
{
}

CommServer::~CommServer()
{
}

bool
CommServer::SelectInput()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(mReadFd,&readfds);

    return select(mReadFd+1,&readfds, 0, 0, 0) > 0;
}

void
CommServer::PutOutput(const char* out)
{
    strcpy(mBuffer + sizeof(long), out);
    unsigned int len = strlen(out);
    unsigned int netlen = htonl(len);
    memcpy(mBuffer,&netlen,sizeof(netlen));
    write(mWriteFd, mBuffer, len + sizeof(netlen));
}

bool
CommServer::GetInput()
{
    memset(mBuffer,0,sizeof(mBuffer));
    // read the first message segment
    if (! SelectInput())
    {
        GetLog()->Debug() << "CommServer::GetInput false\n";
        return false;
    }

    GetLog()->Debug() << "CommServer::GetInput true\n";

    unsigned int bytesRead = read(mReadFd, mBuffer, sizeof(mBuffer));

    if (bytesRead < sizeof(unsigned int))
    {
        return false;
    }

    // msg is prefixed with it's total length
    unsigned int msgLen = ntohl(*(unsigned int*)mBuffer);

    // read remaining message segments
    unsigned int msgRead = bytesRead - sizeof(unsigned int);
    char *offset = mBuffer + bytesRead;

    while (msgRead < msgLen)
    {
        if (! SelectInput())
        {
            return false;
        }

        msgRead += read(mReadFd, offset, sizeof(mBuffer) - msgRead);
        offset += msgRead;
    }
    // zero terminate received data
    (*offset) = 0;

    return true;
}

