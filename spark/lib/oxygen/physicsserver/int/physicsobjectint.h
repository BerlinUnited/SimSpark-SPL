/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

#ifndef OXYGEN_PHYSICSOBJECTINT_H
#define OXYGEN_PHYSICSOBJECTINT_H

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <salt/matrix.h>
#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/genericphysicsobjects.h>

namespace oxygen
{

class OXYGEN_API PhysicsObjectInt
{

public:
    virtual ~PhysicsObjectInt() {}

    /** converts the rotation part of a salt::Matrix to an
        engine-specific matrix that arranges the values in a different way
    */
    virtual void ConvertRotationMatrix(const salt::Matrix& rot, GenericPhysicsMatrix& matrix) = 0;

    /** coverts an engine-specific matrix to the rotation part of a salt::Matrix */
    virtual void ConvertRotationMatrix(const GenericPhysicsMatrix* matrix, salt::Matrix& rot) const = 0;
};

} //namespace oxygen

#endif //OXYGEN_PHYSICSOBJECTINT_H
