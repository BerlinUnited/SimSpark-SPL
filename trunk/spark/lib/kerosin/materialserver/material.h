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
#ifndef KEROSIN_MATERIAL_H
#define KEROSIN_MATERIAL_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <kerosin/kerosin_defines.h>
#include <string>

namespace kerosin
{
class KEROSIN_API Material : public zeitgeist::Leaf
{
    //
    // Functions
    //
public:
    Material();
    virtual ~Material();

    /** binds the managed material, i.e. sets all necessary OpenGL
        states */
    virtual void Bind() = 0;
};

DECLARE_ABSTRACTCLASS(Material);

} //namespace kerosin

#endif //KEROSIN_MATERIAL_H
