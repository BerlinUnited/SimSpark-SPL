/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: perfectvisionperceptor_c.cpp,v 1.2.4.1 2004/02/06 21:56:34 fruit Exp $

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

#include "perfectvisionperceptor.h"

using namespace boost;
using namespace oxygen;

FUNCTION(setInversion)
{
    if (in.size() == 3)
    {
        PerfectVisionPerceptor* pp = static_cast<PerfectVisionPerceptor*>(obj);
        pp->SetInversion(boost::any_cast<bool>(in[0]),
                         boost::any_cast<bool>(in[1]),
                         boost::any_cast<bool>(in[2]));
    }
}

void
CLASS(PerfectVisionPerceptor)::DefineClass()
{
    DEFINE_BASECLASS(kerosin/Perceptor);
    DEFINE_FUNCTION(setInversion);
}
