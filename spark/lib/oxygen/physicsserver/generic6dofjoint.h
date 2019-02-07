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
#ifndef OXYGEN_GENERIC6DOFJOINT_H
#define OXYGEN_GENERIC6DOFJOINT_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/joint.h>

namespace oxygen
{
class Generic6DOFJointInt;

/** Generic6DOFJoint is a placeholder class for "six degrees of 
    freedom" - joints. They are supported by some 3D modelling tools
    and simspark shall eventually be able to import models created
    with these tools. All other joint classes are derived from this one.
*/

class OXYGEN_API Generic6DOFJoint : public Joint
{
public:
    Generic6DOFJoint();
    virtual ~Generic6DOFJoint();
    
    virtual void OnLink();
    
private:
    static boost::shared_ptr<Generic6DOFJointInt> mGeneric6DOFJointImp;
};

DECLARE_ABSTRACTCLASS(Generic6DOFJoint);

} //namespace oxygen

#endif //OXYGEN_GENERIC6DOFJOINT_H
