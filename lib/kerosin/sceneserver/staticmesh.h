/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: staticmesh.h,v 1.10 2004/04/22 17:17:39 rollmark Exp $

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
#ifndef KEROSIN_STATICMESH_H
#define KEROSIN_STATICMESH_H

#include <oxygen/sceneserver/basenode.h>
#include <oxygen/geometryserver/geometryserver.h>

namespace kerosin
{

class Material;

/** \class StaticMesh is a BaseNode that renders a TriMesh. The mesh
    is imported using the GeometryServer and all referenced materials
    are automatically loaded using the MaterialServer.
 */
class StaticMesh : public oxygen::BaseNode
{
    //
    // Types
    //
public:
    typedef std::vector<oxygen::IndexBuffer> TIndexBuffers;

    //
    // Function
    //
public:
    StaticMesh();
    ~StaticMesh();

    /** loads the mesh with the given name and parameters*/
    bool Load(const std::string& name,
              const zeitgeist::ParameterList& parameter);

    /** loads the mesh with the given name */
    bool Load(const std::string& name);

    /** empty! this prevents the bounding box from being updated, as
        it cannot change
    */
    virtual void ComputeBoundingBox();

    /** calculates the local bounding box */
    void CalcBoundingBox();

    /** returns the scale vector that is used to render the mesh */
    const salt::Vector3f& GetScale();

    /** sets the scale vector that is used to render the mesh */
    void SetScale(const salt::Vector3f& scale);

protected:
    virtual void RenderInternal();

    //
    // Members
    //
protected:
    /** vector of scale factors along each axis */
    salt::Vector3f mScale;

    /** the mesh to render */
    boost::shared_ptr<oxygen::TriMesh> mMesh;

    /** the materials used to render the mesh */
    std::vector<boost::shared_ptr<Material> > mMaterials;
};

DECLARE_CLASS(StaticMesh);

} //namespace kerosin

#endif //KEROSIN_STATICMESH_H
