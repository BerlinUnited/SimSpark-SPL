/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: communit.cpp,v 1.4 2004/05/01 17:13:05 fruit Exp $

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
#include "communit.h"
#include "types.h"
#include <netinet/in.h>
#include <iostream>
#include <rcssnet/exception.hpp>
#include <cerrno>

using namespace std;
using namespace rcss::net;

CommUnit::CommUnit() :
    mStreamBuf(mSocket),mInStream(&mStreamBuf)
{
}

CommUnit::~CommUnit()
{
    CloseConnection ();
}

void CommUnit::CloseConnection()
{
  mSocket.close();
}

bool CommUnit::OpenConnection(std::string host, int port)
{
  mSocket.open();

  cout << "(CommUnit) connecting to ";
  cout << host << ":" << port << endl;

  Addr local(INADDR_ANY,INADDR_ANY);
  mSocket.bind(local);

  try
    {
      Addr server(port,string(host));
      mSocket.connect(server);
    }

  catch (ConnectErr error)
    {
      cerr << "(CommUnit) connection failed with: \n\t"
           << strerror(errno) << endl;
      return false;
    }

  if (mSocket.isConnected())
    {
      cout << "(CommUnit) connected successfully" << endl;
      return true;
    }

  return false;
}

bool CommUnit::SelectInput(bool wait)
{
    int fd = mSocket.getFD();

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd,&readfds);

    timeval time;
    time.tv_sec = 0;
    time.tv_usec = 0;

    return select
        (
         fd+1, &readfds, 0, 0,
         wait ? 0:&time
         ) > 0;
}

string CommUnit::GetMessage ()
{
    if (! SelectInput(false))
        {
            return "";
        }

    static char line[MAX_MSG_LEN];
    mInStream.getline(line,MAX_MSG_LEN);
    return line;
}

void CommUnit::PutMessage(const string& message)
{
    mSocket.send(message.c_str(),message.size());
}
