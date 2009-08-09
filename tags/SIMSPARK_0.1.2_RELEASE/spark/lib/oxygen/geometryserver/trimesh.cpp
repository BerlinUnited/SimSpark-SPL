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
#include "trimesh.h"

using namespace oxygen;
using namespace std;
using namespace boost;

TriMesh::TriMesh() : mVertexCount(0)
{
}

TriMesh::~TriMesh()
{
}

void TriMesh::SetPos(shared_array<float> pos, int vertexCount)
{
    mPos = pos;
    mVertexCount = vertexCount;
}

void TriMesh::SetTexCoords(shared_array<float> texCoords)
{
    mTexCoords = texCoords;
}

void TriMesh::SetNormals(shared_array<float> normal)
{
    mNormal = normal;
}

const shared_array<float> TriMesh::GetPos() const
{
    return mPos;
}

int TriMesh::GetVertexCount() const
{
    return mVertexCount;
}

const shared_array<float> TriMesh::GetTexCoords() const
{
    return mTexCoords;
}

const shared_array<float> TriMesh::GetNormals() const
{
    return mNormal;
}

void TriMesh::AddFace(boost::shared_ptr<IndexBuffer> indeces,
                       const std::string& material)
{
    mFaces.push_back(Face(indeces, material));
}

void TriMesh::AddFace(const Face& face)
{
    mFaces.push_back(face);
}

const TriMesh::TFaces& TriMesh::GetFaces() const
{
    return mFaces;
}

const std::string& TriMesh::GetName() const
{
    return mName;
}

void TriMesh::SetName(const std::string& name)
{
    mName = name;
}


