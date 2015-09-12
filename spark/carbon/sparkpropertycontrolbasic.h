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

#ifndef SPARKPROPERTYCONTROLBASIC_H
#define SPARKPROPERTYCONTROLBASIC_H
/**
 \file sparkpropertycontrolbasic.h
 \brief Contains SparkProperty::BasicControl class.
*/
#include <map>
#include <QString>

#include "sparkpropertycontrol.h"
#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

namespace zeitgeist
{
    class Class;
    class Leaf;
}

class QValidator;

namespace SparkProperty
{

class PropertyManager;
class Property;
class Data;

/*!
 \brief PropertyControl class which supports (creates and handles properties for) basic zeitgeist, oxygen and kerosin classes.
*/
class BasicControl : public PropertyControl
{
private:
    //private types

    /*!
     \brief Class type ids.
    */
    enum EClass
    {
        CL_LEAF = 0,            /*!< zeitgeist::Leaf */
        CL_CLASS,               /*!< zeitgeist::Class */
        CL_BASENODE,            /*!< oxygen::BaseNode */
        CL_TRANSFORM,           /*!< oxygen::Transform */
        CL_RIGIDBODY,           /*!< oxygen::RigidBody */
        CL_JOINT,               /*!< oxygen::Joint */
        CL_HINGEJOINT,          /*!< oxygen::HingeJoint */
        CL_UNIVERSALJOINT,      /*!< oxygen::UniversalJoint */
        CL_WORLD,               /*!< oxygen::World */
        CL_COLLISIONHANDLER,    /*!< oxygen::CollisionHandler */
        CL_CONTACTJOINTHANDLER, /*!< oxygen::ContactJointHandler */
        CL_DRAGCONTROLLER,      /*!< oxygen::DragController */
        CL_MATERIALSOLID        /*!< kerosin::MaterialSolid */
    };

    /*!
     \brief Mapping from path of zeitgeist class to EClass value.
    */
    typedef std::map<QString, EClass> TClassMap;

public:
    //public functions

    /*!
     \brief Constructor.

     \param parent parent property manager
    */
    BasicControl(PropertyManager* parent);

public:
    //public virtual functions

    /*!
     \brief Returns true if the class with the given name is supported.

     \param className name as full class path
    */
    virtual bool supports(const QString& className) const;
    /*!
     \brief Returns true if the class with the given name is supported.

     Also stores the EClass id of the class as id in the descriptor.
     \param descriptor class descriptor with name as full class path
    */
    virtual bool supports(ClassDescriptor& descriptor) const;
    /*!
     \brief Generates properties for the given class if possible and stores them in propList.

     \param leaf leaf to store in properties
     \param descriptor class to generate properties for
     \param propList property list to store properties in
     \return true if at least one property was created, false otherwise
    */
    virtual bool genProperties(TLeafPtr leaf, ClassDescriptor& descriptor, TProperties& propList);
    /*!
     \brief Initializes a TClassMap.
    */
    virtual void init();

protected:
    //protected virtual functions
    
    /*!
     \brief Updates the given property by reading data from SimSpark and storing it in the property.

     \param prop property to store data in
     \return true if updating was successful, false otherwise
    */
    virtual bool updateProperty(boost::shared_ptr<Property> prop);
    /*!
     \brief Updates SimSpark by reading data from the given property.

     \param prop proterty with temp value to apply
     \return true if updating was successful, false otherwise
    */
    virtual bool applyPropertyChange(boost::shared_ptr<Property> prop);

protected:
    //protected functions

    bool genLeaf                (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the Leaf class. */
    bool genClass               (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the Class class. */
    bool genBaseNode            (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the BaseNode class. */
    bool genTransform           (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the Transform class. */
    bool genRigidBody           (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the RigidBody class. */
    bool genJoint               (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the Joint class. */
    bool genHingeJoint          (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the HingeJoint class. */
    bool genUniversalJoint      (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the UniversalJoint class. */
    bool genContactJointHandler (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the ContactJointHandler class. */
    bool genWorld               (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the World class. */
    bool genCollisionHandler    (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the CollisionHandler class. */
    bool genDragController      (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the DragController class. */
    bool genMaterialSolid       (TLeafPtr leaf, const ClassDescriptor& descriptor, TProperties& propList) const; /*!< \brief Generate properties for the MaterialSolid class. */

    bool updateLeaf                (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the Leaf class. */
    bool updateClass               (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the Class class. */
    bool updateBaseNode            (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the BaseNode class. */
    bool updateTransform           (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the Transform class. */
    bool updateRigidBody           (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the RigidBody class. */
    bool updateJoint               (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the Joint class. */
    bool updateHingeJoint          (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the HingeJoint class. */
    bool updateUniversalJoint      (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the UniversalJoint class. */
    bool updateContactJointHandler (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the ContactJointHandler class. */
    bool updateWorld               (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the World class. */
    bool updateCollisionHandler    (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the CollisionHandler class. */
    bool updateDragController      (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the DragController class. */
    bool updateMaterialSolid       (TLeafPtr leaf, Property& prop) const; /*!< \brief Read a value and store in the given property for the MaterialSolid class. */

    bool applyChangeLeaf                (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the Leaf class. */
    bool applyChangeClass               (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the Class class. */
    bool applyChangeBaseNode            (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the BaseNode class. */
    bool applyChangeTransform           (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the Transform class. */
    bool applyChangeRigidBody           (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the RigidBody class. */
    bool applyChangeJoint               (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the Joint class. */
    bool applyChangeHingeJoint          (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the HingeJoint class. */
    bool applyChangeUniversalJoint      (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the UniversalJoint class. */
    bool applyChangeContactJointHandler (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the ContactJointHandler class. */
    bool applyChangeWorld               (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the World class. */
    bool applyChangeCollisionHandler    (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the CollisionHandler class. */
    bool applyChangeDragController      (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the DragController class. */
    bool applyChangeMaterialSolid       (TLeafPtr leaf, Property& prop) const; /*!< \brief Write a value from a property for the MaterialSolid class. */

private:
    //private members

    TClassMap mClassMap; /*!< Mapping from path of zeitgeist class to EClass value.  */
};

} //namespace SparkProperty

#endif // SPARKPROPERTYCONTROL_H
