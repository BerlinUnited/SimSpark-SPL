/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: property.cpp,v 1.1 2008/02/24 16:20:23 rollmark Exp $

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
#include "property.h"
#include "main.h"

//! wxWidgets and zeitgeist both use a 'DECLARE_CLASS' macro
#undef DECLARE_CLASS

#include "simspark.h"
#include <salt/vector.h>
#include <salt/bounds.h>
#include <salt/matrix.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/class.h>
#include <zeitgeist/core.h>
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/sceneserver/scenedict.h>
#include <oxygen/physicsserver/body.h>
#include <oxygen/physicsserver/joint.h>
#include <oxygen/physicsserver/hingejoint.h>
#include <oxygen/physicsserver/universaljoint.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/contactjointhandler.h>
#include <oxygen/physicsserver/dragcontroller.h>
#include <kerosin/materialserver/materialsolid.h>

using namespace std;
using namespace boost;
using namespace salt;
using namespace zeitgeist;
using namespace oxygen;
using namespace kerosin;

inline wxString FormatVector3(const Vector3f& vec)
{
    return wxString::Format(wxT("(%.2f, %.2f, %.2f)"),vec[0],vec[1],vec[2]);
}

inline wxString FormatAABB3(const AABB3& bound)
{
    return FormatVector3(bound.minVec) + wxT(" ") + FormatVector3(bound.maxVec);
}

inline wxString FormatMatrix(const Matrix& m)
{
    wxString str = wxString::Format(wxT("(%.2f"),m.m[0]);
    for (int i=1;i<16;++i)
        {
            str += wxString::Format(wxT(", %.2f"),m.m[i]);
        }
    str += wxT(")");
    return str;
}

inline wxString FormatFloat(const float f)
{
    return wxString::Format(wxT("%.2f"),f);
}

inline wxString FormatInt(const int i)
{
    return wxString::Format(wxT("%d"),i);
}

inline wxString FormatBool(const bool b)
{
    return b ? wxT("true") : wxT("false");
}

inline wxString FormatRGBA(const RGBA& col)
{
    return wxString::Format(wxT("(%.2f, %.2f, %.2f, %.2f)"),col.r(),col.g(),col.b(),col.a());
}

#define FORMAT_FLAG(_i, _flag)\
    wxString((_i & _flag) ? wxT(#_flag" ") : wxT(""))

Property::Property()
{
    Init();
}

void Property::Init()
{
    mClassMap.clear();

    mClassMap[wxT("/classes/zeitgeist/Leaf")] = CL_LEAF;
    mClassMap[wxT("ClassClass")] = CL_CLASS;
    mClassMap[wxT("/classes/oxygen/BaseNode")] = CL_BASENODE;
    mClassMap[wxT("/classes/oxygen/Transform")] = CL_TRANSFORM;
    mClassMap[wxT("/classes/oxygen/Body")] = CL_BODY;
    mClassMap[wxT("/classes/oxygen/Joint")] = CL_JOINT;
    mClassMap[wxT("/classes/oxygen/HingeJoint")] = CL_HINGEJOINT;
    mClassMap[wxT("/classes/oxygen/UniversalJoint")] = CL_UNIVERSALJOINT;
    mClassMap[wxT("/classes/oxygen/World")] = CL_WORLD;
    mClassMap[wxT("/classes/oxygen/CollisionHandler")] = CL_COLLISIONHANDLER;
    mClassMap[wxT("/classes/oxygen/ContactJointHandler")] = CL_CONTACTJOINTHANDLER;
    mClassMap[wxT("/classes/oxygen/DragController")] = CL_DRAGCONTROLLER;
    mClassMap[wxT("/classes/kerosin/MaterialSolid")] = CL_MATERIALSOLID;
}

void Property::GenLeafEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    entries.push_back(Entry(wxT("GetFullPath"), wxString(leaf->GetFullPath().c_str(), wxConvUTF8)));

    const SceneDict::FileRef* ref
        = SceneDict::GetInstance().Lookup(leaf);

    if (ref == 0)
        {
            return;
        }

    wxString strRef = wxString(ref->fname.c_str(), wxConvUTF8)
        + wxString::Format(wxT(":%d"),ref->line);
    entries.push_back(Entry(wxT("rsg file"), strRef));
}

void Property::GenBaseNodeEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    shared_ptr<BaseNode> baseNode = shared_static_cast<BaseNode>(leaf);
    entries.push_back(Entry(wxT("GetWorldBoundingBox"), FormatAABB3(baseNode->GetWorldBoundingBox())));
}

void Property::GenClassEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const Class& cl = *shared_static_cast<Class>(leaf);

    // originating bundle
    shared_ptr<salt::SharedLibrary> bundle = cl.GetBundle();
    entries.push_back(
                      Entry(
                            wxT("GetBundle"),
                            (bundle.get() != 0) ? wxString(bundle->GetName().c_str(), wxConvUTF8) : wxT("<None>")
                            )
                      );

    // base classes
    const Class::TStringList& baseCl = cl.GetBaseClasses();

    for (
         Class::TStringList::const_iterator iter = baseCl.begin();
         iter != baseCl.end();
         ++iter
         )
        {
            entries.push_back(Entry(wxT("GetBaseClasses"), wxString((*iter).c_str(), wxConvUTF8)));
        }

    // supported functions
    const Class::TCommandMap& cmds = cl.GetCommandMap();

    for (
         Class::TCommandMap::const_iterator iter = cmds.begin();
         iter != cmds.end();
         ++iter
         )
        {
            entries.push_back(Entry(wxT("GetCommandMap"), wxString((*iter).first.c_str(), wxConvUTF8)));
        }
}

void Property::GenTransformEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const Transform& trans = *shared_static_cast<Transform>(leaf);
    entries.push_back(Entry(wxT("GetChangedMark"), FormatInt(trans.GetChangedMark())));
    entries.push_back(Entry(wxT("GetLocalTransform"), FormatMatrix(trans.GetLocalTransform())));
    entries.push_back(Entry(wxT("GetWorldTransform"), FormatMatrix(trans.GetWorldTransform())));
}

void Property::GenBodyEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const Body& body = *shared_static_cast<Body>(leaf);
    entries.push_back(Entry(wxT("GetMass"),FormatFloat(body.GetMass())));
    entries.push_back(Entry(wxT("GetVelocity"),FormatVector3(body.GetVelocity())));
    entries.push_back(Entry(wxT("GetAngularVelocity"),FormatVector3(body.GetAngularVelocity())));
    entries.push_back(Entry(wxT("GetPosition"),FormatVector3(body.GetPosition())));
}

void Property::GenSolidMaterialProperty(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const MaterialSolid& mat = *shared_static_cast<MaterialSolid>(leaf);
    entries.push_back(Entry(wxT("GetAmbient"),FormatRGBA(mat.GetAmbient())));
    entries.push_back(Entry(wxT("GetDiffuse"),FormatRGBA(mat.GetDiffuse())));
    entries.push_back(Entry(wxT("GetSpecular"),FormatRGBA(mat.GetSpecular())));
}

void Property::GenHingeEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const HingeJoint& hinge = *shared_static_cast<HingeJoint>(leaf);
    entries.push_back(Entry(wxT("GetAngle"),FormatFloat(hinge.GetAngle())));
    entries.push_back(Entry(wxT("GetAngleRate"),FormatFloat(hinge.GetAngleRate())));
}

void Property::GenUniversalEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const UniversalJoint& universal = *shared_static_cast<UniversalJoint>(leaf);
    entries.push_back(Entry(wxT("GetAngle(0)"),FormatFloat(universal.GetAngle(Joint::AI_FIRST))));
    entries.push_back(Entry(wxT("GetAngle(1)"),FormatFloat(universal.GetAngle(Joint::AI_SECOND))));
    entries.push_back(Entry(wxT("GetAngleRate(0)"),FormatFloat(universal.GetAngleRate(Joint::AI_FIRST))));
    entries.push_back(Entry(wxT("GetAngleRate(1)"),FormatFloat(universal.GetAngleRate(Joint::AI_SECOND))));
}

void Property::GenJointEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const Joint& joint = *shared_static_cast<Joint>(leaf);
    entries.push_back(Entry(wxT("FeedBackEnabled"),FormatBool(joint.FeedBackEnabled())));

    for (int i=0;i<2;++i)
        {
            wxString strIdx = wxT("(") + FormatInt(i) + wxT(")");
            const Joint::EBodyIndex idx = static_cast<Joint::EBodyIndex>(i);

            entries.push_back(Entry(wxT("GetFeedbackForce")+strIdx,FormatVector3(joint.GetFeedbackForce(idx))));
            entries.push_back(Entry(wxT("GetFeedbackTorque")+strIdx,FormatVector3(joint.GetFeedbackTorque(idx))));
        }

    for (int i=0;i<3;++i)
        {
            wxString strIdx = wxT("(") + FormatInt(i) + wxT(")");
            const Joint::EAxisIndex idx = static_cast<Joint::EAxisIndex>(i);

            entries.push_back(Entry(wxT("GetBounce")+strIdx,FormatFloat(joint.GetBounce(idx))));
            entries.push_back(Entry(wxT("GetLowStopDeg")+strIdx,FormatFloat(joint.GetLowStopDeg(idx))));
            entries.push_back(Entry(wxT("GetHighStopDeg")+strIdx,FormatFloat(joint.GetHighStopDeg(idx))));
            entries.push_back(Entry(wxT("GetLowStopPos")+strIdx,FormatFloat(joint.GetLowStopPos(idx))));
            entries.push_back(Entry(wxT("GetHighStopPos")+strIdx,FormatFloat(joint.GetHighStopPos(idx))));
            entries.push_back(Entry(wxT("GetCFM")+strIdx,FormatFloat(joint.GetCFM(idx))));
            entries.push_back(Entry(wxT("GetStopCFM")+strIdx,FormatFloat(joint.GetStopCFM(idx))));
            entries.push_back(Entry(wxT("GetSuspensionCFM")+strIdx,FormatFloat(joint.GetSuspensionCFM(idx))));
            entries.push_back(Entry(wxT("GetStopERP")+strIdx,FormatFloat(joint.GetStopERP(idx))));
            entries.push_back(Entry(wxT("GetSuspensionERP")+strIdx,FormatFloat(joint.GetSuspensionERP(idx))));
            entries.push_back(Entry(wxT("GetLinearMotorVelocity")+strIdx,FormatFloat(joint.GetLinearMotorVelocity(idx))));
            entries.push_back(Entry(wxT("GetAngularMotorVelocity")+strIdx,FormatFloat(joint.GetAngularMotorVelocity(idx))));
            entries.push_back(Entry(wxT("GetMaxMotorForce")+strIdx,FormatFloat(joint.GetMaxMotorForce(idx))));
        }
}

void Property::GenWorldEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const World& world = *shared_static_cast<World>(leaf);
    entries.push_back(Entry(wxT("GetGravity"),FormatVector3(world.GetGravity())));
    entries.push_back(Entry(wxT("GetERP"),FormatFloat(world.GetERP())));
    entries.push_back(Entry(wxT("GetCFM"),FormatFloat(world.GetCFM())));
}

void Property::GenCollisionHandlerEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    shared_ptr<CollisionHandler> ch = shared_static_cast<CollisionHandler>(leaf);
    entries.push_back(Entry(wxT("IsSymmetricHandler"),FormatBool(ch->IsSymmetricHandler())));
}

void Property::GenContactJointEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const ContactJointHandler& cjh = *shared_static_cast<ContactJointHandler>(leaf);

    int mode = cjh.GetContactMode();
    wxString strMode =
        FORMAT_FLAG(mode, dContactMu2) +
        FORMAT_FLAG(mode, dContactFDir1) +
        FORMAT_FLAG(mode, dContactBounce) +
        FORMAT_FLAG(mode, dContactSoftERP) +
        FORMAT_FLAG(mode, dContactSoftCFM) +
        FORMAT_FLAG(mode, dContactMotion1) +
        FORMAT_FLAG(mode, dContactMotion2) +
        FORMAT_FLAG(mode, dContactSlip1) +
        FORMAT_FLAG(mode, dContactSlip2) +
        FORMAT_FLAG(mode, dContactApprox0) +
        FORMAT_FLAG(mode, dContactApprox1_1) +
        FORMAT_FLAG(mode, dContactApprox1_2) +
        FORMAT_FLAG(mode, dContactApprox1);

    entries.push_back(Entry(wxT("GetContactMode"),strMode));
    entries.push_back(Entry(wxT("GetBounceValue"),FormatFloat(cjh.GetBounceValue())));
    entries.push_back(Entry(wxT("GetMinBounceVel"),FormatFloat(cjh.GetMinBounceVel())));
    entries.push_back(Entry(wxT("GetContactSoftERP"),FormatFloat(cjh.GetContactSoftERP())));
    entries.push_back(Entry(wxT("GetContactSoftCFM"),FormatFloat(cjh.GetContactSoftCFM())));
    entries.push_back(Entry(wxT("GetContactSlip1"),FormatFloat(cjh.GetContactSlip1())));
    entries.push_back(Entry(wxT("GetContactSlip2"),FormatFloat(cjh.GetContactSlip2())));
    entries.push_back(Entry(wxT("GetContactMu"),FormatFloat(cjh.GetContactMu())));
}

void Property::GenDragControllerEntries(shared_ptr<Leaf> leaf, TEntryList& entries) const
{
    const DragController& dc = *shared_static_cast<DragController>(leaf);

    entries.push_back(Entry(wxT("GetLinearDrag"),FormatFloat(dc.GetLinearDrag())));
    entries.push_back(Entry(wxT("GetAngularDrag"),FormatFloat(dc.GetAngularDrag())));
}

void Property::GetClassList(boost::shared_ptr<Class> cl, TClassList& clList) const
{
    if (cl.get() == 0)
        {
            return;
        }

    shared_ptr<SimSpark> spark = wxGetApp().GetSpark();
    if (spark.get() == 0)
        {
            return;
        }

    TClassMap::const_iterator iter = mClassMap.find(wxString(cl->GetFullPath().c_str(), wxConvUTF8));
    if (iter != mClassMap.end())
        {
            clList.push_back((*iter).second);
        }

    const Class::TStringList& baseClasses = cl->GetBaseClasses();
    for (
         Class::TStringList::const_iterator iter = baseClasses.begin();
         iter != baseClasses.end();
         ++iter
         )
        {
            string basePath = "/classes/"+(*iter);
            shared_ptr<Class> base = shared_dynamic_cast<Class>
                (spark->GetCore()->Get(basePath));

            if (base.get() == 0)
                {
                    cl->GetLog()->Error() << "(Property) invalid base class " << basePath
                                          << " defined for " << cl->GetFullPath() << std::endl;
                    continue;
                }

            GetClassList(base, clList);
        }
}

void Property::GetClassList(boost::shared_ptr<Leaf> leaf, TClassList& clList) const
{
    clList.clear();

    if (leaf.get() == 0)
        {
            return;
        }

    shared_ptr<Class> cl = leaf->GetClass();
    GetClassList(cl, clList);
}

void Property::GenEntries(shared_ptr<zeitgeist::Leaf> leaf,
                          const TClassList& clList, TEntryList& entries) const
{
    entries.clear();

    if (leaf.get() == 0)
        {
            return;
        }

    for (
         TClassList::const_reverse_iterator iter = clList.rbegin();
         iter != clList.rend();
         ++iter
         )
        {
            switch (*iter)
                {
                default:
                    break;

                case CL_LEAF:
                    GenLeafEntries(leaf, entries);
                    break;

                case CL_BASENODE:
                    GenBaseNodeEntries(leaf, entries);
                    break;

                case CL_CLASS:
                    GenClassEntries(leaf, entries);
                    break;

                case CL_TRANSFORM:
                    GenTransformEntries(leaf, entries);
                    break;

                case CL_BODY:
                    GenBodyEntries(leaf, entries);
                    break;

                case CL_HINGEJOINT:
                    GenHingeEntries(leaf, entries);
                    break;

                case CL_UNIVERSALJOINT:
                    GenUniversalEntries(leaf, entries);
                    break;

                case CL_JOINT:
                    GenJointEntries(leaf, entries);
                    break;

                case CL_WORLD:
                    GenWorldEntries(leaf, entries);
                    break;

                case CL_COLLISIONHANDLER:
                    GenCollisionHandlerEntries(leaf, entries);
                    break;

                case CL_CONTACTJOINTHANDLER:
                    GenContactJointEntries(leaf, entries);
                    break;

                case CL_DRAGCONTROLLER:
                    GenDragControllerEntries(leaf, entries);
                    break;

                case CL_MATERIALSOLID:
                    GenSolidMaterialProperty(leaf, entries);
                    break;
                }
        }
}
