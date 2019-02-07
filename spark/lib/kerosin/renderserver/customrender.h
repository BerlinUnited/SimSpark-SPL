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

   CustomRender
*/
#ifndef KEROSIN_CUSTOMRENDER_H
#define KEROSIN_CUSTOMRENDER_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <kerosin/kerosin_defines.h>

namespace kerosin
{
/** CustomRender is the base class for all nodes installed below the
    Spark RenderControl node. This serves as a hook mechanism to
    render objects or overlays that are not part of the scene graph
*/
class KEROSIN_API CustomRender : public zeitgeist::Leaf
{
    //
    // Functions
    //
public:
    CustomRender();
    virtual ~CustomRender();

    virtual void Render() = 0;
};

DECLARE_ABSTRACTCLASS(CustomRender);

} //namespace kerosin

#endif //KEROSIN_CUSTOMRENDER_H
