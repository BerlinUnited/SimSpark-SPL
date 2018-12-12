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
#include "rosimporter.h"
#include <tinyxml/xmlfunctions.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/physicsserver/transformcollider.h>
#include <oxygen/physicsserver/boxcollider.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <oxygen/physicsserver/capsulecollider.h>
#include <oxygen/physicsserver/contactjointhandler.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/hingejoint.h>
#include <oxygen/physicsserver/sliderjoint.h>
#include <oxygen/physicsserver/universaljoint.h>
#include <oxygen/geometryserver/geometryserver.h>
#include <oxygen/geometryserver/trimesh.h>
#include <kerosin/openglserver/glbase.h>
#include <kerosin/materialserver/materialserver.h>
#include <kerosin/materialserver/materialsolid.h>
#include <kerosin/sceneserver/box.h>
#include <kerosin/sceneserver/sphere.h>
#include <kerosin/sceneserver/capsule.h>
#include <kerosin/sceneserver/staticmesh.h>
#include <kerosin/renderserver/renderserver.h>
#ifndef Q_MOC_RUN
#include <boost/scoped_array.hpp>
#endif

using namespace salt;
using namespace zeitgeist;
using namespace kerosin;
using namespace oxygen;
using namespace boost;
using namespace std;

static const string S_FROMSTRING("<from string>");
static const string S_BODY("body_");
static const string S_GEOMTRANS("geomtrans_");
static const string S_GEOM("geometry_");
static const string S_VISUAL("visual_");
static const string S_MACRO("macro_");
static const string S_UNNAMED("<unnamed>");

#pragma warning(disable: 4244)

// --- RosImporter::TVertexList

boost::shared_array<float> RosImporter::TVertexList::GetPos()
{
    if (pos.get() != 0)
        {
            return pos;
        }

    pos = shared_array<float> (new float[vertexRef.size() * 3]);

    int i=0;
    for (
         TVertexRef::iterator iter = vertexRef.begin();
         iter != vertexRef.end();
         ++iter
         )
        {
            TVertex& vertex = (*iter).second;
            vertex.idx = i;

            pos[(i*3)+0] = vertex.vec[0];
            pos[(i*3)+1] = vertex.vec[1];
            pos[(i*3)+2] = vertex.vec[2];

            ++i;
        }

    return pos;
}

int RosImporter::TVertexList::GetIndex(const std::string& name)
{
    TVertexRef::const_iterator iter = vertexRef.find(name);
    if (iter == vertexRef.end())
        {
            return -1;
        }

    return (*iter).second.idx;
}


void RosImporter::TVertexList::AddVertex(const std::string& name, const TVertex& vertex)
{
    pos.reset();
    vertexRef[name] = vertex;
}

// --- RosImporter

RosImporter::TMacroMap RosImporter::mMacroMap;


RosImporter::RosImporter() : SceneImporter()
{
    mGlobalERP = 0.2;
    mGlobalCFM = 0.0001;
}

RosImporter::~RosImporter()
{
}

RosElements::ERosElement RosImporter::GetType(const TiXmlElement* element) const
{
    return RosElements::GetInstance().Lookup(GetXMLValue(element));
}

string RosImporter::GetName(RosElements::ERosElement element) const
{
    return RosElements::GetInstance().Lookup(element);
}

TiXmlElement* RosImporter::GetFirstChild(TiXmlNode* node, RosElements::ERosElement type)
{
    return ::GetFirstChild(node, GetName(type));
}

TiXmlElement* RosImporter::IterateChildren(TiXmlNode* node, RosElements::ERosElement type)
{
    return ::IterateChildren(node, GetName(type));
}

bool RosImporter::ImportScene(const string& fileName,
                              boost::shared_ptr<BaseNode> parent,
                              boost::shared_ptr<ParameterList> parameter)
{
    // try to open the file
    boost::shared_ptr<salt::RFile> file = GetFile()->OpenResource(fileName);

    if (file.get() == 0)
        {
            GetLog()->Error() << "(RosImporter) ERROR: cannot open file '"
                              << fileName << "'\n";
            return false;
        }

    mFileName = fileName;
    mSceneParent = parent;

    // read entire file into a temporary buffer
    scoped_array<char> buffer(new char[file->Size() + 1]);
    file->Read(buffer.get(), file->Size());
    buffer[file->Size()] = 0;

    return ParseScene(buffer.get(), file->Size(), parent, parameter);
}

bool RosImporter::ParseScene(const string& scene,
                             boost::shared_ptr<BaseNode> parent,
                             boost::shared_ptr<ParameterList> parameter)
{
    mFileName = S_FROMSTRING;
    return ParseScene(scene.c_str(),static_cast<int>(scene.size()),parent,parameter);
}

bool RosImporter::IgnoreNode(const TiXmlNode* node) const
{
    if (node == 0)
        {
            return true;
        }

    switch(node->Type())
        {
        case TiXmlNode::ELEMENT:
            return false;

        default:
            /** ignore declarations, comments and other
                unknown node types
            */
            return true;
        }
}

void RosImporter::PushContext()
{
    RosContext context;
    context.movable = (mStack.empty()) ?
        false : GetContext().movable;

    mStack.push_back(context);
}

void RosImporter::PopContext()
{
    RosContext& context = mStack.back();
    context.AdjustPos();
    mStack.pop_back();
}

void RosImporter::PushJointContext()
{
    mJointStack.push_back(RosJointContext());
}

void RosImporter::PopJointContext()
{
    mJointStack.pop_back();
}

void RosImporter::RosContext::AdjustPos()
{
    if (adjustedPos)
        {
            return;
        }

    adjustedPos = true;
    if (body.get() == 0)
        {
            return;
        }

    // adjust for the mass center
    boost::shared_ptr<Transform> transform = dynamic_pointer_cast<Transform>
        (body->GetParent().lock());

    if (transform.get() == 0)
        {
            assert(false);
            return;
        }

    massCenter /= totalMass;
    transform->SetLocalPos(transform->GetLocalPos() + massCenter);
    body->SetPosition(body->GetPosition() + massCenter);
}

void RosImporter::RosContext::AddMass(double mass, const Trans& trans)
{
    massCenter += trans.matrix.Pos() * mass;
    totalMass += mass;
}

RosImporter::RosContext& RosImporter::GetContext()
{
    return mStack.back();
}

RosImporter::RosJointContext& RosImporter::GetJointContext()
{
    return mJointStack.back();
}

void RosImporter::ApplyTransform(boost::shared_ptr<Transform> transform, const Trans& trans)
{
    transform->SetLocalTransform(trans.matrix);
}

boost::shared_ptr<Transform> RosImporter::CreateTransform(boost::shared_ptr<BaseNode> parent, const Trans& trans)
{
    boost::shared_ptr<Transform> transform = dynamic_pointer_cast<Transform>
        (GetCore()->New("/oxygen/Transform"));

    ApplyTransform(transform, trans);
    parent->AddChildReference(transform);

    Vector3f pos = transform->GetWorldTransform().Pos();

    return transform;
}

boost::shared_ptr<Transform> RosImporter::GetContextTransform(boost::shared_ptr<BaseNode> parent, const Trans& trans)
{
    RosContext& context = GetContext();
    if (context.transform.get() != 0)
        {
            return context.transform;
        }

    assert(parent.get() != 0);

    Leaf::TLeafList parentList;
    boost::shared_ptr<Leaf> current(parent);
    while (current.get() != 0)
        {
            if (current.get() == mSceneParent.get())
                {
                    break;
                }
            boost::shared_ptr<Transform> transform = dynamic_pointer_cast<Transform>(current);
            if (transform.get() != 0)
                {
                    parentList.push_back(transform);
                }

            current = current->GetParent().lock();
        }

    Trans globalTrans;

    for (
         Leaf::TLeafList::reverse_iterator iter =  parentList.rbegin();
         iter != parentList.rend();
         ++iter
         )
        {
            boost::shared_ptr<Transform> transform
                = static_pointer_cast<Transform>(*iter);

            globalTrans.matrix = globalTrans.matrix * transform->GetLocalTransform();
        }

    globalTrans.matrix = globalTrans.matrix * trans.matrix;

    context.transform = CreateTransform(mSceneParent, globalTrans);
    return context.transform;
}

void RosImporter::SetJointBody(boost::shared_ptr<RigidBody> body)
{
    if (mJointStack.empty())
        {
            return;
        }

    RosJointContext& context = GetJointContext();
    if (context.body.get() == 0)
        {
            context.body = body;
        }
}

boost::shared_ptr<RigidBody> RosImporter::GetContextBody(boost::shared_ptr<BaseNode> parent)
{
    RosContext& context = GetContext();
    if (
        (! context.movable) ||
        (parent.get() == 0)
        )
        {
            return boost::shared_ptr<RigidBody>();
        }

    if (context.body.get() != 0)
        {
            return context.body;
        }

    if (context.transform.get() == 0)
        {
            assert(false);
            return boost::shared_ptr<RigidBody>();
        }

    context.body = dynamic_pointer_cast<RigidBody>
        (GetCore()->New("/oxygen/RigidBody"));


    SetJointBody(context.body);

    parent->AddChildReference(context.body);

    return context.body;
}

bool RosImporter::ParseScene(const char* scene, int size,
                             boost::shared_ptr<BaseNode> parent,
                             boost::shared_ptr<zeitgeist::ParameterList> parameter)
{
    TiXmlDocument document;
    document.Parse(scene);

    if (document.Error())
        {
            GetLog()->Error() << "(RosImporter) ERROR: xml parsing error: "
                              << document.ErrorDesc() << "\n";
            return false;
        }

    TiXmlElement* xmlRoot = document.RootElement();
    if (xmlRoot == 0)
        {
            GetLog()->Error() << "(RosImporter) ERROR: empty xml document\n";
            return false;
        }

    if (
        (GetType(xmlRoot) != RosElements::RE_SIMULATION) &&
        (GetType(xmlRoot) != RosElements::RE_ROSIINCLUDEFILE)
        )
        {
            GetLog()->Error() << "(RosImporter) ERROR: unknown xml root element type "
                              << GetXMLValue(xmlRoot) << "\n";
            return false;
        }

    for (
         TiXmlNode* node = xmlRoot->FirstChild();
         node != 0;
         node = xmlRoot->IterateChildren(node)
         )
        {
            if (IgnoreNode(node))
                {
                    continue;
                }

            TiXmlElement* element = static_cast<TiXmlElement*>(node);
            bool ok = true;

            RosElements::ERosElement type = GetType(element);
            switch (type)
                {
                default:
                    GetLog()->Error() << "(RosImporter::ParseScene) ERROR: skipping unknown toplevel element "
                                      << GetXMLPath(element) << "\n";
                    break;

                case RosElements::RE_APPEARANCEDEFINITION:
                    ok = ReadAppearenceDef(element);
                    break;

                case RosElements::RE_SCENE:
                    ok = ReadScene(parent, element);
                    break;

                case RosElements::RE_MACRO:
                    ok = ReadMacro(parent, element);
                    break;

                case RosElements::RE_VERTEXLIST:
                    ok = ReadVertexList(element);
                    break;
                }

            if (! ok)
                {
                    break;
                }

        }

    return true;
}

bool RosImporter::ReadAttribute(TiXmlElement* element, const string& attribute,
                                string& value, bool succeedIfMissing)
{
    if (element == 0)
        {
            return false;
        }

    if (! GetXMLAttribute(element, attribute, value))
        {
            if (! succeedIfMissing)
                {
                    string name = S_UNNAMED;
                    ReadAttribute(element, RA_NAME, name, true);

                    GetLog()->Error() << "(RosImporter) ERROR: missing string attribute " << attribute << " in "
                                      << GetXMLPath(element) << " name " << name << " \n";
                    return false;
                }
        }

    return true;
}

bool RosImporter::ReadAttribute(TiXmlElement* element, const string& attribute,
                                double& value, bool succeedIfMissing)
{
    if (element == 0)
        {
            return false;
        }

    if (! GetXMLAttribute(element, attribute, value))
        {
            if (! succeedIfMissing)
                {
                    string name = S_UNNAMED;
                    ReadAttribute(element, RA_NAME, name, true);

                    GetLog()->Error() << "(RosImporter) ERROR: missing float attribute " << attribute << " in "
                                      << GetXMLPath(element) << " name " << name << "\n";
                    return false;
                }
        }

    return true;
}


bool RosImporter::ReadRGBA(TiXmlElement* element, RGBA& rgba)
{
    int r,g,b;
    if (
        (! GetXMLAttribute(element, RA_R, r)) ||
        (! GetXMLAttribute(element, RA_G, g)) ||
        (! GetXMLAttribute(element, RA_B, b))
        )
        {
            string name = S_UNNAMED;
            ReadAttribute(element, RA_NAME, name, true);

            GetLog()->Error() << "(RosImporter) ERROR: missing color attributes in "
                              << GetXMLPath(element) << " name " << name << "\n";
            return false;
        }

    rgba.r() = (static_cast<float>(r) / 255.0);
    rgba.g() = (static_cast<float>(g) / 255.0);
    rgba.b() = (static_cast<float>(b) / 255.0);

    double a;
    rgba.a() = GetXMLAttribute(element, RA_A, a) ? a : 1.0;

    return true;
}

bool RosImporter::ReadVector(TiXmlElement* element, Vector3f& vec, bool succeedIfMissing)
{
    if (
        (! GetXMLAttribute(element, RA_X, vec[0])) ||
        (! GetXMLAttribute(element, RA_Y, vec[1])) ||
        (! GetXMLAttribute(element, RA_Z, vec[2]))
        )
        {
            if (! succeedIfMissing)
                {
                    string name = S_UNNAMED;
                    ReadAttribute(element, RA_NAME, name, true);

                    GetLog()->Error() << "(RosImporter) ERROR: invalid or missing vector attributes in "
                                      << GetXMLPath(element) << " name " << name << "\n";
                    return false;
                }
        }

    return true;
}

bool RosImporter::ReadAppearenceDef(TiXmlElement* element)
{
    boost::shared_ptr<MaterialServer> materialServer = dynamic_pointer_cast<MaterialServer>
        (GetCore()->Get("/sys/server/material"));

    if (materialServer.get() == 0)
        {
            GetLog()->Error() << "(RosImporter) ERROR: failed to lookup MaterialServer node\n";
            return false;
        }

    string name;
    if (! ReadAttribute(element, RA_NAME, name))
        {
            return false;
        }

    RGBA rgba;

    TiXmlElement* colorElem = GetFirstChild(element, RosElements::RE_COLOR);
    if (colorElem == 0)
        {
            // default to a white material
            GetLog()->Debug() << "(RosImporter) missing color attribute in AppearanceDefinition\n";
            rgba = RGBA(1.0,1.0,1.0,1.0);
        }


    if (! ReadRGBA(colorElem, rgba))
        {
            return false;
        }

    boost::shared_ptr<MaterialSolid> material = dynamic_pointer_cast<MaterialSolid>
        (GetCore()->New("/kerosin/MaterialSolid"));

    if (material.get() == 0)
        {
            return false;
        }

    material->SetName(name);
    material->SetDiffuse(rgba);

    materialServer->RegisterMaterial(material);
    GetLog()->Debug() << "(RosImporter) defined SolidMaterial " << name << "\n";

    return true;
}

bool RosImporter::ReadAmbientLight(TiXmlElement* element)
{
    RGBA rgba;
    TiXmlElement* colorElem = GetFirstChild(element, RosElements::RE_AMBIENTLIGHTCOLOR);
    if (
        (colorElem == 0) ||
        (! ReadRGBA(colorElem, rgba))
        )
        {
            return false;
        }

    boost::shared_ptr<RenderServer> renderServer = dynamic_pointer_cast<RenderServer>
        (GetCore()->Get("/sys/server/render"));

    if (renderServer.get() == 0)
        {
            GetLog()->Error() << "(RosImporter) ERROR: failed to lookup RenderServer node\n";
            return false;
        }

    renderServer->SetAmbientColor(rgba);
    return true;
}

bool RosImporter::ReadScene(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    RosContextScope scope(this);
    if (parent.get() == 0)
        {
            return false;
        }

    GetLog()->Debug() << "(RosImporter) reading scene node\n";

    ReadDefaultAppearance(element);
    ReadGlobalPhsyParams(element);
    ReadAmbientLight(element);

    // TODO:
    // Background surface="..."
    // GlobalPhysicalParameters gravity="" erp="" cfm="" defaultRollingFrictionCoefficient=""
    // SimulationParameters stepLength="" standardLength="" applyDynamicsForceFactor=""

    return ReadChildElements(parent, element);
}

bool RosImporter::ReadChildElements(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    for (
         TiXmlNode* node = GetFirstChild(element, RosElements::RE_ELEMENTS);
         node != 0;
         node = element->IterateChildren(node)
         )
        {
            if (IgnoreNode(node))
                {
                    continue;
                }

            TiXmlElement* childElement = static_cast<TiXmlElement*>(node);
            if (! ReadElements(parent, childElement))
                {
                    return false;
                }
        }

    return true;
}

bool RosImporter::ReadCompound(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    string name;
    Trans trans;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadTrans(element, trans))
        )
        {
            return false;
        }

    // transform
    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent,trans);
    contextTransform->SetName(name);

    GetLog()->Debug() << "(RosImporter) read compound node " << name << "\n";
    return ReadChildElements(contextTransform, element);
}

bool RosImporter::ReadElements(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    GetLog()->Debug() << "(RosImporter) reading elements node\n";

    for (
         TiXmlNode* node = element->FirstChild();
         node != 0;
         node = element->IterateChildren(node)
         )
        {
            if (IgnoreNode(node))
                {
                    continue;
                }

            TiXmlElement* element = static_cast<TiXmlElement*>(node);
            bool ok = true;

            RosElements::ERosElement type = GetType(element);
            switch (type)
                {
                default:
                    // treat unknown tags like a <element> tag
                    GetLog()->Error() << "(RosImporter::ReadElements) ERROR: skipping unknown element '"
                                      << GetXMLValue(element) << "' "
                                      << GetXMLPath(element) << "\n";
                    ok = ReadElements(parent, element);
                    break;

                case RosElements::RE_ELEMENTS:
                    ok = ReadElements(parent,element);
                    break;

                case RosElements::RE_COMPOUND:
                    ok = ReadCompound(parent,element);
                    break;

                case RosElements::RE_MOVABLE:
                    ok = ReadMovable(parent,element);
                    break;

                case RosElements::RE_BOX:
                    ok = ReadBox(parent,element);
                    break;

                case RosElements::RE_SPHERE:
                    ok = ReadSphere(parent,element);
                    break;

                case RosElements::RE_CYLINDER:
                    ok = ReadCylinder(parent,element);
                    break;

                case RosElements::RE_CAPSULE:
                    ok = ReadCapsule(parent,element);
                    break;

                case RosElements::RE_UNIVERSAL:
                    ok = ReadUniversal(parent,element);
                    break;

                case RosElements::RE_HINGE:
                    ok = ReadHinge(parent,element);
                    break;

                case RosElements::RE_SLIDER:
                    ok = ReadSlider(parent, element);
                    break;

                case RosElements::RE_USE:
                    ok = ReadUse(parent,element);
                    break;

                case RosElements::RE_COMPLEXSHAPE:
                    ok = ReadComplexShape(parent,element);
                    break;
                }

            if (! ok)
                {
                    return false;
                }
        }

    return true;
}

bool RosImporter::ReadMovable(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    RosContextScope scope(this);
    GetContext().movable = true;

    GetLog()->Debug() << "(RosImporter) reading moveable node\n";

    for (
         TiXmlNode* node = element->FirstChild();
         node != 0;
         node = element->IterateChildren(node)
         )
        {
            if (IgnoreNode(node))
                {
                    continue;
                }

            TiXmlElement* element = static_cast<TiXmlElement*>(node);
            if (element == 0)
                {
                    continue;
                }

            bool ok = true;

            RosElements::ERosElement type = GetType(element);
            switch (type)
                {
                default:
                    // treat unknown tags like a <element> tag
                    ok = ReadElements(parent,element);
                    break;

                case RosElements::RE_ELEMENTS:
                    ok = ReadElements(parent,element);
                    break;
                }

            if (! ok)
                {
                    return false;
                }
        }

    return true;
}

bool RosImporter::ReadTrans(TiXmlElement* element, Trans& trans)
{
    trans.matrix.Identity();

    TiXmlElement* transElem = GetFirstChild(element,RosElements::RE_TRANSLATION);
    if (transElem != 0)
        {
            Vector3f vec;
            if (! ReadVector(transElem, vec))
                {
                    return false;
                }

            trans.matrix.Translate(vec);
        }

    TiXmlElement* rotElem = GetFirstChild(element,RosElements::RE_ROTATION);
    if (rotElem != 0)
        {
            Vector3f rot;
            if (! ReadVector(rotElem, rot))
                {
                    return false;
                }

            trans.matrix.RotateX(gDegToRad(rot[0]));
            trans.matrix.RotateY(gDegToRad(rot[1]));
            trans.matrix.RotateZ(gDegToRad(rot[2]));
        }

    return true;
}

boost::shared_ptr<TransformCollider> RosImporter::CreateTransformCollider(boost::shared_ptr<BaseNode> parent, const Trans& trans)
{
    boost::shared_ptr<TransformCollider> transCollider = dynamic_pointer_cast<TransformCollider>
        (GetCore()->New("/oxygen/TransformCollider"));

    parent->AddChildReference(transCollider);

    transCollider->SetRotation(trans.matrix);
    transCollider->SetPosition(trans.matrix.Pos());

    return transCollider;
}

boost::shared_ptr<ContactJointHandler> RosImporter::CreateContactJointHandler()
{
    boost::shared_ptr<ContactJointHandler> handler = dynamic_pointer_cast<ContactJointHandler>
        (GetCore()->New("/oxygen/ContactJointHandler"));

    handler->SetContactSoftERPMode(true);
    handler->SetContactSoftERP(mGlobalERP);
    handler->SetContactSoftCFMMode(true);
    handler->SetContactSoftCFM(mGlobalCFM);

    return handler;
}


bool RosImporter::ReadGlobalPhsyParams(TiXmlElement* element)
{
    // prsetset defaults
    mGlobalERP = 0.2;
    mGlobalCFM = 0.0001;

    /** TODO: figure out how RoSim interprets the gravity value (a
        single value is given, not a vector)
    */
    double gravity = 1.0;

    TiXmlElement* physElem = GetFirstChild(element, RosElements::RE_GLOBALPHYSICALPARAMETERS);
    if (physElem == 0)
        {
            return true;
        }

    ReadAttribute(physElem, RA_GRAVITY, gravity, true);
    ReadAttribute(physElem, RA_ERP, mGlobalERP, true);
    ReadAttribute(physElem, RA_CFM, mGlobalCFM, true);

    return true;
}

bool RosImporter::ReadDefaultAppearance(TiXmlElement* element)
{
    TiXmlElement* appearElem = GetFirstChild(element,RosElements::RE_DEFAULTAPPEARANCE);
    if (appearElem != 0)
        {
            return ReadAttribute(appearElem, RA_REF, mDefaultAppearance.ref);
        }

    // fall back to predefined MaterialServer default material
    mDefaultAppearance.ref = "default";
    return true;
}

bool RosImporter::ReadAppearance(TiXmlElement* element, Appearance& appear)
{
    TiXmlElement* appearElem = GetFirstChild(element,RosElements::RE_APPEARANCE);
    if (appearElem != 0)
        {
            return ReadAttribute(appearElem, RA_REF, appear.ref);
        }

    appear = mDefaultAppearance;
    return true;
}

bool RosImporter::ReadPhysical(TiXmlElement* element, Physical& physical)
{
    physical.valid = false;

    TiXmlElement* physElem = GetFirstChild(element,RosElements::RE_PHYSICALATTRIBUTES);
    if (physElem == 0)
        {
            return true;
        }

    // check for mass
    TiXmlElement* massElem = GetFirstChild(physElem, RosElements::RE_MASS);
    if (
        (massElem != 0) &&
        (! ReadAttribute(massElem, RA_VALUE, physical.mass))
        )
        {
            return false;
        }

    // a mass value is enough for a valid physical body description
    physical.valid = true;

    // check if a collider is present
    int canCollide = 0;
    if (GetXMLAttribute(element, RA_CANCOLLIDE, canCollide))
        {
            physical.canCollide = (canCollide == 1);
        }

    // check for mass center
    TiXmlElement* centerElem = GetFirstChild(physElem,RosElements::RE_CENTEROFMASS);
    if (
        (centerElem != 0) &&
        (! ReadVector(centerElem, physical.massCenter))
        )
        {
            return false;
        }

    // TODO: read friction def

    return true;
}

bool RosImporter::ReadCylinder(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    GetLog()->Debug() << "(RosImporter) cylinder geom unsupported yet. Created a capsule geom\n";
    return ReadCapsule(parent, element);
}

bool RosImporter::ReadCapsule(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    string name;
    double radius;
    double height;
    Trans trans;
    Appearance appear;
    Physical physical;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadAttribute(element, RA_RADIUS, radius)) ||
        (! ReadAttribute(element, RA_HEIGHT, height)) ||
        (! ReadTrans(element, trans)) ||
        (! ReadAppearance(element, appear)) ||
        (! ReadPhysical(element, physical))
        )
        {
            return false;
        }

    // transform
    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent, Trans());
    boost::shared_ptr<Transform> transform = CreateTransform(contextTransform, trans);
    transform->SetName(name);

    // visual
    boost::shared_ptr<Capsule> capsule = dynamic_pointer_cast<Capsule>
        (GetCore()->New("/kerosin/Capsule"));
    transform->AddChildReference(capsule);

    capsule->SetName(S_VISUAL+name);
    capsule->SetParams(radius, height);
    capsule->SetMaterial(appear.ref);

    // physical
    boost::shared_ptr<RigidBody> body = GetContextBody(transform);
    if (body.get() != 0)
        {
            body->SetName(S_BODY+name);
            body->SetCapsuleTotal(physical.mass, radius, height);
            GetContext().AddMass(physical.mass, Trans());
        }

    if (physical.canCollide)
        {
            // geometry
            boost::shared_ptr<CapsuleCollider> collider = dynamic_pointer_cast<CapsuleCollider>
                (GetCore()->New("/oxygen/CapsuleCollider"));

            transform->AddChildReference(collider);
            collider->SetName(S_GEOM+name);
            collider->SetParams(radius, height);

            // collision handler
            boost::shared_ptr<ContactJointHandler> handler = CreateContactJointHandler();
            collider->AddChildReference(handler);
        }

    GetLog()->Debug() << "(RosImporter) created capsule " << name << "\n";
    return ReadChildElements(transform, element);
}

bool RosImporter::ReadSphere(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    string name;
    double radius;
    Trans trans;
    Appearance appear;
    Physical physical;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadAttribute(element, RA_RADIUS, radius)) ||
        (! ReadTrans(element, trans)) ||
        (! ReadAppearance(element, appear)) ||
        (! ReadPhysical(element, physical))
        )
        {
            return false;
        }

    // transform
    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent, Trans());
    boost::shared_ptr<Transform> transform = CreateTransform(contextTransform, trans);
    transform->SetName(name);

    // visual
    boost::shared_ptr<Sphere> sphere = dynamic_pointer_cast<Sphere>
        (GetCore()->New("/kerosin/Sphere"));
    transform->AddChildReference(sphere);

    sphere->SetName(S_VISUAL+name);
    sphere->SetRadius(radius);
    sphere->SetMaterial(appear.ref);

    boost::shared_ptr<RigidBody> body = GetContextBody(transform);
    if (body.get() != 0)
        {
            body->SetName(S_BODY+name);
            body->SetSphereTotal(physical.mass, radius);
            GetContext().AddMass(physical.mass, Trans());
        }

    // geometry
    boost::shared_ptr<SphereCollider> collider = dynamic_pointer_cast<SphereCollider>
        (GetCore()->New("/oxygen/SphereCollider"));

    transform->AddChildReference(collider);
    collider->SetRadius(radius);

    if (physical.canCollide)
        {
            // collision handler
            boost::shared_ptr<ContactJointHandler> handler = CreateContactJointHandler();
            collider->AddChildReference(handler);
        }

    GetLog()->Debug() << "(RosImporter) created sphere " << name << "\n";
    return ReadChildElements(transform, element);
}

bool RosImporter::ReadBox(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    string name;
    double length;
    double width;
    double height;
    Trans trans;
    Appearance appear;
    Physical physical;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadAttribute(element, RA_LENGTH, length)) ||
        (! ReadAttribute(element, RA_WIDTH, width)) ||
        (! ReadAttribute(element, RA_HEIGHT, height)) ||
        (! ReadTrans(element, trans)) ||
        (! ReadAppearance(element, appear)) ||
        (! ReadPhysical(element, physical))
        )
        {
            return false;
        }

    // transform
    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent, Trans());
    boost::shared_ptr<Transform> transform = CreateTransform(contextTransform, trans);
    transform->SetName(name);

    // visual
    boost::shared_ptr<Box> box = dynamic_pointer_cast<Box>
        (GetCore()->New("/kerosin/Box"));
    transform->AddChildReference(box);

    Vector3f boxDim = Vector3f(length, width, height);
    box->SetName(S_VISUAL+name);
    box->SetExtents(boxDim);
    box->SetMaterial(appear.ref);

    boost::shared_ptr<RigidBody> body = GetContextBody(transform);
    if (body.get() != 0)
        {
            body->SetName(S_BODY+name);
            body->SetBoxTotal(physical.mass, boxDim);
            GetContext().AddMass(physical.mass, Trans());
        }

    if (physical.canCollide)
        {
            // geometry
            boost::shared_ptr<BoxCollider> collider = dynamic_pointer_cast<BoxCollider>
                (GetCore()->New("/oxygen/BoxCollider"));

            transform->AddChildReference(collider);
            collider->SetName(S_GEOM+name);
            collider->SetBoxLengths(boxDim);

            // collision handler
            boost::shared_ptr<ContactJointHandler> handler = CreateContactJointHandler();
            collider->AddChildReference(handler);
        }

    GetLog()->Debug() << "(RosImporter) created box " << name << "\n";
    return ReadChildElements(transform, element);
}

bool RosImporter::ReadAnchorPoint(TiXmlElement* element, Vector3f& anchor)
{
    TiXmlElement* anchorElem = GetFirstChild(element,RosElements::RE_ANCHORPOINT);
    if (anchorElem == 0)
        {
            GetLog()->Error() << "(RosImporter) ERROR: missing anchorpoint in "
                              << GetXMLPath(element) << "\n";
            return false;
        }

    return ReadVector(anchorElem, anchor);
}

bool RosImporter::ReadAxis(TiXmlElement* element, RosElements::ERosElement type, JointAxis& axis)
{
    TiXmlElement* axisElem = GetFirstChild(element,type);
    if (axisElem == 0)
        {
            GetLog()->Error() << "(RosImporter) ERROR: missing axis in "
                              << GetXMLPath(element) << "\n";
            return false;
        }

    if (! ReadVector(axisElem, axis.dir))
        {
            return false;
        }

    TiXmlElement* deflectElem = GetFirstChild(axisElem, RosElements::RE_DEFLECTION);
    if (deflectElem != 0)
        {
            double loStop;
            double hiStop;

            if (
                (! GetXMLAttribute(deflectElem, RA_MIN, loStop)) ||
                (! GetXMLAttribute(deflectElem, RA_MAX, hiStop))
                )
                {
                    GetLog()->Error() << "(RosImporter) ERROR: invalid axis deflection in "
                                      << GetXMLPath(element) << "\n";
                    return false;
                }

            axis.setDeflection = true;
            axis.loStop = gDegToRad(loStop);
            axis.hiStop = gDegToRad(hiStop);
        }

    return true;
}

boost::shared_ptr<RigidBody> RosImporter::GetJointParentBody()
{
    if (mStack.size() < 2)
        {
            GetLog()->Debug() << "RosImporter::GetJointParentBody cannot get joint parent body with stack size of "
                              << mStack.size() << "\n";
            return boost::shared_ptr<RigidBody>();
        }

    TRosStack::reverse_iterator iter = mStack.rbegin();
    ++iter;

    for (;
         iter != mStack.rend();
         ++iter
         )
        {
            RosContext& context = (*iter);
            boost::shared_ptr<RigidBody> body = context.body;

            if (body.get() != 0)
                {
                    GetLog()->Debug() << "RosImporter::GetJointParentBody found " << body->GetFullPath() << "\n";
                    return body;
                }
        }


    GetLog()->Debug() << "RosImporter::GetJointParentBody not found\n";
    return boost::shared_ptr<RigidBody>();
}

boost::shared_ptr<RigidBody> RosImporter::GetJointChildBody(boost::shared_ptr<BaseNode> parent)
{
    if (parent.get() == 0)
        {
            return boost::shared_ptr<RigidBody>();
        }

    for (Leaf::TLeafList::iterator iter = parent->begin();iter != parent->end();++iter)
        {
            boost::shared_ptr<RigidBody> body = dynamic_pointer_cast<RigidBody>(*iter);
            if (body.get() == 0)
                {
                    continue;
                }

            GetLog()->Debug() << "RosImporter::GetJointParentBody found " << body->GetFullPath() << "\n";
            return body;
        }

    for (Leaf::TLeafList::iterator iter = parent->begin();iter != parent->end();++iter)
        {
            boost::shared_ptr<BaseNode> node = dynamic_pointer_cast<BaseNode>(*iter);
            if (node.get() == 0)
                {
                    continue;
                }

            boost::shared_ptr<RigidBody> body = GetJointChildBody(node);
            if (body.get() != 0)
                {
                    return body;
                }
        }

    return boost::shared_ptr<RigidBody>();
}

bool RosImporter::ReadUniversal(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    RosContextScope scope(this);
    GetContext().movable = true;
    RosJointScope jointScope(this);

    string name;
    Vector3f anchor;
    JointAxis axis1;
    JointAxis axis2;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadAnchorPoint(element, anchor)) ||
        (! ReadAxis(element, RosElements::RE_AXIS1, axis1)) ||
        (! ReadAxis(element, RosElements::RE_AXIS2, axis2))
        )
        {
            return false;
        }

    // transform
    Trans anchorTrans;
    anchorTrans.matrix.Translate(anchor);
    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent, anchorTrans);

    // joint
    boost::shared_ptr<UniversalJoint> universal = dynamic_pointer_cast<UniversalJoint>
        (GetCore()->New("/oxygen/UniversalJoint"));
    contextTransform->AddChildReference(universal);

    if (! ReadChildElements(universal, element))
        {
            return false;
        }

    boost::shared_ptr<RigidBody> body1 = GetJointParentBody();
    boost::shared_ptr<RigidBody> body2 = GetJointContext().body;

    if (
        (body1.get() == 0) ||
        (body2.get() == 0)
        )
        {
            GetLog()->Error() << "(RosImporter::ReadUniversal) found no bodies to attach hinge to in "
                              << GetXMLPath(element) << " named " << name << "\n";
            return false;
        }

    universal->SetName(name);
    Attach(universal, body1, body2, axis1, axis2);
    return true;
}

bool RosImporter::ReadHinge(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    RosContextScope scope(this);
    GetContext().movable = true;
    RosJointScope jointScope(this);

    string name;
    Vector3f anchor;
    JointAxis axis;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadAnchorPoint(element, anchor)) ||
        (! ReadAxis(element, RosElements::RE_AXIS, axis))
        )
        {
            return false;
        }

    // transform
    Trans anchorTrans;
    anchorTrans.matrix.Translate(anchor);
    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent, anchorTrans);

    // joint
    boost::shared_ptr<HingeJoint> hinge = dynamic_pointer_cast<HingeJoint>
        (GetCore()->New("/oxygen/HingeJoint"));
    contextTransform->AddChildReference(hinge);

    if (! ReadChildElements(hinge, element))
        {
            return false;
        }

    boost::shared_ptr<RigidBody> body1 = GetJointParentBody();
    boost::shared_ptr<RigidBody> body2 = GetJointContext().body;

    if (
        (body1.get() == 0) ||
        (body2.get() == 0)
        )
        {
            GetLog()->Error() << "(RosImporter::ReadHinge) found no bodies to attach hinge to in "
                              << GetXMLPath(element) << " named " << name << "\n";
            return false;
        }

    hinge->SetName(name);
    Attach(hinge, body1, body2, axis);

    GetLog()->Debug() << "(RosImporter) created hinge joint " << name << "\n";
    return true;
}

bool RosImporter::ReadSlider(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    RosContextScope scope(this);
    GetContext().movable = true;
    RosJointScope jointScope(this);

    string name;
    JointAxis axis;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadAxis(element, RosElements::RE_AXIS, axis))
        )
        {
            return false;
        }

    // joint
    boost::shared_ptr<SliderJoint> slider = dynamic_pointer_cast<SliderJoint>
        (GetCore()->New("/oxygen/SliderJoint"));
    parent->AddChildReference(slider);

    if (! ReadChildElements(slider, element))
        {
            return false;
        }

    boost::shared_ptr<RigidBody> body1 = GetJointParentBody();
    boost::shared_ptr<RigidBody> body2 = GetJointContext().body;

    if (
        (body1.get() == 0) &&
        (body2.get() == 0)
        )
        {
            GetLog()->Error() << "(RosImporter::ReadHinge) found no bodies to attach hinge to in "
                              << GetXMLPath(element) << " named " << name << "\n";
            return false;
        }

    slider->SetName(name);
    Attach(slider, body1, body2, axis);

    GetLog()->Debug() << "(RosImporter) created hinge joint " << name << "\n";
    return true;
}

bool RosImporter::ReadMacro(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    string name;
    if (! ReadAttribute(element, RA_NAME, name))
        {
            return false;
        }

    boost::shared_ptr<TiXmlElement> macro(new TiXmlElement(*element));
    mMacroMap[name] = macro;

    GetLog()->Debug() << "(RosImporter) defined macro " << name << "\n";
    return true;
}

bool RosImporter::ReadUse(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    string name;
    string instance;
    Trans trans;

    if (
        (! ReadAttribute(element, RA_MACRONAME, name)) ||
        (! ReadAttribute(element, RA_INSTANCENAME, instance, true)) ||
        (! ReadTrans(element, trans))
        )
        {
            return false;
        }

    TMacroMap::const_iterator iter = mMacroMap.find(name);
    if (iter == mMacroMap.end())
        {
            GetLog()->Error() << "(RosImporter) use of undefined macro " << name << " in "
                              << GetXMLPath(element) << "\n";
            return false;
        }

    // todo: apply Translation/Rotation
    // todo: <SubstituteSurfaces>
    // todo: <SubstituteAllSurfaces>

    if (instance.empty())
        {
            instance = name;
        }

    GetLog()->Debug() << "(RosImporter) START instancing macro " << name
                      << " as instance " << instance << "\n";

    boost::shared_ptr<TiXmlElement> tree = (*iter).second;
    bool ok = ReadElements(parent, tree.get());

    GetLog()->Debug() << "(RosImporter) END instancing macro " << name << "\n";
    return ok;
}

bool RosImporter::ReadVertexList(TiXmlElement* element)
{
    string listName;
    if (! ReadAttribute(element, RA_NAME, listName))
        {
            return false;
        }

    mVertexListMap[listName] = TVertexList();

    // work with a reference in to the map to avoid an expensive copy
    TVertexList& vertices = mVertexListMap[listName];

    for (
         TiXmlNode* node = GetFirstChild(element, RosElements::RE_VERTEX);
         node != 0;
         node = element->IterateChildren(node)
         )
        {
            if (IgnoreNode(node))
                {
                    continue;
                }

            TiXmlElement* element = static_cast<TiXmlElement*>(node);

            RosElements::ERosElement type = GetType(element);
            switch (type)
                {
                default:
                    GetLog()->Error() << "(RosImporter::ReadVertices) ERROR: skipping unknown element "
                                      << GetXMLPath(element) << "\n";
                    break;

                case RosElements::RE_VERTEX:
                    {
                        TVertex vertex;
                        string name;

                        if (
                            (! ReadAttribute(element, RA_NAME, name)) ||
                            (! ReadVector(element, vertex.vec))
                            )
                            {
                                return false;
                            }

                        vertices.AddVertex(name, vertex);
                        break;
                    }
                }
        }

    GetLog()->Debug() << "(RosImporter) read vertex list " << listName << "\n";
    return true;
}

bool RosImporter::ReadComplexShape(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    boost::shared_ptr<GeometryServer> geometryServer = dynamic_pointer_cast<GeometryServer>
        (GetCore()->Get("/sys/server/geometry"));

    if (geometryServer.get() == 0)
        {
            GetLog()->Error() << "(RosImporter) ERROR: failed to lookup GeometryServer node\n";
            return false;
        }

    string name;
    Trans trans;
    Appearance appear;
    Physical physical;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadTrans(element, trans)) ||
        (! ReadAppearance(element, appear)) ||
        (! ReadPhysical(element, physical))
        )
        {
            return false;
        }

    // look for an inlined vertex list and parse it
    TiXmlElement* vertListElem =
        GetFirstChild(element, RosElements::RE_VERTEXLIST);

    if (
        (vertListElem != 0) &&
        (! ReadVertexList(vertListElem))
        )
        {
            return false;
        }

    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent, Trans());
    boost::shared_ptr<Transform> transform = CreateTransform(contextTransform, trans);
    transform->SetName(name);

    if (! ReadPhysicalRep(parent,element))
        {
            return false;
        }

    boost::shared_ptr<TriMesh> mesh(new TriMesh);
    mesh->SetName(name);

    if (! ReadGraphicalRep(element, mesh, appear))
        {
            return false;
        }

    geometryServer->RegisterMesh(mesh);
    boost::shared_ptr<StaticMesh> staticMesh = dynamic_pointer_cast<StaticMesh>
        (GetCore()->New("/kerosin/StaticMesh"));

    transform->AddChildReference(staticMesh);
    staticMesh->Load(name);

    GetLog()->Debug() << "(RosImporter) read complex shape " << name << "\n";
    return ReadChildElements(transform, element);
}

bool RosImporter::ReadGraphicalRep(TiXmlElement* element, boost::shared_ptr<TriMesh> mesh, const Appearance& appear)
{
    TiXmlElement* graphRepElem = GetFirstChild(element, RosElements::RE_GRAPHICALREPRESENTATION);
    if (graphRepElem == 0)
        {
            string name = S_UNNAMED;
            ReadAttribute(element, RA_NAME, name, true);

            GetLog()->Error() << "(RosImporter) ERROR: missing graphical representation in "
                              << GetXMLPath(element) << " name " << name << " \n";

            return false;
        }

    string vertexListName;
    if (! ReadAttribute(graphRepElem, RA_VERTEXLIST, vertexListName))
        {
            return false;
        }

    TVertexListMap::iterator iter = mVertexListMap.find(vertexListName);
    if (iter == mVertexListMap.end())
        {
            string name;
            ReadAttribute(element, RA_NAME, name, true);

            GetLog()->Error() << "(RosImporter) ERROR: undefined vertex list " << vertexListName << " in "
                              << GetXMLPath(element) << " name " << name << " \n";

            return false;
        }

    TComplexGeomList geomList;
    if (! ReadComplexElements(graphRepElem, geomList))
        {
            return false;
        }

    TVertexList& vertexList = (*iter).second;
    BuildTriMesh(mesh, vertexList, geomList, appear);

    GetLog()->Debug() << "(RosImporter) read graphical representation\n";

    return true;
}

bool RosImporter::ReadComplexElements(TiXmlElement* element, TComplexGeomList& geomList)
{
    for (
         TiXmlNode* node = element->FirstChild();
         node != 0;
         node = element->IterateChildren(node)
         )
        {
            if (IgnoreNode(node))
                {
                    continue;
                }

            TiXmlElement* element = static_cast<TiXmlElement*>(node);

            RosElements::ERosElement type = GetType(element);
            switch (type)
                {
                default:
                    GetLog()->Error() << "(RosImporter::ReadComplexElements) ERROR: skipping unknown element "
                                      << GetXMLPath(element) << "\n";
                    break;

                case RosElements::RE_POLYGON:
                    {
                        ComplexGeom geom(CG_Polygon);
                        if (! ReadComplexGeom(element, geom))
                            {
                                return false;
                            }

                        geomList.push_back(geom);
                        break;
                    }

                case RosElements::RE_TRIANGLESTRIP:
                    {
                        ComplexGeom geom(CG_TriangleStrip);
                        if (! ReadComplexGeom(element, geom))
                            {
                                return false;
                            }

                        geomList.push_back(geom);
                        break;
                    }
                }
        }

    return true;
}

bool RosImporter::ReadComplexGeom(TiXmlElement* element, ComplexGeom& geom)
{
    for (
         TiXmlNode* node = GetFirstChild(element, RosElements::RE_VERTEX);
         node != 0;
         node = element->IterateChildren(node)
         )
        {
            TiXmlElement* element = static_cast<TiXmlElement*>(node);

            RosElements::ERosElement type = GetType(element);
            switch (type)
                {
                default:
                    GetLog()->Error() << "(RosImporter::ReadComplexGeom) ERROR: skipping unknown element "
                                      << GetXMLPath(element) << "\n";
                    break;

                case RosElements::RE_VERTEX:
                    {
                        string ref;
                        if (! ReadAttribute(element, RA_REF, ref))
                            {
                                return false;
                            }

                        geom.vertices.push_back(ref);

                        break;
                    }
                }
        }

    //    std::reverse(geom.vertices.begin(), geom.vertices.end());
    return true;
}

void RosImporter::BuildTriMesh(boost::shared_ptr<TriMesh> mesh, TVertexList& vertexList,
                               const TComplexGeomList& geomList, const Appearance& appear)
{
    GetLog()->Debug() << "(RosImporter) building trimesh for " << mesh->GetName() << "\n";
    mesh->SetPos(vertexList.GetPos(), vertexList.GetPosCount());

    boost::shared_ptr<IndexBuffer> ibuffer(new IndexBuffer);
    IndexBuffer& ibufferRef = (*ibuffer);

    // build triangles
    for (
         TComplexGeomList::const_iterator iter = geomList.begin();
         iter != geomList.end();
         ++iter
         )
        {
            const ComplexGeom& geom = (*iter);

            switch (geom.type)
                {
                default:
                    continue;

                case CG_Polygon:
                    BuildPolygon(ibufferRef, vertexList, geom);
                    break;
                }
        }

    mesh->AddFace(ibuffer,appear.ref);
}

void RosImporter::BuildPolygon(IndexBuffer& ibuffer, TVertexList& vertexList, const ComplexGeom& geom)
{
    int nVerts = static_cast<int>(geom.vertices.size());
    int j = (nVerts - 2);

    for (int i = 0;i<j;++i)
        {
            ibuffer.Cache(vertexList.GetIndex(geom.vertices[0]));
            ibuffer.Cache(vertexList.GetIndex(geom.vertices[i+1]));
            ibuffer.Cache(vertexList.GetIndex(geom.vertices[i+2]));
        }
}

bool RosImporter::ReadPhysicalRep(boost::shared_ptr<BaseNode> parent, TiXmlElement* element)
{
    string name = S_UNNAMED;
    ReadAttribute(element, RA_NAME, name, true);

    TiXmlElement* physicalRep
        = GetFirstChild(element, RosElements::RE_PHYSICALREPRESENTATION);

    if (physicalRep == 0)
        {
            GetLog()->Error() << "(RosImporter) ERROR: missing physical representation in "
                              << GetXMLPath(element) << " name " << name << " \n";

            return false;
        }

    for (
         TiXmlNode* node = physicalRep->FirstChild();
         node != 0;
         node = physicalRep->IterateChildren(node)
         )
        {
            if (IgnoreNode(node))
                {
                    continue;
                }

            TiXmlElement* element = static_cast<TiXmlElement*>(node);

            RosElements::ERosElement type = GetType(element);
            switch (type)
                {
                default:
                    GetLog()->Error() << "(RosImporter::ReadPhysicalRep) ERROR: skipping unknown element "
                                      << GetXMLPath(element) << "\n";
                    break;

                case RosElements::RE_SIMPLEBOX:
                    if (! ReadSimpleBox(parent, element))
                        {
                            return false;
                        }
                    break;

                case RosElements::RE_SIMPLESPHERE:
                    if (!  ReadSimpleSphere(parent, element))
                        {
                            return false;
                        }
                    break;

                case RosElements::RE_SIMPLECYLINDER:
                    //! simulate cylinder with a capsule
                case RosElements::RE_SIMPLECAPSULE:
                    if (! ReadSimpleCapsule(parent, element))
                        {
                            return false;
                        }
                    break;
                }
        }

    GetLog()->Debug() << "(RosImporter) read physical representation\n";
    return true;
}

bool RosImporter::ReadSimpleBox(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element)
{
    string name;
    double length;
    double width;
    double height;
    Trans trans;
    Physical physical;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadAttribute(element, RA_LENGTH, length)) ||
        (! ReadAttribute(element, RA_WIDTH, width)) ||
        (! ReadAttribute(element, RA_HEIGHT, height)) ||
        (! ReadTrans(element, trans)) ||
        (! ReadPhysical(element, physical))
        )
        {
            return false;
        }

    // transform
    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent, trans);
    Vector3f boxDim = Vector3f(length, width, height);

    boost::shared_ptr<RigidBody> body = GetContextBody(contextTransform);
    if (body.get() != 0)
        {
            body->AddBoxTotal(physical.mass, boxDim, trans.matrix);
            GetContext().AddMass(physical.mass, trans);
        }

    if (physical.canCollide)
        {
            // geometry
            boost::shared_ptr<TransformCollider> transCollider
                = CreateTransformCollider(contextTransform,trans);
            transCollider->SetName(S_GEOMTRANS+name);

            boost::shared_ptr<BoxCollider> collider = dynamic_pointer_cast<BoxCollider>
                (GetCore()->New("/oxygen/BoxCollider"));

            transCollider->AddChildReference(collider);
            collider->SetName(S_GEOM+name);
            collider->SetBoxLengths(boxDim);

            // collision handler
            boost::shared_ptr<ContactJointHandler> handler = CreateContactJointHandler();
            collider->AddChildReference(handler);
        }

    GetLog()->Debug() << "(RosImporter) created simple box " << name << "\n";
    return true;
}

bool RosImporter::ReadSimpleSphere(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element)
{
    string name;
    double radius;
    Trans trans;
    Physical physical;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadAttribute(element, RA_RADIUS, radius)) ||
        (! ReadTrans(element, trans)) ||
        (! ReadPhysical(element, physical))
        )
        {
            return false;
        }

    // transform
    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent, trans);

    boost::shared_ptr<RigidBody> body = GetContextBody(contextTransform);
    if (body.get() != 0)
        {
            body->AddSphereTotal(physical.mass, radius, trans.matrix);
            GetContext().AddMass(physical.mass, trans);
        }

    if (physical.canCollide)
        {
            // geometry
            boost::shared_ptr<TransformCollider> transCollider
                = CreateTransformCollider(body,trans);
            transCollider->SetName(S_GEOMTRANS+name);

            boost::shared_ptr<SphereCollider> collider = dynamic_pointer_cast<SphereCollider>
                (GetCore()->New("/oxygen/SphereCollider"));

            transCollider->AddChildReference(collider);
            collider->SetRadius(radius);

            // collision handler
            boost::shared_ptr<ContactJointHandler> handler = CreateContactJointHandler();
            collider->AddChildReference(handler);
        }

    GetLog()->Debug() << "(RosImporter) created simple sphere " << name << "\n";
    return true;
}

bool RosImporter::ReadSimpleCapsule(boost::shared_ptr<oxygen::BaseNode> parent, TiXmlElement* element)
{
    string name;
    double radius;
    double height;
    Trans trans;
    Physical physical;

    if (
        (! ReadAttribute(element, RA_NAME, name, true)) ||
        (! ReadAttribute(element, RA_RADIUS, radius)) ||
        (! ReadAttribute(element, RA_HEIGHT, height)) ||
        (! ReadTrans(element, trans)) ||
        (! ReadPhysical(element, physical))
        )
        {
            return false;
        }

    // transform
    boost::shared_ptr<Transform> contextTransform = GetContextTransform(parent, trans);

    boost::shared_ptr<RigidBody> body = GetContextBody(contextTransform);
    if (body.get() != 0)
        {
            body->AddCapsuleTotal(physical.mass, radius, height, trans.matrix);
            GetContext().AddMass(physical.mass, trans);
        }

    if (physical.canCollide)
        {
            // geometry
            boost::shared_ptr<TransformCollider> transCollider
                = CreateTransformCollider(body,trans);
            transCollider->SetName(S_GEOMTRANS+name);

            boost::shared_ptr<CapsuleCollider> collider = dynamic_pointer_cast<CapsuleCollider>
                (GetCore()->New("/oxygen/CapsuleCollider"));

            transCollider->AddChildReference(collider);
            collider->SetName(S_GEOM+name);
            collider->SetParams(radius, height);

            // collision handler
            boost::shared_ptr<ContactJointHandler> handler = CreateContactJointHandler();
            collider->AddChildReference(handler);
        }

    GetLog()->Debug() << "(RosImporter) created simple capsule " << name << "\n";
    return true;
}

void RosImporter::Attach(boost::shared_ptr<Joint> joint, boost::shared_ptr<RigidBody> body1, boost::shared_ptr<RigidBody> body2,
                         const JointAxis& axis1, const JointAxis& axis2)
{
    if (joint.get() == 0)
        {
            assert(false);
            return;
        }

    JointAttach ja;
    ja.joint = joint;
    ja.body1 = body1;
    ja.body2 = body2;
    ja.axis1 = axis1;
    ja.axis2 = axis2;

    AttachJoint(ja);
}

void RosImporter::AttachJoint(const JointAttach& ja)
{
    ja.joint->Attach(ja.body1, ja.body2);

    boost::shared_ptr<HingeJoint> hinge = dynamic_pointer_cast<HingeJoint>(ja.joint);
    if (hinge.get() != 0)
        {
            hinge->SetAxis(ja.axis1.dir);
            hinge->SetAnchor(Vector3f(0.0,0.0,0.0));

            if (ja.axis1.setDeflection)
                {
                    //replaced ODE-specific contants with their numeric values to make this engine-unspecific
                    hinge->SetParameter(0 /*formerly dParamLoStop*/, ja.axis1.loStop);
                    hinge->SetParameter(1 /*formerly dParamHiStop*/, ja.axis1.hiStop);
                    hinge->SetParameter(0 /*formerly dParamLoStop*/, ja.axis1.loStop);
                }

            return;
        }

    boost::shared_ptr<UniversalJoint> universal = dynamic_pointer_cast<UniversalJoint>(ja.joint);
    if (universal.get() != 0)
        {
            universal->SetAxis1(ja.axis1.dir);
            universal->SetAxis2(ja.axis2.dir);
            universal->SetAnchor(Vector3f(0.0,0.0,0.0));

            if (ja.axis1.setDeflection)
                {
                    //replaced ODE-specific contants with their numeric values to make this engine-unspecific
                    universal->SetParameter(0 /*formerly dParamLoStop*/, ja.axis1.loStop);
                    universal->SetParameter(1 /*formerly dParamHiStop*/, ja.axis1.hiStop);
                    universal->SetParameter(0 /*formerly dParamLoStop*/, ja.axis1.loStop);
                }

            if (ja.axis2.setDeflection)
                {
                    //replaced ODE-specific contants with their numeric values to make this engine-unspecific
                    universal->SetParameter(256 /*formerly dParamLoStop2*/, ja.axis2.loStop);
                    universal->SetParameter(257 /*formerly dParamHiStop2*/, ja.axis2.hiStop);
                    universal->SetParameter(256 /*formerly dParamLoStop2*/, ja.axis2.loStop);
                }

            return;
        }

    boost::shared_ptr<SliderJoint> slider = dynamic_pointer_cast<SliderJoint>(ja.joint);
    if (slider.get() != 0)
        {
            // slider axis is set via parent transform node
            // slider->SetAxis(ja.axis1);
            return;
        }

    assert(false);
}
