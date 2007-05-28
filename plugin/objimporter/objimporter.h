/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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
#ifndef OBJIMPORTER_H
#define OBJIMPORTER_H

#include <oxygen/geometryserver/meshimporter.h>

struct VTNIndex
{
    unsigned int v;  // vertex index
    unsigned int vt; // texture coord index
    unsigned int vn; // normal index
};

struct FaceIndex
{
    VTNIndex vertex[3];
};

typedef std::list<FaceIndex> TFaceIndexList;

/** \class ObjImporter is a MeshImporter the reads the Wavefront .obj format.

     This is a _very_ simple implementation that only reads the vertices, the
     texture coordinates, the vertex normals, and the texture information.
     Everything else is ignored for now.
 */
class ObjImporter : public oxygen::MeshImporter
{
public:
    ObjImporter();
    ~ObjImporter();

    virtual boost::shared_ptr<oxygen::TriMesh> ImportMesh
    (const std::string& name,const zeitgeist::ParameterList& parameter);

protected:

    void ExplodeString(std::string & input, 
                       std::vector<std::string> & output, 
                       std::string & del);
};

DECLARE_CLASS(ObjImporter);

#endif // OBJIMPORTER_H
