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

#ifndef OXYGEN_UNIVERSALJOINTINT_H
#define OXYGEN_UNIVERSALJOINTINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

class OXYGEN_API UniversalJointInt
{

public:
    virtual void CreateUniversalJoint(long world) = 0;
    virtual void SetAnchor(const salt::Vector3f& anchor) = 0;
    virtual salt::Vector3f GetAnchor1() = 0;
    virtual salt::Vector3f GetAnchor2() = 0;
    virtual void SetAxis1(const salt::Vector3f& axis) = 0;
    virtual void SetAxis2(const salt::Vector3f& axis) = 0;
    virtual salt::Vector3f GetAxis1() const = 0;
    virtual salt::Vector3f GetAxis2() const = 0;
    virtual float GetAngle1() const = 0;
    virtual float GetAngle2() const = 0;
    virtual float GetAngleRate1() const = 0;
    virtual float GetAngleRate2() const = 0;
    virtual void SetParameter(int parameter, float value) = 0;
    virtual float GetParameter(int parameter) const = 0; 
    virtual long GetJointID() = 0;
};

} //namespace oxygen

#endif //OXYGEN_UNIVERSALJOINTINT_H
