/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: commserver.cpp,v 1.2.2.1 2003/12/22 15:21:53 rollmark Exp $

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

using namespace std;

CommServer::CommServer()
{
}

bool CommServer::Init(std::string host, int port)
{
  return mCommUnit.OpenConnection(host,port);
}


bool CommServer::GetMessage()
{
  static char rawmsg[MAX_MSG_LEN];

  int l =  mCommUnit.GetMessage(rawmsg, MAX_MSG_LEN);
  if (l == 0)
    {
      return (false);
    }

  // zero terminate and parse received data
  rawmsg[l] = 0;
  ParseTuples(rawmsg, strlen(rawmsg));

  return (true);
}


void CommServer::ParseTuples(const char* msg, const size_t& len)
{
  // clear all previous vectors
  mPositions.clear();

  //extract all breakets
  static char tmp[MAX_MSG_LEN];
  static char data[MAX_MSG_LEN];

  sscanf(msg, "( %[^)] )", data);

  for(uint i = 0; i < len; i++)
    {
      switch(data[i])
        {
        case 'p' :
          // player pos
          salt::Vector3f vec;
          sscanf(data, "%[a-z] %f %f %f",tmp, &vec[0], &vec[1], &vec[2]);

          cout << " pos x: "<< vec[0]
               << " pos y:" << vec[1]
               << " pos z:" << vec[2]
               << endl;

          mPositions.push_back(vec);
          break;
        }
    }
}

const CommServer::TPositions& CommServer::GetPositions()
{
  return mPositions;
}
