/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: commserver.h,v 1.2.2.1 2003/12/22 15:21:53 rollmark Exp $

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
#ifndef _COMMSERVER_H
#define _COMMSERVER_H

#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>
#include "types.h"
#include "communit.h"
#include <vector>

class CommServer
{
 public:
  typedef std::vector<salt::Vector3f> TPositions;

 public:
  CommServer();
  virtual ~CommServer() {};

  bool Init(std::string host, int port);
  bool GetMessage();
  const TPositions& GetPositions();

 protected:
    void ParseTuples(const char* msg, const size_t& len);

 protected:
    // socket wrapper
    CommUnit mCommUnit;

    // vector of parsed out positions
    TPositions mPositions;
};
#endif
