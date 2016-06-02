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

#include "sparkpropertycontrol.h"
#include "sparkpropertyexecuter.h"
#include "sparkcontroller.h"
#include "sparkpropertymanager.h"
#include "guisimcontrol.h"

namespace SparkProperty
{

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

PropertyControl::PropertyControl(PropertyManager* parent, const QString& name, boost::shared_ptr<PropertyExecuter> executer) 
    : mParent(parent), mName(name), mExecuter(executer)
{
}

PropertyControl::PropertyControl(PropertyManager* parent, const QString& name) 
    : mParent(parent), mName(name)
{
    if (parent->mSparkController.get())
    {
        createExecuter(parent->mSparkController);
    }
}

PropertyControl::~PropertyControl()
{
    unregisterExecuter();
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

void PropertyControl::createExecuter(boost::shared_ptr<SparkController> controller)
{
    mSparkController = controller;

    if (mExecuter.get() != 0)
    {
        mExecuter.reset();
    }

    boost::shared_ptr<GuiSimControl> simcontrol = controller->getSimulationControl().lock();
    if (simcontrol.get() == 0)
    {
        LOG_ERROR() << "Cant create PropertyExecuter. Gui SimulationControl was deleted.";
    }
    else
    {
        mExecuter = boost::shared_ptr<PropertyExecuter>(new PropertyExecuter(this));
        simcontrol->registerSimControl(mExecuter);
    }
}

void PropertyControl::unregisterExecuter()
{
    if (mExecuter.get())
    {
        if (mSparkController.get())
        {
            boost::shared_ptr<GuiSimControl> simcontrol = mSparkController->getSimulationControl().lock();
            if (simcontrol.get() != 0)
                simcontrol->unregisterSimControl(mExecuter);
        }
        else
            LOG_ERROR() << "Cant unregister executer. Spark-controller not specified.";
    }
}

void PropertyControl::queueRead(boost::shared_ptr<Property>& prop)
{
    if (mExecuter.get())
    {
        mExecuter->queueGet(prop);
    }
    else
        LOG_ERROR() << "Cant read property. No PropertyExecuter initialized.";
}

void PropertyControl::queueWrite(boost::shared_ptr<Property>& prop)
{
    if (mExecuter.get())
    {
        mExecuter->queueSet(prop);
    }
    else
        LOG_ERROR() << "Cant read property. No PropertyExecuter initialized.";
}

const QString& PropertyControl::getName() const
{
    return mName;
}

//--------------------------------------------------------------
// Protected slots
//--------------------------------------------------------------

void PropertyControl::setPropertyValueProtected(Property& prop, Data* newValue) const
{
    prop.setValueProtected(newValue);
}

void PropertyControl::acceptChange(boost::shared_ptr<SparkProperty::Property> prop)
{
    prop->acceptTempValue();
}

void PropertyControl::rejectChange(boost::shared_ptr<SparkProperty::Property> prop)
{
    prop->rejectTempValue();
}

void PropertyControl::updated(boost::shared_ptr<SparkProperty::Property> prop, bool ok)
{
    prop->updateFinished(ok);
}

} //namespace SparkProperty

