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
#ifndef KEROSIN_BOX_H
#define KEROSIN_BOX_H

#include <kerosin/kerosin_defines.h>
#include "singlematnode.h"

namespace kerosin
{
class Material;

/** Box is a SingleMatNode that automatically loads and renders a box
    mesh that is scaled to the given extents.
 */
class KEROSIN_API Box : public SingleMatNode
{
    //
    // Function
    //
public:
    Box();
    virtual ~Box();

    /** sets the extents box */
    void SetExtents(const salt::Vector3f& extents);

    /** returns the extents of box */
    const salt::Vector3f& GetExtents();

protected:
    virtual void OnLink();
};

DECLARE_CLASS(Box);

} //namespace kerosin

#endif //KEROSIN_BOX_H
