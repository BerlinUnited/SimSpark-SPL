/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: forceeffector.h,v 1.3 2003/09/11 02:31:39 tomhoward Exp $

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
#ifndef OXYGEN_FORCEEFFECTOR_H
#define OXYGEN_FORCEEFFECTOR_H

#include "effector.h"

namespace oxygen
{

class ForceEffector : public Effector
{
    //
    // functions
    //
public:
    ForceEffector();

    /** Apply the force to the physics aspect attached to base.
        \return true on success, false if no body was found */
    virtual bool Perform(boost::shared_ptr<BaseNode> &base, float deltaTime);

    //! accumulates a force to be applied to a physics aspect
    void AddForce(const salt::Vector3f& force);

    void SetMaxForce(float maxForce)    {       mMaxForce = maxForce;   }
    //
    // members
    //
private:
    // the accumulated force this effector applies
    salt::Vector3f mForce;
    //! maximum amount of force, this effector can apply
    float mMaxForce;
};

DECLARE_CLASS(ForceEffector);

} // namespace oxygen

#endif //OXYGEN_FORCEEFFECTOR_H
