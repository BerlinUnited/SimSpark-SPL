/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: kickeffector_c.cpp,v 1.1.2.2 2004/02/01 19:04:25 fruit Exp $

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

#include "kickeffector.h"

using namespace oxygen;

FUNCTION(setKickMargin)
{
    if (in.size() == 1)
    {
        KickEffector* ke = static_cast<KickEffector*>(obj);
        ke->SetKickMargin(boost::any_cast<float>(in[0]));
    }
}

FUNCTION(setForceFactor)
{
    if (in.size() == 1)
    {
        KickEffector* ke = static_cast<KickEffector*>(obj);
        ke->SetForceFactor(boost::any_cast<float>(in[0]));
    }
}

FUNCTION(setSteps)
{
    if (in.size() == 2)
    {
        KickEffector* ke = static_cast<KickEffector*>(obj);
        ke->SetSteps(boost::any_cast<int>(in[0]),
                     boost::any_cast<int>(in[1]));
    }
}

FUNCTION(setNoiseParams)
{
    if (in.size() == 3)
    {
        KickEffector* ke = static_cast<KickEffector*>(obj);
        ke->SetNoiseParams(boost::any_cast<float>(in[0]),
                           boost::any_cast<float>(in[1]),
                           boost::any_cast<float>(in[2]));
    }
}

FUNCTION(setMaxPower)
{
    if (in.size() == 1)
    {
        KickEffector* ke = static_cast<KickEffector*>(obj);
        ke->SetMaxPower(boost::any_cast<float>(in[0]));
    }
}

void CLASS(KickEffector)::DefineClass()
{
    DEFINE_BASECLASS(kerosin/Effector);
    DEFINE_FUNCTION(setKickMargin);
    DEFINE_FUNCTION(setForceFactor);
    DEFINE_FUNCTION(setSteps);
    DEFINE_FUNCTION(setNoiseParams);
    DEFINE_FUNCTION(setMaxPower);
}
