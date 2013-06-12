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

#include "pluginmanager.h"
#include "carbon.h"
#include "windowmanager.h"
#include "simulationmanager.h"
#include "simulation.h"
#include "simulationsetup.h"
#include "windowdefinition.h"
#include "attachableframe.h"
#include "plugin.h"
#include "signalplugin.h"
#include "numberedactionreceiver.h"
#include "settings.h"
#include "libraryloader.h"
#include "guisettings_pluginsettings.h"
#include "settingsdialog.h"

#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QFileDialog>

#define COLUMN_COUNT 5

using namespace boost;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

int PluginManager::mNextClassId = 1;
int PluginManager::getNextPluginId()
{
    int next = mNextPluginId;
    mNextPluginId++;
    return next;
}
int PluginManager::getNextClassId()
{
    int next = mNextClassId;
    mNextClassId++;
    return next;
}
void PluginManager::setNextClassId(int id)
{
    mNextClassId = (mNextClassId > id ? mNextClassId : id);
}

//--------------------------------------------------------------
// PluginManager
//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

PluginManager::PluginManager(Carbon* parent)
    : mSettings(parent->getSettings())
{
    mParent = parent;
    mLibraryLoader = 0;

    //Settings widget
    mPluginSettingsWidget = 0;
    mNextPluginId = 1;
}

PluginManager::~PluginManager()
{
    while (mPluginMap.size() > 0)
    {
        deletePlugin(mPluginMap.begin()->first);
    }
}

void PluginManager::initializeSettingWidget()
{
    if (mPluginSettingsWidget == 0)
    {
        mPluginSettingsWidget = new GuiSettings::PluginSettings(mSettings);
        Carbon::get()->getSettingsDialog()->includeSettingWidget(tr("Gui"), tr("Plugin Manager"),
            (SettingsWidget**)&mPluginSettingsWidget, 0, ":config");

        //Setting connections
        connect(this, SIGNAL(settingChanged(QString)), mPluginSettingsWidget, SLOT(updateSetting(QString)));
        connect(mPluginSettingsWidget, SIGNAL(settingChanged(QString)), this, SLOT(updateSetting(QString)));

        //Init values
        mPluginSettingsWidget->updateSetting("mPluginPaths");
        mPluginSettingsWidget->updateSetting("mPluginFiles");
    }
    else
    {
        LOG_WARNING() << "SimulationSettingsWidget already initialized.";
    }
}

bool PluginManager::init()
{
    //Default settings
    mExtensionFile = Carbon::getUserDataFolder() + "Extensions.xml";
    mDefaultPluginPath = "plugins/";
    mPluginIncludeDirectories.clear();
    mPluginIncludeFiles.clear();

    //Create extension dir
    QDir extensionDir(Carbon::getUserDataFolder());
    if (!extensionDir.exists())
    {
        bool success = QDir(QDir::rootPath()).mkpath(extensionDir.absolutePath());
        if (!success)
            LOG_ERROR() << "Could not find or create directory " << extensionDir.absolutePath() << " for 'Extensions.xml' file.";
    }

    //Load settings
    if (mSettings->beginGroup("gui_PluginManager", true)) //lock
    {
        mExtensionFile = mSettings->value("mExtensionFile", mExtensionFile).toString();
        mPluginIncludeDirectories    = mSettings->value("mPluginPaths", mPluginIncludeDirectories).toStringList();
        mPluginIncludeFiles          = mSettings->value("mPluginFiles", mPluginIncludeFiles).toStringList();
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to load PluginManager settings from. Using default values.";

    bool listChanged = false;

    //Add commandline paths
    for (auto it = mParent->mInitialPluginDirectories.begin(); it != mParent->mInitialPluginDirectories.end(); it++)
    {
        QDir dir(*it);
        if (!dir.exists())
        {
            LOG_WARNING() << "Plugin include directory " << *it << " not found.";
            continue;
        }

        if (mPluginIncludeDirectories.contains(*it))
            continue;

        mPluginIncludeDirectories.push_back(dir.absolutePath());
        listChanged = true;
    }

    //If include list empty, add default path
    if (mPluginIncludeDirectories.empty())
    {
        mPluginIncludeDirectories.push_back(mDefaultPluginPath);
        listChanged = true;
    }

    if (listChanged)
    {
        if (mSettings->beginGroup("gui_PluginManager", true)) //lock
        {
            mSettings->setValue("mPluginPaths", mPluginIncludeDirectories);
            mSettings->endGroup(true); //unlock
        }
        else
            LOG_ERROR() << "Could not access Settings object to load PluginManager settings from. Using default values.";
    }

    //Init LibraryLoader
    mLibraryLoader = new LibraryLoader();
    initLibraries();

    //Load extensions file
    mExtensions = mParent->getSimulationManager()->loadSimulationSetup(mExtensionFile);
    if (mExtensions.get() == 0)
    {
        LOG_WARNING() << "Could not find extension file. Creating a new one.";
        mExtensions = shared_ptr<SimulationSetup>(new SimulationSetup());
        mExtensions->setFileName(mExtensionFile);
        mParent->getSimulationManager()->saveSimulationSetup(mExtensions, true, false);
    }

    //Flag as extensions
    for (auto it = mExtensions->getAddPlugins().begin(); it != mExtensions->getAddPlugins().end(); it++)
    {
        it->get()->setIsExtension(true);
    }

    //Load extensions
    loadPlugins(*mExtensions);

    return true;
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

shared_ptr<PluginTableModel> PluginManager::getTableModel()
{
    return shared_ptr<PluginTableModel>(new PluginTableModel(mPluginMap));
}

bool PluginManager::loadPlugins(const SimulationSetup& setup)
{
    return loadPlugins(setup.getAddPlugins(), setup.getRemovePlugins(), setup.getReloadAll(), setup.getRemoveAll());
}

bool PluginManager::loadPlugins(const SimulationSetup::TPluginDefinitions& addListRef, const SimulationSetup::TPluginDefinitions& removeList, bool reloadall, bool removeall)
{
    //find plugins to keep
    std::map<int, std::pair<AbstractPlugin*, shared_ptr<PluginDefinition>>> keeplist;
    std::map<int, std::pair<AbstractPlugin*, shared_ptr<PluginDefinition>>> reloadlist;
    std::map<int, AbstractPlugin*> removelist;
    SimulationSetup::TPluginDefinitions addList = addListRef;

    //Iterate all plugins
    for (auto it = mPluginMap.begin(); it != mPluginMap.end(); it++)
    {
        AbstractPlugin* plugin = it->second;
        bool dontremove = false;

        //Check add list
        for (auto p_it = addList.begin(); p_it != addList.end(); )
        {
            if (p_it->get()->pluginMatches(plugin, false))
            {
                dontremove = true;
                //identical plugin, keep or reload
                if (p_it->get()->getReloadDefinition() == PluginDefinition::RD_RELOAD ||
                    (p_it->get()->getReloadDefinition() == PluginDefinition::RD_INHERIT && reloadall))
                    reloadlist.insert(std::make_pair(it->first, std::make_pair(it->second, *p_it)));
                else
                {
                    keeplist.insert(std::make_pair(it->first, std::make_pair(it->second, *p_it)));
                    it->second->setPluginDefinition(*p_it); //Update the creating definition
                }

                p_it = addList.erase(p_it);
            }
            else
                p_it++;
        }

        //Extensions are never deleted
        if (plugin->isExtension())
            dontremove = true;

        //check removelist
        if (!dontremove)
        {
            //All undefined plugins should be removed? Remove this!
            if (removeall)
                removelist.insert(std::make_pair(it->first, it->second));
            else
            for (SimulationSetup::TPluginDefinitions::const_iterator p_it = addList.begin(); p_it != addList.end(); p_it++)
            {
                //This is a plugin that matches a remove definition? Remove this!
                if (p_it->get()->pluginMatches(plugin, true))
                {
                    //definition matches, delete
                    removelist.insert(std::make_pair(it->first, it->second));
                    break;
                }
            }
        }
    }

    bool allGood = true;

    //Remove plugins in deletelist
    for (auto it = removelist.begin(); it != removelist.end(); it++)
    {
        bool success = deletePlugin(*it->second);
        if (!success)
            allGood = false;
    }

    //Reload plugins in reloadlist
    for (auto it = reloadlist.begin(); it != reloadlist.end(); it++)
    {
        AbstractPlugin* plugin = reloadPlugin(*it->second.first, &*it->second.second);
        if (!plugin)
            allGood = false;
        else
        {
            //Additional information
            plugin->setPluginDefinition(it->second.second);
        }
    }

    //Update plugins in keeplist
    for (auto it = keeplist.begin(); it != keeplist.end(); it++)
    {
        AbstractPlugin* plugin = reinitPlugin(*it->second.first, &*it->second.second);
        if (!plugin)
            allGood = false;
    }

    //Add plugins in addlist which havent been there already
    for (SimulationSetup::TPluginDefinitions::iterator p_it = addList.begin(); p_it != addList.end(); p_it++)
    {
        AbstractPlugin* plugin = createPlugin(**p_it);
        if (!plugin)
            allGood = false;
        else
        {
            //Additional information
            plugin->setIsExtension(p_it->get()->isExtension()); //update extension flag
        }
    }

    return allGood;
}

bool PluginManager::deletePlugin(AbstractPlugin& plugin, bool removeDefFromSimulation)
{
    return deletePlugin(plugin.getPluginId(), removeDefFromSimulation);
}

bool PluginManager::deletePlugin(int pluginId, bool removeDefFromSimulation)
{
    TPluginMap::iterator it = mPluginMap.find(pluginId);

    if (it == mPluginMap.end())
    {
        //Nothing to delete, return
        return false;
    }

    LOG_DEBUG() << "Deleting plugin with id " << pluginId << ".";

    return deletePlugin(it->second->getClassId(), pluginId, true, removeDefFromSimulation);
}

AbstractPlugin* PluginManager::reloadPlugin(AbstractPlugin& plugin, PluginDefinition* definition)
{
    TPluginMap::iterator it = mPluginMap.find(plugin.getPluginId());

    if (it == mPluginMap.end())
    {
        //Nothing to reload, return
        return 0;
    }

    LOG_DEBUG() << "Reloading plugin with id " << plugin.getPluginId() << " (" << plugin.getCaption() << ").";

    //Store data
    int classId = plugin.getClassId();
    QString className = plugin.getClassName();
    QString caption = plugin.getCaption();
    EPluginType pluginType = plugin.getPluginType();
    QStringList attachments;
    QStringList parameters = plugin.getPluginDefinition()->getParameters();
    if (pluginType == PT_FRAME)
    {
        WindowDefinition* window = mParent->getWindowManager()->getWindow(plugin.getPluginId());
        if (window == 0)
        {
            LOG_ERROR() << "Attachment of plugin " << plugin.getPluginId() << " (" << plugin.getCaption() << ") not found.";
            return 0;
        }
        attachments << window->getFrameAttachmentPoint()->getName();

        for (auto it = plugin.getPluginDefinition()->getAttachments().begin(); it != plugin.getPluginDefinition()->getAttachments().end(); it++)
        {
            if (!attachments.contains(*it))
                attachments << *it;
        }
    }

    //If a new definition is given, use the new parameters and attachments
    if (definition)
    {
        parameters = definition->getParameters();
        attachments = definition->getAttachments();
    }

    //Delete
    deletePlugin(plugin.getClassId(), plugin.getPluginId(), true);

    //Recreate
    if (pluginType == PT_NORMAL)
        return createSimplePlugin(className, caption, parameters);
    if (pluginType == PT_SIGNAL)
        return createSignalPlugin(className, caption, parameters);
    if (pluginType == PT_FRAME)
        return createAttachableFrame(className, caption, attachments, parameters);

    return 0;
}

AbstractPlugin* PluginManager::reinitPlugin(AbstractPlugin& plugin, PluginDefinition* definition)
{
    TPluginMap::iterator it = mPluginMap.find(plugin.getPluginId());

    if (it == mPluginMap.end())
    {
        //Not in managed list, return
        return 0;
    }

    LOG_DEBUG() << "Reinitializing plugin with id " << plugin.getPluginId() << " (" << plugin.getCaption() << ").";

    //If a new definition is given, use the new parameters and attachments
    if (definition)
    {
        plugin.getPluginDefinition()->mParameterList = definition->getParameters();
        plugin.getPluginDefinition()->mAttachmentList = definition->getAttachments();
    }

    //Frames could be reattached
    if (plugin.getPluginType() == PT_FRAME)
    {
        AttachableFrame& frame = (AttachableFrame&)plugin;
        WindowDefinition* window = mParent->getWindowManager()->getWindow(plugin.getPluginId());
        if (window == 0)
        {
            LOG_ERROR() << "Attachment of plugin " << plugin.getPluginId() << " (" << plugin.getCaption() << ") not found.";
            return 0;
        }

        //New attachment defined?
        QString attachment = window->getFrameAttachmentPoint()->getName();
        QString newAttachment = attachment;
        if (plugin.getPluginDefinition()->mAttachmentList.size() > 0)
            newAttachment = plugin.getPluginDefinition()->mAttachmentList.at(0);

        //Reattach
        if (newAttachment.compare(attachment) != 0)
            mParent->getWindowManager()->changeAttachment(&frame, newAttachment);
    }

    //Reinitialize
    plugin.init(plugin.getPluginDefinition()->mParameterList);

    return &plugin;
}

AbstractPlugin* PluginManager::createPlugin(const PluginDefinition& definition, bool addDefToSimulation)
{
    return createPlugin(shared_ptr<PluginDefinition>(new PluginDefinition(definition)), addDefToSimulation);
}

AbstractPlugin* PluginManager::createPlugin(shared_ptr<PluginDefinition> definition, bool addDefToSimulation)
{
    //Check if plugin exists
    bool exists = PluginFactory::getFactory().containsClass(definition->getName());
    if (!exists)
    {
        //Link plugin library if neccessary
        int id = mLibraryLoader->search(definition->getLibraryName());
        if (id != 0)
        {
            bool loaded = mLibraryLoader->load(id);
            if (!loaded)
                LOG_ERROR() << "Failed to load library for plugin " << definition->getName();
            else
            {
                bool exported = mLibraryLoader->exportPlugins(id);
                if (!exported)
                {
                    LOG_ERROR() << "Plugin export function for plugin " << definition->getName() << " not found.";
                }
            }
        }

        //Check again
        exists = PluginFactory::getFactory().containsClass(definition->getName());
        if (!exists)
        {
            LOG_ERROR() << "Plugin class " << definition->getName() << " does not exist.";
            return 0;
        }
    }

    //Create the plugin
    AbstractPlugin* plugin = 0;
    if (definition->getPluginType() == PT_NORMAL)
    {
        plugin = createSimplePlugin(definition->getName(), definition->getCaption(), definition->getParameters());
    }
    else if (definition->getPluginType() == PT_SIGNAL)
    {
        plugin = createSignalPlugin(definition->getName(), definition->getCaption(), definition->getParameters());
    }
    else if (definition->getPluginType() == PT_FRAME)
    {
        plugin = createAttachableFrame(definition->getName(), definition->getCaption(), definition->getAttachments(), definition->getParameters());
    }

    if (plugin != 0)
    {
        //Update plugin definition (also updates id of the definition to the plugins id)
        plugin->setPluginDefinition(definition);

        //Add new plugin definition to simulation setup of current simulation (as copy, not with the same shared pointer)
        if (addDefToSimulation)
        {
            shared_ptr<PluginDefinition> copiedDefinition(new PluginDefinition(*definition));

            SimulationManager* simManager = mParent->getSimulationManager();
            simManager->getSimulation()->getSetupNc()->insertAddPlugin(copiedDefinition);
            simManager->getSimulation()->updatePluginListChanged();
        }
    }

    return plugin;
}

Plugin* PluginManager::createSimplePlugin(const QString& name, const QString& caption, const QStringList& parameters)
{
    LOG_DEBUG() << "Trying to create simple plugin " << caption << " of class " << name << ".";

    int classId = PluginFactory::getFactory().getClassIdByName(name);
    if (classId == 0)
    {
        LOG_ERROR() << "Plugin class with name " << name << " not found. New plugins have to be specfied in their own .cpp/.h files," <<
            " and have to register with the PluginFactory using the registration Macro. See Plugin documentation for more info.";
        return 0;
    }

    EPluginType type = PluginFactory::getFactory().getPluginType(classId);
    if (type != PT_NORMAL)
    {
        LOG_ERROR() << "Plugin class with name " << name << " could not be created because it is no Simple Plugin but has type '" << PluginType::getTypeString(type).toStdString().c_str() << "'.";
        return 0;
    }

    //Create SignalPlugin
    if (!PluginFactory::getFactory().canCreatePlugin(classId, true))
        return 0;

    LOG_DEBUG() << "Creating simple plugin with name " << name << " and caption " << caption << ".";
    Plugin* plugin = dynamic_cast<Plugin*>(PluginFactory::getFactory().getPluginInstance(classId));
    if (plugin == 0)
    {
        LOG_ERROR() << "Plugin class with name " << name << " could not be instantiated. Factory returned 0.";
        return 0;
    }
    LOG_DEBUG() << "Created plugin with name " << name << " and assigned id: " << plugin->getPluginId();
    plugin->setCaption(caption);

    //Store plugin
    mPluginMap.insert(std::make_pair(plugin->getPluginId(), plugin));

    //Calling init function
    plugin->init(parameters);

    //Store instantiation info
    addInstanceIndex(plugin->getClassId(), plugin->getPluginId());
    emit pluginAdded(plugin->getPluginId());
    emit pluginAdded((AbstractPlugin*)plugin);
    emit pluginOfClassAdded(plugin->getClassId());

    return plugin;
}

SignalPlugin* PluginManager::createSignalPlugin(const QString& name, const QString& caption, const QStringList& parameters)
{
    LOG_DEBUG() << "Trying to create signalplugin " << caption << " of class " << name << ".";

    int classId = PluginFactory::getFactory().getClassIdByName(name);
    if (classId == 0)
    {
        LOG_ERROR() << "Plugin class with name " << name << " not found. New plugins have to be specfied in their own .cpp/.h files," <<
            " and have to register with the PluginFactory using the registration Macro. See Plugin documentation for more info.";
        return 0;
    }

    EPluginType type = PluginFactory::getFactory().getPluginType(classId);
    if (type != PT_SIGNAL)
    {
        LOG_ERROR() << "Plugin class with name " << name << " could not be created because it is no Signal Plugin but has type '" << PluginType::getTypeString(type).toStdString().c_str() << "'.";
        return 0;
    }

    //Create SignalPlugin
    LOG_DEBUG() << "Creating signal plugin with name " << name << " and caption " << caption << ".";
    SignalPlugin* plugin = dynamic_cast<SignalPlugin*>(PluginFactory::getFactory().getPluginInstance(classId));
    if (plugin == 0)
    {
        LOG_ERROR() << "SignalPlugin class with name " << name << " could not be instantiated. Factory returned 0.";
        return 0;
    }
    LOG_DEBUG() << "Created signal plugin with name " << name << " and assigned id: " << plugin->getPluginId();
    plugin->setCaption(caption);

    //Store plugin
    mPluginMap.insert(std::make_pair(plugin->getPluginId(), plugin));

    //Calling init function
    plugin->init(parameters);

    //Store instantiation info
    addInstanceIndex(plugin->getClassId(), plugin->getPluginId());
    emit pluginAdded(plugin->getPluginId());
    emit pluginAdded((AbstractPlugin*)plugin);
    emit pluginOfClassAdded(plugin->getClassId());

    return plugin;
}

AttachableFrame* PluginManager::createAttachableFrame(const QString& name, const QString& caption, const QStringList& targets, const QStringList& parameters)
{
    LOG_DEBUG() << "Trying to create plugin " << caption << " of class " << name << " at attachmentpoint " <<
        (targets.size() > 0 ? targets.at(0) : " <undefined>.");

    if (targets.size() == 0)
    {
        LOG_WARNING() << "No attachment targets defined for Plugin with class name " << name << ". Using default attachments.";
        FrameAttachmentPoint* point = mParent->getWindowManager()->findDefaultFrameAttachmentPoint(true);
        if (!point)
        {
            LOG_ERROR() << "could not create plugin. No free default attachment point found.";
            return 0;
        }
        return createAttachableFrame(name, caption, point->getName(), parameters);
    }

    QString targetlist;
    for (int i = 0; i < targets.size(); i++)
    {
        if (mParent->getWindowManager()->canAttach(targets.at(i)) || targets.size() == 1)
            return createAttachableFrame(name, caption, targets.at(i));

        targetlist.append(targets.at(i));
        if (i != targets.size() -1)
            targetlist.append(", ");
    }

    LOG_ERROR() << "Could not create Plugin class with name " << name << " because none of the attachment points specified could be found: " << targetlist;
    return 0;
}

AttachableFrame* PluginManager::createAttachableFrame(const QString& name, const QString& caption, const QString& target, const QStringList& parameters)
{
    if (!mParent->getWindowManager()->canAttach(target))
    {
        mParent->getWindowManager()->findDefaultFrameAttachmentPoint(true);
        LOG_ERROR() << "Could not create Plugin class with name " << name << " because the attachment point " << target << " was not found.";
        return 0;
    }

    int classId = PluginFactory::getFactory().getClassIdByName(name);
    if (classId == 0)
    {
        LOG_ERROR() << "Plugin class with name " << name << " not found. New plugins have to be specfied in their own .cpp/.h files," <<
            " and have to register with the PluginFactory using the registration Macro. See Plugin documentation for more info.";
        return 0;
    }

    EPluginType type = PluginFactory::getFactory().getPluginType(classId);
    if (type != PT_FRAME)
    {
        LOG_ERROR() << "Plugin class with name " << name << " could not be created because it is no AttachableFrame but has type '" << PluginType::getTypeString(type).toStdString().c_str() << "'.";
        return 0;
    }

    //Create Frame
    LOG_DEBUG() << "Creating attachable frame plugin with name " << name << " and caption " << caption << ".";
    AttachableFrame* frame = dynamic_cast<AttachableFrame*>(PluginFactory::getFactory().getPluginInstance(classId));
    if (frame == 0)
    {
        LOG_ERROR() << "Frame class with name " << name << " could not be instantiated. Factory returned 0.";
        return 0;
    }
    LOG_DEBUG() << "Created frame with name " << name << " and assigned id: " << frame->getPluginId();
    frame->setCaption(caption);

    //Connect to AttachmentPoint
    bool success = mParent->getWindowManager()->createAttachment(frame, target, caption);
    if (success)
    {
        //Store frame
        mPluginMap.insert(std::make_pair(frame->getPluginId(), frame));
    }
    else
    {
        LOG_ERROR() << "Failed to attach frame class with name " << name << ". Deleting it.";
        return 0;
    }

    //Calling init function
    frame->init(parameters);

    //Store instantiation info
    addInstanceIndex(frame->getClassId(), frame->getPluginId());
    emit pluginAdded(frame->getPluginId());
    emit pluginAdded((AbstractPlugin*)frame);
    emit pluginOfClassAdded(frame->getClassId());

    return frame;
}

AbstractPlugin* PluginManager::findPlugin(const QString& name, const QString& caption, int type)
{
    bool searchName = (name.compare("") != 0);
    bool searchCaption = (caption.compare("") != 0);
    bool searchType = (type >= 0 && type <= 2);

    if (!searchName && !searchCaption && !searchType)
    {
        LOG_WARNING() << "Plugin search query matches with any plugin. Specify class name, plugin caption and/or type.";
    }

    std::vector<AbstractPlugin*> matches = findPlugins(name, caption, type);

    if (matches.size() > 1)
    {
        //Warning, ambiguous
        QString description;
        description += (searchName    ? "name "      : "any name"     ) + (searchName    ? name    : "") +
                       (searchCaption ? ", caption " : ", any caption") + (searchCaption ? caption : "") +
                       (searchType    ? ", type "    : ", any type"   ) + (searchType    ? QString("%1").arg(type).toStdString().c_str() : "");

        LOG_WARNING() << "Plugin description matches with more than one (" << matches.size() << ") plugins. Description: " << description;
    }

    if (matches.size() >= 1)
        return matches.at(0);

    return 0;
}

std::vector<AbstractPlugin*> PluginManager::findPlugins(const QString& name, const QString& caption, int type)
{
    std::vector<AbstractPlugin*> found;

    bool searchName = (name.compare("") != 0);
    bool searchCaption = (caption.compare("") != 0);
    bool searchType = (type >= 0 && type <= 2);

    /*QString description;
    description += (searchName    ? "name "      : "any name"     ) + (searchName    ? name    : "") +
                   (searchCaption ? ", caption " : ", any caption") + (searchCaption ? caption : "") +
                   (searchType    ? ", type "    : ", any type"   ) + (searchType    ? QString("%1").arg(type).toStdString().c_str() : "");
    LOG_DEBUG() << "Searching Plugins with: " << description;*/

    int count = 0;
    TPluginMap::iterator pos =  mPluginMap.end();
    for (auto it = mPluginMap.begin(); it != mPluginMap.end(); it++)
    {
        bool matches = true;
        if (searchName)
        {
            if (it->second->getClassName().compare(name) != 0)
                matches = false;
        }
        if (matches && searchCaption)
        {
            if (it->second->getCaption().compare(caption) != 0)
                matches = false;
        }
        if (matches && searchType)
        {
            if (it->second->getPluginType() != (EPluginType)type)
                matches = false;
        }

        if (matches)
        {
            //found one plugin
            found.push_back(it->second);
        }
    }

    return found;
}

bool PluginManager::declareExtensionPlugin(int pluginId, bool extension, bool updateCurrentSimulationSetup)
{
    AbstractPlugin* plugin = getPlugin(pluginId);
    if (plugin == 0)
    {
        LOG_ERROR() << "Plugin with id " << pluginId << " not found.";
        return false;
    }

    if (plugin->isExtension() == extension)
        return false;

    //State changed
    if (extension == true)
    {
        //Add extension

        //Get or create the plugin definition
        if (plugin->getPluginDefinition().get() == 0)
            plugin->setPluginDefinition(shared_ptr<PluginDefinition>(new PluginDefinition(plugin)));
        shared_ptr<PluginDefinition> definition = plugin->getPluginDefinition();
        definition->setId(plugin->getPluginId());

        //Store definition in extension list
        bool found = false;
        for (auto it = mExtensions->getAddPlugins().begin(); it != mExtensions->getAddPlugins().end(); it++)
        {
            if (it->get()->getId() == definition->getId())
            {
                //definition for this plugin already in extensions list
                found = true;
                break;
            }
        }
        if (!found)
        {
            //Add to list
            definition->setIsExtension(true);
            plugin->setIsExtension(true);
            mExtensions->insertAddPlugin(definition);

            //Remove from SimulationSetup
            if (updateCurrentSimulationSetup)
            {
                bool ok = Carbon::get()->getSimulationManager()->getSimulation()->getSetupNc()->eraseMatchingAddPlugin(plugin);
                if (ok)
                    LOG_DEBUG() << "Plugin definition moved from current Simulation Setup to Extension Plugin list.";
                else
                    LOG_WARNING() << "Plugin definition not found current Simulation Setup. Cant remove it to update Extension Plugin list.";
            }
        }
    }
    else
    {
        //Remove extension

        //Find extension in extension list
        bool found = false;
        int index = 0;
        int id = 0;
        for (auto it = mExtensions->getAddPlugins().begin(); it != mExtensions->getAddPlugins().end(); it++)
        {
            if (it->get()->getId() == plugin->getPluginId())
            {
                //definition for this plugin found, remove it
                found = true;
                break;
            }
            index++;
        }

        if (!found)
        {
            return false;
        }

        //erase
        plugin->setIsExtension(false);
        mExtensions->eraseAddPlugin(index);

        //Add to SimulationSetup
        if (updateCurrentSimulationSetup)
        {
            Carbon::get()->getSimulationManager()->getSimulation()->getSetupNc()->insertAddPlugin(
                shared_ptr<PluginDefinition>(new PluginDefinition(*plugin->getPluginDefinition()))); // Add as copy
            LOG_DEBUG() << "Plugin definition moved from Extension-Plugin list to current Simulation Setup to Extension Plugin list.";
        }
    }

    //Save extension changes
    mParent->getSimulationManager()->saveSimulationSetup(mExtensions, true, false);
    return true;
}

void PluginManager::notifyDestruction(int pluginId)
{
    //Remove the plugin from all lists.
    LOG_DEBUG() << "Plugin with id " << pluginId << " got deleted. Removing from manager.";

    //Search class of the plugin from the instance list
    int classId = 0;
    for (TPluginInstanceMap::iterator it = mPluginInstanceMap.begin(); it != mPluginInstanceMap.end(); it++)
    {
        std::map<int, int>::iterator pos = it->second.find(pluginId);
        if (pos != it->second.end())
        {
            //found plugin, delete
            deletePlugin(it->first, pluginId, false);
            return;
        }
    }
}

AbstractPlugin* PluginManager::getPlugin(int pluginId) const
{
    for (auto it = mPluginMap.begin(); it != mPluginMap.end(); it++)
    {
        if (it->first == pluginId)
        {
            return it->second;
        }
    }

    return 0;
}

int PluginManager::getInstanceIndex(const AbstractPlugin& plugin) const
{
    TPluginInstanceMap::const_iterator it = mPluginInstanceMap.find(plugin.getClassId());
    if (it == mPluginInstanceMap.end())
    {
        LOG_ERROR() << "Plugin class " << plugin.getClassName() << " not found in manager.";
        return 0;
    }

    std::map<int,int>::const_iterator it_sub = it->second.find(plugin.getPluginId());
    if (it_sub == it->second.end())
    {
        LOG_ERROR() << "Plugin instance " << plugin.getClassName() << ":" << plugin.getCaption() <<
                       " not found in manager.";
        return 0;
    }

    return it_sub->second;
}

int PluginManager::getInstanceCount(int classId) const
{
    TPluginInstanceMap::const_iterator it = mPluginInstanceMap.find(classId);
    if (it == mPluginInstanceMap.end())
    {
        return 0;
    }

    return it->second.size();
}

int PluginManager::getMaxInstanceCount(int classId) const
{
    return PluginFactory::getFactory().getMaxInstanceCount(classId);
}

void PluginManager::createPluginContextMenu(QMenu* menu, std::vector<NumberedActionReceiver*>& actionReceivers,
    QObject* signalTarget, const char* slot, const PluginDefinition& matchDefinition)
{
    //Add plugins identifications to list
    std::vector<AbstractPlugin*> allplugins = findPlugins(matchDefinition.getName(), matchDefinition.getCaption(), matchDefinition.getPluginType());
    for (auto it = allplugins.begin(); it != allplugins.end(); it++)
    {
        NumberedActionReceiver* newReceiver = new NumberedActionReceiver((*it)->getPluginId());
        actionReceivers.push_back(newReceiver);
        QString plugin = (*it)->getCaption() + " (" + (*it)->getClassName() + ", " + (*it)->getPluginTypeString() + ")";
        QAction* action = menu->addAction(plugin, newReceiver, SLOT(receiveAction()));
        connect(newReceiver, SIGNAL(actionReceived(int)), this, SLOT(onLoadTask(int)));
    }
}

QVariant PluginManager::getPluginClassInfo(int classId, AbstractPlugin::EPluginInfo info)
{
    return getPluginClassInfo(classId, AbstractPlugin::getClassInfoName(info));
}

QVariant PluginManager::getPluginClassInfo(int classId, const QString& field)
{
    QString name = PluginFactory::getFactory().getClassNameById(classId);
    if (name.compare("") == 0)
    {
        LOG_ERROR() << "Plugin with class " << classId << " not found";
        return QVariant();
    }

    if (mSettings->beginGroup("gui_PluginInfo", true)) //lock
    {
        mSettings->beginGroup(name);

        QVariant returning;

        returning = mSettings->value(field, QVariant());

        mSettings->endGroup();
        mSettings->endGroup(true); //unlock

        return returning;
    }
    else
        LOG_ERROR() << "Could not access Settings object to read class info for plugin " << name << " from.";

    return QVariant();
}

void PluginManager::setPluginClassInfo(int classId, AbstractPlugin::EPluginInfo info, const QVariant& value)
{
    setPluginClassInfo(classId, AbstractPlugin::getClassInfoName(info), value);
}

void PluginManager::setPluginClassInfo(int classId, const QString& field, const QVariant& value)
{
    QString name = PluginFactory::getFactory().getClassNameById(classId);
    if (name.compare("") == 0)
    {
        LOG_ERROR() << "Plugin with class " << classId << " not found";
        return;
    }

    if (mSettings->beginGroup("gui_PluginInfo", true)) //lock
    {
        mSettings->beginGroup(name);

        mSettings->setValue(field, value);

        mSettings->endGroup();
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save class information for plugin " << name;
}

bool PluginManager::loadData(boost::shared_ptr<Settings> data)
{
    bool loaded = false;
    for (auto it = mPluginMap.begin(); it != mPluginMap.end(); it++)
    {
        bool loadedOne = it->second->loadData(data);
        if (loadedOne)
            loaded = true;
    }
    return loaded;
}

bool PluginManager::saveData(boost::shared_ptr<Settings> data)
{
    bool loaded = false;
    for (auto it = mPluginMap.begin(); it != mPluginMap.end(); it++)
    {
        bool loadedOne = it->second->loadData(data);
        if (loadedOne)
            loaded = true;
    }
    return loaded;
}

const LibraryLoader* PluginManager::getLibraryLoader() const
{
    return mLibraryLoader;
}

LibraryLoader* PluginManager::getLibraryLoader()
{
    return mLibraryLoader;
}

const QStringList& PluginManager::getPluginIncludeDirectories() const
{
    return mPluginIncludeDirectories;
}

const QStringList& PluginManager::getPluginIncludeFiles() const
{
    return mPluginIncludeFiles;
}

bool PluginManager::addPluginIncludeDirectory()
{
    //Choose Path from dialog
    QFileDialog dialog((QWidget*)Carbon::get()->getWindowManager()->getMainWindow(),
        QString(tr("Add plugin include directory")),
        mDefaultPluginPath);
    dialog.setFilter(QDir::Dirs);
    dialog.setToolTip(tr("Choose a directory with plugin libraries to include. All libraries in the directory will be linked to the application."));

    //Execute dialog
    bool accepted = dialog.exec();
    if (!accepted)
        return false;

    QStringList list = dialog.selectedFiles();
    if (list.size() == 0)
        return false;
    QString path = list.first();

    //Add directory
    return addPluginIncludeDirectory(path);
}

bool PluginManager::addPluginIncludeDirectory(const QString& path)
{
    QDir dir(path);
    QString absolutePath = dir.absolutePath();

    mIncludePathsMutex.lock();

    bool added = mLibraryLoader->addDirectory(absolutePath);
    if (added)
    {
        //Load all libraries and export plugins (already loaded plugins are skipped)
        mLibraryLoader->load();
        mLibraryLoader->exportPlugins();

        //Save new directory
        mPluginIncludeDirectories.append(absolutePath);
        if (mSettings->beginGroup("gui_PluginManager", true)) //lock
        {
            mSettings->setValue("mPluginPaths", mPluginIncludeDirectories);
            mSettings->endGroup(true); //unlock
        }
        else
            LOG_ERROR() << "Could not access Settings.";
    }

    mIncludePathsMutex.unlock();

    return added;
}

bool PluginManager::removePluginIncludeDirectory(const QString& path)
{
    QDir dir(path);
    QString absolutePath(dir.absolutePath());

    mIncludePathsMutex.lock();

    bool removed = false;
    for (auto it = mPluginIncludeDirectories.begin(); it != mPluginIncludeDirectories.end();)
    {
        if (it->compare(absolutePath) == 0)
        {
            //Remove
            mLibraryLoader->removeDirectory(*it);

            it = mPluginIncludeDirectories.erase(it);
            removed = true;
            continue;
        }
        it++;
    }

    if (removed)
    {
        //Save changed include directory list
        if (mSettings->beginGroup("gui_PluginManager", true)) //lock
        {
            mSettings->setValue("mPluginPaths", mPluginIncludeDirectories);
            mSettings->endGroup(true); //unlock
        }
        else
            LOG_ERROR() << "Could not access Settings.";
    }

    mIncludePathsMutex.unlock();
    return removed;
}

bool PluginManager::addPluginIncludeFile()
{
    //Choose File from dialog
    QFileDialog dialog((QWidget*)Carbon::get()->getWindowManager()->getMainWindow(),
        QString(tr("Add plugin library")),
        mDefaultPluginPath);
    dialog.setFilter(QDir::Files);
    dialog.setToolTip(tr("Choose a plugin libraries to include. The libraries will be linked to the application."));

    //Execute dialog
    bool accepted = dialog.exec();
    if (!accepted)
        return false;

    bool added = false;
    QStringList list = dialog.selectedFiles();
    for (auto it = list.begin(); it != list.end(); it++)
    {
        //Add file
        bool addedOne = addPluginIncludeFile(*it);
        if (addedOne) added = true;
    }

    return added;
}

bool PluginManager::addPluginIncludeFile(const QString& path)
{
    QFileInfo info(path);
    QString absolutePath = info.absoluteFilePath();

    mIncludePathsMutex.lock();

    bool added = mLibraryLoader->addFile(absolutePath);
    if (added)
    {
        bool loaded = mLibraryLoader->load(absolutePath);
        if (!loaded)
        {
            LOG_ERROR() << "Error while loading library " << absolutePath << ".";
            mIncludePathsMutex.unlock();
            return false;
        }
        bool success = mLibraryLoader->exportPlugins(absolutePath);
        if (!success)
        {
            LOG_ERROR() << "Error while exporting plugins from library " << absolutePath << ".";
            mIncludePathsMutex.unlock();
            return false;
        }

        //Save changed include file list
        mPluginIncludeFiles.append(absolutePath);
        if (mSettings->beginGroup("gui_PluginManager", true)) //lock
        {
            mSettings->setValue("mPluginFiles", QVariant(mPluginIncludeFiles));
            mSettings->endGroup(true); //unlock
        }
        else
            LOG_ERROR() << "Could not access Settings.";


        mIncludePathsMutex.unlock();
        return success;
    }
    mIncludePathsMutex.unlock();
    return false;
}

bool PluginManager::removePluginIncludeFile(const QString& path)
{
    QFileInfo info(path);
    QString absolutePath(info.absolutePath());

    mIncludePathsMutex.lock();

    bool removed = false;
    for (auto it = mPluginIncludeFiles.begin(); it != mPluginIncludeFiles.end();)
    {
        if (it->compare(absolutePath) == 0)
        {
            //Libraries are not unloaded from running execution

            //Remove
            it = mPluginIncludeFiles.erase(it);
            removed = true;
            continue;
        }
        it++;
    }

    if (removed)
    {
        //Save changed include file list
        if (mSettings->beginGroup("gui_PluginManager", true)) //lock
        {
            mSettings->setValue("mPluginFiles", mPluginIncludeFiles);
            mSettings->endGroup(true); //unlock
        }
        else
            LOG_ERROR() << "Could not access Settings.";

    }

    mIncludePathsMutex.unlock();
    return removed;
}

void PluginManager::initLibraries()
{
    //Add all single files
    mLibraryLoader->addFiles(mPluginIncludeFiles);

    //Add all include directories
    for (auto it = mPluginIncludeDirectories.begin(); it != mPluginIncludeDirectories.end(); it++)
        mLibraryLoader->addDirectory(*it);

    //Link all included plugin libraries (one by one for detailed log infos)
    for (auto it = mLibraryLoader->librariesBegin(); it != mLibraryLoader->librariesEnd(); it++)
    {
        //Load the library (already loaded ones are skipped and return true)
        bool loaded = mLibraryLoader->load(it->mId);
        if (!loaded)
        {
            LOG_ERROR() << "Failed to load library " << it->mLibrary->fileName() << ": " << it->mLibrary->errorString();
            continue;
        }

        //Export plugins from the library (already exported ones are skipped and return true)
        bool exported = mLibraryLoader->exportPlugins(it->mId);
        if (!exported)
        {
            LOG_ERROR() << "Failed to export plugins from library " << it->mLibrary->fileName() << ": " << it->mLibrary->errorString();
            continue;
        }
    }
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void PluginManager::updateSetting(QString name)
{
    //Single setting values that are changed in the SettingsWidget are updated here

    bool loaded = false;
    if (mSettings->beginGroup("gui_PluginManager", true)) //lock
    {
        mIncludePathsMutex.lock();
        bool unknown = false;
        loaded = true;

        if      (name.compare("mPluginPaths") == 0) mPluginIncludeDirectories = mSettings->value(name, mPluginIncludeDirectories).toStringList();
        else if (name.compare("mPluginFiles") == 0) mPluginIncludeFiles       = mSettings->value(name, mPluginIncludeFiles).toStringList();
        else
        {
            unknown = true;
            loaded = false;
        }

        mSettings->endGroup(true); //unlock
        mIncludePathsMutex.unlock();
    }
    else
        LOG_ERROR() << "Could not access the Settings object to store the PluginManagers include directories to.";

    if (loaded)
    {
        //Include files and directories may have changed. Update all.
        initLibraries();
    }
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool PluginManager::deletePlugin(int classId, int pluginId, bool erase, bool removeDefFromSimulation)
{
    TPluginMap::iterator it = mPluginMap.find(pluginId);
    if (it == mPluginMap.end())
    {
        //Nothing to delete, return
        return false;
    }

    if (erase == true)
    {
        emit aboutToDeletePlugin(it->second);
    }
    emit aboutToRemovePlugin(it->second);

    //Delete all menu entries that have been created by this plugin
    Carbon::get()->getMenuManager()->cleanEntries(it->second);

    //Remove extension
    if (!erase && it->second->isExtension())
    {
        declareExtensionPlugin(pluginId, false, false);
    }

    //Remove source definition of plugin in current Simulation
    if (removeDefFromSimulation)
    {
        bool ok = Carbon::get()->getSimulationManager()->getSimulation()->getSetupNc()->eraseMatchingAddPlugin(it->second);
        if (ok)
            LOG_DEBUG() << "Plugin definition removed from current Simulation Setup.";
        else
            LOG_DEBUG() << "Plugin definition not found current Simulation Setup. Cant remove.";
    }

    //Notify window manager if a window is deleted
    if (it->second->getPluginType() == PT_FRAME)
        Carbon::get()->getWindowManager()->notifyDestruction((AttachableFrame*)it->second);

    //Remove instance from list
    removeInstanceIndex(classId, pluginId);

    //Remove plugin from list
    if (erase)
    {
        AbstractPlugin* ptr = it->second;
        it->second = 0;
        mPluginMap.erase(it);
        if (ptr->getPluginType() == PT_FRAME)
        {
            ((AttachableFrame*)ptr)->deleteLater();
        }
        else
        {
            delete ptr;
        }
    }
    else
    {
        it->second = 0;
        mPluginMap.erase(it);
    }

    LOG_DEBUG() << "Plugin " << pluginId << " deleted.";
    emit pluginRemoved(pluginId);
    emit pluginOfClassRemoved(classId);

    return true;
}

void PluginManager::removeInstanceIndex(int classId, int pluginId)
{
    TPluginInstanceMap::iterator it = mPluginInstanceMap.find(classId);
    if (it == mPluginInstanceMap.end())
    {
        return;
    }

    std::map<int,int>::iterator it_sub = it->second.find(pluginId);
    if (it_sub == it->second.end())
    {
        return;
    }

    it->second.erase(it_sub);
    if (it->second.size() == 0)
    {
        mPluginInstanceMap.erase(it);
    }
}

void PluginManager::addInstanceIndex(int classId, int pluginId)
{
    TPluginInstanceMap::iterator it = mPluginInstanceMap.find(classId);
    if (it == mPluginInstanceMap.end())
    {
        mPluginInstanceMap.insert(std::make_pair(classId, std::map<int,int>()));
        it = mPluginInstanceMap.find(classId);
    }

    std::map<int,int>::iterator it_sub = it->second.find(pluginId);
    if (it_sub == it->second.end())
    {
        it->second.insert(std::make_pair(pluginId, 1));
    }
    else
    {
        it_sub->second++;
    }
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

//--------------------------------------------------------------
// PluginTableModel
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

PluginTableModel::PluginTableModel(PluginManager::TPluginMap& list) :
        mPlugins(list)
{
    mHeaderData.push_back(QVariant(tr("Location")));
    mHeaderData.push_back(QVariant(tr("Caption")));
    mHeaderData.push_back(QVariant(tr("Class")));
    mHeaderData.push_back(QVariant(tr("Type")));
    mHeaderData.push_back(QVariant(tr("Attachment")));
    emit headerDataChanged(Qt::Horizontal, 0, 4);

    mPluginManager = Carbon::get()->getPluginManager();
    mSimulationManager = Carbon::get()->getSimulationManager();

    connect(mPluginManager, SIGNAL(pluginAdded(int)), this, SLOT(updatePluginAdded(int)));
    connect(mPluginManager, SIGNAL(pluginRemoved(int)), this, SLOT(updatePluginRemoved(int)));
}

PluginTableModel::~PluginTableModel()
{
}

int PluginTableModel::rowCount (const QModelIndex & parent) const
{
    return mPlugins.size();
}

int PluginTableModel::columnCount (const QModelIndex & parent) const
{
    return COLUMN_COUNT;
}

QVariant PluginTableModel::data (const QModelIndex & index, int role) const
{
    int row = index.row();
    int col = index.column();
    AbstractPlugin* plugin = getPluginAt(row);

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (plugin == 0)
            return QVariant(tr("#Error#"));

        EPluginType type;
        FrameAttachmentPoint* attachment;
        switch (col)
        {
        case 0:
            //Extension
            if (plugin->isExtension())
                return QVariant(tr("(Extension)"));
            else
                return QVariant(mSimulationManager->getSimulation()->getSetup()->getName());
            break;
        case 1:
            //Caption
            return QVariant(plugin->getCaption());
            break;
        case 2:
            //Class
            return QVariant(plugin->getClassName());
            break;
        case 3:
            //Type
            return QVariant(PluginType::getTypeString(plugin->getPluginType()));
            break;
        case 4:
            //Attachment
            type = plugin->getPluginType();
            switch (type)
            {
            case PT_NORMAL:
                //fall through
            case PT_NONE:
                //fall through
            case PT_SIGNAL:
                return QVariant("");
                break;
            case PT_FRAME:
                attachment = Carbon::get()->getWindowManager()->findFrameAttachmentPoint((AttachableFrame*)plugin);
                if (attachment == 0)
                    return QVariant("");
                else
                    return attachment->getName();
                break;
            }
            break;
        default:
            return QVariant("");
            break;
        }
        return QVariant("");
    }
    else
    if (role == Qt::SizeHintRole)
    {
        return QSize(100,30);
    }
    else
    if (role == Qt::CheckStateRole)
    {
        if (plugin == 0)
            return QVariant(tr("#Error#"));

        switch(col)
        {
        case 0:
            //Extension
            return plugin->isExtension() ? Qt::Checked : Qt::Unchecked;
            break;
        case 1: //fall
        case 2: //fall
        case 3: //fall
        case 4: //fall
        default:
            break;
        }
    }
    else
    if (role == Qt::ToolTipRole)
    {
       if (plugin == 0)
            return QVariant(tr("#Error#"));

        switch(col)
        {
        case 0:
            //Extension
            return QVariant(tr("The location where the plugin definition is saved.\n" \
                "If checked, the plugin will be an 'Extension' and loaded with every simulation instead of only the current simulation setup.\n" \
                "Otherwise it will be stored in the current simulation setup."));
            break;
        case 1:
            //Caption
            return QVariant(tr("The caption of the plugin instance. Used to display a name for the plugin instance and to store settings for the plugin instance with."));
            break;
        case 2:
            //Class
            return QVariant(tr("The name of the plugin class."));
            break;
        case 3:
            //Type
            return QVariant(tr("The type of the plugin class.\n" \
                "Simple-Plugins are usually used to be executed in a Plugin-Thread, Signal-Plugins can be used to communicate with each other, \n" \
                "and Frame-Plugins are visual plugins that can be attached to and moved between the Gui attachment points."));
            break;
        case 4:
            //Attachment
            return QVariant(tr("Name of the current attachment point the plugin is attached to. Only Frame-Plugins can be attached."));
            break;
        default:
            return QVariant(tr("#Illegal column Index#"));
            break;
        }
    }

    return QVariant();
}

bool PluginTableModel::setData (const QModelIndex & index, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        int row = index.row();
        int col = index.column();

        AbstractPlugin* plugin = getPluginAt(row);
        if (plugin == 0)
            return false;

        switch (col)
        {
        case 0:
            //Extension
            return false;
            break;
        case 1:
            //Caption
            plugin->setCaption(value.toString());
            emit pluginCaptionChanged(plugin);
            break;
        case 2:
            //Name (unchangeable)
            break;
        case 3:
            //Type (unchangeable)
            return false;
            break;
        case 4:
            //Attachment
            if (plugin->getPluginType() == PT_FRAME)
            {
                //Try to change attachment
                if (!Carbon::get()->getWindowManager()->
                        changeAttachment((AttachableFrame*)plugin, value.toString()))
                    return false;
                emit pluginAttachmentChanged(plugin, value.toString());
            }
            else
                return false;
            break;
        default:
            return false;
            break;
        }
    }
    else if (role == Qt::CheckStateRole)
    {
        int row = index.row();
        int col = index.column();

        AbstractPlugin* plugin = getPluginAt(row);
        if (plugin == 0)
            return false;

        switch (col)
        {
        case 0:
            mPluginManager->declareExtensionPlugin(plugin->getPluginId(), value.toBool(), true);
            break;
        case 1: //fall
        case 2: //fall
        case 3: //fall
        case 4: //fall
            break;
        default:
            break;
        }
    }
    else
    {
        return false;
    }

    emit dataChanged(index, index);
    return true;
}


bool PluginTableModel::setHeaderData (int section, Qt::Orientation orientation, const QVariant & value, int role)
{
    if (role == Qt::EditRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section >= 0 && section < COLUMN_COUNT)
            {
                mHeaderData.at(section);
                emit headerDataChanged(Qt::Horizontal, section, section);
            }
        }
        else
        {
            LOG_WARNING() << "Vertical orientation header data cant be changed.";
            return false;
        }
    }
    else
    {
        return false;
    }

    return false;
}

QVariant PluginTableModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section >= 0 && section < COLUMN_COUNT)
            {
                return mHeaderData.at(section);
            }
            else
            {
                return QVariant();
            }
        }
        else
        {
            if (section >= 0 && section < (int)mPlugins.size())
            {
                AbstractPlugin* plugin = getPluginAt(section);
                if (plugin == 0)
                    return QVariant(tr("#Error#"));
                else
                    return plugin->getPluginId();
            }
            else
            {
                return QVariant();
            }
        }
    }
    else
    if (role == Qt::ToolTipRole)
    {
        switch(section)
        {
        case 0:
            //Extension
            return QVariant(tr("The location where the plugin definition is saved.\n" \
                "If checked, the plugin will be an 'Extension' and loaded with every simulation instead of only the current simulation setup.\n \
                Otherwise it will be stored in the SimulationSetup."));
            break;
        case 1:
            //Caption
            return QVariant(tr("The caption of the plugin instance. Used to display a name for the plugin instance and to store settings for the plugin instance with."));
            break;
        case 2:
            //Class
            return QVariant(tr("The name of the plugin class."));
            break;
        case 3:
            //Type
            return QVariant(tr("The type of the plugin class.\n" \
                "Simple-Plugins are usually used to be executed in a Plugin-Thread, Signal-Plugins can be used to communicate with each other, \n" \
                "and Frame-Plugins are visual plugins that can be attached to and moved between the Gui attachment points."));
            break;
        case 4:
            //Attachment
            return QVariant(tr("Name of the current attachment point the plugin is attached to. Only Frame-Plugins can be attached."));
            break;
        default:
            return QVariant(tr("#Illegal column Index#"));
            break;
        }
    }
    return QVariant();
}

Qt::ItemFlags PluginTableModel::flags ( const QModelIndex & index ) const
{
    int row = index.row();
    int col = index.column();

    AbstractPlugin* plugin = getPluginAt(row);
    if (plugin == 0)
        return 0;

    switch (col)
    {
    case 0:
        //Extension checkbox
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
        break;
    case 1:
        //Caption
        return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
        break;
    case 2:
        //Name
        //fallthrough
    case 3:
        //Type
        return Qt::ItemIsEnabled;
        break;
    case 4:
        //Attachments
        if (plugin->getPluginType() == PT_FRAME)
        {
            return Qt::ItemIsEditable | Qt::ItemIsEnabled  | Qt::ItemIsSelectable;
        }
        else
            return 0;
        break;
    default:
        break;
    }
    return 0;
}

AbstractPlugin* PluginTableModel::getPluginAt(int index) const
{
    PluginManager::TPluginMap::iterator it = mPlugins.begin();
    if (it == mPlugins.end())
        return 0;

    for (int i = 0; i < index; i++)
    {
        it++;
        if (it == mPlugins.end())
            return 0;
    }

    return it->second;
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void PluginTableModel::updateCaption(int pluginId)
{
    int row = 0;
    for (PluginManager::TPluginMap::iterator it = mPlugins.begin(); it != mPlugins.end(); it++)
    {
        if (it->second->getPluginId() == pluginId)
        {
            emit dataChanged(index(row, 0), index(row, 0));
            return;
        }
        row++;
    }
}

void PluginTableModel::updatePluginAdded()
{
    //Has already been added, simply update model
    beginInsertRows(QModelIndex(), mPlugins.size(), mPlugins.size());
    endInsertRows();

    dataChanged(index(mPlugins.size()-1,0), index(mPlugins.size()-1, COLUMN_COUNT));
}

void PluginTableModel::updatePluginAdded(int pluginId)
{
    int i = 0;
    for (PluginManager::TPluginMap::iterator it = mPlugins.begin(); it != mPlugins.end(); it++)
    {
        if (it->second->getPluginId() == pluginId)
        {
            //update rows from where plugin was added
            QModelIndex start = index(i, 0);
            QModelIndex end = index(mPlugins.size()-1, COLUMN_COUNT);
            beginInsertRows(QModelIndex(), i, i);
            endInsertRows();
            dataChanged(start, end);
            break;
        }
        i++;
    }
}

void PluginTableModel::updatePluginRemoved(int pluginId)
{
    int i = 0;
    int header = 0;
    int size = mPlugins.size();
    for (PluginManager::TPluginMap::iterator it = mPlugins.begin(); it != mPlugins.end(); it++)
    {
        //changed row is this, whose displayed id is bigger than the pluginId
        header = headerData(i, Qt::Vertical).toInt();
        if (header == pluginId)
        {
            //plugin is still there
            return;
        }
        if (header > pluginId || i == size-1) //since its sorted, its the one with bigger header id or the last one
        {
            //update rows from where plugin was removed
            int from = ((i == 0) ? 0 : i - 1);
            QModelIndex start = index(from, 0);
            QModelIndex end = index(mPlugins.size()-1, COLUMN_COUNT);
            beginRemoveRows(QModelIndex(), from, from);
            endRemoveRows();
            dataChanged(start, end);
            return;
        }
        i++;
    }
}
