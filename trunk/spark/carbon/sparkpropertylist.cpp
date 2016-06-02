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

#include "sparkpropertylist.h"
#include "sparkpropertymanager.h"
#include "carbon.h"
#include "sparkcontroller.h"
#include "simspark.h"
#include "sparkpropertycontrolbasic.h"

#include <zeitgeist/leaf.h>
#include <QTableWidget>

using namespace boost;
using namespace zeitgeist;

namespace SparkProperty
{

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

PropertyList::PropertyList()
{
}

PropertyList::~PropertyList()
{
    clearProperties();
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

boost::shared_ptr<SimSpark> PropertyList::getSpark()
{
    if (mSparkController.get() == 0)
    {
        LOG_ERROR() << "No spark controller";
        return boost::shared_ptr<SimSpark>();
    }

    return mSparkController->getSpark();
}

void PropertyList::updateProperties()
{
    for (auto it = mProperties.begin(); it != mProperties.end(); it++)
    {
        mPropertyManager->readProperty(*it);
    }
}

void PropertyList::clearProperties()
{
    mProperties.clear();
}

void PropertyList::init(boost::weak_ptr<zeitgeist::Leaf> leaf, boost::shared_ptr<SparkController> controller)
{
    mLeaf.reset();

    shared_ptr<Leaf> locked = leaf.lock();
    if (locked.get() == 0)
        return;

    mLeaf = leaf;
    mSparkController = controller;

    //Create PropertyManager
    mPropertyManager = shared_ptr<PropertyManager>(new PropertyManager(this, controller));

    //Add PropertyControl for default classes and call init for extensions
    mPropertyManager->registerPropertyControl(boost::shared_ptr<BasicControl>(new BasicControl(&*mPropertyManager)));
    mPropertyManager->init();
}

void PropertyList::refresh(boost::shared_ptr<Leaf> leaf)
{
    mLeaf = leaf;
    refresh();
}

boost::shared_ptr<Property> PropertyList::getProperty(int index) const
{
    if (index < 0 || index >= (int)mProperties.size())
    {
        LOG_ERROR() << "Property index out of range: " << index;
        return boost::shared_ptr<Property>();
    }

    return mProperties.at(index);    
}

int PropertyList::getSize() const
{
    return mProperties.size();
}

const TProperties& PropertyList::getProperties() const
{
    return mProperties;
}

const shared_ptr<const PropertyManager> PropertyList::getPropertyManager() const
{
    return mPropertyManager;
}

void PropertyList::readProperty(int index)
{
    if (index < 0 || index >= (int)mProperties.size())
    {
        LOG_ERROR() << "Property index out of range: " << index;
        return;
    }

    mPropertyManager->readProperty(mProperties.at(index));
}

void PropertyList::writeProperty(int index)
{
    if (index < 0 || index >= (int)mProperties.size())
    {
        LOG_ERROR() << "Property index out of range: " << index;
        return;
    }

    mPropertyManager->writeProperty(mProperties.at(index));
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void PropertyList::refresh()
{
    boost::shared_ptr<Leaf> locked = mLeaf.lock();
    if (locked.get() == 0)
        return;
        
    if (mPropertyManager.get() == 0)
        return;

    //Get supported class list
    mPropertyManager->getClassList(locked, mClassList);

    //Generate properties
    clearProperties();
    mPropertyManager->genProperties(locked, mClassList, mProperties);

    //Register to property update notification
    for (auto it = mProperties.begin(); it != mProperties.end(); it++)
    {
        it->get()->setParent(this);
    }
}

void PropertyList::updatePropertyChanged(int index)
{
    if (index < 0 || index >= (int)mProperties.size())
    {
        LOG_ERROR() << "Index " << index << " out of bounds.";
        return;
    }

    emit propertyChanged(index);
}

void PropertyList::updatePropertyChanged(Property* entry)
{
    int index = -1;
    for (auto it = mProperties.begin(); it != mProperties.end(); it++)
    {
        index++;
        if (&**it == entry)
        {
            updatePropertyChanged(index);
            return;
        }
    }

    //Property was already removed, ignore
    //LOG_DEBUG() << "Property pointer not found in list.";
}

//--------------------------------------------------------------
// Protected Slots
//--------------------------------------------------------------

void PropertyList::notifyPropertyChanged(Property* prop)
{
    updatePropertyChanged(prop);
}

} //namespace SparkProperty
