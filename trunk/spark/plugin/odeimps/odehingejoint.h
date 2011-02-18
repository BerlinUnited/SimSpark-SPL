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

#ifndef ODEHINGEJOINT_H
#define ODEHINGEJOINT_H

#include "odegeneric6dofjoint.h"
#include <oxygen/physicsserver/int/hingejointint.h>

class HingeJointImp : public oxygen::HingeJointInt, public Generic6DOFJointImp
{
    /** See physicsserver/int/hingejointint.h for documentation */
    
public:
    HingeJointImp();
    long CreateHingeJoint(long worldID);
    void SetAnchor(const salt::Vector3f& anchor, long jointID);
    salt::Vector3f GetAnchor1(long jointID);
    salt::Vector3f GetAnchor2(long jointID);
    void SetAxis(const salt::Vector3f& axis, long jointID);
    salt::Vector3f GetAxis(long jointID);
    float GetAngle(long jointID) const;
    float GetAngleRate(long jointID) const;
    float GetTorque(long jointID) const;
private:
    dJointFeedback mFeedback;

};

DECLARE_CLASS(HingeJointImp);

#endif //ODEHINGEJOINT_H
