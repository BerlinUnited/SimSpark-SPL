/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: geometryserver.h,v 1.1 2004/04/22 17:01:18 rollmark Exp $

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

#include <zeitgeist/class.h>
#include <zeitgeist/node.h>
#include "trimesh.h"

namespace oxygen
{

/** \class GeometryServer manages a set of triangle meshes and a set
    of plugins to import or generate them. Triangle meshes are used
    for collision detection and rendering.
 */
class GeometryServer : public zeitgeist::Node
{
protected:
    typedef std::map<std::string, boost::shared_ptr<TriMesh> > TMeshMap;

public:
    GeometryServer();
    ~GeometryServer();

    /** creates an instance of \param importerName and registers it as a
        MeshImporter to the GeometryServer
    */
    bool InitMeshImporter(const std::string& importerName);

    /** imports the trimesh \param name, with the given parameters or
        returns a cached reference
    */
    boost::shared_ptr<TriMesh> GetMesh
    (const std::string& name, const zeitgeist::ParameterList& parameter);

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


