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

#ifndef SPARKPROPERTYEXECUTER_H
#define SPARKPROPERTYEXECUTER_H
/**
 \file sparkpropertyexecuter.h
 \brief Contains SparkProperty::PropertyExecuter class.
*/

#include "isimcontrol.h"

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <QObject>
#include <QSemaphore>
#include <list>

namespace SparkProperty
{

class PropertyControl;
class Property;

/*!
 \brief ISimControl derived class that executes read and write accesses for SparkProperty::Property objects in its parent thread.

 Property objects are interface objects to functions of SimSpark.
 Reading data by calling getter functions and writing data by calling setter functions is only safe if done by the SimulationServer thread, so the
 thread cant change data of the Simulation while it is accessed externally.
 Read and write update calls of a Property are queued in a PropertyExecuter which calls the implementation of the Get/Set access in the Spark PropertyControl
 assigned with this executer as soon as a simulation cycle of the GuiSimControl node the executer is registered to ends.\n

 Accessing the Get- and Set-Queue is done with a stepped wait system.
 The Simulation Server will only wait for an extremely short time to access the Set- or Get-Queue.
 If the access attempt fails because of a timeout the normal server simulation is continued and will try to access the queue again in the next cycle with
 successively increasing wait time.
 This way the Simulation is guaranteed to not spend to much time waiting for the main thread even in case of some Gui-Element or interaction flooding the
 input queues.

 \sa SparkProperty::Property
*/
class PropertyExecuter : public QObject, public ISimControl
{
    Q_OBJECT

public:
    //public functions

    /*!
     \brief Constructor.

     \param parent parent property control
    */
    PropertyExecuter(PropertyControl* parent);
    /*!
     \brief Destructor.
    */
    ~PropertyExecuter();

    /*!
     \brief Queue a get access for a property.

     As soon as the executer is called by the simulation server a get call for this property is invoked in the parent PropertyControl.
     \param prop
    */
    void queueGet(boost::shared_ptr<Property> prop);
    /*!
     \brief Queue a set access for a property.

     As soon as the executer is called by the simulation server a set call for this property is invoked in the parent PropertyControl.
     \param prop
    */
    void queueSet(boost::shared_ptr<Property> prop);

protected:
    //protected functions

    /*!
     \brief Executes queued get and set accesses.

     Called by endCycle() and waitCycle().
    */
    void onCycle();

protected:
    //protected virtual functions

    /*!
     \brief Called when a simulation cycle ends.
    */
    virtual void endCycle();
    /*!
     \brief Called when the simulation performs a wait-cycle.
    */
    virtual void waitCycle();

public: signals:
    //public signals

    /*!
     \brief Emitted when a write update for a property was accepted.

     Sent by the simulation server thread.
     Caught by the parent SparkProperty::PropertyControl in the controls thread.
     \param prop property that was accepted
    */
    void acceptChange(boost::shared_ptr<SparkProperty::Property> prop);
    /*!
     \brief Emitted when a write update for a property was rejected.

     Sent by the simulation server thread.
     Caught by the parent SparkProperty::PropertyControl in the controls thread.
     \param prop property that was accepted
    */
    void rejectChange(boost::shared_ptr<SparkProperty::Property> prop);
    /*!
     \brief Emitted when a read update for a property finished.

     Sent by the simulation server thread.
     Caught by the parent SparkProperty::PropertyControl in the controls thread.
     \param prop property that was updated
     \param success true if updating was successful, false otherwise
    */
    void updated(boost::shared_ptr<SparkProperty::Property> prop, bool success);

private:
    //private members

    PropertyControl* mParent; /*!< Parent property control that contains the set and get functions. */

    std::list<boost::shared_ptr<Property>> mGetQueue; /*!< Queue of properties to Get-update. */
    std::list<boost::shared_ptr<Property>> mSetQueue; /*!< Queue of properties to Set-update. */

    QSemaphore mSetSemaphore; /*!< Semaphore for Get-Queue. */
    QSemaphore mGetSemaphore; /*!< Semaphore for Set-Queue. */

    int mFailedSetCount; /*!< Amount of failed attempts to access Set-Queue. Resetted after successful attempt. */
    int mFailedGetCount; /*!< Amount of failed attempts to access Set-Queue. Resetted after successful attempt. */
    int mSetWait; /*!< Wait time in ms for the next attempt to acquire access to the Set-Queue. */
    int mGetWait; /*!< Wait time in ms for the next attempt to acquire access to the Set-Queue. */
};

} //namespace SparkProperty

#endif // SPARKPROPERTYEXECUTER_H
