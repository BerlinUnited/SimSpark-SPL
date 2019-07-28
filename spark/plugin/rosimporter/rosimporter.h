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
#ifndef ROSIMPORTER_H
#define ROSIMPORTER_H

#include <map>
#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif
#include <salt/matrix.h>
#include <oxygen/sceneserver/sceneimporter.h>
#include <oxygen/geometryserver/trimesh.h>
#include <tinyxml/tinyxml.h>
#include "roselements.h"

namespace oxygen
{
    class BaseNode;
    class Transform;
    class TransformCollider;
    class ContactJointHandler;
    class RigidBody;
    class Transform;
    class Joint;
}

namespace kerosin
{
    class RGBA;
}

class RosImporter : public oxygen::SceneImporter
{
public:
    struct Trans
    {
    public:
        salt::Matrix matrix;

    public:
        Trans()
        {
            matrix.Identity();
        }
    };

    /** JointAxis describes a single joint axis along with associated
        joint parameters
     */
    struct JointAxis
    {
    public:
        salt::Vector3f dir;

        //! true, iff loStop and hiStop angle are valid
        bool setDeflection;

        //! low stop angle (rad)
        double loStop;

        //! high stop angle (rad)
        double hiStop;

    public:
        JointAxis()
            : dir(0.0,0.0,0.0), setDeflection(false),
              loStop(0.0), hiStop(0.0)
        {
        }
    };

    /** JointAttach describes how a joint connects two bodies along
        with the associated axis parameters
    */
    struct JointAttach
    {
    public:
        boost::shared_ptr<oxygen::Joint> joint;
        boost::shared_ptr<oxygen::RigidBody> body1;
        boost::shared_ptr<oxygen::RigidBody> body2;
        JointAxis axis1;
        JointAxis axis2;

    public:
        JointAttach()
        {
        }
    };

    /** RosContext refers to the current Transform parent node that is
        used to construct associated visual, geometric and physical
        nodes. It further describes mass and mass center of composite
        bodies.
     */
    struct RosContext
    {
    public:
        boost::shared_ptr<oxygen::Transform> transform;
        boost::shared_ptr<oxygen::RigidBody> body;
        bool adjustedPos;
        salt::Vector3f massCenter;
        double totalMass;
        bool movable;

    public:
        RosContext()
            : adjustedPos(false), massCenter(0.0,0.0,0.0),
              totalMass(0.0), movable(false)
        {
        }

        /** moves the Transform parent node in order to put the Body
            into the mass center of a composite body
        */
        void AdjustPos();

        /** accumulates mass and adjusts the mass center */
        void AddMass(double mass, const Trans& trans);
    };

    /** declare a stack of RosContext nodes */
    typedef std::vector<RosContext > TRosStack;

    /** RosJointContext holds the child Body node that is connected to
        the Body refered to in the current RosContext
    */
    struct RosJointContext
    {
        boost::shared_ptr<oxygen::RigidBody> body;
    };

    /** declare a stack of RosContext nodes */
    typedef std::vector<RosJointContext> TRosJointStack;

    /** Appearance holds a material definition */
    struct Appearance
    {
    public:
        std::string ref;
    };

    /** Physical holds physical properties of the current
        RosContext.
    */
    struct Physical
    {
    public:
        bool valid;
        double mass;
        bool canCollide;
        salt::Vector3f massCenter;

    public:
        Physical()
            : valid(false), mass(0.0),
              canCollide(true), massCenter(0.0, 0.0, 0.0) {}
    };

    /** define a registry of macros; a macro is a XML subtree with the
        <Macro> node as the root element
    */
    typedef std::map<std::string, boost::shared_ptr<TiXmlElement> > TMacroMap;

    struct TVertex
    {
    public:
        //! the vertex data
        salt::Vector3f vec;

        /** the index into an associated flat vertex array as used in
            the TriMesh class
        */
        int idx;

    public:
        TVertex() : idx(-1) {}
    };

    /** define a mapping from name to vertex name as defined within a
        VertexList element
    */
    typedef std::map<std::string, TVertex> TVertexRef;

    struct TVertexList
    {
    protected:
        /** mapping from vertex name to vector and index into the pos
            array
        */
        TVertexRef vertexRef;

        /** flat array of vertices with 3 consecutive floats
            representing a vertex vector
        */
        boost::shared_array<float> pos;

    public:
        const TVertexRef& GetVertexRef() { return vertexRef; }
        void AddVertex(const std::string& name, const TVertex& vertex);
        boost::shared_array<float> GetPos();
        int GetIndex(const std::string& name);
        int GetPosCount() const { return static_cast<int>(vertexRef.size()); }
    };

    /** define a mapping from name to vertex list name */
    typedef std::map<std::string, TVertexList> TVertexListMap;

    /** define a vector of string references into a vertex list */
    typedef std::vector<std::string> TVertexRefVec;

    /** the possible types of complex geoms that make up a
        ComplexShape
    */
    enum EComplexGeom
        {
            CG_None = 0,
            CG_Polygon,
            CG_TriangleStrip
        };

    /** define a complex geom and it's associated vertices */
    struct ComplexGeom
    {
    public:
        EComplexGeom type;
        TVertexRefVec vertices;

    public:
        ComplexGeom(EComplexGeom t = CG_None) : type(t) {}
    };

    /** defines a list of complex geoms */
    typedef std::list<ComplexGeom> TComplexGeomList;

public:
    RosImporter();
    virtual ~RosImporter();

    virtual bool ImportScene(const std::string& fileName,
                             boost::shared_ptr<oxygen::BaseNode> parent,
                             boost::shared_ptr<zeitgeist::ParameterList> parameter);

    virtual bool ParseScene(const std::string& scene,
                            boost::shared_ptr<oxygen::BaseNode> parent,
                            boost::shared_ptr<zeitgeist::ParameterList> parameter);

protected:
    void PushContext();
    void PopContext();
    void PushJointContext();
    void PopJointContext();
    RosContext& GetContext();
    RosJointContext& GetJointContext();
    boost::shared_ptr<oxygen::Transform> GetContextTransform(boost::shared_ptr<oxygen::BaseNode> parent, const Trans& trans);
    boost::shared_ptr<oxygen::Transform> CreateTransform(boost::shared_ptr<oxygen::BaseNode> parent, const Trans& trans);
    boost::shared_ptr<oxygen::RigidBody> GetContextBody(boost::shared_ptr<oxygen::BaseNode> parent);
    void SetJointBody(boost::shared_ptr<oxygen::RigidBody> body);

    virtual bool ParseScene(const char* scene, int size,
                            boost::shared_ptr<oxygen::BaseNode> parent,
                            boost::shared_ptr<zeitgeist::ParameterList> parameter);


    TiXmlElement* GetFirstChild(TiXmlNode* node, RosElements::ERosElement type);
    TiXmlElement* IterateChildren(TiXmlNode* node, RosElements::ERosElement type);

    bool IgnoreNode(const TiXmlNode* node) const;

    RosElements::ERosElement GetType(const TiXmlElement* element) const;
    std::string GetName(RosElements::ERosElement element) const;

    void ApplyTransform(boost::shared_ptr<oxygen::Transform> transform, const Trans& trans);

    boost::shared_ptr<oxygen::TransformCollider> CreateTransformCollider
    (boost::shared_ptr<oxygen::BaseNode> parent, const Trans& trans);

    boost::shared_ptr<oxygen::ContactJointHandler>
    CreateContactJointHandler();

    bool ReadAmbientLight(TiXmlElement* element);

    bool ReadAttribute(TiXmlElement* element, const std::string& attribute, double& value, bool succeedIfMissing=false);
    bool ReadAttribute(TiXmlElement* element, const std::string& attribute, std::string& value, bool succeedIfMissing=false);
    bool ReadRGBA(TiXmlElement* element, kerosin::RGBA& rgba);
    bool ReadVector(TiXmlElement* element, salt::Vector3f& vec, bool succeedIfMissing = false);

    bool ReadAppearenceDef(TiXmlElement* element);
    bool ReadScene(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);

    bool ReadMacro(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadUse(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);

    bool ReadChildElements(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);

    bool ReadElements(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadCompound(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadMovable(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);

    bool ReadTrans(TiXmlElement* element, Trans& trans);
    bool ReadDefaultAppearance(TiXmlElement* element);
    bool ReadGlobalPhsyParams(TiXmlElement* element);

    bool ReadAppearance(TiXmlElement* element, Appearance& appear);
    bool ReadPhysical(TiXmlElement* element, Physical& physical);

    bool ReadBox(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadSphere(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadCylinder(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadCapsule(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);

    bool ReadAnchorPoint(TiXmlElement* element, salt::Vector3f& anchor);
    bool ReadAxis(TiXmlElement* element, RosElements::ERosElement type, JointAxis& axis);

    boost::shared_ptr<oxygen::RigidBody> GetJointParentBody();
    boost::shared_ptr<oxygen::RigidBody> GetJointChildBody(boost::shared_ptr<oxygen::BaseNode> parent);

    bool ReadHinge(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadSlider(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadUniversal(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);

    bool ReadVertexList(TiXmlElement* element);
    bool ReadComplexShape(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadGraphicalRep(TiXmlElement* element, boost::shared_ptr<oxygen::TriMesh> mesh, const Appearance& appear);
    bool ReadComplexElements(TiXmlElement* element, TComplexGeomList& geomList);
    bool ReadComplexGeom(TiXmlElement* element, ComplexGeom& geom);
    void BuildTriMesh(boost::shared_ptr<oxygen::TriMesh> mesh, TVertexList& vertexList, const TComplexGeomList& geomList, const Appearance& appear);
    void BuildPolygon(oxygen::IndexBuffer& ibuffer, TVertexList& vertexList, const ComplexGeom& geom);

    bool ReadPhysicalRep(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadSimpleBox(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadSimpleSphere(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);
    bool ReadSimpleCapsule(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element);

    void Attach(boost::shared_ptr<oxygen::Joint> joint, boost::shared_ptr<oxygen::RigidBody> body1, boost::shared_ptr<oxygen::RigidBody> body2,
                const JointAxis& axis1, const JointAxis& axis2 = JointAxis());
    void AttachJoint(const JointAttach& ja);

private:
    /** RosContextScope is a helper class that creates a new
        RosContext and destroys it when it goes ot ouf scope
    */
    struct RosContextScope
    {
    public:
        RosContextScope(RosImporter* i)
            : importer(i)
        {
            importer->PushContext();
        }

        ~RosContextScope()
        {
            importer->PopContext();
        }

    protected:
        RosImporter* importer;
    };

    /** RosJointScope is a helper class that creates a new joint scope
        and destroys it when it goes out of scope
    */
    struct RosJointScope
    {
        RosJointScope(RosImporter* i)
            : importer(i)
        {
            importer->PushJointContext();
        }

        ~RosJointScope()
        {
            importer->PopJointContext();
        }

    protected:
        RosImporter* importer;
    };

protected:
    /** reference to the parent node under wich the imported scene is
        constructed
    */
    boost::shared_ptr<oxygen::BaseNode> mSceneParent;

    /** the last supplied fileName */
    std::string mFileName;

    /** the default appearence definition if omitted in a ROS node */
    Appearance mDefaultAppearance;

    /** default global erp value */
    double mGlobalERP;

    /** default global cfm value */
    double mGlobalCFM;

    /** the registry of vertex lists */
    TVertexListMap mVertexListMap;

    /** the static macro registry is shared across RosImporter
        instances
    */
    static TMacroMap mMacroMap;

    /** the stack of RosContext instances */
    TRosStack mStack;

    /* the stack of JointContext instances */
    TRosJointStack mJointStack;
};

DECLARE_CLASS(RosImporter);

#endif // ROSIMPORTER_H
