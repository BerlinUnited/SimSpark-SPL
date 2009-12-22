/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: fixedjoint.h 108 2009-11-25 10:20:10Z a-held $

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
#ifndef OXYGEN_ODEFIXEDJOINT_H
#define OXYGEN_ODEFIXEDJOINT_H

#include <oxygen/physicsserver/ode/odegeneric6dofjoint.h>
#include <oxygen/physicsserver/int/fixedjointint.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen{

class OXYGEN_API ODEFixedJoint : public FixedJointInt, public ODEGeneric6DOFJoint{

public:
    ODEFixedJoint();
    long CreateFixedJoint(long world);
    void SetFixed();
};

} //namespace oxygen

#endif //OXYGEN_ODEFIXEDJOINT_H
