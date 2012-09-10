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
#ifndef KEROSIN_SINGLEMATNODE_H
#define KEROSIN_SINGLEMATNODE_H

#include <kerosin/kerosin_defines.h>
#include "staticmesh.h"

namespace kerosin
{
class Material;

/** \class SingleMatNode is a BaseNode that renders itself using a
    single material. It provides methods to manage this material.
 */
class KEROSIN_API SingleMatNode : public StaticMesh
{
    //
    // Function
    //
public:
    SingleMatNode();
    virtual ~SingleMatNode();

    /** queries the MaterialServer for the material \param name and
        caches a reference to it. It returns true on success.
     */
    bool SetMaterial(const std::string& name);

    /** returns the material assigned to this node */
    boost::shared_ptr<Material> GetMaterial();
};

DECLARE_CLASS(SingleMatNode);

} //namespace kerosin

#endif //KEROSIN_SINGLEMATNODE_H
