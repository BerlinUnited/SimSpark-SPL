/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: joint.h 107 2009-11-25 06:09:10Z a-held $

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

#include <oxygen/physicsserver/generic6dofjoint.h>
#include <oxygen/physicsserver/int/generic6dofjointint.h>

using namespace oxygen;
using namespace boost;

boost::shared_ptr<Generic6DOFJointInt> Generic6DOFJoint::mGeneric6DOFJointImp;

Generic6DOFJoint::Generic6DOFJoint() : Joint(){

}

Generic6DOFJoint::~Generic6DOFJoint(){

}

void Generic6DOFJoint::OnLink()
{
    Joint::OnLink();

    if (mGeneric6DOFJointImp.get() == 0)
        mGeneric6DOFJointImp = shared_dynamic_cast<Generic6DOFJointInt>
            (GetCore()->New("Generic6DOFJointImp"));
}
