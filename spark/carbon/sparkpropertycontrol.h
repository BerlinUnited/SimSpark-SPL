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

#ifndef SPARKPROPERTYCONTROL_H
#define SPARKPROPERTYCONTROL_H
/**
 \file sparkpropertycontrol.h
 \brief Contains SparkProperty::PropertyControl class.
*/

#include "sparkproperty.h"

#include <QObject>

class SparkController;

namespace SparkProperty
{

class PropertyManager;
class PropertyExecuter;

/*!
 \brief Abstract parent class for PropertyControl classes, which create and handle read and write update for Property objects for a specific set of classes.

 A PropertyControl class supports a set of zeitgeist-Classes.
 They create Property objects to read from and write to functions of this class of a zeitgeist leaf by calling getter and setter functions.\n

 PropertyControl classes can be defined separately from each other so they can be loaded as plugins.
 When extending a SimSpark simulation by loading a plugin bundle, simply add a new Gui-Plugin which provides a PropertyControl class
 that can create and handle Properties for new classes used in the additional bundle.\n

 A PropertyControl class has three functions:\n
 1) Create a set of properties for a set of classes. All available Property controls will be asked initially if they support a class.
 The class can be identified by the given class descriptor.\n
 2) Execute a read update for a created Property.\n
 3) Potentially execute a write update from a created Property.\n

 Derived classes have to implement the virtual functions supports(), genProperties(), updateProperty(), applyPropertyChange() and may reinmplement
 init().\n

 The update call is done indirectly via a PropertyExecuter. Any Read and write update call is queued in a ISimControl derived PropertyExecuter, which calls
 the updateProperty() and applyPropertyChange() functions of the PropertyControl when a simulation cycle finished.
 \sa SparkProperty::Property
*/
class PropertyControl : public QObject
{
    Q_OBJECT

    friend class PropertyExecuter;

public:
    //public functions

    /*!
     \brief Complete constructor.

     Also accepts a property executer. The executer can be created after the constructor with createExecuter().
     \param parent parent property manager
     \param name name of the property control, should be unique
     \param executer property executer for the control
    */
    PropertyControl(PropertyManager* parent, const QString& name, boost::shared_ptr<PropertyExecuter> executer);
    /*!
     \brief Constructor.

     \param parent parent property manager
     \param name name for the property control, should be unique
    */
    PropertyControl(PropertyManager* parent, const QString& name);
    /*!
     \brief Destructor.
    */
    ~PropertyControl();

    /*!
     \brief Create an executer for read and write updates.

     \param controller controller to queue read and write update executions in
    */
    void createExecuter(boost::shared_ptr<SparkController> controller);
    /*!
     \brief Unregisters the executer from its parent SparkController.
    */
    void unregisterExecuter();

    /*!
     \brief Queue a property read update with the contained executer.

     \param prop property to read into
    */
    void queueRead(boost::shared_ptr<Property>& prop);
    /*!
     \brief Queue a property write update with the contained executer.

     \param prop property to write from
    */
    void queueWrite(boost::shared_ptr<Property>& prop);

    /*!
     \brief Returns the name of the PropertyControl.
    */
    const QString& getName() const;

protected:
    //protected functions

    /*!
     \brief Set the value of a property.

     \param prop property to set value in
     \param newValue new value for the property
    */
    void setPropertyValueProtected(Property& prop, Data* newValue) const;

public:
    //public virtual functions

    /*!
     \brief Return true if this PropertyControl supports properties for the class with the given name.

     \param className name of the class
     \return true if class is supported
    */
    virtual bool supports(const QString& className) const = 0;
    /*!
     \brief Return true if this PropertyControl supports properties for the class with this descriptor.

     Has the same function as supports(const QString& className), except for that it is more efficient and gives the PropertyControl
     the opportunity to store a class id for the supported class in the ClassDescriptor to identify the class again without string comparison.
     \param descriptor description of the class
     \return true if class is supported
    */
    virtual bool supports(ClassDescriptor& descriptor) const = 0;
    /*!
     \brief Generate properties for the given leaf and class and store them in propList.

     \param leaf leaf to work with in the generated properties
     \param descriptor class to generate properties for
     \param propList list to store property in
     \return true if there were properties generated
    */
    virtual bool genProperties(TLeafPtr leaf, ClassDescriptor& descriptor, TProperties& propList) = 0;
    /*!
     \brief Optional init function.

     Is called once by the PropertyManager after registering the PropertyControl.
    */
    virtual void init() {}

protected:
    //protected virtual functions

    /*!
     \brief Read value and store it in the property.

     \param prop property to store read data in
    */
    virtual bool updateProperty(boost::shared_ptr<Property> prop) = 0;
    /*!
     \brief Apply data stored in the property.

     \param prop property to apply temporary data from
    */
    virtual bool applyPropertyChange(boost::shared_ptr<Property> prop) = 0;

protected slots:
    //protected slots
    
    /*!
     \brief Accept the temporary data in the property.

     Update result will be forwarded to the property.
    */
    void acceptChange(boost::shared_ptr<SparkProperty::Property> prop);
    /*!
     \brief Reject the temporary data in the property.

     Update result will be forwarded to the property.
    */
    void rejectChange(boost::shared_ptr<SparkProperty::Property> prop);
    /*!
     \brief Notification that property was updated.

     Update result will be forwarded to the property.
     \param prop updated property
     \param success true for successful read update, false in case of error
    */
    void updated(boost::shared_ptr<SparkProperty::Property> prop, bool success);

protected:
    //protected members

    PropertyManager* mParent; /*!< Parent property manager. */
    boost::shared_ptr<SparkController> mSparkController; /*!< SparkController to interact with. */
    boost::shared_ptr<PropertyExecuter> mExecuter; /*!< Executer which calls the read and write update functions. */

    QString mName; /*!< Name of the PropertyControl. */
};

} //namespace SparkProperty

#endif // SPARKPROPERTYCONTROL_H
