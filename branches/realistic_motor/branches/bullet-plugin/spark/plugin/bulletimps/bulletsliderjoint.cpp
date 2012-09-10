/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id$

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

#include "bulletsliderjoint.h"

using namespace oxygen;
using namespace boost;
using namespace salt;

SliderJointImp::SliderJointImp() : Generic6DOFJointImp()
{
}

long SliderJointImp::CreateSliderJoint(long world)
{
//     dWorldID ODEWorld = (dWorldID) world;
//     dJointID ODEJoint = dJointCreateSlider(ODEWorld, 0);
//     return (long) ODEJoint;
}

void SliderJointImp::SetSliderAxis(Vector3f& up, long jointID)
{
//     dJointID ODEJoint = (dJointID) jointID;
//     dJointSetSliderAxis(ODEJoint,up[0],up[1],up[2]);
}

float SliderJointImp::GetPosition(long jointID)
{
//     dJointID ODEJoint = (dJointID) jointID;
//     return dJointGetSliderPosition(ODEJoint);
}

float SliderJointImp::GetPositionRate(long jointID)
{
//     dJointID ODEJoint = (dJointID) jointID;
//     return dJointGetSliderPositionRate(ODEJoint);
}
