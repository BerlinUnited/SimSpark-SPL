/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: carbehavior.cpp,v 1.2 2008/03/27 21:11:53 rollmark Exp $

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
#include "carbehavior.h"
#include <iostream>
#include <sstream>

using namespace std;

CarBehavior::CarBehavior()
{
}

string CarBehavior::Init()
{
    // use the scene effector to build the agent
    return "(scene rsg/agent/buggy.rsg)";
}

string CarBehavior::Think(const std::string& message)
{
    // demo: just drive forward and backward...

    static int n = 0;
    static float velocity = 100;

    ++n;
    if (n > 900)
        {
            velocity *= -1;
            n = 0;
        }

    stringstream ss;
    ss << "(hinge2 "  << velocity << ")";

    return ss.str();
}
