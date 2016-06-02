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

#include "plugin.h"
#include "pluginmanager.h"
#include "menumanager.h"
#include "carbon.h"
#include "settings.h"
#include "plugininstantiatormodel.h"

#include <iostream>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

PluginFactory* PluginFactory::mFactory = 0;

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

AbstractPlugin::AbstractPlugin(EPluginType type) :
    mPluginDefinition(boost::shared_ptr<PluginDefinition>())
{
    mPluginType = type;
    mPluginId = 0; //initialized in initAbstractPlugin
    mCaption = "";
    mHasVisualRepresentation = false;
    mIsExtension = false;
    mExecutionType = ET_ANY;
    mParent = 0;
}

void AbstractPlugin::initAbstractPlugin(Carbon* carbon, int pluginId)
{
    if (mParent != 0 && mParent != carbon)
        LOG_FATAL() << "Multiple definition of different parent carbon pointers for abstract plugin.";

    mPrivateSettings = carbon->getSettings();
    mParent = carbon;

    mPluginId = pluginId;
}

AbstractPlugin::~AbstractPlugin()
{
    //Remove plugin from Manager
    Carbon::get()->getPluginManager()->notifyDestruction(mPluginId);
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

int AbstractPlugin::getPluginId() const
{
    return mPluginId;
}

const QString& AbstractPlugin::getCaption() const
{
    return mCaption;
}

void AbstractPlugin::setCaption(const QString& caption)
{
    if (mCaption.compare(caption) == 0)
        return;

    mCaption = caption;

    //Update creator definition
    if (mPluginDefinition.get())
        mPluginDefinition->setCaption(caption);
}

bool AbstractPlugin::hasVisualRepresentation() const
{
    return mHasVisualRepresentation;
}

QWidget* AbstractPlugin::getVisualRepresentation() const
{
    LOG_WARNING() << "Plugin does not have a visual representation. Check with hasVisualRepresentation() first!";
    return 0;
}

bool AbstractPlugin::isQObject() const
{
    return mIsQObject;
}

QObject* AbstractPlugin::getQObject() const
{
    LOG_WARNING() << "Plugin is not a QObject. Check with isQObject() first!";
    return 0;
}

int AbstractPlugin::getInstanceIndex() const
{
    return Carbon::get()->getPluginManager()->getInstanceIndex(*this);
}

int AbstractPlugin::getMaxInstances() const
{
    return Carbon::get()->getPluginManager()->getMaxInstanceCount(getClassId());
}

EPluginType AbstractPlugin::getPluginType() const
{
    return mPluginType;
}

QString AbstractPlugin::getPluginTypeString() const
{
    return PluginType::getTypeString(mPluginType);
}

bool AbstractPlugin::isExtension() const
{
    return mIsExtension;
}

void AbstractPlugin::setExecutionType(AbstractPlugin::EExecutionType type)
{
    mExecutionType = type;
}

AbstractPlugin::EExecutionType AbstractPlugin::getExecutionType() const
{
    return mExecutionType;
}

void AbstractPlugin::setPluginDefinition(boost::shared_ptr<PluginDefinition> definition)
{
    mPluginDefinition = definition;
    mPluginDefinition->setId(getPluginId());
}

boost::shared_ptr<PluginDefinition> AbstractPlugin::getPluginDefinition() const
{
    return mPluginDefinition;
}

QVariant AbstractPlugin::getClassInfo(EPluginInfo info)
{
    return Carbon::get()->getPluginManager()->getPluginClassInfo(getClassId(), info);
}

QVariant AbstractPlugin::getClassInfo(const QString& name)
{
    return Carbon::get()->getPluginManager()->getPluginClassInfo(getClassId(), name);
}

QVariant AbstractPlugin::getClassInfo(int classId, EPluginInfo info)
{
    return Carbon::get()->getPluginManager()->getPluginClassInfo(classId, info);
}

QVariant AbstractPlugin::getClassInfo(int classId, const QString& name)
{
    return Carbon::get()->getPluginManager()->getPluginClassInfo(classId, name);
}

QString AbstractPlugin::getClassInfoName(EPluginInfo info)
{
    switch (info)
    {
    case PI_DESCRIPTION: return "iDescription"; break;
    case PI_TAGS:        return "iTags"; break;
    case PI_ICON_NAME:   return "iIconName"; break;
    default:
        LOG_ERROR() << "Class info field id " << info << " out of range.";
        break;
    }
    return "iERROR";
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void AbstractPlugin::setClassInfo(int classId, EPluginInfo info, const QVariant& value)
{
    Carbon::get()->getPluginManager()->setPluginClassInfo(classId, info, value);
}

void AbstractPlugin::setClassInfo(int classId, const QString& field, const QVariant& value)
{
    Carbon::get()->getPluginManager()->setPluginClassInfo(classId, field, value);
}

void AbstractPlugin::setClassInfo(EPluginInfo info, const QVariant& value)
{
    Carbon::get()->getPluginManager()->setPluginClassInfo(getClassId(), info, value);
}

void AbstractPlugin::setClassInfo(const QString& field, const QVariant& value)
{
    Carbon::get()->getPluginManager()->setPluginClassInfo(getClassId(), field, value);
}

void AbstractPlugin::setIsExtension(bool extension)
{
    mIsExtension = extension;

    if (mPluginDefinition.get())
        mPluginDefinition->setIsExtension(extension);
}

PluginManager* AbstractPlugin::getPluginManager()
{
    LOG_ASSERT(mParent);
    return mParent->getPluginManager();
}

Carbon* AbstractPlugin::getCarbon()
{
    LOG_ASSERT(Carbon::get());

    if (mParent == 0)
        mParent = Carbon::get();

    return mParent;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

//--------------------------------------------------------------
// AbstractPluginInstantiator
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

AbstractPluginInstantiator::AbstractPluginInstantiator(int id, const QString& name, const QString& caption, EPluginType type, 
    int maxInstanceCount, const char* library) :
    mName(name),
    mDefaultCaption(caption),
    mPluginType(type),
    mId(id),
    mMaxInstanceCount(maxInstanceCount),
    mLibrary(library)
{
}

AbstractPluginInstantiator::~AbstractPluginInstantiator()
{

}

const QString& AbstractPluginInstantiator::getName() const
{
    return mName;
}

const QString& AbstractPluginInstantiator::getCaption() const
{
    return mDefaultCaption;
}

const QString& AbstractPluginInstantiator::getLibrary() const
{
    return mLibrary;
}

int AbstractPluginInstantiator::getId() const
{
    return mId;
}

EPluginType AbstractPluginInstantiator::getPluginType() const
{
    return mPluginType;
}

int AbstractPluginInstantiator::getMaxInstanceCount() const
{
    return mMaxInstanceCount;
}

//--------------------------------------------------------------
// PluginFactory
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

bool PluginFactory::registerPluginInstantiator(int classId, AbstractPluginInstantiator* instantiator)
{
    //This is called before the logging system is initialized, therefore store the error messages and request them later
    QString errorMessage = "";

    auto pos = mInstantiatorMap.find(classId);
    if (pos != mInstantiatorMap.end())
    {
        errorMessage = QString("Plugin class with class id '%1' was already added to the instantiator list.\n").arg(classId);
        PluginFactory::addErrorMessage(errorMessage);
        return false;
    }

    QString name = instantiator->getName();
    int foundId = getClassIdByName(name);
    if (foundId != 0)
    {
        errorMessage = QString("Plugin class with class name '%1' was already added to the instantiator list with id '%2'.\n").arg(name).arg(foundId);
        PluginFactory::addErrorMessage(errorMessage);
        return false;
    }

    //Register the new instantiator
    mInstantiatorMap.insert(std::make_pair(classId, instantiator));
    return true;
}

AbstractPlugin* PluginFactory::getPluginInstance( int classId) 
{
    if (! canCreatePlugin(classId))
        return 0;

    TInstantiatorMap::iterator it = mInstantiatorMap.find(classId);

    //Instantiate
    AbstractPluginInstantiator* instantiator = it->second;
    AbstractPlugin* newPlugin = instantiator->instantiate();

    //Post-init plugin parameters
    newPlugin->initAbstractPlugin(Carbon::get(), Carbon::get()->getPluginManager()->getNextPluginId()); //Carbon pointer and plugin id
    newPlugin->setCaption(getClassNameById(classId)); // plugin caption
    return newPlugin;
}

int PluginFactory::getMaxInstanceCount(int classId)
{
    TInstantiatorMap::iterator it = mInstantiatorMap.find(classId);
    if (it == mInstantiatorMap.end())
    {
        LOG_WARNING() << "Could not find class with id " << classId << ".";
        return 0;
    }

    return it->second->getMaxInstanceCount();
}

bool PluginFactory::canCreatePlugin(int classId, bool logError)
{
    TInstantiatorMap::iterator it = mInstantiatorMap.find(classId);
    if (it == mInstantiatorMap.end())
    {
        //Instantiator not found
        if (logError)
            LOG_ERROR() << "Cant create plugin with class id " << classId << ": Class instantiator not found.";
        return false;
    }

    int maximumCount = it->second->getMaxInstanceCount();
    int actualCount = Carbon::get()->getPluginManager()->getInstanceCount(classId);

    if (actualCount >= maximumCount)
    {
        if(logError)
            LOG_ERROR() << "Cant create plugin with class id " << classId << ": Maximum amount of instances reached.";
        return false;
    }

    return true;
}

int PluginFactory::getClassIdByName(const QString& name)
{
    for (TInstantiatorMap::iterator it = mInstantiatorMap.begin(); it != mInstantiatorMap.end(); it++)
    {
        if (it->second->getName().compare(name) == 0) return it->first;
    }
    return 0;
}

QString PluginFactory::getClassNameById(int id)
{
    TInstantiatorMap::iterator pos =  mInstantiatorMap.find(id);
    if (pos == mInstantiatorMap.end()) return QString("");

    return pos->second->getName();
}

QString PluginFactory::getSourceLibraryById(int id)
{
    TInstantiatorMap::iterator pos =  mInstantiatorMap.find(id);
    if (pos == mInstantiatorMap.end()) return QString("");

    return pos->second->getLibrary();
}

QString PluginFactory::getSourceLibraryByName(const QString& name)
{
    int id = getClassIdByName(name);
    
    if (id > 0)
        return getSourceLibraryById(id);
    return "";
}

bool PluginFactory::containsClass(const QString& name)
{
    return getClassIdByName(name) != 0;
}

boost::shared_ptr<PluginInstantiatorTableModel> PluginFactory::getTableModel()
{
    return boost::shared_ptr<PluginInstantiatorTableModel>(new PluginInstantiatorTableModel(mInstantiatorMap));
}

void PluginFactory::addErrorMessage(const QString& message)
{
    //Store and display error message
    if (Carbon::isInitialized())
    {
        LOG_ERROR() << message;
    }
    else
    {
        std::cerr << message.toStdString();
    }

    mErrorMessages.push_back(message);
}

EPluginType PluginFactory::getPluginType(int classId)
{
    TInstantiatorMap::iterator pos =  mInstantiatorMap.find(classId);
    if (pos == mInstantiatorMap.end()) return PT_NONE;

    return pos->second->getPluginType();
}

const PluginFactory::TInstantiatorMap& PluginFactory::getInstantiators()
{
    return mInstantiatorMap;
}

PluginFactory& PluginFactory::getFactory()
{
    if (mFactory == 0)
    {
        mFactory = new PluginFactory();
    }
    return *mFactory;
}

void PluginFactory::registerStaticFactory(PluginFactory* factory)
{
    if (PluginFactory::mFactory == 0)
        PluginFactory::mFactory = factory;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------
