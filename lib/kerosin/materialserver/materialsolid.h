/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: materialsolid.h,v 1.1 2004/03/20 12:54:07 rollmark Exp $

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
#ifndef KEROSIN_MATERIALSOLID_H
#define KEROSIN_MATERIALSOLID_H

#include "material.h"
#include <salt/vector.h>

namespace kerosin
{
class MaterialSolid : public Material
{
    //
    // Functions
    //
public:
    MaterialSolid();
    virtual ~MaterialSolid();

    virtual void Bind();

    /** sets the color of the solid surface */
    void SetColor(const salt::Vector3f& color);

    //
    // Members
    //
protected:
    /** the color of the solid surface */
    salt::Vector3f mColor;
};

DECLARE_CLASS(MaterialSolid);
};

#endif // KEROSIN_MATERIALSOLID_H
