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

#ifndef OXYGEN_SLIDERJOINTINT_H
#define OXYGEN_SLIDERJOINTINT_H

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
class RigidBody;

class OXYGEN_API SliderJointInt
{
public:
    virtual ~SliderJointInt() {}

    /** Creates a Slider Joint within the given world */
    virtual long CreateSliderJoint(long world) = 0;

    /** returns the slider linear position, i.e. the slider's
        `extension'. When the axis is set, the current position of the
        attached bodies is examined and that position will be the zero
        position.
    */
    virtual float GetPosition(long jointID) = 0;

    /** returns the time derivative of the sliders linear position */
    virtual float GetPositionRate(long jointID) = 0;

    /** Sets the direction of the slider joint's axis */
    virtual void SetSliderAxis(salt::Vector3f& up, long jointID) = 0;
};

} //namespace oxygen

#endif //OXYGEN_SLIDERJOINTINT_H
