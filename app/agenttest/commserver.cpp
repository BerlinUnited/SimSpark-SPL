/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: commserver.cpp,v 1.9 2004/10/04 08:21:14 rollmark Exp $

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
    : mReadFd(3), mWriteFd(4), rdfd(mReadFd)
{
    mBufferSize = DEFAULT_BUFFER_SIZE;
    mBuffer = new char[mBufferSize];
}

CommServer::~CommServer()
{
    delete [] mBuffer;
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
    strcpy(mBuffer + sizeof(unsigned int), out);
    unsigned int len = strlen(out);
    unsigned int netlen = htonl(len);
    memcpy(mBuffer,&netlen,sizeof(netlen));
    write(mWriteFd, mBuffer, len + sizeof(netlen));
}

bool
CommServer::GetInput()
{
    for (int count = 0; count < 100; count++)
        {
            
            int res = rdfd.readLengthPrefixed(&mBuffer, &mBufferSize);
            if (res < 0)
                {
                    GetLog()->Debug() << "CommServer::GetInput: readLeangthPrefixed returned -1\n";
                    return false;
                }
            else if (res > 0)
                {
                    // successful read
                    if (res == mBufferSize)
                        {
                            GetLog()->Debug() << "CommServer::GetInput: read exactly buffer size! can't null term\n";
                            return false;
                        }
                    mBuffer[res] = 0;
                    return true;
                }

            // if we are here, res == 0, which means not enough input

            // this blocks until more input
            if (! SelectInput())
                {
                    GetLog()->Debug() << "CommServer::GetInput select returned false\n";
                    return false;
                }
        }

    GetLog()->Debug() << "CommServer::GetInput: fell out of for loop\n";
    return false;
}

