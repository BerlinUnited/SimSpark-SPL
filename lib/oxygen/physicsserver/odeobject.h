/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: odeobject.h,v 1.3 2003/08/31 21:53:45 fruit Exp $

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

   ODEObject
*/
#ifndef OXYGEN_ODEOBJECT_H
#define OXYGEN_ODEOBJECT_H

#include "../sceneserver/basenode.h"
#include <ode/ode.h>

namespace oxygen
{

class PhysicsServer;

/** ODEObject is the base of all classes encapsulating ODE concepts
 */
class ODEObject : public BaseNode
{
    //
    // Functions
    //
protected:
    virtual bool ConstructInternal();

    //
    // Members
    //
protected:
    boost::shared_ptr<PhysicsServer>    mPhysicsServer;
};

    DECLARE_CLASS(ODEObject);

} //namespace oxygen

#endif //OXYGEN_ODEOBJECT_H

