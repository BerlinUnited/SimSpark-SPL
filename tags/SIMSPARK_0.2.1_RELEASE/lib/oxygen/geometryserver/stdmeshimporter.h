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
#ifndef OXYGEN_STDMESHIMPORTER
#define OXYGEN_STDMESHIMPORTER

#include <oxygen/oxygen_defines.h>
#include "meshimporter.h"

namespace oxygen
{
/** \class StdMeshImporter is a MeshImporter that generates a standard
     set of meshes. These are spheres, boxes and capsule.
*/
class OXYGEN_API StdMeshImporter : public MeshImporter
{
public:
    StdMeshImporter();
    virtual ~StdMeshImporter();

    /** generates a mesh referenced by the given name and optional
        list of parameters */
    virtual boost::shared_ptr<TriMesh> ImportMesh
    (const std::string& name,const zeitgeist::ParameterList& parameter);

    /** returns a unique name for each parameterized capsule
        mesh, and \param name otherwise
     */
    virtual std::string MangleName
    (const std::string& name,const zeitgeist::ParameterList& parameter);

protected:
    /** constructs a unit sphere */
    boost::shared_ptr<TriMesh> UnitSphereMesh();

    /** constructs a unit box */
    boost::shared_ptr<TriMesh> UnitBoxMesh();

    /** constructs a unit capsule */
    boost::shared_ptr<TriMesh> UnitCapsule
    (const zeitgeist::ParameterList& parameter);

    /** constructs a flat unit cylinder */
    boost::shared_ptr<TriMesh> UnitCylinder();
};

DECLARE_CLASS(StdMeshImporter);

} // namespace oxygen

#endif // OXYGEN_STDMESHIMPORTER
