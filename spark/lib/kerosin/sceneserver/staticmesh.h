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
#ifndef KEROSIN_STATICMESH_H
#define KEROSIN_STATICMESH_H

#include <kerosin/kerosin_defines.h>
#include <kerosin/renderserver/rendernode.h>
#include <oxygen/geometryserver/geometryserver.h>

namespace kerosin
{

class Material;

/** \class StaticMesh is a Render that renders a TriMesh. The mesh is
    imported using the GeometryServer and all referenced materials are
    automatically loaded using the MaterialServer.
 */
class KEROSIN_API StaticMesh : public RenderNode
{
    //
    // Types
    //
public:
    typedef std::vector<oxygen::IndexBuffer> TIndexBuffers;
    typedef std::vector<boost::shared_ptr<Material> > TMaterialList;
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

    /** renders the node */
    virtual void RenderInternal();

    /** returns the managed mesh */
    const boost::shared_ptr<oxygen::TriMesh> GetMesh();

    /** returns the name of the loaded mesh */
    const std::string& GetMeshName();

    /** returns the parameter list of the loaded mesh */
    const zeitgeist::ParameterList& GetMeshParameter();

    /** returns the materials used to render the mesh */
    const TMaterialList& GetMaterials() const;

    /** a hint for the graphics engine if the mesh casts shadows or now.
        By default, StaticMeshes cast shadows. The respective graphics engine
        might ignore this flag if the GE cannot cast shadows.
    */
    bool CastShadows() const;

    /** set the flag if the mesh should cast shadows or not.
        The actual graphics engine might ignore this flag if it cannot render
        shadows or disables shadows globally.
    */
    void SetCastShadows(bool shadows);

    /** Set the flag if the GE should use an external mesh for rendering.
        By default, using external meshes is switched off. For this to work,
        the external mesh name has to be set.
    */
    void SetUseExternalMesh(bool external = true);
    /** Get the flag if the GE should use an external mesh for rendering.
        If set to true, the GE can use a different (possibly more advanced)
        mesh. The hint for the kind of external mesh should be given by the
        Get/SetExternalMeshName() methods of this class.
    */
    bool UseExternalMesh() const;
    /** Set the name of the external mesh.
        \string name the external mesh name
    */
    void SetExternalMeshName(const std::string& name);
    /** Get the name of the external mesh to use.
        \return the name
    */
    const std::string& GetExternalMeshName() const;
    //! Set an additional scale for the external mesh
    void SetExternalMeshScale(const salt::Vector3f& scale);
    /** Get the external mesh scale.
        @return the external mesh scale */
    const salt::Vector3f& ExternalMeshScale() const;

    /**
     * change the material of the mesh
     *
     * @param oldMat the name of old material
     * @param newMat the name of new materila
     *
     */
    void ChangeMaterial(const std::string& oldMat,const std::string& newMat);

    /**
     * @return the names of materials, separated by space
     */
    std::vector<std::string> GetMaterialNames() const;

    /**
     * reset the materials according to the names
     *
     * @param names a vector of material names
     */
    void ResetMaterials(const std::vector<std::string>& names);

    //
    // Members
    //
protected:
    /** vector of scale factors along each axis */
    salt::Vector3f mScale;

    /** the mesh to render */
    boost::shared_ptr<oxygen::TriMesh> mMesh;

    /** the materials used to render the mesh */
    TMaterialList mMaterials;

    /** the name of the loaded mesh */
    std::string mMeshName;

    /** the list of parameters the mesh was loaded with */
    zeitgeist::ParameterList mMeshParameter;

    //! the flag if the mesh should cast shadows
    bool mCastShadows;

    //! the flag if the GE can use an external mesh
    bool mUseExternalMesh;
    //! the external mesh name
    std::string mExternalMeshName;
    //! additional scale factors along each axis for external meshes
    salt::Vector3f mExternalMeshScale;
};

DECLARE_CLASS(StaticMesh);

} //namespace kerosin

#endif //KEROSIN_STATICMESH_H
