/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: randomserver_c.cpp,v 1.2 2004/02/12 14:07:24 fruit Exp $

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
#include "randomserver.h"


using namespace boost;
using namespace zeitgeist;

FUNCTION(seed)
{
    if (in.size() == 1)
    {
        RandomServer* rs = static_cast<RandomServer*>(obj);
        rs->Seed(boost::any_cast<int>(in[0]));
    }
}

OUT_FUNCTION(uniformRND)
{
    if (in.size() == 2)
    {
        RandomServer* rs = static_cast<RandomServer*>(obj);
        out = rb_float_new(rs->GetUniformRandom<float>(boost::any_cast<float>(in[0]),
                                                       boost::any_cast<float>(in[1])));
    }
}

OUT_FUNCTION(normalRND)
{
    if (in.size() == 2)
    {
        RandomServer* rs = static_cast<RandomServer*>(obj);
        out = rb_float_new(rs->GetNormalRandom<float>(boost::any_cast<float>(in[0]),
                                                      boost::any_cast<float>(in[1])));
    }
}

OUT_FUNCTION(exponentialRND)
{
    if (in.size() == 1)
    {
        RandomServer* rs = static_cast<RandomServer*>(obj);
        out = rb_float_new(rs->GetExponentialRandom<float>
                           (boost::any_cast<float>(in[0])));
    }
}

void
CLASS(RandomServer)::DefineClass()
{
    DEFINE_BASECLASS(zeitgeist/Node);
    DEFINE_FUNCTION(seed);
    DEFINE_FUNCTION(uniformRND);
    DEFINE_FUNCTION(normalRND);
    DEFINE_FUNCTION(exponentialRND);
}
