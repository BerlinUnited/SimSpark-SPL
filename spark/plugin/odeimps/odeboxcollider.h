/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$
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
#ifndef ODEBOXCOLLIDER_H
#define ODEBOXCOLLIDER_H

#include "odeconvexcollider.h"
#include <oxygen/physicsserver/int/boxcolliderint.h>

class BoxColliderImp : public oxygen::BoxColliderInt, public ConvexColliderImp
{
    /** See physicsserver/int/boxcolliderint.h for documentation */

public:
    BoxColliderImp();
    void SetBoxLengths(const salt::Vector3f& extents, long geomID);
    void GetBoxLengths(salt::Vector3f& extents, long geomID);
    float GetPointDepth(const salt::Vector3f& pos, long geomID);
    long CreateBox();
};

DECLARE_CLASS(BoxColliderImp);

#endif //ODEBOXCOLLIDER_H
