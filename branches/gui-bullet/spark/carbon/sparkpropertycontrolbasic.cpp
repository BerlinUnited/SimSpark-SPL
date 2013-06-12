/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
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

#include "sparkpropertycontrolbasic.h"
#include "sparkpropertymanager.h"
#include "sparkpropertydata.h"
#include "sparkproperty.h"
#include "cutelogger/logger.h"

#include <salt/vector.h>
#include <salt/bounds.h>
#include <salt/matrix.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/class.h>
#include <zeitgeist/core.h>
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/sceneserver/scenedict.h>
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/joint.h>
#include <oxygen/physicsserver/hingejoint.h>
#include <oxygen/physicsserver/universaljoint.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/contactjointhandler.h>
#include <oxygen/physicsserver/dragcontroller.h>
#include <kerosin/materialserver/materialsolid.h>

#include <QStringList>

using namespace oxygen;
using namespace kerosin;
using namespace zeitgeist;
using namespace salt;
using namespace boost;

#define ADD_READ_PROPERTY(NAME, CAPTION, DATA, ...) \
    list.push_back(boost::shared_ptr<Property>(new Property(leaf, descriptor, NAME, QObject::tr(CAPTION), DATA, false, 0, ##__VA_ARGS__)))
#define ADD_READWRITE_PROPERTY(NAME, CAPTION, DATA, VALIDATOR, ...) \
    list.push_back(boost::shared_ptr<Property>(new Property(leaf, descriptor, NAME, QObject::tr(CAPTION), DATA, true, VALIDATOR, ##__VA_ARGS__)))

#define SET_VALUE(DATAPTR) setPropertyValueProtected(prop, DATAPTR)
#define GET_VALUE(TYPE) boost::shared_static_cast<const TYPE>(prop.getTempValue())->mValue

#define FORMAT_FLAG(VALUE, FLAG)\
    QString((VALUE & FLAG) ? QString(#FLAG" ") : QString(""))

namespace SparkProperty
{

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

BasicControl::BasicControl(PropertyManager* parent) :
    PropertyControl(parent, "BasicControl")
{
    init();
}

//--------------------------------------------------------------
// Public virtual
//--------------------------------------------------------------

void BasicControl::init()
{
    mClassMap.clear(); //Indices of entries must never change after init

    mClassMap["/classes/zeitgeist/Leaf"]             = CL_LEAF;
    mClassMap["ClassClass"]                          = CL_CLASS;
    mClassMap["/classes/oxygen/BaseNode"]            = CL_BASENODE;
    mClassMap["/classes/oxygen/Transform"]           = CL_TRANSFORM;
    mClassMap["/classes/oxygen/RigidBody"]           = CL_RIGIDBODY;
    mClassMap["/classes/oxygen/Joint"]               = CL_JOINT;
    mClassMap["/classes/oxygen/HingeJoint"]          = CL_HINGEJOINT;
    mClassMap["/classes/oxygen/UniversalJoint"]      = CL_UNIVERSALJOINT;
    mClassMap["/classes/oxygen/World"]               = CL_WORLD;
    mClassMap["/classes/oxygen/CollisionHandler"]    = CL_COLLISIONHANDLER;
    mClassMap["/classes/oxygen/ContactJointHandler"] = CL_CONTACTJOINTHANDLER;
    mClassMap["/classes/oxygen/DragController"]      = CL_DRAGCONTROLLER;
    mClassMap["/classes/kerosin/MaterialSolid"]      = CL_MATERIALSOLID;
}

bool BasicControl::supports(const QString& className) const
{
    return (mClassMap.find(className) != mClassMap.end());
}

bool BasicControl::supports(ClassDescriptor& descriptor) const
{
    //already flagged as supporter?
    if (descriptor.mDefaultControl == this)
        return true;

    //another control flagged as supporter?
    if (descriptor.mDefaultControl != 0)
        return false;

    //find support
    auto pos = mClassMap.find(descriptor.mName);
    if (pos == mClassMap.end())
        return false;

    //store support-id (class enum as int)
    descriptor.mDefaultControl = const_cast<BasicControl*>(this);
    descriptor.mId = pos->second;

    return true;
}

bool BasicControl::genProperties(TLeafPtr leaf, ClassDescriptor& descriptor, TProperties& propList)
{
    if (!supports(descriptor))
        return false;

    //Check the leaf
    if (!leaf.get())
    {
        LOG_ERROR() << "Illegal leaf pointer.";
        return false;
    }

    //Create the property
    bool ok = false;
    int last = propList.size();
    switch (descriptor.mId)
    {
        case CL_LEAF:                ok = genLeaf(leaf, descriptor, propList);                break;
        case CL_CLASS:               ok = genClass(leaf, descriptor, propList);               break;
        case CL_BASENODE:            ok = genBaseNode(leaf, descriptor, propList);            break;
        case CL_TRANSFORM:           ok = genTransform(leaf, descriptor, propList);           break;
        case CL_RIGIDBODY:           ok = genRigidBody(leaf, descriptor, propList);           break;
        case CL_JOINT:               ok = genJoint(leaf, descriptor, propList);               break;
        case CL_HINGEJOINT:          ok = genHingeJoint(leaf, descriptor, propList);          break;
        case CL_UNIVERSALJOINT:      ok = genUniversalJoint(leaf, descriptor, propList);      break;
        case CL_WORLD:               ok = genWorld(leaf, descriptor, propList);               break;
        case CL_COLLISIONHANDLER:    ok = genCollisionHandler(leaf, descriptor, propList);    break;
        case CL_CONTACTJOINTHANDLER: ok = genContactJointHandler(leaf, descriptor, propList); break;
        case CL_DRAGCONTROLLER:      ok = genDragController(leaf, descriptor, propList);      break;
        case CL_MATERIALSOLID:       ok = genMaterialSolid(leaf, descriptor, propList);       break;
    }

    //Queue update for all added entries
    for (int i = last; i < (int)propList.size(); i++)
    {
        mParent->readProperty(propList.at(i));
    }

    if (!ok)
    {
        LOG_ERROR() << "Control '" << mName.toStdString().c_str() << ": Generating property for class " <<
            descriptor.mName.toStdString().c_str() << "(" << descriptor.mId << ") failed.";
        return false;
    }

    return true;
}

//--------------------------------------------------------------
// Protected virtual
//--------------------------------------------------------------

bool BasicControl::updateProperty(boost::shared_ptr<Property> prop)
{
    if (!supports(prop->getClassNc()))
        return false;

    //Lock the leaf
    shared_ptr<Leaf> leaf(prop->getLeaf().lock());
    if (!leaf.get())
        return false;

    //Read the property
    bool ok = false;
    switch (prop->getClass().mId)
    {
        case CL_LEAF:                ok = updateLeaf(leaf, *prop);                break;
        case CL_CLASS:               ok = updateClass(leaf, *prop);               break;
        case CL_BASENODE:            ok = updateBaseNode(leaf, *prop);            break;
        case CL_TRANSFORM:           ok = updateTransform(leaf, *prop);           break;
        case CL_RIGIDBODY:           ok = updateRigidBody(leaf, *prop);           break;
        case CL_JOINT:               ok = updateJoint(leaf, *prop);               break;
        case CL_HINGEJOINT:          ok = updateHingeJoint(leaf, *prop);          break;
        case CL_UNIVERSALJOINT:      ok = updateUniversalJoint(leaf, *prop);      break;
        case CL_WORLD:               ok = updateWorld(leaf, *prop);               break;
        case CL_COLLISIONHANDLER:    ok = updateCollisionHandler(leaf, *prop);    break;
        case CL_CONTACTJOINTHANDLER: ok = updateContactJointHandler(leaf, *prop); break;
        case CL_DRAGCONTROLLER:      ok = updateDragController(leaf, *prop);      break;
        case CL_MATERIALSOLID:       ok = updateMaterialSolid(leaf, *prop);       break;
    }

    if (!ok)
    {
        for (auto it = mClassMap.begin(); it != mClassMap.end(); it++)
        {
            if (it->second == prop->getClass().mId)
            {
                LOG_WARNING() << "Control '" << mName.toStdString().c_str() << ": Reading property " <<
                    prop->getCaption() << " for class " << it->first << " failed.";
                return false;
            }
        }
        LOG_WARNING() << "Control '" << mName.toStdString().c_str() << ": Reading property " <<
            prop->getCaption() << " failed.";
        return false;
    }

    return true;
}

bool BasicControl::applyPropertyChange(boost::shared_ptr<Property> prop)
{
    if (!supports(prop->getClassNc()))
        return false;

    //Lock the leaf
    shared_ptr<Leaf> leaf(prop->getLeaf().lock());
    if (!leaf.get())
        return false;

    if (prop->getUpdateFlag() != UF_WRITING)
    {
        LOG_ERROR() << "No writing flag.";
        return false;
    }

    if (prop->getTempValue().get() == 0)
    {
        LOG_ERROR() << "No temp value.";
        return false;
    }

    //Write the property
    bool ok = false;
    switch (prop->getClass().mId)
    {
        case CL_LEAF:                ok = applyChangeLeaf(leaf, *prop);                break;
        case CL_CLASS:               ok = applyChangeClass(leaf, *prop);               break;
        case CL_BASENODE:            ok = applyChangeBaseNode(leaf, *prop);            break;
        case CL_TRANSFORM:           ok = applyChangeTransform(leaf, *prop);           break;
        case CL_RIGIDBODY:           ok = applyChangeRigidBody(leaf, *prop);           break;
        case CL_JOINT:               ok = applyChangeJoint(leaf, *prop);               break;
        case CL_HINGEJOINT:          ok = applyChangeHingeJoint(leaf, *prop);          break;
        case CL_UNIVERSALJOINT:      ok = applyChangeUniversalJoint(leaf, *prop);      break;
        case CL_WORLD:               ok = applyChangeWorld(leaf, *prop);               break;
        case CL_COLLISIONHANDLER:    ok = applyChangeCollisionHandler(leaf, *prop);    break;
        case CL_CONTACTJOINTHANDLER: ok = applyChangeContactJointHandler(leaf, *prop); break;
        case CL_DRAGCONTROLLER:      ok = applyChangeDragController(leaf, *prop);      break;
        case CL_MATERIALSOLID:       ok = applyChangeMaterialSolid(leaf, *prop);       break;
    }

    if (!ok)
    {
        for (auto it = mClassMap.begin(); it != mClassMap.end(); it++)
        {
            if (it->second == prop->getClass().mId)
            {
                LOG_WARNING() << "Control '" << mName.toStdString().c_str() << ": Writing property " <<
                    prop->getCaption() << " for class " << it->first << " failed.";
                return false;
            }
        }
        LOG_WARNING() << "Control '" << mName.toStdString().c_str() << ": Writing property " <<
            prop->getCaption() << " failed.";
        return false;

    }

    return true;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//=====================
// Leaf

bool BasicControl::genLeaf(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    ADD_READ_PROPERTY("GetFullPath", "FullPath", new DString());

    //Created from source file?
    const SceneDict::FileRef* ref
        = SceneDict::GetInstance().Lookup(leaf);
    if (ref != 0)
    {
        QString strRef = QString(ref->fname.c_str()) + ":"
            + QString::number(ref->line);

        ADD_READ_PROPERTY("RsgFile", "SourceFile", new DString());
    }
    return true;
}
bool BasicControl::updateLeaf(TLeafPtr leaf, Property& prop) const
{
    if (prop.getName().compare("GetFullPath") == 0)
    {
        SET_VALUE(new DString(leaf->GetFullPath()));
    }
    else if (prop.getName().compare("RsgFile") == 0)
    {
        const SceneDict::FileRef* ref
            = SceneDict::GetInstance().Lookup(leaf);

        if (ref == 0)
            return false; // no source file? error

        QString strRef = QString(ref->fname.c_str()) + ":"
            + QString::number(ref->line);

        SET_VALUE(new DString(strRef));
    }
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeLeaf(TLeafPtr leaf, Property& prop) const
{
    //No Supported setters
    return false;
}

//=====================
// Class

bool BasicControl::genClass(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    ADD_READ_PROPERTY("GetBundle", "Bundle", new DString(), QObject::tr("Originating bundle."));
    ADD_READ_PROPERTY("GetBaseClasses", "BaseClasses", new DString());
    ADD_READ_PROPERTY("GetCommandMap", "CommandMap", new DString());
    return true;
}

bool BasicControl::updateClass(TLeafPtr leaf, Property& prop) const
{
    const Class& cl = *shared_static_cast<Class>(leaf);

    if (prop.getName().compare("GetBundle") == 0)
    {
        // originating bundle
        boost::shared_ptr<salt::SharedLibrary> bundle = cl.GetBundle();
        SET_VALUE((bundle.get() != 0) ? new DString(bundle->GetName()) : new DString(std::string("<None>")));
    }
    else if (prop.getName().compare("GetBaseClasses") == 0)
    {
        // base classes
        const Class::TStringList& baseCl = cl.GetBaseClasses();
        QString concat;
        for (auto it = baseCl.begin(); it != baseCl.end(); ++it)
        {
            concat += QString(it->c_str());
            if (it != baseCl.end())
                concat.append("; ");
        }
        SET_VALUE(new DString(concat));
    }
    else if (prop.getName().compare("GetCommandMap") == 0)
    {
        // supported functions
        const Class::TCommandMap& cmds = cl.GetCommandMap();
        QString concat;
        for (auto it = cmds.begin(); it != cmds.end(); ++it)
        {
            concat += QString(it->first.c_str());
            if (it != cmds.end())
                concat.append("; ");
        }
        SET_VALUE(new DString(concat));
    }
    else
        return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeClass(TLeafPtr leaf, Property& prop) const
{
    //No Supported setters
    return false;
}

//=====================
// BaseNode

bool BasicControl::genBaseNode(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    boost::shared_ptr<BaseNode> baseNode = boost::shared_static_cast<BaseNode>(leaf);
    ADD_READ_PROPERTY("GetWorldBoundingBox", "WorldBoundingBox", new DAABB3());
    return true;
}

bool BasicControl::updateBaseNode(TLeafPtr leaf, Property& prop) const
{
    boost::shared_ptr<BaseNode> baseNode = boost::shared_static_cast<BaseNode>(leaf);
    if (prop.getName().compare("GetWorldBoundingBox") == 0) {SET_VALUE(new DAABB3(baseNode->GetWorldBoundingBox()));}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeBaseNode(TLeafPtr leaf, Property& prop) const
{
    //No Supported setters
    return false;
}

//=====================
// Transform

bool BasicControl::genTransform(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    const Transform& trans = *shared_static_cast<Transform>(leaf);
    ADD_READ_PROPERTY("GetChangedMark"   , "ChangedMark",    new DInt());
    ADD_READWRITE_PROPERTY("GetLocalTransform", "LocalTransform", new DMat4x4(), Data::doubleMatrixValidator(), QObject::tr("Local transformation matrix"));
    ADD_READWRITE_PROPERTY("GetWorldTransform", "WorldTransform", new DMat4x4(), Data::doubleMatrixValidator(), QObject::tr("World transformation matrix"));
    return true;
}

bool BasicControl::updateTransform(TLeafPtr leaf, Property& prop) const
{
    const Transform& trans = *shared_static_cast<Transform>(leaf);
    if      (prop.getName().compare("GetChangedMark") == 0)    {SET_VALUE(new DInt(trans.GetChangedMark()));}
    else if (prop.getName().compare("GetLocalTransform") == 0) {SET_VALUE(new DMat4x4(trans.GetLocalTransform()));}
    else if (prop.getName().compare("GetWorldTransform") == 0) {SET_VALUE(new DMat4x4(trans.GetWorldTransform()));}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeTransform(TLeafPtr leaf, Property& prop) const
{
    Transform& trans = *shared_static_cast<Transform>(leaf);
    //Supported setters
    if      (prop.getName().compare("GetLocalTransform") == 0) {trans.SetLocalTransform(GET_VALUE(DMat4x4));}
    else if (prop.getName().compare("GetWorldTransform") == 0) {trans.SetWorldTransform(GET_VALUE(DMat4x4));}
    else return false; //Setter not supported
    return true;
}

//=====================
// RigidBody

bool BasicControl::genRigidBody(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    const RigidBody& body = *shared_static_cast<RigidBody>(leaf);
    ADD_READWRITE_PROPERTY("GetMass",            "Mass",            new DFloat   (), Data::doubleValidator(0));
    ADD_READWRITE_PROPERTY("GetVelocity",        "Velocity",        new DVector3f(), Data::vector3fValidator());
    ADD_READWRITE_PROPERTY("GetAngularVelocity", "AngularVelocity", new DVector3f(), Data::vector3fValidator());
    ADD_READWRITE_PROPERTY("GetPosition",        "Position",        new DVector3f(), Data::vector3fValidator());
    return true;
}

bool BasicControl::updateRigidBody(TLeafPtr leaf, Property& prop) const
{
    const RigidBody& body = *shared_static_cast<RigidBody>(leaf);
    if      (prop.getName().compare("GetMass") == 0)            {SET_VALUE(new DFloat   (body.GetMass())           );}
    else if (prop.getName().compare("GetVelocity") == 0)        {SET_VALUE(new DVector3f(body.GetVelocity())       );}
    else if (prop.getName().compare("GetAngularVelocity") == 0) {SET_VALUE(new DVector3f(body.GetAngularVelocity()));}
    else if (prop.getName().compare("GetPosition") == 0)        {SET_VALUE(new DVector3f(body.GetPosition())       );}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeRigidBody(TLeafPtr leaf, Property& prop) const
{
    RigidBody& body = *shared_static_cast<RigidBody>(leaf);
    if      (prop.getName().compare("GetMass") == 0)            {body.SetMass           (GET_VALUE(DFloat));}
    else if (prop.getName().compare("GetVelocity") == 0)        {body.SetVelocity       (GET_VALUE(DVector3f));}
    else if (prop.getName().compare("GetAngularVelocity") == 0) {body.SetAngularVelocity(GET_VALUE(DVector3f));}
    else if (prop.getName().compare("GetPosition") == 0)        {body.SetPosition       (GET_VALUE(DVector3f));}
    else return false; //Getter not supported
    return true;
}

//=====================
// Joint

bool BasicControl::genJoint(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    ADD_READ_PROPERTY("FeedBackEnabled", "FeedBackEnabled", new DBool());

    for (int i = 0; i < 2; ++i)
    {
        QString strIdx = "(" + QString::number(i) + ")";
        const Joint::EBodyIndex idx = static_cast<Joint::EBodyIndex>(i);

        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetFeedbackForce("  + strIdx + ")", QObject::tr("FeedBackForce%1").arg(strIdx),  new DVector3f())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetFeedbackTorque(" + strIdx + ")", QObject::tr("FeedBackTorque%1").arg(strIdx), new DVector3f())));
    }

    for (int i = 0; i < 3; ++i)
    {
        QString strIdx = "(" + QString::number(i) + ")";
        const Joint::EAxisIndex idx = static_cast<Joint::EAxisIndex>(i);

        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetBounce("               + strIdx + ")", QObject::tr("Bounce%1"              ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetLowStopDeg("           + strIdx + ")", QObject::tr("LowStopDeg%1"          ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetHighStopDeg("          + strIdx + ")", QObject::tr("HighStopDeg%1"         ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetLowStopPos("           + strIdx + ")", QObject::tr("LowStopPos%1"          ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetHighStopPos("          + strIdx + ")", QObject::tr("HighStopPos%1"         ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetCFM("                  + strIdx + ")", QObject::tr("CFM%1"                 ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetStopCFM("              + strIdx + ")", QObject::tr("StopCFM%1"             ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetSuspensionCFM("        + strIdx + ")", QObject::tr("SuspensionCFM%1"       ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetStopERP("              + strIdx + ")", QObject::tr("StopERP%1"             ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetSuspensionERP("        + strIdx + ")", QObject::tr("SuspensionERP%1"       ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetLinearMotorVelocity("  + strIdx + ")", QObject::tr("LinearMotorVelocity%1" ).arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetAngularMotorVelocity(" + strIdx + ")", QObject::tr("AngularMotorVelocity%1").arg(strIdx), new DFloat())));
        list.push_back(shared_ptr<Property>(new Property(leaf, descriptor, "GetMaxMotorForce("        + strIdx + ")", QObject::tr("MaxMotorForce%1"       ).arg(strIdx), new DFloat())));
    }
    return true;
}

bool BasicControl::updateJoint(TLeafPtr leaf, Property& prop) const
{
    const Joint& joint = *shared_static_cast<Joint>(leaf);
    if (prop.getName().compare("FeedBackEnabled") == 0)     {SET_VALUE(new DFloat(joint.FeedBackEnabled()));}
    else
    {
        QStringList splits = prop.getName().split(QRegExp("\\(|\\)"), QString::SkipEmptyParts);
        if (splits.size() != 2)
            return false;
        int param = splits.at(1).toInt();
        const Joint::EBodyIndex idb = static_cast<Joint::EBodyIndex>(param);
        const Joint::EAxisIndex ida = static_cast<Joint::EAxisIndex>(param);
        QString name = splits.at(0);

        if      (name.compare("GetFeedbackForce"       ) == 0) { if (param >= 2) return false; SET_VALUE(new DVector3f(joint.GetFeedbackForce(idb)));}
        else if (name.compare("GetFeedbackTorque"      ) == 0) { if (param >= 2) return false; SET_VALUE(new DVector3f(joint.GetFeedbackTorque(idb)));}
        else if (name.compare("GetBounce"              ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetBounce(ida)));}
        else if (name.compare("GetLowStopDeg"          ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetLowStopDeg(ida)));}
        else if (name.compare("GetHighStopDeg"         ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetHighStopDeg(ida)));}
        else if (name.compare("GetLowStopPos"          ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetLowStopPos(ida)));}
        else if (name.compare("GetHighStopPos"         ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetHighStopPos(ida)));}
        else if (name.compare("GetCFM"                 ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetCFM(ida)));}
        else if (name.compare("GetStopCFM"             ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetStopCFM(ida)));}
        else if (name.compare("GetSuspensionCFM"       ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetSuspensionCFM(ida)));}
        else if (name.compare("GetStopERP"             ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetStopERP(ida)));}
        else if (name.compare("GetSuspensionERP"       ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetSuspensionERP(ida)));}
        else if (name.compare("GetLinearMotorVelocity" ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetLinearMotorVelocity(ida)));}
        else if (name.compare("GetAngularMotorVelocity") == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetAngularMotorVelocity(ida)));}
        else if (name.compare("GetMaxMotorForce"       ) == 0) { if (param >= 3) return false; SET_VALUE(new DFloat(joint.GetMaxMotorForce(ida)));}
        else return false; //Getter not supported
    }
    return true;
}

bool BasicControl::applyChangeJoint(TLeafPtr leaf, Property& prop) const
{
    //No Supported setters
    return false;
}

//=====================
// HingeJoint

bool BasicControl::genHingeJoint(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    const HingeJoint& hinge = *shared_static_cast<HingeJoint>(leaf);
    ADD_READ_PROPERTY("GetAngle",     "Angle",     new DFloat()); //todo
    ADD_READ_PROPERTY("GetAngleRate", "AngleRate", new DFloat()); //todo
    return true;
}

bool BasicControl::updateHingeJoint(TLeafPtr leaf, Property& prop) const
{
    const HingeJoint& hinge = *shared_static_cast<HingeJoint>(leaf);
    if      (prop.getName().compare("GetAngle") == 0)     {SET_VALUE(new DFloat(hinge.GetAngle()));}
    else if (prop.getName().compare("GetAngleRate") == 0) {SET_VALUE(new DFloat(hinge.GetAngleRate()));}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeHingeJoint(TLeafPtr leaf, Property& prop) const
{
    //No Supported setters
    return false;
}

//=====================
// UniversalJoint

bool BasicControl::genUniversalJoint(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    const UniversalJoint& univ = *shared_static_cast<UniversalJoint>(leaf);
    ADD_READ_PROPERTY("GetAngle(0)",     "Angle(0)",     new DFloat()); //todo
    ADD_READ_PROPERTY("GetAngle(1)",     "Angle(1)",     new DFloat()); //todo
    ADD_READ_PROPERTY("GetAngleRate(0)", "AngleRate(0)", new DFloat()); //todo
    ADD_READ_PROPERTY("GetAngleRate(1)", "AngleRate(1)", new DFloat()); //todo
    return true;
}

bool BasicControl::updateUniversalJoint(TLeafPtr leaf, Property& prop) const
{
    const UniversalJoint& univ = *shared_static_cast<UniversalJoint>(leaf);
    if      (prop.getName().compare("GetAngle(0)") == 0)     {SET_VALUE(new DFloat(univ.GetAngle(Joint::AI_FIRST)));}
    else if (prop.getName().compare("GetAngle(1)") == 0)     {SET_VALUE(new DFloat(univ.GetAngle(Joint::AI_SECOND)));}
    else if (prop.getName().compare("GetAngleRate(0)") == 0) {SET_VALUE(new DFloat(univ.GetAngleRate(Joint::AI_FIRST)));}
    else if (prop.getName().compare("GetAngleRate(1)") == 0) {SET_VALUE(new DFloat(univ.GetAngleRate(Joint::AI_SECOND)));}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeUniversalJoint(TLeafPtr leaf, Property& prop) const
{
    //No Supported setters
    return false;
}

//=====================
// ContactJointHandler

bool BasicControl::genContactJointHandler(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    const ContactJointHandler& cjh = *shared_static_cast<ContactJointHandler>(leaf);
    ADD_READ_PROPERTY     ("GetContactMode",    "ContactMode",    new DString());
    ADD_READWRITE_PROPERTY("GetBounceValue",    "BounceValue",    new DFloat(), Data::doubleValidator());
    ADD_READWRITE_PROPERTY("GetMinBounceVel",   "MinBounceVel",   new DFloat(), Data::doubleValidator());
    ADD_READWRITE_PROPERTY("GetContactSoftERP", "ContactSoftERP", new DFloat(), Data::doubleValidator());
    ADD_READWRITE_PROPERTY("GetContactSoftCFM", "ContactSoftCFM", new DFloat(), Data::doubleValidator());
    ADD_READWRITE_PROPERTY("GetContactMu",      "ContactMu",      new DFloat(), Data::doubleValidator());
    ADD_READ_PROPERTY     ("GetContactSlip1",   "ContactSlip1",   new DFloat()); //todo
    ADD_READ_PROPERTY     ("GetContactSlip2",   "ContactSlip2",   new DFloat()); //todo
    return true;
}

bool BasicControl::updateContactJointHandler(TLeafPtr leaf, Property& prop) const
{
    const ContactJointHandler& cjh = *shared_static_cast<ContactJointHandler>(leaf);

    if      (prop.getName().compare("GetContactMode"   ) == 0)
    {
        int mode = cjh.GetContactMode();
        QString strMode = //replace ODE constants with their actual values
            FORMAT_FLAG(mode, 1    ) + /*dContactMu2*/
            FORMAT_FLAG(mode, 2    ) + /*dContactFDir1*/
            FORMAT_FLAG(mode, 4    ) + /*dContactBounce*/
            FORMAT_FLAG(mode, 8    ) + /*dContactSoftERP*/
            FORMAT_FLAG(mode, 16   ) + /*dContactSoftCFM*/
            FORMAT_FLAG(mode, 32   ) + /*dContactMotion1*/
            FORMAT_FLAG(mode, 64   ) + /*dContactMotion2*/
            FORMAT_FLAG(mode, 256  ) + /*dContactSlip1*/
            FORMAT_FLAG(mode, 512  ) + /*dContactSlip2*/
            FORMAT_FLAG(mode, 0    ) + /*dContactApprox0*/
            FORMAT_FLAG(mode, 4096 ) + /*dContactApprox1_1*/
            FORMAT_FLAG(mode, 8192 ) + /*dContactApprox1_2*/
            FORMAT_FLAG(mode, 12288);  /*dContactApprox1*/

        SET_VALUE(new DString(strMode));
    }
    else if (prop.getName().compare("GetBounceValue"   ) == 0) {SET_VALUE(new DFloat(cjh.GetBounceValue())   );}
    else if (prop.getName().compare("GetMinBounceVel"  ) == 0) {SET_VALUE(new DFloat(cjh.GetMinBounceVel())  );}
    else if (prop.getName().compare("GetContactSoftERP") == 0) {SET_VALUE(new DFloat(cjh.GetContactSoftERP()));}
    else if (prop.getName().compare("GetContactSoftCFM") == 0) {SET_VALUE(new DFloat(cjh.GetContactSoftCFM()));}
    else if (prop.getName().compare("GetContactSlip1"  ) == 0) {SET_VALUE(new DFloat(cjh.GetContactSlip1())  );}
    else if (prop.getName().compare("GetContactSlip2"  ) == 0) {SET_VALUE(new DFloat(cjh.GetContactSlip2())  );}
    else if (prop.getName().compare("GetContactMu"     ) == 0) {SET_VALUE(new DFloat(cjh.GetContactMu())     );}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeContactJointHandler(TLeafPtr leaf, Property& prop) const
{
    ContactJointHandler& cjh = *shared_static_cast<ContactJointHandler>(leaf);

    //Supported setters
    if      (prop.getName().compare("GetBounceValue") == 0)    {cjh.SetBounceValue   (GET_VALUE(DFloat));}
    else if (prop.getName().compare("GetMinBounceVel") == 0)   {cjh.SetMinBounceVel  (GET_VALUE(DFloat));}
    else if (prop.getName().compare("GetContactSoftERP") == 0) {cjh.SetContactSoftERP(GET_VALUE(DFloat));}
    else if (prop.getName().compare("GetContactSoftCFM") == 0) {cjh.SetContactSoftCFM(GET_VALUE(DFloat));}
    else if (prop.getName().compare("GetContactMu") == 0)      {cjh.SetContactMu     (GET_VALUE(DFloat));}
    else return false; //Setter not supported
    return true;
}

//=====================
// World

bool BasicControl::genWorld(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    const World& world = *shared_static_cast<World>(leaf);
    ADD_READWRITE_PROPERTY("GetGravity",  "Gravity", new DVector3f(), Data::vector3fValidator(),
        "Gravity Vector");
    ADD_READWRITE_PROPERTY("GetERP",      "ERP",     new DFloat(),    Data::doubleValidator(0.0, 1.0),
        "Error Reduction Parameter of the world. Specifies what proportion of a joint error will be fixed during the next simulation step.\n" \
        "ERP=0: no correcting force, bodies will eventually drift apart.\n" \
        "ERP=1: attempt to fix all joint error during the next step. Not recommended. Error wont be fixed completely due to various internal approximations.\n" \
        "Value of ERP=0.1 to 0.8 is recommended.");
    ADD_READWRITE_PROPERTY("GetCFM",      "CFM",     new DFloat(),    Data::doubleValidator(0,0),
        "Constraint Force Mixing (CFM) value (softness of constraints). \nIf set to zero, the constraint will be hard. \n" \
        "If set to a positive value, it will be possible to violate the constraint by 'pushing on it'");
    return true;
}

bool BasicControl::updateWorld(TLeafPtr leaf, Property& prop) const
{
    const World& world = *shared_static_cast<World>(leaf);
    if      (prop.getName().compare("GetGravity") == 0) {SET_VALUE(new DVector3f(world.GetGravity()));}
    else if (prop.getName().compare("GetERP") == 0)     {SET_VALUE(new DFloat   (world.GetERP()));}
    else if (prop.getName().compare("GetCFM") == 0)     {SET_VALUE(new DFloat   (world.GetCFM()));}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeWorld(TLeafPtr leaf, Property& prop) const
{
    World& world = *shared_static_cast<World>(leaf);
    if      (prop.getName().compare("GetGravity") == 0)  {world.SetGravity(GET_VALUE(DVector3f));}
    else if (prop.getName().compare("GetERP") == 0)      {world.SetERP    (GET_VALUE(DFloat));}
    else if (prop.getName().compare("GetCFM") == 0)      {world.SetCFM    (GET_VALUE(DFloat));}
    else return false;//Setter not supported
    return true;
}

//=====================
// CollisionHandler

bool BasicControl::genCollisionHandler(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    boost::shared_ptr<CollisionHandler> ch = shared_static_cast<CollisionHandler>(leaf);
    ADD_READ_PROPERTY("IsSymmetricHandler", "SymmetricHandler", new DBool());
    return true;
}

bool BasicControl::updateCollisionHandler(TLeafPtr leaf, Property& prop) const
{
    boost::shared_ptr<CollisionHandler> ch = shared_static_cast<CollisionHandler>(leaf);
    if (prop.getName().compare("IsSymmetricHandler") == 0) {SET_VALUE(new DBool(ch->IsSymmetricHandler()));}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeCollisionHandler(TLeafPtr leaf, Property& prop) const
{
    //No Supported setters
    return false;
}

//=====================
// DragController

bool BasicControl::genDragController(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    const DragController& dc = *shared_static_cast<DragController>(leaf);
    ADD_READWRITE_PROPERTY("GetLinearDrag",  "LinearDrag",  new DFloat(dc.GetLinearDrag()),  Data::doubleValidator());
    ADD_READWRITE_PROPERTY("GetAngularDrag", "AngularDrag", new DFloat(dc.GetAngularDrag()), Data::doubleValidator());
    return true;
}

bool BasicControl::updateDragController(TLeafPtr leaf, Property& prop) const
{
    const DragController& dc = *shared_static_cast<DragController>(leaf);
    if      (prop.getName().compare("GetLinearDrag") == 0)  {SET_VALUE(new DFloat(dc.GetLinearDrag()));}
    else if (prop.getName().compare("GetAngularDrag") == 0) {SET_VALUE(new DFloat(dc.GetAngularDrag()));}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeDragController(TLeafPtr leaf, Property& prop) const
{
    DragController& dc = *shared_static_cast<DragController>(leaf);
    if      (prop.getName().compare("GetLinearDrag") == 0)  {dc.SetLinearDrag (GET_VALUE(DFloat));}
    else if (prop.getName().compare("GetAngularDrag") == 0) {dc.SetAngularDrag(GET_VALUE(DFloat));}
    else return false; //Setter not supported
    return true;
}

//=====================
// MaterialSolid

bool BasicControl::genMaterialSolid(TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& list) const
{
    const MaterialSolid& mat = *shared_static_cast<MaterialSolid>(leaf);
    ADD_READWRITE_PROPERTY("GetAmbient",  "Ambient",  new DRGBA(), Data::rgbaValidator());
    ADD_READWRITE_PROPERTY("GetDiffuse",  "Diffuse",  new DRGBA(), Data::rgbaValidator());
    ADD_READWRITE_PROPERTY("GetSpecular", "Specular", new DRGBA(), Data::rgbaValidator());
    return true;
}

bool BasicControl::updateMaterialSolid(TLeafPtr leaf, Property& prop) const
{
    const MaterialSolid& mat = *shared_static_cast<MaterialSolid>(leaf);
    if      (prop.getName().compare("GetAmbient") == 0)  {SET_VALUE(new DRGBA(mat.GetAmbient()));}
    else if (prop.getName().compare("GetDiffuse") == 0)  {SET_VALUE(new DRGBA(mat.GetDiffuse()));}
    else if (prop.getName().compare("GetSpecular") == 0) {SET_VALUE(new DRGBA(mat.GetSpecular()));}
    else return false; //Getter not supported
    return true;
}

bool BasicControl::applyChangeMaterialSolid(TLeafPtr leaf, Property& prop) const
{
    MaterialSolid& mat = *shared_static_cast<MaterialSolid>(leaf);
    //Supported setters
    if      (prop.getName().compare("GetAmbient") == 0)  {mat.SetAmbient (GET_VALUE(DRGBA));}
    else if (prop.getName().compare("GetDiffuse") == 0)  {mat.SetDiffuse (GET_VALUE(DRGBA));}
    else if (prop.getName().compare("GetSpecular") == 0) {mat.SetSpecular(GET_VALUE(DRGBA));}
    else return false; //Setter not supported
    return true;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

}//namespace SparkProperty
