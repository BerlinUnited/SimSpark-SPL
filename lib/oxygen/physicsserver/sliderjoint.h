/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sliderjoint.h,v 1.1 2004/04/14 18:22:55 rollmark Exp $

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

#include "joint.h"

namespace oxygen
{
class Body;

class SliderJoint : public Joint
{
public:
    SliderJoint();
    virtual ~SliderJoint();

    /**  returns the slider linear position, i.e. the slider's
         `extension'. When the axis is set, the current position of
         the attached bodies is examined and that position will be the
         zero position.
    */
    float GetSliderPosition();

    /** returns the time derivative of the sliders linear position */
    float GetSliderPositionRate();

protected:
    /** attaches the joint to some new bodies */
    virtual void Attach(boost::shared_ptr<Body> body1,
                        boost::shared_ptr<Body> body2);

    /** creates a new contact joint */
    virtual void OnLink();
};

DECLARE_CLASS(SliderJoint);

} // namespace oxygen

#endif // OXYGEN_SLIDERJOINT_H
