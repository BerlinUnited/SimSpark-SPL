/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: visionperceptor_c.cpp,v 1.2 2004/02/12 14:07:27 fruit Exp $

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

#include "visionperceptor.h"

using namespace boost;
using namespace oxygen;

FUNCTION(setNoiseParams)
{
    if (in.size() == 4)
    {
        VisionPerceptor* vp = static_cast<VisionPerceptor*>(obj);
        vp->SetNoiseParams(boost::any_cast<float>(in[0]),
                           boost::any_cast<float>(in[1]),
                           boost::any_cast<float>(in[2]),
                           boost::any_cast<float>(in[3]));
    }
}

FUNCTION(addNoise)
{
    if (in.size() == 1)
    {
        VisionPerceptor* vp = static_cast<VisionPerceptor*>(obj);
        vp->AddNoise(boost::any_cast<bool>(in[0]));
    }
}

FUNCTION(setPredicateName)
{
    if (in.size() == 1)
    {
        VisionPerceptor* vp = static_cast<VisionPerceptor*>(obj);
        vp->SetPredicateName(boost::any_cast<char*>(in[0]));
    }
}

FUNCTION(setSenseMyPos)
{
  if (in.size() == 1)
    {
        VisionPerceptor* vp = static_cast<VisionPerceptor*>(obj);
        vp->SetSenseMyPos(boost::any_cast<bool>(in[0]));
    }
}

void
CLASS(VisionPerceptor)::DefineClass()
{
    DEFINE_BASECLASS(kerosin/Perceptor);
    DEFINE_FUNCTION(setNoiseParams);
    DEFINE_FUNCTION(addNoise);
    DEFINE_FUNCTION(setPredicateName);
    DEFINE_FUNCTION(setSenseMyPos);
}
