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

#ifndef SPARKPROPERTYMANAGER_H
#define SPARKPROPERTYMANAGER_H
/**
 \file sparkpropertymanager.h
 \brief Contains SparkProperty::PropertyManager class.
*/

#include "sparkproperty.h"

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <QObject>

class SparkController;
class SimSpark;

namespace SparkProperty
{

class PropertyControl;

/*!
 \brief A class assigned to a SparkProperty::SparkPropertyList. Manages registration and application of SparkProperty::PropertyControl derived classes.

 \todo It is thinkable to use different front-ends for SparkProperties.
 In this case it would be cleaner and a bit more efficient to only have one PropertyManager designed as single Plugin that is loaded with ass the Plugins
 that need it, or better even to move the whole Property-System into a Plugin instead of having it in the main Gui.
 This would also reduce the initialization complexity for new SparkProperty::PropertyControl derived classes with searching and registering to the PropertyManager.

 \todo Currently SparkProperty::PropertyControl are not allowed to offer Properties for the same class.
 The first PropertyControl that supports a given calss will be chosen as default control to generate and execute the Property.
 This could be extended to have different implementations of properties or combine different properties for the same class from different plugins.
 However, because this might in some cases be really confusing for the user without having a good visual feedback, this feature was intentionally not
 allowed at this point.
*/
class PropertyManager : public QObject
{
    Q_OBJECT

    friend class PropertyControl;

public:
    //public functions

    /*!
     \brief Complete constructor.

     \param parent parent property list
     \param controller assigned SparkController
    */
    PropertyManager(PropertyList* parent, boost::shared_ptr<SparkController> controller);
    /*!
     \brief Destructor.
    */
    ~PropertyManager();

    /*!
     \brief Calls property generation of all the registered SparkProperty::PropertyControl objects.

     The call will check for every class in the classList if there is at least one PropertyControl that provides properties for the class.
     The FIRST (!!) one found will be used as default PropertyControl for this class.

     Then properties are generated and added to the propList.
     \param leaf leaf to work with and generate properties for
     \param classList list of classes to generate properties for
     \param propList list to store generated properties in
    */
    void genProperties(TLeafPtr leaf, TClassList& classList, TProperties& propList) const;

    /*!
     \brief Start a write update for a property using the PropertyControl that created the Property.

     Sets the write update flag of the Property.
     Returns true if writing was started successfully or there was not temp value in the Property to write from.
     Returns false if starting the update failed, if the PropertyControl of this Property was removed or if the inspected leaf was removed in the mean time.
     \param prop property to queue write update for
     \return true if successful or nothing to update, false in case of error
    */
    bool writeProperty(boost::shared_ptr<Property>& prop);
    /*!
     \brief Start a read update for a property using the PropertyControl that created the Property.

     Sets the read update flag of the Property.
     Returns true if reading was started successfully.
     Returns false if starting the update failed, if the PropertyControl of this Property was removed or if the inspected leaf was removed in the mean time.
     \param prop property to queue write update for
     \return true if successful or nothing to update, false in case of error
    */
    bool readProperty(boost::shared_ptr<Property>& prop);

    /*!
     \brief Returns the first registered PropertyControl that supports the specified class.

     \param classPtr calss to search PropertyControl for
     \return first supporting PropertyControl or null pointer if there is no supporter
    */
    boost::shared_ptr<PropertyControl> getSupportingControl(ClassDescriptor& classPtr) const;
    /*!
     \brief Collect all the base classes of the given leaf and store descriptors for them and the leafs class in classList.

     \param leaf leaf to inspect
     \param classList target list for class descriptors
    */
    void getClassList(TLeafPtr leaf, TClassList& classList) const;

    /*!
     \brief Emit a registerPropertyControlRequest().
    */
    void init();

    /*!
     \brief Add a PropertyControl to the list.
    */
    void registerPropertyControl(boost::shared_ptr<PropertyControl> control);
    /*!
     \brief Remove a PropertyControl from the list.
    */
    void unregisterPropertyControl(boost::shared_ptr<PropertyControl> control);
    /*!
     \brief Remove a PropertyControl from the list.
    */
    void unregisterPropertyControl(PropertyControl* control);

private:
    //private  functions

    /*!
     \brief Collect all the base classes of the given class and store descriptors for them and the class itself in classList.

     \param classPtr SimSpark class
     \param classList target list for class descriptors
     \param spark SimSpark object the leaf belongs to
    */
    void getClassListPrivate(TClassPtr classPtr, TClassList& classList, const boost::shared_ptr<SimSpark>& spark) const;

public: signals:
    //public signals

    /*!
     \brief Emitted at time of initialization. Request, that different sources may register their SparkProperty::PropertyControl derived controls.
    */
    void registerPropertyControlRequest();

protected:
    //protected members

    boost::shared_ptr<SparkController> mSparkController; /*!< SparkController to work with. */

private:
    //private members

    PropertyList* mParent; /*!< Parent property list. */
    QList<boost::shared_ptr<PropertyControl>> mControls; /*!< List of registers SparkProperty::PropertyControl derived objects. */
};

} //namespace SparkProperty

#endif //SPARKPROPERTYMANAGER_H
