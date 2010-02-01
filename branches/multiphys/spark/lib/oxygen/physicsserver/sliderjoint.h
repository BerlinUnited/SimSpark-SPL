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
#ifndef OXYGEN_SLIDERJOINT_H
#define OXYGEN_SLIDERJOINT_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/generic6dofjoint.h>

namespace oxygen
{
class RigidBody;
class SliderJointInt;

class OXYGEN_API SliderJoint : public Generic6DOFJoint
{
public:
    SliderJoint();
    virtual ~SliderJoint();

    /** returns the slider linear position, i.e. the slider's
        `extension'. When the axis is set, the current position of the
        attached bodies is examined and that position will be the zero
        position.
    */
    float GetPosition();

    /** returns the time derivative of the sliders linear position */
    float GetPositionRate();

protected:
    /** attaches the joint to some new bodies */
    virtual void Attach(boost::shared_ptr<RigidBody> body1,
                        boost::shared_ptr<RigidBody> body2);

    /** creates a new slider joint */
    virtual void OnLink();
    
private:
    static boost::shared_ptr<SliderJointInt> mSliderJointImp;
};

DECLARE_CLASS(SliderJoint);

} // namespace oxygen

#endif // OXYGEN_SLIDERJOINT_H
