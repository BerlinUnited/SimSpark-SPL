/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentaspect.h,v 1.4 2003/09/10 05:54:53 tomhoward Exp $

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
#ifndef OXYGEN_AGENTASPECT_H
#define OXYGEN_AGENTASPECT_H

// #ifdef HAVE_CONFIG_H
// #include <config.h>
// #endif

#include "../sceneserver/basenode.h"
#include "effector.h"
#include "perceptor.h"

namespace oxygen
{
#if 0
}
#endif

class AgentAspect : public BaseNode
{
public:
    //! this method must be implemented for the agents 'think' behavior
    virtual void Think(float deltaTime) = 0;

protected:
    salt::Vector3f GetVelocity() const;
private:
    //! update internal state before physics calculation ... class Think()
    void PrePhysicsUpdateInternal(float deltaTime);
};

DECLARE_ABSTRACTCLASS(AgentAspect);

} // namespace oxygen

#endif //OXYGEN_AGENTASPECT_H
