/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: odeobject.h,v 1.5 2004/04/05 08:47:08 rollmark Exp $

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
#ifndef OXYGEN_ODEOBJECT_H
#define OXYGEN_ODEOBJECT_H

#include <oxygen/sceneserver/basenode.h>
#include <ode/ode.h>

namespace oxygen
{

class PhysicsServer;

/** ODEObject is the base of all classes encapsulating ODE concepts
 */
class ODEObject : public BaseNode
{
public:
    //
    // Functions
    //
    ODEObject() : BaseNode() {};
    virtual ~ODEObject() {};
};

DECLARE_ABSTRACTCLASS(ODEObject);

} //namespace oxygen

#endif //OXYGEN_ODEOBJECT_H
