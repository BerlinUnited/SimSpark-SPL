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
#ifndef OXYGEN_FIXEDJOINTINT_H
#define OXYGEN_FIXEDJOINTINT_H

#include <oxygen/oxygen_defines.h>

namespace oxygen{

class OXYGEN_API FixedJointInt{

public:
    virtual ~FixedJointInt() {}

    /** Creates a fixed joint within the physics world specified
        by \param worldID
    */
    virtual long CreateFixedJoint(long world) = 0;

    /** Declares that the joint specified by \param jointID is a
        fixed joint
    */
    virtual void SetFixed(long jointID) = 0;
};

} //namespace oxygen

#endif //OXYGEN_FIXEDJOINTINT_H
