/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: box.h,v 1.1 2004/03/20 15:50:09 rollmark Exp $

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
#ifndef KEROSIN_BOX_H
#define KEROSIN_BOX_H

#include "singlematnode.h"

namespace kerosin
{
class Material;

class Box : public SingleMatNode
{
    //
    // Function
    //
public:
    Box();
    virtual ~Box();

    void SetExtents(const salt::Vector3f& extents);
    const salt::Vector3f& GetExtents();

protected:
    virtual void RenderInternal();

    //
    // Members
    //
protected:
    salt::Vector3f mExtents;
};

DECLARE_CLASS(Box);

} //namespace kerosin

#endif //KEROSIN_BOX_H
