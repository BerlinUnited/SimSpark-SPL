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

#include "sparkpropertymanager.h"
#include "sparkpropertylist.h"
#include "sparkcontroller.h"
#include "sparkpropertydata.h"
#include "sparkpropertycontrol.h"
#include "simspark.h"

using namespace boost;
using namespace zeitgeist;

namespace SparkProperty
{

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

PropertyManager::PropertyManager(PropertyList* parent, shared_ptr<SparkController> controller) :
    mParent(parent), mSparkController(controller)
{
}

PropertyManager::~PropertyManager()
{
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

void PropertyManager::init()
{
    emit registerPropertyControlRequest();
}

void PropertyManager::genProperties(TLeafPtr leaf, TClassList& classList, TProperties& propList) const
{
    propList.clear();

    if (leaf.get() == 0)
    {
        return;
    }

    //Iterate all classes (backwards)
    for (auto class_it = classList.rbegin(); class_it != classList.rend(); ++class_it)
    {
        //Find supporting PropertyControl for this class (the control may register itself as default control)
        boost::shared_ptr<PropertyControl> supporter(getSupportingControl(*class_it));

        if (supporter.get())
        {
            //Generate properties for this class (automatically queues a readProperty-access)
            supporter->genProperties(leaf, *class_it, propList);
        }
        else
        {
            LOG_ERROR() << "No PropertyControl for class " << class_it->mName << " found."; 
        }
    }
}

bool PropertyManager::writeProperty(boost::shared_ptr<Property>& prop)
{
    boost::shared_ptr<zeitgeist::Leaf> leaf(prop->mLeaf.lock());
    if (leaf.get() == 0)
    {
        LOG_INFO() << "Entry leaf does not exist anymore.";
        return false;
    }

    //Was property changed?
    if (prop->getTempValue().get() == 0)
        return true; // nothing to update

    //Set update flag
    bool started = prop->startUpdate(UF_WRITING);
    if (!started)
        return false;

    //Get control that provides this property
    boost::shared_ptr<PropertyControl> supporter(getSupportingControl(prop->mClass));

    if (supporter.get())
    {
        //Let the PropertyControl queue a write access (simulation update)
        supporter->queueWrite(prop);
    }
    else
    {
        LOG_ERROR() << "No PropertyControl for property " << prop->mCaption << " found."; 
        return false;
    }

    return true;
}

bool PropertyManager::readProperty(boost::shared_ptr<Property>& prop)
{
    boost::shared_ptr<zeitgeist::Leaf> leaf(prop->mLeaf.lock());
    if (leaf.get() == 0)
    {
        LOG_INFO() << "Entry leaf does not exist anymore.";
        return false;
    }

    //Get control that provides this property
    boost::shared_ptr<PropertyControl> supporter(getSupportingControl(prop->mClass));

    //Set update flag
    bool started = prop->startUpdate(UF_READING);
    if (!started)
        return false;

    if (supporter.get())
    {
        //Let the PropertyControl queue a read access (property update)
        supporter->queueRead(prop);
    }
    else
    {
        LOG_ERROR() << "No PropertyControl for property " << prop->mCaption << " found."; 
        return false;
    }

    return true;
}

boost::shared_ptr<PropertyControl> PropertyManager::getSupportingControl(ClassDescriptor& descriptor) const
{
    //Find supporting PropertyControl for this class
    for (auto con_it = mControls.begin(); con_it != mControls.end(); con_it++)
    {
        if (con_it->get()->supports(descriptor))
        {
            return *con_it;
        }
    }

    return boost::shared_ptr<PropertyControl>();
}

void PropertyManager::getClassList(TLeafPtr leaf, TClassList& classList) const
{
    classList.clear();

    if (leaf.get() == 0)
        return;

    boost::shared_ptr<SimSpark> spark = mParent->getSpark();
    if (spark.get() == 0)
        return;

    boost::shared_ptr<zeitgeist::Class> classPtr = leaf->GetClass();
    if (classPtr.get() == 0)
        return;

    getClassListPrivate(classPtr, classList, spark);
}

void PropertyManager::registerPropertyControl(boost::shared_ptr<PropertyControl> control)
{
    bool found = false;
    for (auto it = mControls.begin(); it != mControls.end(); it++)
    {
        if (&**it == &*control)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        LOG_DEBUG() << "Registered PropertyControl " << control->getName();
        mControls.push_back(control);
    }
    else
    {
        LOG_DEBUG() << "PropertyControl " << control->getName() << " already registered.";
    }
}

void PropertyManager::unregisterPropertyControl(boost::shared_ptr<PropertyControl> simControl)
{
    unregisterPropertyControl(&*simControl);
}

void PropertyManager::unregisterPropertyControl(PropertyControl* control)
{
    bool found = false;
    for (auto it = mControls.begin(); it != mControls.end(); it++)
    {
        if (&**it == control)
        {
            mControls.erase(it);
            found = true;
            break;
        }
    }

    if (!found)
        LOG_DEBUG() << "Cant unregister PropertyControl " << control->getName() << ": control not found.";
    else
        LOG_DEBUG() << "Unregistered PropertyControl.";
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void PropertyManager::getClassListPrivate(TClassPtr classPtr, TClassList& classList, const boost::shared_ptr<SimSpark>& spark) const
{
    std::string path = classPtr->GetFullPath();

    //Get all direct base classes
    zeitgeist::Class::TStringList baseClasses = classPtr->GetBaseClasses();

    //Check if class is supported
    PropertyControl* supporter = 0;
    for (auto con_it = mControls.begin(); con_it != mControls.end(); con_it++)
    {
        if (con_it->get()->supports(path.c_str()))
        {
            if (supporter != 0)
            {
                LOG_WARNING() << "Multiple supporter PropertyControl objects for class " << QString(path.c_str()) << 
                    ". First one: " << supporter->getName() << " This one: " << con_it->get()->getName();
            }
            else
                supporter = &**con_it;
        }
    }

    //Is this class supported?
    if (supporter != 0) 
    {
        //Add class to class list
        classList.push_back(ClassDescriptor(QString(path.c_str())));
    }

    //Recursively collect base classes
    for (auto base_it = baseClasses.begin(); base_it != baseClasses.end(); base_it++)
    {
        std::string basePath = "/classes/" + (*base_it);

        //Convert base class string to class object
        boost::shared_ptr<zeitgeist::Class> base = shared_dynamic_cast<Class>
            (spark->GetCore()->Get(basePath));

        if (base.get() == 0)
        {
            LOG_ERROR() << "Invalid base class " << QString(basePath.c_str())
                << " defined for " << QString(classPtr->GetFullPath().c_str()) << ".";
        }
        else
        {
            //Collect base classes
            getClassListPrivate(base, classList, spark);
        }
    }
}

} //namspace SparkProperty
