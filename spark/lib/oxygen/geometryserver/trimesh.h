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
#ifndef OXYGEN_TRIMESH
#define OXYGEN_TRIMESH

#include "indexbuffer.h"
#include <list>
#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <string>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{

/** \class TriMesh (triangle mesh) encapsulates a list of vertices,
    texture coordinates and normals together with an associated list
    of index lists. Each index list additionally stores the name of a
    material. Triangle meshes describe arbitrary geometry. They are
    used for collision detection and rendering.
    Vertices, texture coordinates and normales are each stored as a
    list of floats, with three consecutive values for each
    vector. Indeces are stored using the IndexBuffer class as as list
    of unsigned int values.
 */
class OXYGEN_API TriMesh
{
public:
    struct Face
    {
    public:
        boost::shared_ptr<IndexBuffer> indeces;
        std::string material;

    public:
        Face(boost::shared_ptr<IndexBuffer> i, std::string m)
            : indeces(i), material(m) {};
    };

    typedef std::list<Face> TFaces;

public:
    TriMesh();
    ~TriMesh();

    /** sets the list of vertices and their number*/
    void SetPos(boost::shared_array<float> pos, int vertexCount);

    /** returns the list of vertices */
    const boost::shared_array<float> GetPos() const;

    /** returns the number of stored vertices */
    int GetVertexCount() const;

    /** sets the list of texture coordinates */
    void SetTexCoords(boost::shared_array<float> texCoords);

    /** returns the list of texture coordinates */
    const boost::shared_array<float> GetTexCoords() const;

    /** sets the list of surface normals */
    void SetNormals(boost::shared_array<float> normal);

    /** returns the list of surface normals */
    const boost::shared_array<float> GetNormals() const;

    /** adds a face, i.e. a list of indeces. Each consecutive triple
        describes one triangle */
    void AddFace(boost::shared_ptr<IndexBuffer> indeces,
                  const std::string& material = "default");

    /** adds a face, i.e. a list of indeces. Each consecutive triple
        describes one triangle */
    void AddFace(const Face& face);

    /** returns the list of faces */
    const TFaces& GetFaces() const;

    /** returns the name of this trimesh */
    const std::string& GetName() const;

    /** sets the name of this trimesh */
    void SetName(const std::string& name);

protected:
    /** the unique name of this trimesh */
    std::string mName;

    /** the number of stored vertices */
    int mVertexCount;

    /** the list of vertices */
    boost::shared_array<float> mPos;

    /** the list of texture coordinates */
    boost::shared_array<float> mTexCoords;

    /** the list of surface normals */
    boost::shared_array<float> mNormal;

    /** the list of faces */
    TFaces mFaces;
};

} // namespace oxygen

#endif // OXYGEN_TRIMESH
