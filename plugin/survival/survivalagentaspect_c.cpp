/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: survivalagentaspect_c.cpp,v 1.1.4.1 2003/11/19 19:07:39 rollmark Exp $

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
#include "survivalagentaspect.h"

using namespace boost;

FUNCTION(setWanderingRate)
{
        if (in.size() == 1)
        {
                SurvivalAgentAspect *saa = static_cast<SurvivalAgentAspect*>(obj);
                saa->SetWanderingRate(any_cast<float>(in[0]));
        }
}

FUNCTION(setWanderingStrength)
{
        if (in.size() == 1)
        {
                SurvivalAgentAspect *saa = static_cast<SurvivalAgentAspect*>(obj);
                saa->SetWanderingStrength(any_cast<float>(in[0]));
        }
}

void CLASS(SurvivalAgentAspect)::DefineClass()
{
        DEFINE_BASECLASS(kerosin/AgentAspect);
        DEFINE_FUNCTION(setWanderingRate);
        DEFINE_FUNCTION(setWanderingStrength);
}
