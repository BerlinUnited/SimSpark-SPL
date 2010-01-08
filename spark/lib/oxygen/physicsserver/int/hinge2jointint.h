/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: hingejoint.h 108 2009-11-25 10:20:10Z a-held $

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
#ifndef OXYGEN_HINGE2JOINTINT_H
#define OXYGEN_HINGE2JOINTINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen{

class OXYGEN_API Hinge2JointInt{

public:
    virtual long CreateHinge2Joint(long world) = 0;
    virtual void SetAnchor(const salt::Vector3f& anchor,
                           const salt::Vector3f& up,
                           const salt::Vector3f& right,
                           long jointID) = 0;
    virtual salt::Vector3f GetAnchor1(long jointID) = 0;
    virtual salt::Vector3f GetAnchor2(long jointID) = 0;
    virtual float GetAngle(long jointID) = 0;
    virtual float GetAngleRate1(long jointID) = 0;
    virtual float GetAngleRate2(long jointID) = 0;
};

} //namespace oxygen

#endif //OXYGEN_HINGE2JOINTINT_H
