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
#ifndef OXYGEN_GEOMETRYSERVER_H
#define OXYGEN_GEOMETRYSERVER_H

#include <oxygen/oxygen_defines.h>
#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include "meshexporter.h"
#include "trimesh.h"

namespace oxygen
{

/** \class GeometryServer manages a set of triangle meshes and a set
    of plugins to import or generate them. Triangle meshes are used
    for collision detection and rendering.
 */
class OXYGEN_API GeometryServer : public zeitgeist::Node
{
protected:
    typedef std::map<std::string, boost::shared_ptr<TriMesh> > TMeshMap;

public:
    GeometryServer();
    ~GeometryServer();

    /** creates an instance of a mesh importer and registers it to the GeometryServer.
     * \param importerName the (class) name of the importer
     * \returns true if successful
    */
    bool InitMeshImporter(const std::string& importerName);

    /** imports the trimesh \param name, with the given parameters or
        returns a cached reference
    */
    boost::shared_ptr<TriMesh>
    GetMesh (const std::string& name, const zeitgeist::ParameterList& parameter);

    /** register a trimesh to the GeometryServer. The name of the mesh
        has to be set
    */
    void RegisterMesh(boost::shared_ptr<TriMesh> mesh);

    /** Create an instance of a mesh exporter and register it to the GeometryServer.
        A MeshExporter is a class that takes care to additionally register/export
        meshes registered at the GeometryServer at another class (potentially
         external to spark). This has to be done only when spark/kerosin
        rendering alone is not enough.
        \param name name of the MeshExporter class name
        \returns true if successful
    */
    bool InitMeshExporter(const std::string& name);

protected:
    /** registers the standard mesh importer */
    virtual void OnLink();

protected:
    /** the registry of cached trimeshes */
    TMeshMap mMeshMap;
};

DECLARE_CLASS(GeometryServer);

} //namespace oxygen

#endif //OXYGEN_GEOMETRYSERVER_H
