/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: commserver.h,v 1.3 2004/04/07 13:23:50 fruit Exp $

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
#ifndef COMMSERVER_H
#define COMMSERVER_H

#include <string>
#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

class CommServer : public zeitgeist::Leaf
{
public:
    CommServer();
    virtual ~CommServer();

    bool GetInput();
    void PutOutput(const char* out);
    void PutOutput(const std::string& str) { PutOutput(str.c_str()); }

    const char* GetMsg() const { return mBuffer + sizeof(long); }

protected:
    bool SelectInput();

protected:
    // the input file descriptor
    const int mReadFd;

    // the output file descriptor
    const int mWriteFd;

    // the receive buffer
    char mBuffer[4096];
};

DECLARE_CLASS(CommServer);

#endif // COMMSERVER_H
