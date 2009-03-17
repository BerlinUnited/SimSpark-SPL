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
#ifndef OXYGEN_MESHIMPORTER_H
#define OXYGEN_MESHIMPORTER_H

#include <oxygen/oxygen_defines.h>
#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include "trimesh.h"

namespace oxygen
{

/** \class MeshImporter defines an interface for classes that import
    or generate trimeshes. MeshImporter classes are registered to the
    GeometryServer
*/
class OXYGEN_API MeshImporter : public zeitgeist::Leaf
{
public:
    MeshImporter() : Leaf() {};
    virtual ~MeshImporter() {};

    /** loads or generates a mesh referenced by the given name and
        optional list of parameters
    */
    virtual boost::shared_ptr<TriMesh> ImportMesh
    (const std::string& name,__attribute__((unused))
     const zeitgeist::ParameterList& parameter) = 0;

    /** returns a unique name for each combination of name and
        parameter. This is only useful for parameterized
        MeshImporters. The default implementation returns \param name
     */
    virtual std::string MangleName
    (const std::string& name, const zeitgeist::ParameterList& parameter);
};

DECLARE_ABSTRACTCLASS(MeshImporter);

} // namespace oxgen

#endif // OXYGEN_MESHIMPORTER_H
