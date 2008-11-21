/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of simspark
   Tue May 9 2006
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2007 RoboCup Soccer Server 3D Maintenance Group
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

   MeshExporter

   NOTE: This class serves as an (abstract) exporter to register 
         meshes with other graphic engines (possibly also for exporting).

   HISTORY:
       30/03/07 - OO  - Initial version

*/
#ifndef OXYGEN_MESHEXPORTER_H
#define OXYGEN_MESHEXPORTER_H

#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include "trimesh.h"

namespace oxygen
{

/** \class MeshExporter.
    This class can be used to implement custom render server, which needs to be
    informed about the meshes used for a specific simulation. By registering a
    MeshExporter implementation to the GeometryServer, meshes can be registered
    (exported) to another class.
*/
class MeshExporter : public zeitgeist::Leaf
{
public:
    virtual ~MeshExporter() {}

    /** Register / Export the given mesh.
        \param mesh the mesh to register / export.
    */
    virtual void RegisterMesh(boost::shared_ptr<TriMesh> mesh) = 0;
};

DECLARE_ABSTRACTCLASS(MeshExporter);

} // end namespace oxygen

#endif // OXYGEN_MESHEXPORTER
