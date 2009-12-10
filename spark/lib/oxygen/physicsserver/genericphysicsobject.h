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
#ifndef OXYGEN_GENERICPHYSICSOBJECT_H
#define OXYGEN_GENERICPHYSICSOBJECT_H

#include <oxygen/oxygen_defines.h>

namespace oxygen
{
/** GenericPhysicsObject is an empty class that has only supportive 
    purposes. In many cases, it is required to pass a pointer or a
    reference to an engine-specific object on to the abstract layer,
    usually when a method of a different class is called. In that case, 
    the pointer or reference must first be cast to a pointer or reference
    to this generic class. After the method call has been delegated by 
    the abstract layer, the callee can then re-cast the pointer or 
    reference type to the expected engine-specific type.
    
    This is done optimistically, i.e. callees trust that callers
    will not cast a pointer or reference to a wrong engine-specific type 
    to a GenericPhysicsObject pointer and then commit a prank call.
*/

class OXYGEN_API GenericPhysicsObject
{

};

} //namespace oxygen

#endif //OXYGEN_GENERICPHYSICSOBJECT_H
