/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: gamecontrolserver_c.cpp,v 1.4 2004/02/12 14:07:22 fruit Exp $

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

#include "gamecontrolserver.h"

using namespace oxygen;

FUNCTION(initParser)
{
  if (in.size() == 1)
    {
      GameControlServer* gcs = dynamic_cast<GameControlServer*>(obj);
      gcs->InitParser(boost::any_cast<char*>(in[0]));
    }
}

FUNCTION(initEffector)
{
  if (in.size() == 1)
    {
      GameControlServer* gcs = dynamic_cast<GameControlServer*>(obj);
      gcs->InitEffector(boost::any_cast<char*>(in[0]));
    }
}

FUNCTION(initControlAspect)
{
  if (in.size() == 1)
    {
      GameControlServer* gcs = dynamic_cast<GameControlServer*>(obj);
      gcs->InitControlAspect(boost::any_cast<char*>(in[0]));
    }
}

void CLASS(GameControlServer)::DefineClass()
{
  DEFINE_BASECLASS(zeitgeist/Node);
  DEFINE_FUNCTION(initParser);
  DEFINE_FUNCTION(initEffector);
  DEFINE_FUNCTION(initControlAspect);
}
