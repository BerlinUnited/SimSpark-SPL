/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: spadesactevent.cpp,v 1.1.2.1 2003/12/03 17:50:18 rollmark Exp $

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
#include "spadesactevent.h"
#include "spadesserver.h"

using namespace oxygen;

void SpadesActEvent::Print (std::ostream & o) const
{
}

bool SpadesActEvent::realizeEventWorldModel(spades::WorldModel* pWM)
{
  // we can't create a shared_ptr to the SpadesServer here, as this
  // shared_ptr wouldn't know of other shared_ptrs to the
  // SpadesServer
  SpadesServer* spadesServer = dynamic_cast<SpadesServer*>(pWM);

  if (spadesServer == NULL)
        {
          // we can't use the LogServer here :(
          return false;
        }

  // call the game control server to realize the list of actions

  return true;
}
