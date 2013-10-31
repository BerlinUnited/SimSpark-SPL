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
#ifndef OXYGEN_GENERICPHYSICSOBJECTS_H
#define OXYGEN_GENERICPHYSICSOBJECTS_H

#include <oxygen/oxygen_defines.h>

namespace oxygen
{
/** GenericPhysicsObjects declares empty classes that have supportive 
    purposes. In many cases, it is required to pass a pointer or a
    reference to an engine-specific object on to the abstract layer,
    usually when a method of a different class is called. In that case, 
    the pointer or reference must first be cast to a pointer or reference
    to one of the generic classes defined here.
*/

class OXYGEN_API GenericPhysicsMatrix{};
class OXYGEN_API GenericContact{};
class OXYGEN_API GenericMass{ public: virtual ~GenericMass() {} };
class OXYGEN_API GenericJointFeedback{};
class OXYGEN_API GenericSurfaceParameter{};

} //namespace oxygen

#endif //OXYGEN_GENERICPHYSICSOBJECTS_H
