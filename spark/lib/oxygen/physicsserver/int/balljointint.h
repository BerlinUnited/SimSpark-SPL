/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: space.h 102 2009-11-18 07:24:29Z a-held $

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

#ifndef OXYGEN_BALLJOINTINT_H
#define OXYGEN_BALLJOINTINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
class RigidBody;
class Joint;

class OXYGEN_API BallJointInt
{
public:    
    virtual void SetAnchor(const salt::Vector3f& anchor, long jointID) = 0;
    virtual salt::Vector3f GetAnchor1(long jointID) = 0;
    virtual salt::Vector3f GetAnchor2(long jointID) = 0;
    virtual long CreateBallJoint(long world) =  0;
};

} //namespace oxygen

#endif //OXYGEN_BALLJOINTINT_H
