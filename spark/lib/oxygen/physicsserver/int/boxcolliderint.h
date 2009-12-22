/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: space.h 102 2009-11-18 07:24:29Z a-held $

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

#ifndef OXYGEN_BOXCOLLIDERINT_H
#define OXYGEN_BOXCOLLIDERINT_H

#include <salt/vector.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

class OXYGEN_API BoxColliderInt
{

public:    
    virtual void SetBoxLengths(const salt::Vector3f& extents) = 0;
    virtual void GetBoxLengths(salt::Vector3f& extents) = 0;
    virtual float GetPointDepth(const salt::Vector3f& pos) = 0;
    virtual long CreateBox() = 0;
};

} //namespace oxygen

#endif //OXYGEN_BOXCOLLIDERINT_H
