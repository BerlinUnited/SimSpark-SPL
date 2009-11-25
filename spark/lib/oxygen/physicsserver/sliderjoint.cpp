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
#include <oxygen/physicsserver/sliderjoint.h>
#include <zeitgeist/logserver/logserver.h>

using namespace oxygen;
using namespace boost;
using namespace salt;

SliderJoint::SliderJoint() : Generic6DOFJoint()
{
}

SliderJoint::~SliderJoint()
{
}

void SliderJoint::OnLink()
{
    dWorldID world = GetWorldID();
    if (world == 0)
        {
            return;
        }

    mODEJoint = dJointCreateSlider(world, 0);
}

void SliderJoint::Attach(shared_ptr<RigidBody> body1, shared_ptr<RigidBody> body2)
{
    Joint::Attach(body1,body2);

    // relative slider axis points up
    Vector3f up(GetWorldTransform().Rotate(Vector3f(0,0,1)));
    dJointSetSliderAxis(mODEJoint,up[0],up[1],up[2]);
}

float SliderJoint::GetPosition()
{
    return dJointGetSliderPosition(mODEJoint);
}

float SliderJoint::GetPositionRate()
{
    return dJointGetSliderPositionRate(mODEJoint);
}

void SliderJoint::SetParameter(int parameter, float value)
{
    dJointSetSliderParam(mODEJoint, parameter, value);
}

float SliderJoint::GetParameter(int parameter) const
{
    return dJointGetSliderParam(mODEJoint, parameter);
}





