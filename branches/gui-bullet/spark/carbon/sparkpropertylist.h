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

#ifndef SPARKPROPERTYLIST_H
#define SPARKPROPERTYLIST_H
/**
 \file sparkpropertylist.h
 \brief Contains SparkProperty::PropertyList class.
*/

#include "sparkproperty.h"

#include <QTableWidget>
#include "sparkpropertymanager.h"

class QTableWidget;
class SparkController;
class SimSpark;

namespace SparkProperty
{

/*!
 \brief an extended list of SparkProperty::Property objects.

 Contains a SparkProperty::PropertyManager and the currently inspected zeitgeist::Leaf.
 Whenever a new leaf is chosen, the list of contained Property objects is cleared and re-filled.

 \todo Initialization is only done once. After loading new plugins that provide additional PropertyControl objects a re-initialization is
 required to integrate them. This can be done with a server-client communication that requests reinitialization upon plugin creation or user-controlled
 with a reinit button.
*/
class PropertyList : public QObject
{
    Q_OBJECT

    friend class PropertyManager;
    friend class Property;

public:
    //public functions

    /*!
     \brief Constructor.
    */
    PropertyList();
    /*!
     \brief Destructor.
    */
    ~PropertyList();

    /*!
     \brief Queue a read access for every single Property in the list.
    */
    void updateProperties();
    /*!
     \brief Clears list of Property objects.
    */
    void clearProperties();

    /*!
     \brief Initialize the PropertyList

     \param leaf leaf to generate properties for
     \param sparkcontroller spark context controller to work with
    */
    void init(boost::weak_ptr<zeitgeist::Leaf> leaf, boost::shared_ptr<SparkController> sparkcontroller);
    /*!
     \brief Specify a new leaf to inspect and call refresh().

     \param leaf new leaf
    */
    void refresh(boost::shared_ptr<zeitgeist::Leaf> leaf);

    /*!
     \brief Return a property by index.

     \param index index of property to return
     \return null pointer in case of illegal index
    */
    boost::shared_ptr<Property> getProperty(int index) const;
    /*!
     \brief Return the amount of properties.
    */
    int getSize() const;
    /*!
     \brief Return the list of Property objects.
    */
    const TProperties& getProperties() const;
    /*!
     \brief Return the property manager.
    */
    const boost::shared_ptr<const PropertyManager> getPropertyManager() const;

    /*!
     \brief Queue read access for one Property.

     \param index index of property
    */
    void readProperty(int index);
    /*!
     \brief Queue write access for one Property.

     \param index index of property
    */
    void writeProperty(int index);

protected:
    //protected functions

    /*!
     \brief Initializes or clears and recreates the list of properties.

     Creates a class list using the assigned PropertyManager, clears the property list, and generates Property objects for the currently
     inspected leaf by letting the PropertyManager request the creation of Property objects from all of the registered PropertyControl objects.
    */
    void refresh();

    /*!
     \brief Emit propertyChanged() for the Property at the given index.

     \param index index of the Property
    */
    void updatePropertyChanged(int index);
    /*!
     \brief Emit propertyChanged() if the Property is still in the managed list.

     Properties that have been removed from the list while updating are ignored.
     Called by the PropertyExecuters thread.
     \param prop property that was updated.
    */
    void updatePropertyChanged(boost::shared_ptr<Property> prop);
    /*!
     \brief Emit propertyChanged() if the Property is still in the managed list.

     Properties that have been removed from the list while updating are ignored.
     Called by the PropertyExecuters thread.
     \param prop property that was updated.
    */
    void updatePropertyChanged(Property* prop);

    /*!
     \brief Returns SimSpark of the current SparkController or null pointer if not initialized.
    */
    boost::shared_ptr<SimSpark> getSpark();

public: signals:
    //public signals

    /*!
     \brief emitted when a property was changed.

     \param index index of the property
    */
    void propertyChanged(int index);

protected slots:
    //protected slots

    /*!
     \brief Notification by a PropertyControl that a Property was changed.

     \param prop changed property
    */
    void notifyPropertyChanged(Property* prop);

protected:
    //protected members

    boost::shared_ptr<SparkController> mSparkController; /*!< Assigned SparkController. */
    SparkProperty::TClassList mClassList; /*!< List of currently supported classes. */
    SparkProperty::TProperties mProperties; /*!< List of properties for the currently inspected leaf. */
    boost::weak_ptr<zeitgeist::Leaf> mLeaf; /*!< Currently inspected leaf. */
    boost::shared_ptr<PropertyManager> mPropertyManager; /*!< PropertyManager which handles collection of PropertyControl objects and calls their virtual functions. */
};

} //namespace SparkProperty

#endif // SPARKPROPERTYLIST_H
