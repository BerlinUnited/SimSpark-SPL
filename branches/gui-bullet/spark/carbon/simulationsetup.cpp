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

#include "simulationsetup.h"

#include "carbon.h"
#include "simulationmanager.h"
#include "simulation.h"

using namespace boost;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SimulationSetup::SimulationSetup(QString name, QString filename, bool reloadall, bool removeall,
    const TPluginDefinitions& addlist, const TPluginDefinitions& removelist, const TTaskDefinitions& tasklist, bool saved) :
    mName(name),
    mFileName(filename),
    mRemoveAll(removeall),
    mReloadAll(reloadall),
    mAddPlugins(addlist),
    mRemovePlugins(removelist),
    mTaskDefinitions(tasklist),
    mSaved(saved)
{
    mDontSave = false;
}

SimulationSetup::SimulationSetup(QString name, QString filename, bool reloadall, bool removeall, bool saved)
{
    mName = name;
    mFileName = filename;
    mRemoveAll = removeall;
    mReloadAll = removeall;
    mSaved = saved;
    mDontSave = false;

    mTaskDefinitions.clear();
    mAddPlugins.clear();
    mRemovePlugins.clear();
}

SimulationSetup::SimulationSetup(const SimulationSetup& source) :
    mName(source.mName),
    mFileName(source.mFileName),
    mRemoveAll(source.mRemoveAll),
    mReloadAll(source.mReloadAll),
    mTaskDefinitions(source.mTaskDefinitions),
    mAddPlugins(source.mAddPlugins),
    mRemovePlugins(source.mRemovePlugins),
    mSaved(source.mSaved)
{
    mDontSave = false;
}

SimulationSetup& SimulationSetup::operator=(const SimulationSetup& source)
{
    mName = source.mName;
    mFileName = source.mFileName;
    mRemoveAll = source.mRemoveAll;
    mReloadAll = source.mReloadAll;
    mTaskDefinitions = source.mTaskDefinitions;
    mAddPlugins = source.mAddPlugins;
    mRemovePlugins = source.mRemovePlugins;
    mSaved = source.mSaved;

    return *this;
}

SimulationSetup::SimulationSetup()
{
    mName = "";
    mFileName = "";
    mRemoveAll = false;
    mReloadAll = false;
    mSaved = false;
    mDontSave = false;

    mTaskDefinitions.clear();
    mAddPlugins.clear();
    mRemovePlugins.clear();
}

SimulationSetup::~SimulationSetup()
{
}

const SimulationSetup::TPluginDefinitions& SimulationSetup::getAddPlugins() const
{
    return mAddPlugins;
}

const SimulationSetup::TPluginDefinitions& SimulationSetup::getRemovePlugins() const
{
    return mRemovePlugins;
}

const SimulationSetup::TTaskDefinitions& SimulationSetup::getTaskDefinitions() const
{
    return mTaskDefinitions;
}

SimulationSetup::TPluginDefinitions& SimulationSetup::getAddPlugins()
{
    return mAddPlugins;
}

SimulationSetup::TPluginDefinitions& SimulationSetup::getRemovePlugins()
{
    return mRemovePlugins;
}

SimulationSetup::TTaskDefinitions& SimulationSetup::getTaskDefinitions()
{
    return mTaskDefinitions;
}

const QString& SimulationSetup::getName() const
{
    return mName;
}

const QString& SimulationSetup::getFileName() const
{
    return mFileName;
}

bool SimulationSetup::getReloadAll() const
{
    return mReloadAll;
}

bool SimulationSetup::getRemoveAll() const
{
    return mRemoveAll;
}

bool SimulationSetup::hasTaskDefinition(TaskDefinition::ETaskType type) const
{
    return findNextTaskDefinition(0, type) != -1;
}

int SimulationSetup::findNextTaskDefinition(int startIndex, TaskDefinition::ETaskType type) const
{
    for (int i = (startIndex < 0 ? 0 : startIndex); i < (int)mTaskDefinitions.size(); i++)
    {
        if (mTaskDefinitions.at(i)->getType() == type)
        {
            return i;
        }
    }

    return -1;
}

int SimulationSetup::findNextThreadDefinition(int startIndex) const
{
    for (int i = (startIndex < 0 ? 0 : startIndex); i < (int)mTaskDefinitions.size(); i++)
    {
        if (mTaskDefinitions.at(i)->isThread())
        {
            return i;
        }
    }

    return -1;
}

int SimulationSetup::findNextProcessDefinition(int startIndex) const
{
    for (int i = (startIndex < 0 ? 0 : startIndex); i < (int)mTaskDefinitions.size(); i++)
    {
        if (mTaskDefinitions.at(i)->isProcess())
        {
            return i;
        }
    }

    return -1;
}

int SimulationSetup::findNextSparkDefinition(int startIndex) const
{
    for (int i = startIndex; i < (int)mTaskDefinitions.size(); i++)
    {
        if (mTaskDefinitions.at(i)->isSpark())
        {
            return i;
        }
    }

    return -1;
}

shared_ptr<PluginDefinition> SimulationSetup::insertAddPlugin(const PluginDefinition& addplugin)
{
    //Only plugin definitions that specify a plugin completely can be used as "addPlugin" definition
    shared_ptr<PluginDefinition> plugin(new PluginDefinition(addplugin));
    bool success = insertAddPlugin(plugin);

    if (success)
        return plugin;

    return shared_ptr<PluginDefinition>();
}

bool SimulationSetup::insertAddPlugin(boost::shared_ptr<PluginDefinition> addplugin)
{
    //Only plugin definitions that specify a plugin completely can be used as "addPlugin" definition
    if (addplugin->getName().compare("") == 0)
    {
        LOG_ERROR() << "Illegal plugin definition. Plugins need a name.";
        return false;
    }

    if (addplugin->getCaption().compare("") == 0)
        addplugin->setCaption(addplugin->getName());

    //add
    mAddPlugins.push_back(addplugin);
    mSaved = false;

    updateDataChanged(SD_ADDLIST);
    return true;
}

shared_ptr<PluginDefinition> SimulationSetup::insertRemovePlugin(const PluginDefinition& removeplugin)
{
    //Vague plugin definitions that specify a range of plugins can be used as "removePlugin" definition
    //Detailed data (like task definitions) are not needed

    shared_ptr<PluginDefinition> plugin(new PluginDefinition(removeplugin));
    bool success = insertRemovePlugin(plugin);

    if (success)
        return plugin;

    return shared_ptr<PluginDefinition>();
}

bool SimulationSetup::insertRemovePlugin(boost::shared_ptr<PluginDefinition> removeplugin)
{
    //Vague plugin definitions that specify a range of plugins can be used as "removePlugin" definition
    //Detailed data (like task definitions) are not needed

    removeplugin->clearParameters();

    //add
    mRemovePlugins.push_back(removeplugin);
    mSaved = false;

    updateDataChanged(SD_REMOVELIST);
    return true;
}

shared_ptr<TaskDefinition> SimulationSetup::insertTask(const TaskDefinition& task)
{
    mTaskDefinitions.push_back(boost::shared_ptr<TaskDefinition>(new TaskDefinition(task)));
    mSaved = false;

    updateDataChanged(SD_TASKLIST);
    return mTaskDefinitions.back();
}

void SimulationSetup::insertAddPlugins(TPluginDefinitions& addplugins)
{
    if (addplugins.size() == 0)
        return;

    for (TPluginDefinitions::iterator it = addplugins.begin(); it != addplugins.end(); it++)
    {
        insertAddPlugin(**it);
    }
}

void SimulationSetup::insertRemovePlugins(TPluginDefinitions& removeplugins)
{
    if (removeplugins.size() == 0)
        return;

    for (TPluginDefinitions::iterator it = removeplugins.begin(); it != removeplugins.end(); it++)
    {
        insertRemovePlugin(**it);
    }
}

void SimulationSetup::insertTasks(TTaskDefinitions& tasks)
{
    if (tasks.size() == 0)
        return;

    for (TTaskDefinitions::iterator it = tasks.begin(); it != tasks.end(); it++)
    {
        insertTask(**it);
    }
}

int SimulationSetup::eraseMatchingAddPlugin(const AbstractPlugin* plugin)
{
    //Check if there are ids to identify
    int match = 0;
    for (auto it = mAddPlugins.begin(); it != mAddPlugins.end(); )
    {
        //Same plugin-id than plugin?
        if (it->get()->getId() != -1 && it->get()->getId() == plugin->getPluginId())
        {
            //found!
            LOG_DEBUG() << "Removing plugin definition for plugin with id " << it->get()->getId() << " from add list: " << it->get()->getCaption() << "(" << it->get()->getName() << ")";
            it = mAddPlugins.erase(it);
            mSaved = false;
            updateDataChanged(SD_ADDLIST);
            match++;
        }
        else
            it++;
    }

    if (match == -1)
    {
        //Try to remove from class and caption
        for (auto it = mAddPlugins.begin(); it != mAddPlugins.end(); it++)
        {
            if (it->get()->pluginMatches(plugin, false))
            {
                LOG_DEBUG() << "Removing matching plugin definition from add list: " << it->get()->getCaption() << "(" << it->get()->getName() << ")";
                it = mAddPlugins.erase(it);
                mSaved = false;
                updateDataChanged(SD_ADDLIST);
                match++;
            }
            else
                it++;
        }
    }

    return match;
}

int SimulationSetup::eraseMatchingRemovePlugin(const AbstractPlugin* plugin)
{
    int match = 0;
    for (auto it = mRemovePlugins.begin(); it != mRemovePlugins.end(); )
    {
        if (it->get()->pluginMatches(plugin, true))
        {
            LOG_DEBUG() << "Removing matching plugin definition from remove list: " << it->get()->getCaption() << "(" << it->get()->getName() << ")";
            it = mRemovePlugins.erase(it);
            mSaved = false;
            updateDataChanged(SD_REMOVELIST);
            match++;
        }
        else
            it++;
    }
    return match;
}

void SimulationSetup::eraseAddPlugin(int index)
{
    if (index >= 0 && index < (int)mAddPlugins.size())
    {
        mAddPlugins.erase(mAddPlugins.begin()+index);
        mSaved = false;
        updateDataChanged(SD_ADDLIST);
    }
    else
    {
        LOG_ERROR() << "Index " << index << " out of bounds. (List size is " << mAddPlugins.size() << ")";
    }
}

void SimulationSetup::eraseRemovePlugin(int index)
{
    if (index >= 0 && index < (int)mRemovePlugins.size())
    {
        mRemovePlugins.erase(mRemovePlugins.begin()+index);
        mSaved = false;
        updateDataChanged(SD_REMOVELIST);
    }
    else
    {
        LOG_ERROR() << "Index " << index << " out of bounds. (List size is " << mRemovePlugins.size() << ")";
    }
}

void SimulationSetup::eraseTask(int index)
{
    if (index >= 0 && index < (int)mTaskDefinitions.size())
    {
        mTaskDefinitions.erase(mTaskDefinitions.begin()+index);
        mSaved = false;
        updateDataChanged(SD_TASKLIST);
    }
    else
    {
        LOG_ERROR() << "Index " << index << " out of bounds. (List size is " << mTaskDefinitions.size() << ")";
    }
}

bool SimulationSetup::eraseAddPlugin(PluginDefinition* definition)
{
    for (auto it = mAddPlugins.begin(); it != mAddPlugins.end(); it++)
    {
        if (&**it == definition)
        {
            mAddPlugins.erase(it);
            return true;
        }
    }

    return false;
}

bool SimulationSetup::eraseRemovePlugin(PluginDefinition* definition)
{
    for (auto it = mRemovePlugins.begin(); it != mRemovePlugins.end(); it++)
    {
        if (&**it == definition)
        {
            mRemovePlugins.erase(it);
            return true;
        }
    }

    return false;
}

bool SimulationSetup::eraseTask(TaskDefinition* definition)
{
    for (auto it = mTaskDefinitions.begin(); it != mTaskDefinitions.end(); it++)
    {
        if (&**it == definition)
        {
            mTaskDefinitions.erase(it);
            return true;
        }
    }

    return false;
}

void SimulationSetup::clearAddPlugins()
{
    if (mAddPlugins.size() == 0)
        return;

    mAddPlugins.clear();
    updateDataChanged(SD_ADDLIST);
}

void SimulationSetup::clearRemovePlugins()
{
    if (mRemovePlugins.size() == 0)
        return;

    mRemovePlugins.clear();
    updateDataChanged(SD_REMOVELIST);
}

void SimulationSetup::clearTasks()
{
    if (mTaskDefinitions.size() == 0)
        return;

    mTaskDefinitions.clear();
    updateDataChanged(SD_TASKLIST);
}

void SimulationSetup::setRemoveAll(bool removeall)
{
    if (mRemoveAll != removeall)
    {
        mRemoveAll = removeall;
        mSaved = false;
        updateDataChanged(SD_REMOVEALL);
    }
}

void SimulationSetup::setReloadAll(bool reloadall)
{
    if (mReloadAll != reloadall)
    {
        mReloadAll = reloadall;
        mSaved = false;
        updateDataChanged(SD_RELOADALL);
    }
}

bool SimulationSetup::isToSave() const
{
    return !mDontSave;
}

bool SimulationSetup::isSaved() const
{
    return mSaved;
}

void SimulationSetup::setSaved(bool state)
{
    mSaved = state;
}

void SimulationSetup::setDontSave(bool state)
{
    mDontSave = state;
}

void SimulationSetup::readData(const SimulationSetup& source, bool deep)
{
    if (&source == this)
        return;

    mName = source.mName;
    mFileName = source.mFileName;
    mRemoveAll = source.mRemoveAll;
    mReloadAll = source.mReloadAll;
    if (deep)
    {
        mTaskDefinitions.clear();
        mAddPlugins.clear();
        mRemovePlugins.clear();

        for (auto it = source.mTaskDefinitions.begin(); it != source.mTaskDefinitions.end(); it++)
            mTaskDefinitions.push_back(boost::shared_ptr<TaskDefinition>(new TaskDefinition(**it)));
        for (auto it = source.mAddPlugins.begin(); it != source.mAddPlugins.end(); it++)
            mAddPlugins.push_back(boost::shared_ptr<PluginDefinition>(new PluginDefinition(**it)));
        for (auto it = source.mRemovePlugins.begin(); it != source.mRemovePlugins.end(); it++)
            mRemovePlugins.push_back(boost::shared_ptr<PluginDefinition>(new PluginDefinition(**it)));
    }
    else
    {
        mTaskDefinitions = source.mTaskDefinitions;
        mAddPlugins = source.mAddPlugins;
        mRemovePlugins = source.mRemovePlugins;
    }
    mSaved = false;

    updateDataChanged(SD_UNDEFINED);
}

void SimulationSetup::setFileName(const QString& name)
{
    if (mFileName.compare(name) == 0)
        return;

    mFileName = name;
    updateDataChanged(SD_FILENAME);
}

void SimulationSetup::setName(const QString& value)
{
    if (mName.compare(value) == 0)
        return;

    mName = value;
    updateDataChanged(SD_NAME);
}

bool SimulationSetup::equals(const SimulationSetup& compare) const
{
    //compare files
    if (compare.getFileName().compare("") == 0) return false;
    if (        getFileName().compare("") == 0) return false;
    return compare.getFileName().compare(getFileName()) == 0;
}

bool SimulationSetup::isCurrentSimulation() const
{
    return Carbon::get()->getSimulationManager()->getSimulation()->hasSetup(*this);
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SimulationSetup::updateDataChanged(ESetupData field)
{
    emit dataChanged(field);
}
