/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentaspect.cpp,v 1.3.4.2 2003/12/09 20:23:59 rollmark Exp $

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
#include "agentaspect.h"
#include "../physicsserver/body.h"

using namespace boost;
using namespace oxygen;
using namespace salt;

bool AgentAspect::RealizeActions(boost::shared_ptr<ActionObject::TList> /*actions*/)
{
  // traverse the actionlist, find the corresponding effector
  // for each action and realize it
  return true;
}

BaseParser::TPredicateList AgentAspect::QueryPerceptors()
{
  // traverse all Perceptors and build a list of the collected
  // Predicates
  return BaseParser::TPredicateList();
}

