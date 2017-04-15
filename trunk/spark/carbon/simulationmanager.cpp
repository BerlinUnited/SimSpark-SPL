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

#include "simulationmanager.h"
#include "windowmanager.h"
#include "pluginmanager.h"
#include "carbon.h"
#include "settings.h"
#include "guisettings_simulationsettings.h"
#include "settingsdialog.h"
#include "simulation.h"
#include "simulationsetup.h"
#include "simulationtask.h"
#include "serverthread.h"
#include "taskdefinition.h"
#include "setupdialog.h"
#include "namedactionreceiver.h"
#include "numberedactionreceiver.h"

#include <QDir>
#include <QDomDocument>
#include <QTemporaryFile>
#include <QFileDialog>
#include <QMessageBox>

using namespace boost;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SimulationManager::SimulationManager(Carbon* parent) :
    mSimulation(new Simulation())
{
    //Initialize variables
    mParent = parent;
    mSettings = parent->getSettings();
    mSetupList.clear();
    mSetupIncludeDirectories.clear();
    mScriptIncludeDirectories.clear();
    mExecutableIncludeDirectories.clear();
    mSetupIncludeFiles.clear();
    mScriptIncludeFiles.clear();
    mExecutableIncludeFiles.clear();
    mReloadingSetups = false;
    mClosed = false;
    mServerThread = shared_ptr<ServerThread>();
    mNextTaskId = 1;

    //Load settings
    loadDefaultSettings();
    loadSettings();

    //Create user data folders
    QDir setupsDir(mParent->getUserDataFolder()+"setups/");
    if (!setupsDir.exists())
    {
        bool success = QDir(QDir::rootPath()).mkpath(setupsDir.absolutePath());
        if (!success)
            LOG_ERROR() << "Could not find or setups directory " << setupsDir.absolutePath() << " for simulation setup files.";
    }

    //Load setup list
    addIncludeDirectory(mDefaultSetupPath);

    //Settings widget
    mSimulationSettingsWidget = 0;

    //Shutdown signal
    connect(mParent, SIGNAL(aboutToShutdown()), this, SLOT(cleanup()));
}

SimulationManager::~SimulationManager()
{
    cleanup();
}

void SimulationManager::initializeSettingWidget()
{
    if (mSimulationSettingsWidget == 0)
    {
        mSimulationSettingsWidget = new GuiSettings::SimulationSettings(mSettings);
        Carbon::get()->getSettingsDialog()->includeSettingWidget(tr("Gui"), tr("Simulation Manager"), (SettingsWidget**)&mSimulationSettingsWidget, 0, ":config");

        //Setting connections
        connect(this, SIGNAL(settingChanged(QString)), mSimulationSettingsWidget, SLOT(updateSetting(QString)));
        connect(mSimulationSettingsWidget, SIGNAL(settingChanged(QString)), this, SLOT(updateSetting(QString)));
    }
    else
    {
        LOG_WARNING() << "SimulationSettingsWidget already initialized.";
    }
}

void SimulationManager::init()
{
    //Add commandline given paths for setup include directories
    for (auto it = mParent->mInitialSetupDirectories.begin(); it != mParent->mInitialSetupDirectories.end(); it++)
    {
        QDir dir(*it);
        if (!dir.exists())
        {
            LOG_WARNING() << "Setup include directory " << *it << " not found.";
            continue;
        }
        addSetupIncludePath(*it);
    }

    bool changed = false;

    //Add commandline given paths for script include directories
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        for (auto it = mParent->mInitialScriptDirectories.begin(); it != mParent->mInitialScriptDirectories.end(); it++)
        {
            QDir dir(*it);
            if (!dir.exists())
            {
                LOG_WARNING() << "Script include directory " << *it << " not found.";
                continue;
            }
            QString path = dir.absolutePath();

            if (!mScriptIncludeDirectories.contains(path))
            {
                mScriptIncludeDirectories.push_back(*it);
                changed = true;
            }
         }

        if (changed)
            mSettings->setValue("mScriptPaths", mScriptIncludeDirectories);

        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access settings object.";

    //Add commandline given paths for executable include directories
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        for (auto it = mParent->mInitialExecutableDirectories.begin(); it != mParent->mInitialExecutableDirectories.end(); it++)
        {
            QDir dir(*it);
            if (!dir.exists())
            {
                LOG_WARNING() << "Executable include directory " << *it << " not found.";
                continue;
            }
            QString path = dir.absolutePath();

            if (!mExecutableIncludeDirectories.contains(path))
            {
                mExecutableIncludeDirectories.push_back(*it);
                changed = true;
            }
         }

        if (changed)
            mSettings->setValue("mExecutablePaths", mExecutableIncludeDirectories);

        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access settings object.";

    //Load setups from setup include directories
    loadSimulationSetups();
}

int SimulationManager::getSetupIndex(const QString& name) const
{
    int i = 0;
    for (TSetupList::const_iterator it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        if ((*it)->getName().compare(name) == 0)
        {
            return i;
        }
        i++;
    }

    return -1;
}


int SimulationManager::getSetupIndexByFilePath(const QString& path) const
{
    int i = 0;
    for (TSetupList::const_iterator it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        if ((*it)->getFileName().compare(path) == 0)
        {
            return i;
        }
        i++;
    }

    return -1;
}

bool SimulationManager::containsSetup(const QString& name) const
{
    for (TSetupList::const_iterator it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        if ((*it)->getName().compare(name) == 0)
        {
            return true;
        }
    }

    return false;
}

const SimulationManager::TSetupList& SimulationManager::getSetupList() const
{
    return mSetupList;
}

void SimulationManager::setDefaultSavePath(const QString& value)
{
    mDefaultSavePath = value;
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mSettings->setValue("mDefaultSetupPath", mDefaultSavePath);
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save default setup save path to.";
}

void SimulationManager::setDefaultPluginPath(const QString& value)
{
    mDefaultPluginPath = value;
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mSettings->setValue("mDefaultPluginPath", mDefaultPluginPath);
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save default plugin path to.";

}

void SimulationManager::setDefaultSetupPath(const QString& value)
{
    mDefaultSetupPath = value;
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mSettings->setValue("mDefaultSetupPath", mDefaultSetupPath);
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save default setup path to.";

}

void SimulationManager::setDefaultSetupExtensionText(const QString& value)
{
    mDefaultSetupExtensionText = value;
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mSettings->setValue("mDefaultSetupExtensionText", mDefaultSetupExtensionText);
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save default setup extension text to.";
}

void SimulationManager::setDefaultSetupExtensionSuffix(const QString& value)
{
    mDefaultSetupExtensionSuffix = value;
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mSettings->setValue("mDefaultSetupExtensionSuffix", mDefaultSetupExtensionSuffix);
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save default setup extension suffix to.";
}

const QString& SimulationManager::getDefaultSavePath() const
{
    return mDefaultSavePath;
}

const QString& SimulationManager::getDefaultPluginPath() const
{
    return mDefaultPluginPath;
}

const QString& SimulationManager::getDefaultSetupPath() const
{
    return mDefaultSetupPath;
}

const QString& SimulationManager::getDefaultSetupExtensionText() const
{
    return mDefaultSetupExtensionText;
}

const QString& SimulationManager::getDefaultSetupExtensionSuffix() const
{
    return mDefaultSetupExtensionSuffix;
}

const QStringList& SimulationManager::getExecutableExtensions() const
{
    return mExecutableExtensions;
}

const QString& SimulationManager::getExecutableExtensionText() const
{
    return mExecutableExtensionText;
}

const QString& SimulationManager::getExecutableSuffix() const
{
    return mExecutableSuffix;
}

const QStringList& SimulationManager::getScriptExtensions() const
{
    return mScriptExtensions;
}

const QString& SimulationManager::getScriptExtensionText() const
{
    return mScriptExtensionText;
}

const QString& SimulationManager::getScriptSuffix() const
{
    return mScriptSuffix;
}

const QStringList& SimulationManager::getScriptIncludeDirectories() const
{
    return mScriptIncludeDirectories;
}

const QStringList& SimulationManager::getExecutableIncludeDirectories() const
{
    return mExecutableIncludeDirectories;
}

const QStringList& SimulationManager::getSetupIncludeDirectories() const
{
    return mSetupIncludeDirectories;
}

const QStringList& SimulationManager::getScriptIncludeFiles() const
{
    return mScriptIncludeFiles;
}

const QStringList& SimulationManager::getExecutableIncludeFiles() const
{
    return mExecutableIncludeFiles;
}

const QStringList& SimulationManager::getSetupIncludeFiles() const
{
    return mSetupIncludeFiles;
}

QStringList SimulationManager::getFileExtensions() const
{
    return mExecutableExtensions + mScriptExtensions;
}


shared_ptr<ServerThread> SimulationManager::getSparkServerThread() const
{
    return SimulationManager::mServerThread;
}

shared_ptr<ServerThread> SimulationManager::initSparkServerThread(ServerThread* server)
{
    if (mServerThread.get())
    {
        LOG_ERROR() << "Attempt to register a second spark server thread.";
    }
    else
    {
        mServerThread = shared_ptr<ServerThread>(server);
    }

    return mServerThread;
}

void SimulationManager::addSetupIncludePath(const QString& path)
{
    QFileInfo info(path);

    if (info.exists())
    {
        if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
        {
            if (info.isDir())
            {
                bool added = addIncludeDirectory(path);
                if (added)
                {
                    mSettings->setValue("mSetupPaths", mSetupIncludeDirectories);
                }
            }
            if (info.isFile())
            {
                bool added = addIncludeFile(path);
                if (added)
                {
                    mSettings->setValue("mSetupFiles", mSetupIncludeFiles);
                }
            }
            mSettings->endGroup(true); //unlock
        }
        else
            LOG_ERROR() << "Could not access Settings object to save SimulationManager settings to.";
    }
    else
    {
        LOG_ERROR() << "Path " << path << " does not exist.";
    }
}

bool SimulationManager::containsSetupIncludeDirectory(const QString& path) const
{
    QFileInfo dir(path);
    for (QStringList::ConstIterator it = mSetupIncludeDirectories.begin(); it != mSetupIncludeDirectories.end(); it++)
    {
        QDir contained = QDir(*it);
        if (contained == dir.absoluteDir())
        {
            return true;
        }
    }

    return false;
}

bool SimulationManager::containsSetupIncludeFile(const QString& path) const
{
    QFileInfo info(path);

    for (QStringList::ConstIterator it = mSetupIncludeFiles.begin(); it != mSetupIncludeFiles.end(); it++)
    {
        if (QFileInfo(*it) == info)
        {
            return true;
        }
    }

    return false;
}

bool SimulationManager::containsSetupIncludePath(const QString& path) const
{
    QFileInfo info(path);
    if (info.isDir())
    {
        return containsSetupIncludeDirectory(path);
    }
    if (info.isFile())
    {
        QFileInfo info(path);
        bool containsParentDir = containsSetupIncludeDirectory(path);
        if (containsParentDir)
            return true;

        return containsSetupIncludeFile(path);
    }

    return false;
}

const QString& SimulationManager::getDefaultSetup() const
{
    return mDefaultSetup;
}

const SimulationSetup* SimulationManager::getInitializedSetup() const
{
    if (!simulationInitialized())
        return 0;

    return &*mSimulation->getSetup();
}

int SimulationManager::getInitializedSetupIndex() const
{
    if (!simulationInitialized())
        return -1;

    int index = 0;
    for (TSetupList::const_iterator it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        if ((*it)->getName().compare(mSimulation->getSetup()->getName()) == 0)
        {
            return index;
        }
        index++;
    }

    return -1;
}

void SimulationManager::setDefaultSetup(const QString& name)
{
    mDefaultSetup = name;
}

const QString& SimulationManager::getLastLoadedSetup() const
{
    return mLastLoadedSetup;
}

const QString& SimulationManager::getLastInitializedSetup() const
{
    return mLastInitializedSetup;
}

const QString& SimulationManager::getLastLoadedSetupPath() const
{
    return mLastLoadedSetupPath;
}

const QString& SimulationManager::getLastInitializedSetupPath() const
{
    return mLastInitializedSetupPath;
}

void SimulationManager::loadSimulationSetups()
{
    emit aboutToReloadSetupList();
    mReloadingSetups = true;

    LOG_DEBUG() << "Loading simulation setups...";

    mSetupList.clear();

    QStringList completeList;
    //Add all from directories
    bool changed = false;
    for (QStringList::Iterator it = mSetupIncludeDirectories.begin(); it != mSetupIncludeDirectories.end();)
    {
        QDir dir(*it);

        if (!dir.exists())
        {
            //Remove nonexisting directories
            it = mSetupIncludeDirectories.erase(it);
            changed = true;
            continue;
        }

        dir.setNameFilters(mDefaultSetupExtensions);
        dir.setFilter(QDir::Files);

        QFileInfoList entries = dir.entryInfoList();
        for (int i = 0; i < entries.size(); i++)
        {
            QString abs = entries.at(i).absoluteFilePath();
            if (!completeList.contains(abs))
                completeList.push_back(abs);
        }

        it++;
    }
    //Add all from single includes
    for (QStringList::Iterator it = mSetupIncludeFiles.begin(); it != mSetupIncludeFiles.end(); it++)
    {
        if (!completeList.contains(*it))
            completeList.push_back(*it);
    }

    if (changed)
    {
        //Include directories were changed
        if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
        {
            mSettings->setValue("mSetupPaths", mSetupIncludeDirectories);
            mSettings->endGroup(true); //unlock
        }
        else
            LOG_ERROR() << "Could not access settings object.";
    }

    //Load all setups from complete list
    int loaded = 0;
    for (QStringList::Iterator it = completeList.begin(); it != completeList.end(); it++)
    {
        shared_ptr<SimulationSetup> loadedSetup = loadSimulationSetup(*it, true); //and store it
        if (!loadedSetup.get())
        {
            LOG_WARNING() << "Error while loading Simulation Setup. Skipping file " << *it << ".";
        }
        else
            loaded++;
    }


    emit setupListReloaded();
    mReloadingSetups = false;

    int failed = completeList.size() - loaded;
    QString failedString = QString(" Failed to load %1 Setups.").arg(failed);
    LOG_INFO() << loaded << " Simulation Setups loaded." << ((failed > 0) ? failedString.toStdString().c_str() : "");
}

shared_ptr<SimulationSetup> SimulationManager::loadSimulationSetup(const QString& path, bool store)
{
    QFileInfo info(path);

    if (!info.exists())
    {
        LOG_ERROR() << "Path " << path << " does not exist.";
        return shared_ptr<SimulationSetup>();
    }

    if (!info.isReadable())
    {
        LOG_ERROR() << "Path " << path << " is not readable.";
        return shared_ptr<SimulationSetup>();
    }

    if (info.isDir())
    {
        //If a directory is given, load all sub-files
        QDir dir(path);
        dir.setNameFilters(mDefaultSetupExtensions);
        dir.setFilter(QDir::Files);

        QFileInfoList entries = dir.entryInfoList();
        for (int i = 0; i < entries.size(); i++)
        {
            QString abs = entries.at(i).absoluteFilePath();

            LOG_DEBUG() << "Loading contained Setup Definition file " << abs;
            bool success = static_cast<bool>(loadSimulationSetup(abs, false));
            if (!success)
                LOG_WARNING() << "Loading contained Setup Definition " << abs << " failed.";
        }
    }

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        LOG_ERROR() << "Could not open Simulation Setup definition file at " << path << ".";
        return shared_ptr<SimulationSetup>();
    }
    else
    {
        LOG_DEBUG() << "Opened Simulation Setup File " << path << ".";
    }
    QString fname = file.fileName();

    QDomDocument setupDocument;

    QString errorMsg;
    int errorLine;
    int errorColumn;
    if (!setupDocument.setContent(&file, &errorMsg, &errorLine, &errorColumn)) {
        LOG_ERROR() << "Failed to load Simulation Setup definition file " << path << ": error at line " << errorLine << ", column " << errorColumn << " : " << errorMsg;
        file.close();
        return shared_ptr<SimulationSetup>();
    }
    else
    {
        LOG_DEBUG() << "Succeeded to load Simulation Setup definition file.";
    }

    //Is it already contained?
    shared_ptr<SimulationSetup> loadedSetup = loadSimulationSetup(setupDocument, path, fname);
    int index = getSetupIndexByFilePath(loadedSetup->getFileName());
    if (index != -1)
    {
        //update it
        updateSimulationSetup(index, *loadedSetup, false);
    }
    else
    {
        if (store)
        {
            //store it
            mSetupList.push_back(loadedSetup);
        }
    }

    return loadedSetup;
}

shared_ptr<SimulationSetup> SimulationManager::saveSimulationSetup(boost::shared_ptr<SimulationSetup> setup, bool overwrite, bool rememberPath)
{
    shared_ptr<SimulationSetup> returning = shared_ptr<SimulationSetup>();
    int err;

    returning = saveSimulationSetupProtected(setup, err, overwrite, rememberPath);

    if (err == 0)
    {
        //Saved
        LOG_INFO() << "Saved simulation setup " << setup->getName().toStdString().c_str() << " to " << setup->getFileName();
    }
    else if (err == 1)
    {
        LOG_ERROR() << "Failed to save simulation setup " << setup->getName().toStdString().c_str() << " to " << setup->getFileName();
    }
    else if (err == 2)
    {
        //Did not save
        LOG_DEBUG()  << "Did not save simulation setup " << setup->getName().toStdString().c_str();
    }
    return returning;
}

bool SimulationManager::saveAllSimulationSetups(bool askForAll, bool askForEach)
{
    int saved = 0;
    int count = 0;
    int skippedcount = 0;
    QString namelist;
    std::vector<int> indexlist;

    //search all unsaved
    int i = 0;
    for (TSetupList::iterator it = mSetupList.begin(); it != mSetupList.end(); it++)
    {
        if (!(*it)->isToSave())
        {
            //Skip this
            skippedcount++;
        }
        else if ((*it)->isSaved() == false)
        {
            count++;
            if (count != 0)
                namelist.append(", ");
            namelist.append((*it)->getName());
            indexlist.push_back(i);
        }
        i++;
    }

    LOG_DEBUG() << "Found " << count << " unsaved files. " << skippedcount << " skipped.";

    if (count == 0)
        return true;

    if (askForAll && count == 1)
    {
        askForAll = false;
        askForEach = true;
    }

    bool saveAll = true;
    if (askForAll)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle("Save Simulation Setups");
        messageBox.setText(QString(tr("Do you want to save all unsaved Simulation Setup files?")));
        messageBox.setInformativeText(QString(tr("Unsaved Setups: %1%2")).arg(namelist).arg(
            askForEach ? "\n (Press no to decide for each setup individually)" : ""));
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        messageBox.setIcon(QMessageBox::Question);

        int result = messageBox.exec();
        if (result == QMessageBox::Yes)
            saveAll = true;
        if (result == QMessageBox::No)
            saveAll = false;
    }

    //Save all
    for (std::vector<int>::iterator it = indexlist.begin(); it != indexlist.end(); it++)
    {
        bool saveThis = saveAll;

        if (askForEach)
        {
            //Ask individually
            QMessageBox messageBox;
            messageBox.setWindowTitle(tr("Save Simulation Setup"));
            messageBox.setText(QString(tr("Do you want to save unsaved Simulation Setup '%1'?")).arg(mSetupList.at(*it)->getName()));
            messageBox.setInformativeText(QString(tr("File: %1")).
                arg(mSetupList.at(*it)->getFileName().compare("") != 0 ?
                    QString("'%1'").arg(mSetupList.at(*it)->getFileName()) : QString("(not defined)")));
            messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            messageBox.setDefaultButton(QMessageBox::No);
            messageBox.setIcon(QMessageBox::Question);

            int result = messageBox.exec();
            if (result == QMessageBox::Yes)
                saveThis = true;
            if (result == QMessageBox::No)
                saveThis = false;
        }
        else
            saveThis = true;

        if (saveThis)
        {
            //save
            saveSimulationSetup(mSetupList.at(*it), false, false);
            saved++;
        }
    }

    return saved > 0;
}

bool SimulationManager::updateSimulationSetup(const QString& path, const SimulationSetup& setup, bool askReloadCurrent)
{
    int index = getSetupIndexByFilePath(path);
    if (index == -1)
        return false;
    return updateSimulationSetup(index, setup, askReloadCurrent);
}

bool SimulationManager::updateSimulationSetup(int index, const SimulationSetup& setup, bool askReloadCurrent)
{
    if (index < 0 || index >= (int)mSetupList.size())
    {
        LOG_ERROR() << "Setup file index " << index << " out of range.";
        return false;
    }

    //Copy content
    mSetupList.at(index)->readData(setup, true); //deep copy
    emit setupListEntryChanged(index);

    //Current setup changed?
    if (simulationInitialized() &&
        (mSimulation->getSetup()->getName().compare(setup.getName()) == 0) &&
        askReloadCurrent)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle(QString(tr("Reload current Simulation Setup?")));
        messageBox.setText(QString(tr("The currently used Simulation Setup has been changed. Do you want to restart the current Simulation? All running simulation threads will be stopped.")));
        messageBox.setInformativeText(QString(tr("Setup: '%1', File: '%2'")).arg(setup.getName()).
                                      arg(setup.getFileName()));
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::No);
        messageBox.setIcon(QMessageBox::Question);

        int answer = messageBox.exec();

        if (answer == QMessageBox::Yes)
        {
            LOG_INFO() << "Restarting setup...";

            //Restart
            stopSimulation();

            //Initialize new Setup
            initializeSimulation(setup.getName(), true);
        }
        else
            LOG_DEBUG() << "Updated currently used setup without reloading.";
    }

    return true;
}

bool SimulationManager::simulationInitialized() const
{
    return mSimulation->isInitialized();
}

bool SimulationManager::simulationRunning() const
{
    return mSimulation->isRunning();
}

bool SimulationManager::simulationPaused() const
{
    return mSimulation->isPaused();
}

const shared_ptr<Simulation> SimulationManager::getSimulation() const
{
    return mSimulation;
}

shared_ptr<Simulation> SimulationManager::getSimulation()
{
    return mSimulation;
}

bool SimulationManager::initializeSimulationFromFile(const QString& path, bool reinitialize, bool loadData)
{
    QFileInfo file(path);
    if (!file.exists())
    {
        LOG_ERROR() << "File " << path << " not found.";
        return false;
    }

    //If already initialized, only proceed in reinitialize is set
    bool reinitialized = false;
    bool loaded = false;
    if (simulationInitialized() && mSimulation->getSetup()->getFileName().compare(file.absoluteFilePath()) == 0)
    {
        if (reinitialize)
            reinitialized = true;
        else
        {
            LOG_INFO() << "Setup from file " << path << " already initialized in current Simulation.";
            loaded = true;
        }
    }

    //Try to load the setup
    bool success = false;
    if (!loaded)
    {
        shared_ptr<SimulationSetup> loadedSetup = loadSimulationSetup(file.absoluteFilePath(), true);

        if (!loadedSetup.get())
        {
            LOG_ERROR() << "Failed to load simulation setup from file " << path << ".";
            return false;
        }

        mLastLoadedSetup = loadedSetup->getName();
        mLastLoadedSetupPath = loadedSetup->getFileName();
        mLastInitializedSetup = loadedSetup->getName();
        mLastInitializedSetupPath = loadedSetup->getFileName();
        emit aboutToInitializeSetup(loadedSetup->getName(), loadedSetup->getFileName());

        //Stop current simulation
        if (simulationRunning())
            stopSimulation();

        bool initializedState = mSimulation->initialize(*loadedSetup);
        if (!initializedState)
        {
            LOG_ERROR() << "Error while loading Simulation with setup " << mSimulation->getSetup()->getName() << ".";
            return false;
        }

        emit setupInitialized(mSimulation->getSetup()->getName(), mSimulation->getSetup()->getFileName());
        LOG_INFO() << (reinitialized ? "Reinitialized" : "Initialized") << " Simulation with setup " << mSimulation->getSetup()->getName() << ".";

        success = true;
    }
    else //already loaded, load only data if requested
    {
        if (loadData)
        {
            //Stop current simulation
            bool running = simulationRunning();
            LOG_INFO() << "Reloading initialized simulation." << (running ? "Simulation will be stopped and restarted." : "");

            if (running)
                stopSimulation();

            //Load data
            mSimulation->loadData();

            //Restart simulation
            if (running)
                runSimulation();

            success = true;
        }
        else
            success = false;
    }

    return success;
}

bool SimulationManager::initializeSimulation(const QString& name, bool reinitialize)
{
    if (!containsSetup(name))
    {
        LOG_ERROR() << "No such setup " << name << " found.";
        return false;
    }

    //If already initialized, only proceed in reinitialize is set
    bool reinitialized = false;
    if (simulationInitialized() && mSimulation->getSetup()->getName().compare(name) == 0)
    {
        if (reinitialize)
            reinitialized = true;
        else
        {
            LOG_INFO() << "Setup " << name << " already initialized in current Simulation.";
            return 0;
        }
    }

    shared_ptr<SimulationSetup> setup(mSetupList.at(getSetupIndex(name)));
    emit aboutToInitializeSetup(name, setup->getFileName());

    //Stop current simulation
    if (simulationRunning())
        stopSimulation();

    bool initializedState = mSimulation->initialize(*setup);
    if (!initializedState)
    {
        LOG_ERROR() << "Error while initializing Simulation with setup " << mSimulation->getSetup()->getName() << ".";
        return false;
    }

    //Initialized setup from setup list
    mLastInitializedSetup = name;
    mLastInitializedSetupPath = setup->getFileName();
    emit setupInitialized(name, setup->getFileName());
    LOG_INFO() << (reinitialized ? "Reinitialized" : "Initialized") << " Simulation with setup " << mSimulation->getSetup()->getName() << ".";

    return true;
}

bool SimulationManager::runSimulation(bool runTasks)
{
    if (!simulationInitialized())
    {
        LOG_WARNING() << "Cant run. No Simulation initialized.";
        return false;
    }

    bool running = mSimulation->run(runTasks);
    if (!running)
    {
        LOG_ERROR() << "Failed to run Simulation.";
        return false;
    }

    emit setupRunning(mSimulation->getSetup()->getName());

    return true;
}

bool SimulationManager::stopSimulation(bool stopall)
{
    if (!simulationRunning() && !stopall)
        return false;

    mSimulation->stop(stopall);
    emit setupStopped(mSimulation->getSetup()->getName());

    return true;
}

bool SimulationManager::pauseSimulation()
{
    bool paused = mSimulation->pause();

    if (paused)
       emit setupPaused(mSimulation->getSetup()->getName());

    return paused;
}

bool SimulationManager::restartSimulation()
{
    bool restarted = mSimulation->restart();

    return restarted;
}


shared_ptr<TaskDefinition> SimulationManager::newTaskDefinition(TaskDefinition::ETaskType type) const
{
    const QList<QVariant>* list;
    switch(type)
    {
    case TaskDefinition::TT_SERVERTHREAD: list = &mDefaultServerThreadData; break;
    case TaskDefinition::TT_PLUGINTHREAD: list = &mDefaultPluginThreadData; break;
    case TaskDefinition::TT_PROCESS:      list = &mDefaultProcessData; break;
    case TaskDefinition::TT_SPARKPROCESS: list = &mDefaultSparkProcessData; break;
    default:
        LOG_ERROR() << "Unknown task type " << type;
        return shared_ptr<TaskDefinition>(new TaskDefinition());
        break;
    }

    if (list->size() != 7)
    {
        LOG_ERROR() << "Task definition list has illegal size.";
        return shared_ptr<TaskDefinition>(new TaskDefinition());
    }

    return shared_ptr<TaskDefinition>(new TaskDefinition(type,
        list->at(0).toString(), //name
        (QThread::Priority)list->at(1).toInt(), //threadpriority
        list->at(2).toString(), //executable
        list->at(3).toString(), //main script
        list->at(4).toString(), //secondary script
        list->at(5).toStringList(), //additional scripts
        list->at(6).toStringList())); //parameters
}

bool SimulationManager::hasExecutable(const QString& name) const
{
    return findExecutable(name).compare("") != 0;
}

QString SimulationManager::findExecutable(const QString& name) const
{
    return findFile(name, mExecutableIncludeDirectories, mExecutableIncludeFiles, "Executable", mExecutableSuffix);
}

bool SimulationManager::hasScript(const QString& name) const
{
    return findScript(name).compare("") != 0;
}

QString SimulationManager::findScript(const QString& name) const
{
    return findFile(name, mScriptIncludeDirectories, mScriptIncludeFiles, "Script", mScriptSuffix);
}

QString SimulationManager::findFile(const QString& name, const QStringList& includeDirectories, const QStringList& includeFiles, const std::string& type, const QString& suffix) const
{
    mIncludePathsMutex.lock();

    QString suffixName;
    bool searchWithSuffix = false;
    int from = name.lastIndexOf("\\|/");
    if (from == -1)
        from = 0;
    if (name.indexOf('.', from) != -1)
    {
        //has suffix
    }
    else
    {
        //add default suffix otionally
        if (suffix.compare("") == 0)
        {
            searchWithSuffix = false;
        }
        else
        {
            searchWithSuffix = true;
            suffixName = name + "." + suffix;
        }
    }

    //Search in all include directories
    bool found = false;
    QString foundPath("");
    for (QStringList::ConstIterator it = includeDirectories.begin(); it != includeDirectories.end(); it++)
    {
        QDir dir(*it);

        if (!dir.exists())
        {
            LOG_WARNING() << "Directory " << dir.absolutePath() << " does not exist.";
            continue;
        }

        if (dir.entryList().contains(name))
        {
            if (found == false)
            {
                foundPath = dir.absolutePath().append('/').append(name);
                found = true;
            }
            else
            {
                QString ambPath = dir.absolutePath().append('/').append(name);
                if (ambPath.compare(foundPath) != 0)
                    LOG_WARNING() << type.c_str() << " name " << name << " is ambiguous. It matches " << foundPath << " and " << ambPath;
            }
        }

        if (searchWithSuffix)
        if (dir.entryList().contains(suffixName))
        {
            if (found == false)
            {
                foundPath = dir.absolutePath().append('/').append(suffixName);
                found = true;
            }
            else
            {
                QString ambPath = dir.absolutePath().append('/').append(suffixName);
                if (ambPath.compare(foundPath) != 0)
                    LOG_WARNING() << type.c_str() << " name " << name << " is ambiguous. It matches " << foundPath << " and " << ambPath;
            }
        }
    }

    //Search single include files
    for (QStringList::ConstIterator it = includeFiles.begin(); it != includeFiles.end(); it++)
    {
        QFileInfo info(*it);

        if (!info.exists())
        {
            LOG_WARNING() << "Path " << info.absolutePath() << " does not exist.";
            continue;
        }

        if (info.fileName().compare(name) == 0)
        {
            if (found == false)
            {
                foundPath = info.absoluteFilePath();
                found = true;
            }
            else
            {
                QString ambPath = info.absoluteFilePath();
                LOG_WARNING() << type.c_str() << " name " << name << " is ambiguous. It matches " << foundPath << " and " << ambPath;
            }
        }

        if (searchWithSuffix)
        if (info.fileName().compare(suffixName) == 0)
        {
            if (found == false)
            {
                foundPath = info.absoluteFilePath();
                found = true;
            }
            else
            {
                QString ambPath = info.absoluteFilePath();
                LOG_WARNING() << type.c_str() << " name " << name << " is ambiguous. It matches " << foundPath << " and " << ambPath;
            }
        }
    }

    mIncludePathsMutex.unlock();

    return foundPath;
}

void SimulationManager::findScripts(QStringList& target, bool absolute) const
{
    return findFiles(target, mScriptIncludeDirectories, mScriptIncludeFiles, QStringList("*."+mScriptSuffix), absolute);
}

void SimulationManager::findScripts(QStringList& target, const QStringList& directories, bool absolute) const
{
    return findFiles(target, directories, QStringList(), QStringList("*."+mScriptSuffix), absolute);
}

void SimulationManager::findFiles(QStringList& target, const QStringList& includeDirectories, const QStringList& includeFiles,
    const QStringList& filters, bool absolute) const
{
    mIncludePathsMutex.lock();

    //Search in all include directories
    bool found = false;
    QString foundPath("");
    for (QStringList::ConstIterator it = includeDirectories.begin(); it != includeDirectories.end(); it++)
    {
        QDir dir(*it);

        if (!dir.exists())
        {
            LOG_WARNING() << "Directory " << dir.absolutePath() << " does not exist.";
            continue;
        }

        QFileInfoList files = dir.entryInfoList(filters);
        for (auto files_it = files.begin(); files_it != files.end(); files_it++)
        {
            target.append(absolute ? files_it->absoluteFilePath() : files_it->fileName());
        }
    }

    //Search single include files
    for (QStringList::ConstIterator it = includeFiles.begin(); it != includeFiles.end(); it++)
    {
        QFileInfo info(*it);

        if (!info.exists())
        {
            LOG_WARNING() << "Path " << info.absolutePath() << " does not exist.";
            continue;
        }

        target.append(absolute ? info.absoluteFilePath() : info.fileName());
    }

    mIncludePathsMutex.unlock();
}

QVariant SimulationManager::getDefaultTaskData(TaskDefinition::ETaskType type, TaskDefinition::ETaskData field) const
{
    return getDefaultTaskData(type, (int)field);
}

QVariant SimulationManager::getDefaultTaskData(TaskDefinition::ETaskType type, int field) const
{
    const QList<QVariant>* list;
    switch(type)
    {
    case TaskDefinition::TT_SERVERTHREAD: list = &mDefaultServerThreadData; break;
    case TaskDefinition::TT_PLUGINTHREAD: list = &mDefaultPluginThreadData; break;
    case TaskDefinition::TT_PROCESS:      list = &mDefaultProcessData; break;
    case TaskDefinition::TT_SPARKPROCESS: list = &mDefaultSparkProcessData; break;
    default:
        LOG_ERROR() << "Unknown task type " << type;
        return QVariant();
        break;
    }

    if (list->size() <= field)
    {
        LOG_ERROR() << "Field id " << field << " out of range. Task definition list has only " << list->size() << " entries.";
        return QVariant();
    }

    return list->at(field);
}

void SimulationManager::setDefaultTaskData(TaskDefinition::ETaskType type, TaskDefinition::ETaskData field, const QVariant& value)
{
    return setDefaultTaskData(type, (int)field, value);
}

void SimulationManager::setDefaultTaskData(TaskDefinition::ETaskType type, int field, const QVariant& value)
{
    QList<QVariant>* list;
    switch(type)
    {
    case TaskDefinition::TT_SERVERTHREAD: list = &mDefaultServerThreadData; break;
    case TaskDefinition::TT_PLUGINTHREAD: list = &mDefaultPluginThreadData; break;
    case TaskDefinition::TT_PROCESS:      list = &mDefaultProcessData; break;
    case TaskDefinition::TT_SPARKPROCESS: list = &mDefaultSparkProcessData; break;
    default:
        LOG_ERROR() << "Unknown task type " << type;
        return;
        break;
    }

    if (list->size() >= field)
    {
        LOG_ERROR() << "Field id " << field << " out of range. Task definition list has only " << list->size() << " entries.";
        return;
    }

    list->replace(field, value);
}

void SimulationManager::lockIncludePaths()
{
    mIncludePathsMutex.lock();
}

void SimulationManager::unlockIncludePaths()
{
    mIncludePathsMutex.unlock();
}

void SimulationManager::createIncludeFileContextMenu(QMenu* menu, std::vector<NamedActionReceiver*>& actionReceivers,
    QObject* signalTarget, const char* slot, const QStringList& includePaths, const QStringList& includeFiles,
    const QStringList& filters, bool absolute)
{
    QMenu* submenu;
    NamedActionReceiver* receiver;
    QAction* action;

    //Add all file directories
    for (auto it = includePaths.begin(); it != includePaths.end(); it++)
    {
        submenu = menu->addMenu(*it);

        //Add gile paths to list
        QStringList files;
        findFiles(files, QStringList(*it), QStringList(), filters, absolute);
        for (auto it = files.begin(); it != files.end(); it++)
        {
            receiver = new NamedActionReceiver(*it);
            actionReceivers.push_back(receiver);
            action = submenu->addAction(*it, receiver, SLOT(receiveAction()));
            connect(receiver, SIGNAL(actionReceived(const QString&)), signalTarget, slot);
        }
    }

    //Add single include files
    if (includeFiles.size() > 0)
    {
        submenu = menu->addMenu(tr("Single include files"));

        for (auto it = includeFiles.begin(); it != includeFiles.end(); it++)
        {
            receiver = new NamedActionReceiver(*it);
            actionReceivers.push_back(receiver);
            action = submenu->addAction(*it, receiver, SLOT(receiveAction()));
            connect(receiver, SIGNAL(actionReceived(const QString&)), signalTarget, slot);
        }
    }
}

void SimulationManager::createScriptContextMenu(QMenu* menu, std::vector<NamedActionReceiver*>& actionReceivers, QObject* signalTarget,
    const char* slot, bool absolute)
{
    lockIncludePaths();
    QStringList includeDirs(getScriptIncludeDirectories());
    QStringList includeFiles(getScriptIncludeFiles());
    unlockIncludePaths();

    QStringList filters;
    filters << QString("*." + getScriptSuffix());

    createIncludeFileContextMenu(menu, actionReceivers, signalTarget, slot, includeDirs, includeFiles, filters, absolute);
}

void SimulationManager::createExecutableContextMenu(QMenu* menu, std::vector<NamedActionReceiver*>& actionReceivers, QObject* signalTarget,
    const char* slot, bool absolute)
{
    lockIncludePaths();
    QStringList includeDirs(getExecutableIncludeDirectories());
    QStringList includeFiles(getExecutableIncludeFiles());
    unlockIncludePaths();

    QStringList filters;
    filters << QString("*." + getExecutableSuffix());

    createIncludeFileContextMenu(menu, actionReceivers, signalTarget, slot, includeDirs, includeFiles, filters, absolute);
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void SimulationManager::loadDefaultSettings()
{
    mIncludePathsMutex.lock();

    mDefaultPluginPath = "plugins/";
    mDefaultSetupPath = Carbon::getUserDataFolder()+"setups/";
    mDefaultSavePath = Carbon::getUserDataFolder()+"save/";
    mDefaultSetupExtensions << "*.xml";
    mDefaultSetupExtensionText = "XML-Files (*.xml)";
    mDefaultSetupExtensionSuffix = "xml";
    mDefaultSetup = "DefaultSetupGrid"; //May be respecified by the main window
    mLastLoadedSetup = "";
    mLastInitializedSetup = "";
    mLastLoadedSetupPath = "";
    mLastInitializedSetupPath = "";

    mSetupIncludeDirectories.clear();
    mSetupIncludeDirectories.push_back(mDefaultSetupPath);
    mScriptIncludeDirectories.clear();
    mExecutableIncludeDirectories.clear();
    mSetupIncludeFiles.clear();
    mScriptIncludeFiles.clear();
    mExecutableIncludeFiles.clear();
    mScriptSuffix = "rb";
    QString dotScriptSuffix = ".rb";
    mScriptExtensions << "*.rb";
    mScriptExtensionText = "Ruby Files (*.rb)";
    #if defined(Q_OS_WIN)
    mExecutableSuffix = "exe";
    QString dotExecutableSuffix = ".exe";
    mExecutableExtensions << "*.exe";
    mExecutableExtensionText = "Executable Files (*.exe)";
    #elif defined(Q_OS_UNIX)
    mExecutableSuffix = "";
    QString dotExecutableSuffix = "";
    mExecutableExtensions << "*";
    mExecutableExtensionText = "Executable Files (*)";
    #endif

    mDefaultServerThreadData.clear();
    mDefaultServerThreadData.push_back("Server");                       //name
    mDefaultServerThreadData.push_back((int)QThread::InheritPriority);  //priority
    mDefaultServerThreadData.push_back("");                             //executable
    mDefaultServerThreadData.push_back("sparkgui" + dotScriptSuffix);   //main script
    mDefaultServerThreadData.push_back("simserver"+ dotScriptSuffix);   //secondary script
    mDefaultServerThreadData.push_back(QStringList());                  //additional scripts
    mDefaultServerThreadData.push_back(QStringList());                  //parameters

    mDefaultPluginThreadData.clear();
    mDefaultPluginThreadData.push_back("Plugin");                       //name
    mDefaultPluginThreadData.push_back((int)QThread::InheritPriority);  //priority
    mDefaultPluginThreadData.push_back("");                             //class
    mDefaultPluginThreadData.push_back("");                             //caption
    mDefaultPluginThreadData.push_back("normal");                       //type
    mDefaultPluginThreadData.push_back(QStringList());                  //parameters

    mDefaultProcessData.clear();
    mDefaultProcessData.push_back("Process");                           //name
    mDefaultProcessData.push_back((int)QThread::InheritPriority);       //priority
    mDefaultProcessData.push_back("rcssagent3d" + dotExecutableSuffix); //executable
    mDefaultProcessData.push_back("");                                  //main script
    mDefaultProcessData.push_back("");                                  //secondary script
    mDefaultProcessData.push_back(QStringList());                       //additional scripts
    mDefaultProcessData.push_back(QStringList());                       //parameters

    mDefaultSparkProcessData.clear();
    mDefaultSparkProcessData.push_back("SparkProcess");                        //name
    mDefaultSparkProcessData.push_back((int)QThread::InheritPriority);         //priority
    mDefaultSparkProcessData.push_back("rcssmonitor3d" + dotExecutableSuffix); //executable
    mDefaultSparkProcessData.push_back("");                                    //main script
    mDefaultSparkProcessData.push_back("");                                    //secondary script
    mDefaultSparkProcessData.push_back(QStringList());                         //additional scripts
    mDefaultSparkProcessData.push_back(QStringList());                         //parameters

    mIncludePathsMutex.unlock();
}

void SimulationManager::loadSettings()
{
    //--------------------------------------------
    //Manager Settings

    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mIncludePathsMutex.lock();

        mDefaultPluginPath           = mSettings->value("mDefaultPluginPath", mDefaultPluginPath).toString();
        mDefaultSetupPath            = mSettings->value("mDefaultSetupPath", mDefaultSetupPath).toString();
        mDefaultSavePath             = mSettings->value("mDefaultSavePath", mDefaultSavePath).toString();
        mDefaultSetupExtensions      = mSettings->value("mDefaultSetupExtensions", mDefaultSetupExtensions).toStringList();
        mDefaultSetupExtensionText   = mSettings->value("mDefaultSetupExtensionText", mDefaultSetupExtensionText).toString();
        mDefaultSetupExtensionSuffix = mSettings->value("mDefaultSetupExtensionSuffix", mDefaultSetupExtensionSuffix).toString();
        mSetupIncludeDirectories     = mSettings->value("mSetupPaths", mSetupIncludeDirectories).toStringList();
        mScriptIncludeDirectories    = mSettings->value("mScriptPaths", mScriptIncludeDirectories).toStringList();
        mExecutableIncludeDirectories= mSettings->value("mExecutablePaths", mExecutableIncludeDirectories).toStringList();
        mSetupIncludeFiles           = mSettings->value("mSetupFiles", mSetupIncludeFiles).toStringList();
        mScriptIncludeFiles          = mSettings->value("mScriptFiles", mScriptIncludeFiles).toStringList();
        mExecutableIncludeFiles      = mSettings->value("mExecutableFiles", mExecutableIncludeFiles).toStringList();
        mLastLoadedSetup             = mSettings->value("mLastLoadedSetup", mLastLoadedSetup).toString();
        mLastInitializedSetup        = mSettings->value("mLastInitializedSetup", mLastInitializedSetup).toString();
        mLastLoadedSetupPath         = mSettings->value("mLastLoadedSetupPath", mLastLoadedSetupPath).toString();
        mLastInitializedSetupPath    = mSettings->value("mLastInitializedSetupPath", mLastInitializedSetupPath).toString();

        mIncludePathsMutex.unlock();

        //--------------------------------------------
        //Task default values

        if (mSettings->contains("mDefaultServerThreadData")) mDefaultServerThreadData = mSettings->value("mDefaultServerThreadData", QVariant(QList<QVariant>())).toList();
        if (mSettings->contains("mDefaultPluginThreadData")) mDefaultPluginThreadData = mSettings->value("mDefaultPluginThreadData", QVariant(QList<QVariant>())).toList();
        if (mSettings->contains("mDefaultProcessData"     )) mDefaultProcessData      = mSettings->value("mDefaultProcessData",      QVariant(QList<QVariant>())).toList();
        if (mSettings->contains("mDefaultSparkProcessData")) mDefaultSparkProcessData = mSettings->value("mDefaultSparkProcessData", QVariant(QList<QVariant>())).toList();

        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to save SimulationManager settings to.";
}

void SimulationManager::saveSettings()
{
    //--------------------------------------------
    //Manager Settings

    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mIncludePathsMutex.lock();

        mSettings->setValue("mDefaultPluginPath", mDefaultPluginPath);
        mSettings->setValue("mDefaultSetupPath", mDefaultSetupPath);
        mSettings->setValue("mDefaultSavePath", mDefaultSavePath);
        mSettings->setValue("mDefaultSetupExtensions", mDefaultSetupExtensions);
        mSettings->setValue("mDefaultSetupExtensionText", mDefaultSetupExtensionText);
        mSettings->setValue("mDefaultSetupExtensionSuffix", mDefaultSetupExtensionSuffix);
        mSettings->setValue("mSetupPaths", mSetupIncludeDirectories);
        mSettings->setValue("mScriptPaths", mScriptIncludeDirectories);
        mSettings->setValue("mExecutablePaths", mExecutableIncludeDirectories);
        mSettings->setValue("mSetupFiles", mSetupIncludeFiles);
        mSettings->setValue("mScriptFiles", mScriptIncludeFiles);
        mSettings->setValue("mExecutableFiles", mExecutableIncludeFiles);
        mSettings->setValue("mLastLoadedSetup", mLastLoadedSetup);
        mSettings->setValue("mLastInitializedSetup", mLastInitializedSetup);
        mSettings->setValue("mLastLoadedSetupPath", mLastLoadedSetupPath);
        mSettings->setValue("mLastInitializedSetupPath", mLastInitializedSetupPath);

        mIncludePathsMutex.unlock();

        //--------------------------------------------
        //Task default values

        mSettings->setValue("mDefaultServerThreadData", mDefaultServerThreadData);
        mSettings->setValue("mDefaultPluginThreadData", mDefaultPluginThreadData);
        mSettings->setValue("mDefaultProcessData",      mDefaultProcessData     );
        mSettings->setValue("mDefaultSparkProcessData", mDefaultSparkProcessData);

        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access Settings object to load SimulationManager settings from. Keeping previous values.";
}

int SimulationManager::removeSimulationSetup(const QString& file, bool askDelete, bool remove)
{
    int index = getSetupIndexByFilePath(file);
    if (index == -1)
        return -1;

    bool success = removeSimulationSetup(index, askDelete, remove);
    if (success)
        return index;
    else
        return -1;
}

bool SimulationManager::removeSimulationSetup(int index, bool askDelete, bool remove)
{
    if (index < 0 || index >= (int)mSetupList.size())
    {
        LOG_ERROR() << "Index " << index << " out of bounds.";
        return false;
    }

    if (getInitializedSetupIndex() == index)
    {
        if (askDelete == true)
            LOG_INFO() << "Cant remove the currently initialized setup.";
        return false;
    }

    bool deleteThis = remove;

    if (askDelete)
    {
        if (mSetupList.at(index)->getFileName().compare("") == 0)
        {
            //no filename specified, nothing to delete
            LOG_DEBUG() << "Setup has to target file specified.";
        }
        else
        {
            QMessageBox messageBox;
            messageBox.setWindowTitle(QString(tr("Removing Setup")));
            messageBox.setText(QString(tr("Do you want to delete the SimulationSetup '%1'?")).arg(mSetupList.at(index)->getName()));
            messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
            messageBox.setDefaultButton(QMessageBox::No);
            messageBox.setIcon(QMessageBox::Question);
            int ret = messageBox.exec();

            if (ret == QMessageBox::Yes)
            {
                deleteThis = true;
            }
            if (ret == QMessageBox::Abort)
            {
                return false;
            }
        }
    }

    //Delete file
    if (deleteThis)
    {
        QFileInfo info(mSetupList.at(index)->getFileName());
        QDir dir = info.absoluteDir();
        QString file = info.fileName();

        if (!info.exists())
        {
            //Setup has no file definition, skip
            LOG_DEBUG() << "No file to delete.";
        }
        else
        {
            bool removed = info.absoluteDir().remove(info.fileName());
            if (removed)
            {
                LOG_INFO() << "File " << info.absoluteFilePath() << " deleted.";
            }
            else
            {
                LOG_INFO() << "Could not delete file " << info.absoluteFilePath() << ".";
            }
        }
    }

    //Erase from list
    mSetupList.erase(mSetupList.begin()+index);

    emit setupListEntryRemoved(index);
    if (!mReloadingSetups)
        emit setupListSingleEntryRemoved(index);

    return true;
}

shared_ptr<SimulationSetup> SimulationManager::saveSimulationSetupByDialog(bool overwrite, bool saveData)
{
    if (!simulationInitialized())
    {
        LOG_INFO() << "Cant save a new SimulationSetup from the current Simulation. Simulation has not been initialized.";
        return shared_ptr<SimulationSetup>();
    }

    //currently initialized setup. Const for this field is only meant to prevent plugins from changing the setup. As long as the changes are checked access is ok.
    shared_ptr<SimulationSetup> setup(mSimulation->getSetup());

    QString startingDirectory = setup->getFileName();

    //Specify the file, a new name & a new caption
    DefineSetupDialog setupdialog;
    setupdialog.setFile(startingDirectory);
    setupdialog.setName(setup->getName());

    setupdialog.setWindowTitle(tr("Save current simulation state"));
    setupdialog.setToolTip(tr("Saves the state of the current Simulation as a simulation setup save file."));

    bool checkFirst = overwrite; // if overwrite is true, check first if file is valid and overwrite without asking if possible
    bool repeat = true;
    while (repeat)
    {
        repeat = false;
        if (!checkFirst)
        {
            int setupreturning = setupdialog.exec();

            if (setupreturning == QDialog::Rejected)
            {
                //Abort
                return shared_ptr<SimulationSetup>();
            }
        }

        QString targetfile = setupdialog.getFilePath();
        QString targetname = setupdialog.getName();

        if (checkFirst)
        {
            targetfile = setup->getFileName(); //The other values are already up-to-date
            setupdialog.setFile(targetfile);
        }

        checkFirst = false;

        int index = getSetupIndexByFilePath(targetfile);
        if (index != -1)
        {
            int nameindex = getSetupIndex(targetname);
            if (nameindex != -1 && nameindex != index)
            {
                repeat = true;
                setupdialog.setMessage("The name of the setup is already in use by another setup in the include directories. Use a different name.");
            }
            else
            {
                //Setup in current setup list, update it
                mSetupList.at(index)->readData(*setup, true);
            }
        }

        if (!repeat)
        {
            //update setup
            setup->setFileName(targetfile);
            setup->setName(targetname);

            //Save as save state to target file
            auto savedSetup = saveSimulationSetup(setup, overwrite, true);
            if (savedSetup.get())
            {
                //Simulation uses this setup
                if (saveData)
                {
                    //Save all plugin data
                    mSimulation->saveData();
                }
            }
            return savedSetup;
        }
    }

    return shared_ptr<SimulationSetup>();
}

bool SimulationManager::loadSimulationSetupByDialog(bool loadFromSavePath, bool loadData)
{
    QString lastDirectory = loadFromSavePath ? mDefaultSavePath : mDefaultSetupPath;
    if (loadFromSavePath)
    {
        //todo : load last saved
    }
    else
    {
        int lastIndex = getSetupIndex(mLastLoadedSetup);
        if (lastIndex != -1)
            lastDirectory = mSetupList.at(lastIndex)->getFileName();
    }

    QFileDialog dialog((QWidget*)mParent->getWindowManager()->getMainWindow(), tr("Load setup from file."),
        lastDirectory, mDefaultSetupExtensionText);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setLabelText(QFileDialog::Accept, tr("Load"));
    dialog.setToolTip(tr("Load a simulation setup file."));
    dialog.setViewMode(QFileDialog::Detail);

    int returning = dialog.exec();
    if (returning != 0)
    {
        QStringList files = dialog.selectedFiles();
        QString file = files.at(0);

        int index = getSetupIndexByFilePath(file);
        bool includepath = containsSetupIncludePath(file); //loading from an include pathw

        //Load setup from file
        return initializeSimulationFromFile(file, false, loadData);
    }

    return false;
}

shared_ptr<SimulationSetup> SimulationManager::loadSimulationSetup(QDomDocument& document, const QString& path, const QString& fname)
{
    //Parse document
    QDomElement docElem = document.documentElement();
    QDomNode n = docElem.firstChild();

    bool hasName = false;
    bool hasCaption = false;
    bool hasAddPlugins = false;
    bool hasRemovePlugins = false;
    bool hasTasks = false;

    QString name("");
    QString caption("");
    SimulationSetup::TPluginDefinitions addplugins;
    SimulationSetup::TPluginDefinitions removeplugins;
    SimulationSetup::TTaskDefinitions tasks;
    addplugins.clear();
    removeplugins.clear();
    tasks.clear();
    bool removeall = false;
    bool reloadall = false;
    QString msg = "(" + fname + "): ";

    while(!n.isNull())
    {

        //Comments
        if (n.isComment())
        {
            //LOG_DEBUG() << "Skipping comment.";
        }
        else
        {
            QString s = n.nodeName();
            QDomElement e = n.toElement(); // try to convert the node to an element.

            if (s.compare("name", Qt::CaseInsensitive) == 0)
            {
                if (hasName)
                    LOG_WARNING() << msg << "QDomDocument '"<< path << "': Ingnoring additional occurence of 'name'.";
                else
                {
                    name = e.text();
                    hasName = true;
                }
            }
            else if (s.compare("addplugins", Qt::CaseInsensitive) == 0)
            {
                if (hasAddPlugins)
                    LOG_WARNING() << msg << "QDomDocument '"<< path << "': Multiple 'addplugins' entries.";

                loadPluginDefinitions(e, addplugins, msg);
                hasAddPlugins = true;

                QDomAttr attr = e.attributeNode("reloadall");
                if (!attr.isNull())
                {
                    if (attr.value().compare("false", Qt::CaseInsensitive) == 0 ||
                            attr.value().compare("0") == 0)
                    {
                        reloadall = false;
                    }
                    else
                    {
                        reloadall = true;
                    }
                }
            }
            else if (s.compare("removeplugins", Qt::CaseInsensitive) == 0)
            {
                if (hasRemovePlugins)
                    LOG_WARNING() << msg << "QDomDocument '"<< path << "': Multiple 'removeplugins' entries.";

                loadPluginDefinitions(e, removeplugins, msg, true);
                hasRemovePlugins = true;

                QDomAttr attr = e.attributeNode("removeall");
                if (!attr.isNull())
                {
                    if (attr.value().compare("false", Qt::CaseInsensitive) == 0 ||
                            attr.value().compare("0") == 0)
                    {
                        removeall = false;
                    }
                    else
                    {
                        removeall = true;
                    }
                }
            }
            else if (s.compare("tasks", Qt::CaseInsensitive) == 0)
            {
                if (hasTasks)
                    LOG_WARNING() << msg << "QDomDocument '"<< path << "': Multiple 'tasks' entries.";

                loadTaskDefinitions(e, tasks, msg);
                hasTasks = true;
            }
            else
            {
                //Some error happened
                if (e.isNull()) LOG_WARNING() << msg << "QDomDocument '"<< path << "': While parsing Simulation Setup document: illegal tag.";
                else LOG_WARNING() << msg << "QDomDocument '"<< path << "': While parsing Simulation Setup document: Illegal tag named " << e.tagName() << " found.";
            }
        }
        //Search next node
        n = n.nextSibling();
    }

    //Add definition
    if (!hasName)
    {
        LOG_ERROR() << msg << "QDomDocument '"<< path << "': Simulation setup definition is missing a name.";
        return shared_ptr<SimulationSetup>();
    }
    if (!hasAddPlugins)
    {
        LOG_WARNING() << msg << "QDomDocument '"<< path << "': Simulation setup definition is missing an 'addplugins list.";
    }
    if (!hasRemovePlugins)
    {
        LOG_WARNING() << msg << "QDomDocument '"<< path << "': Simulation setup definition is missing a 'removeplugins' list.";
    }
    if (!hasTasks)
    {
        LOG_WARNING() << msg << "QDomDocument '"<< path << "': Simulation setup definition is missing a 'tasks' list.";
    }

    //Create the setup
    shared_ptr<SimulationSetup> target(
        new SimulationSetup(name, path, reloadall, removeall, addplugins, removeplugins, tasks, true));

    return target;
}

bool SimulationManager::loadPluginDefinitions(QDomElement& element, SimulationSetup::TPluginDefinitions& target, const QString& msg, bool vague)
{
    //Parse document
    QDomNode n = element.firstChild();
    while(!n.isNull())
    {
        //Comments
        if (n.isComment())
        {
            //LOG_DEBUG() << "Skipping comment.";
        }
        else
        {
            QString s = n.nodeName();
            QDomElement e = n.toElement(); // try to convert the node to an element.
            bool isPlugin = (s.compare("plugin", Qt::CaseInsensitive) == 0);
            bool isFrame = (s.compare("frame", Qt::CaseInsensitive) == 0);
            bool isSignalPlugin = (s.compare("signalplugin", Qt::CaseInsensitive) == 0);
            if(!e.isNull() && (isPlugin || isFrame || isSignalPlugin))
            {
                //Read attributes
                QString captionValue;
                QString nameValue;
                QString parameterValue;
                QString attachmentValue;
                if (!e.hasAttribute("name"))
                {
                    if (vague)
                        nameValue = "";
                    else
                    {
                        LOG_ERROR() << msg << s << " definition is missing a name attribute ('name').";
                        n = n.nextSibling();
                        continue;
                    }
                }
                else
                    nameValue = e.attributeNode("name").value();

                int attachmentCount = 0;
                if (isFrame)
                {
                    if (!e.hasAttribute("attachment"))
                    {
                        if (vague)
                            attachmentValue = "";
                        else
                            //There has to be an attachment, but it can be added later
                            attachmentValue = "";
                    }
                    else
                    {
                        attachmentValue = e.attributeNode("attachment").value();
                        attachmentCount++;
                    }
                }

                if (!e.hasAttribute("parameter"))
                    parameterValue = "";
                else
                    parameterValue = e.attributeNode("parameter").value();

                if (!e.hasAttribute("caption"))
                {
                    if (vague)
                        captionValue = "";
                    else
                    {
                        LOG_WARNING() << msg << s << " definition is missing a caption attribute ('caption').";
                        captionValue == "";
                        n = n.nextSibling();
                        continue;
                    }
                }
                else
                    captionValue = e.attributeNode("caption").value();

                //Plugin Type
                EPluginType pluginType;
                if (isPlugin)
                    pluginType = PT_NORMAL;
                if (isSignalPlugin)
                    pluginType = PT_SIGNAL;
                if (isFrame)
                    pluginType = PT_FRAME;

                //Reload definiotion
                PluginDefinition::EReloadDefinition reloadDefinition;
                if (e.hasAttribute("reload"))
                {
                    QString reloadVal = e.attributeNode("reload").value();

                    if (reloadVal.compare("inherit", Qt::CaseInsensitive) == 0)
                        reloadDefinition = PluginDefinition::RD_INHERIT;
                    else if (reloadVal.compare("true", Qt::CaseInsensitive) == 0 || reloadVal.compare("reload", Qt::CaseInsensitive) == 0 )
                        reloadDefinition = PluginDefinition::RD_RELOAD;
                    else if (reloadVal.compare("false", Qt::CaseInsensitive) == 0 || reloadVal.compare("noreload", Qt::CaseInsensitive) == 0)
                        reloadDefinition = PluginDefinition::RD_NO_RELOAD;
                    else
                    {
                        LOG_WARNING() << msg << "Ignoring unknown reload attribute value " << reloadVal << ".";
                        reloadDefinition = PluginDefinition::RD_INHERIT;
                    }
                }
                else
                    reloadDefinition = PluginDefinition::RD_INHERIT;

                //Add definition
                target.push_back(shared_ptr<PluginDefinition>(new PluginDefinition(nameValue, captionValue, pluginType, reloadDefinition, parameterValue, attachmentValue)));

                //Add additional parameters from nodes
                QDomNode parametersNode = e.firstChildElement("parameters");
                if (!parametersNode.isNull())
                {
                    QDomNode subnode = parametersNode.firstChild();
                    while(!subnode.isNull())
                    {
                        QDomElement subelement = subnode.toElement();
                        if (subelement.isComment())
                        {
                            //ignore
                        }
                        else if (subelement.nodeName().compare("parameter", Qt::CaseInsensitive) == 0 || subelement.nodeName().compare("param", Qt::CaseInsensitive) == 0 )
                        {
                            target.back()->addParameter(subelement.text());
                        }
                        else
                        {
                            LOG_WARNING() << msg << "Illegal subnode " << subnode.nodeName() << ". 'parameter' or 'param' expected.";
                        }

                        //next element
                        subnode = subnode.nextSibling();
                    }
                }

                //Add additional attachment points from nodes
                if (pluginType == PT_FRAME)
                {
                    QDomNode attachmentsNode = e.firstChildElement("attachments");
                    if (!attachmentsNode.isNull())
                    {
                        QDomNode subnode = attachmentsNode.firstChild();
                        while(!subnode.isNull())
                        {
                            QDomElement subelement = subnode.toElement();
                            if (subelement.isComment())
                            {
                                //ignore
                            }
                            else if (subelement.nodeName().compare("attachment", Qt::CaseInsensitive) == 0)
                            {
                                target.back()->addAttachment(subelement.text());
                                attachmentCount++;
                            }
                            else
                            {
                                LOG_WARNING() << msg << "Illegal subnode " << subnode.nodeName() << ". 'attachment' expected.";
                            }

                            //next element
                            subnode = subnode.nextSibling();
                        }
                    }
                }

                if (pluginType == PT_FRAME && attachmentCount == 0 && !vague)
                {
                    LOG_WARNING() << msg << "No attachment points specified for plugin " << target.back()->getName() << "(" << target.back()->getCaption() << ").";
                }
            }
            else
            {
                //Some error happened
                if (e.isNull()) LOG_WARNING() << msg << "While parsing Simulation Setup document: empty.";
                else LOG_WARNING() << msg << "While parsing Simulation Setup document: Illegal tag named " << e.tagName() << " found.";
            }
        }
        //Search next node
        n = n.nextSibling();
    }

    return true;
}

bool SimulationManager::loadTaskDefinitions(QDomElement& element, SimulationSetup::TTaskDefinitions& target, const QString& msg)
{
   //Parse document
    QDomNode n = element.firstChild();
    while(!n.isNull())
    {
        //Comments
        if (n.isComment())
        {
            //Skipping comment
        }
        else
        {
            QDomElement e = n.toElement();

            //Type
            QString typeStr = n.nodeName();
            TaskDefinition::ETaskType type;
            bool foundType = false;

            for (int i = 0; i < TaskDefinition::mMaxTypes; i++)
            {
                if (typeStr.compare(TaskDefinition::mTypeNamesShort[i], Qt::CaseInsensitive) == 0)
                {
                    foundType = true;
                    type = TaskDefinition::mTypes[i];
                    break;
                }
            }

            if (!foundType)
            {
                LOG_ERROR() << msg << typeStr << " is not a legal task type.";
                n = n.nextSibling();
                continue;
            }

            //Name
            QDomAttr nameAttr = e.attributeNode("name");
            QString name;
            if (nameAttr.isNull())
            {
                LOG_WARNING() << msg << "Task definition is missing 'name' attribute.";
                name = "Unknown";
            }
            else
                name = nameAttr.value();

            //Priority
            QDomAttr priorityAttr = e.attributeNode("priority");
            QThread::Priority priority;
            if (priorityAttr.isNull())
            {
                LOG_WARNING() << msg << "Task definition is missing 'priority' attribute.";
                priority = QThread::InheritPriority;
            }
            else
            {
                QString text = priorityAttr.value();
                bool ok;
                int value = text.toInt(&ok);

                if (ok && value >= 0 && value <= 7) priority = (QThread::Priority)value;
                else if (text.compare("idle",         Qt::CaseInsensitive) == 0)  priority = QThread::IdlePriority;
                else if (text.compare("lowest",       Qt::CaseInsensitive) == 0)  priority = QThread::LowestPriority;
                else if (text.compare("normal",       Qt::CaseInsensitive) == 0)  priority = QThread::NormalPriority;
                else if (text.compare("high",         Qt::CaseInsensitive) == 0)  priority = QThread::HighPriority;
                else if (text.compare("highest",      Qt::CaseInsensitive) == 0)  priority = QThread::HighestPriority;
                else if (text.compare("timecritical", Qt::CaseInsensitive) == 0)  priority = QThread::TimeCriticalPriority;
                else if (text.compare("inherit",      Qt::CaseInsensitive) == 0)  priority = QThread::InheritPriority;
                else
                {
                    LOG_WARNING() << msg << "Illegal task priority " << text << ". Using 'inherit' instead.";
                    priority = QThread::InheritPriority;
                }
            }

            //First value
            QDomAttr firstAttr = e.attributeNode("first");
            if (firstAttr.isNull())
                firstAttr = e.attributeNode("executable");
            if (firstAttr.isNull())
                firstAttr = e.attributeNode("pluginclass");
            QString first;
            if (firstAttr.isNull())
            {
                LOG_WARNING() << msg << "Task definition is missing 'first' (alt.: 'executable' or 'pluginclass') attribute.";
                first = "";
            }
            else
                first = firstAttr.value();

            //Second value
            QDomAttr secondAttr = e.attributeNode("second");
            if (secondAttr.isNull())
                secondAttr = e.attributeNode("mainscript");
            if (secondAttr.isNull())
                secondAttr = e.attributeNode("plugincaption");
            QString second;
            if (secondAttr.isNull())
            {
                LOG_WARNING() << msg << "Task definition is missing 'second' (alt.: 'mainscript' or 'plugincaption') attribute.";
                second = "";
            }
            else
                second = secondAttr.value();

            //Third value
            QDomAttr thirdAttr = e.attributeNode("third");
            if (thirdAttr.isNull())
                thirdAttr = e.attributeNode("secondaryscript");
            if (thirdAttr.isNull())
                thirdAttr = e.attributeNode("plugintype");
            QString third;
            if (thirdAttr.isNull())
            {
                LOG_WARNING() << msg << "Task definition is missing 'third' (alt.: 'secondaryscript' or 'plugintype') attribute.";
                third = "";
            }
            else
                third = thirdAttr.value();

            //Scripts & Parameters
            std::vector<QString> firstList;
            std::vector<QString> secondList;
            QDomNode child = e.firstChild();
            while(!child.isNull())
            {
                //Comments
                if (child.isComment())
                {
                    child = child.nextSibling();
                    continue;
                }

                if (child.nodeName().compare("scripts", Qt::CaseInsensitive) == 0 ||
                    child.nodeName().compare("firstlist", Qt::CaseInsensitive) == 0)
                {
                    QDomNode entry = child.firstChild();
                    while(!entry.isNull())
                    {
                        //Comments
                        if (entry.isComment())
                        {
                            entry = entry.nextSibling();
                            continue;
                        }

                        if (entry.nodeName().compare("entry", Qt::CaseInsensitive) != 0 &&
                            entry.nodeName().compare("script", Qt::CaseInsensitive) != 0)
                        {
                            LOG_WARNING() << msg << "Illegal node " << entry.nodeName() << " in " << child.nodeName() << " node. 'entry' or 'script' expected";
                            entry = entry.nextSibling();
                            continue;
                        }

                        if (!entry.hasChildNodes())
                        {
                            entry = entry.nextSibling();
                            continue;
                        }

                        //Add Text
                        if (!entry.firstChild().isText())
                        {
                            LOG_WARNING() << msg << "Entry with unknown child node. List entry nodes must contain text only.";
                        }
                        else
                        {
                            firstList.push_back(entry.toElement().text());
                        }

                        //Next script
                        entry = entry.nextSibling();
                    }
                }
                else if (child.nodeName().compare("secondlist", Qt::CaseInsensitive) == 0 ||
                         child.nodeName().compare("parameters", Qt::CaseInsensitive) == 0)
                {
                    QDomNode entry = child.firstChild();
                    while(!entry.isNull())
                    {
                        //Comments
                        if (entry.isComment())
                        {
                            entry = entry.nextSibling();
                            continue;
                        }

                        if (entry.nodeName().compare("entry", Qt::CaseInsensitive) != 0 &&
                            entry.nodeName().compare("parameter", Qt::CaseInsensitive) != 0 &&
                            entry.nodeName().compare("param", Qt::CaseInsensitive) != 0)
                        {
                            LOG_WARNING() << msg << "Illegal node " << entry.nodeName() << " in " << child.nodeName() << " node. 'entry', 'parameter', or 'param' expected";

                            entry = entry.nextSibling();
                            continue;
                        }

                        if (!entry.hasChildNodes())
                        {
                            entry = entry.nextSibling();
                            continue;
                        }

                        //Add Text
                        if (!entry.firstChild().isText())
                        {
                            LOG_WARNING() << msg << "Entry with unknown child node. List entry nodes must contain text only.";
                        }
                        else
                        {
                            secondList.push_back(entry.toElement().text());
                        }

                        //Next script
                        entry = entry.nextSibling();
                    }
                }
                else
                {
                    LOG_WARNING() << msg << "Illegal task child node " << child.nodeName() <<
                        ". Expected 'firstlist' or 'scripts' for a first list and 'secondlist' or 'parameters' as second list.";
                }

                child = child.nextSibling();
            }

            //Create Definition
            target.push_back(shared_ptr<TaskDefinition>(new TaskDefinition(type, name, priority, first, second, third, firstList, secondList)));

            //Next thread
            n = n.nextSibling();
        }
    }

    return true;
}

shared_ptr<SimulationSetup> SimulationManager::saveSimulationSetupProtected(SimulationSetup& setup, int& err, bool directOverwrite, bool rememberPath)
{
    shared_ptr<SimulationSetup> ptr(new SimulationSetup(setup));
    return saveSimulationSetupProtected(ptr, err, directOverwrite, rememberPath);
}

shared_ptr<SimulationSetup> SimulationManager::saveSimulationSetupProtected(boost::shared_ptr<SimulationSetup> setup, int& err, bool directOverwrite, bool rememberPath)
{
    bool overwrite = false;
    bool create = false;

    if (!setup->isToSave())
    {
        LOG_DEBUG() << "Setup " << setup->getName() << " flagged to not get saved. Skipping.";
        err = 2;
        return shared_ptr<SimulationSetup>();
    }

    QString msg = "(" + setup->getName() + "): ";

    if (setup->getFileName().compare("") == 0)
    {
        bool chose = chooseFile(*setup, false);
        if (!chose)
        {
            err = 2;
            return shared_ptr<SimulationSetup>();
        }

        overwrite = true; //overwrite later without asking
    }

    QFileInfo info(setup->getFileName());
    QString path = info.absoluteFilePath();

    if (!info.exists())
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly))
        {
            LOG_ERROR() << msg << "Could not create file " << path;
            file.close();
            err = 1;
            shared_ptr<SimulationSetup>();
        }

        file.close();
        info = QFileInfo(path);

        if (!info.exists())
        {
            LOG_ERROR() << msg << "Aborting... " << path;
            err = 1;
            return shared_ptr<SimulationSetup>();
        }
    }

    if (!info.isReadable())
    {
       LOG_ERROR() << msg << "Path " << path << " is not readable.";
       err = 1;
       return shared_ptr<SimulationSetup>();
    }

    if (!info.isFile())
    {
        LOG_ERROR() << msg << "Path " << path << " is not a files path.";
       err = 1;
       return shared_ptr<SimulationSetup>();
    }

    bool updateSaveSetup = false;
    shared_ptr<SimulationSetup> saveSetup(setup);

    if (containsSetupIncludePath(setup->getFileName()))
    {
        int index = getSetupIndexByFilePath(setup->getFileName());

        if (index != -1)
        {
            //Overwriting an existing setup in the list, if saving succeeds, update it and return it later
            saveSetup = getSetupList().at(index);
            updateSaveSetup = true;
        }
        else
        {
            //Not in observed list, add it
            mSetupList.push_back(saveSetup);
        }
    }

    QTemporaryFile file;
    if (!file.open())
    {
        LOG_ERROR() << msg << "Could not open temp Simulation Setup definition file.";
        err = 1;
        return shared_ptr<SimulationSetup>();
    }
    else
    {
        LOG_DEBUG() << msg << "Opened temporary Simulation Setup file for setup " << path << ".";
    }

    //Create document
    QDomDocument setupDocument;

    //Fill standard data
    QDomElement root = setupDocument.createElement("root");
    setupDocument.appendChild(root);

    QDomElement name = setupDocument.createElement("name");
    QDomText nameText = setupDocument.createTextNode(setup->getName());
    name.appendChild(nameText);
    root.appendChild(name);

    QDomElement addplugins = setupDocument.createElement("addplugins");
    addplugins.setAttribute("reloadall", setup->getReloadAll() == true ? "true" : "false");
    bool success1 = savePluginDefinitions(setupDocument, addplugins, setup->getAddPlugins(), msg, false);
    root.appendChild(addplugins);

    QDomElement removeplugins = setupDocument.createElement("removeplugins");
    removeplugins.setAttribute("removeall", setup->getRemoveAll() == true ? "true" : "false");
    bool success2 = savePluginDefinitions(setupDocument, removeplugins, setup->getRemovePlugins(), msg, true);
    root.appendChild(removeplugins);

    QDomElement tasks = setupDocument.createElement("tasks");
    bool success3 = saveTaskDefinitions(setupDocument, tasks, setup->getTaskDefinitions(), msg);
    root.appendChild(tasks);

    //Save to file
    QTextStream stream(&file);
    setupDocument.save(stream, 2);
    file.close();

    bool saveThis = success1 && success2 && success3;

    if (file.error())
    {
        LOG_ERROR() << msg << "Error while writing to temporary setup file: " << file.errorString();

        saveThis = false;
    }

    QDir targetDir = info.absoluteDir();
    bool returning = saveThis;
    if (saveThis)
    {
        bool renamed = targetDir.rename(info.fileName(), "tempsetup.tmp");
        bool copied;
        if (renamed)
            copied = file.copy(path);
        else
        {
            LOG_ERROR() << msg << "Could not rename setup file.";
            err = 1;
            return shared_ptr<SimulationSetup>();
        }

        if (!copied)
        {
            LOG_ERROR() << msg << "Could not save simulation setup file.";

            //Revert
            bool reverted = targetDir.rename("tempsetup.tmp", info.fileName());
            if (!reverted)
            {
                LOG_ERROR() << "Failed to recover after error. See file at " << targetDir.absolutePath().toStdString().c_str() << "/tempsetup.tmp to recover.";
                err = 1;
                return shared_ptr<SimulationSetup>();
            }
        }
    }

    //Delete temp
    if (targetDir.exists())
    {
        targetDir.remove("tempsetup.tmp");
    }

    //If simulation was saved, save data
    if (mSimulation->hasSetup(*setup))
    {
        mSimulation->saveData();
    }

    //Saved successfully
    if (updateSaveSetup)
    {
        //Saved an already contained setup, update it
        saveSetup->readData(*setup, true);
        LOG_DEBUG() << msg << "Overwriting setup " << setup->getName() << " at path " << path << " successful.";
    }
    else
    {
        LOG_DEBUG() << msg << "Saving setup " << setup->getName() << " to " << path << " successful.";
    }
    saveSetup->setSaved(true);

    //Store path?
    if (rememberPath)
    {
        mLastLoadedSetup = setup->getName();
        mLastLoadedSetupPath = setup->getFileName();
    }
    emit setupSaved(saveSetup->getName(), saveSetup->getFileName());

    err = 0;
    return saveSetup;
}

bool SimulationManager::saveTaskDefinitions(QDomDocument& doc, QDomElement& element, const SimulationSetup::TTaskDefinitions& source, const QString& msg)
{
    for (auto it = source.begin(); it != source.end(); it++)
    {
        shared_ptr<TaskDefinition> def(*it);
        QString typeName = def->getTypeStringShort();

        //Type
        QDomElement node = doc.createElement(typeName);
        element.appendChild(node);

        //Name
        node.setAttribute("name", def->getName());
        //Priority
        node.setAttribute("priority", (int)def->getPriority());
        //Executable
        node.setAttribute("first", def->getFirst());
        //Primary Script
        node.setAttribute("second", def->getSecond());
        //Secondary Script
        node.setAttribute("third", def->getThird());

        //Additional Scripts
        if (def->getFirstList().size() > 0)
        {
            QDomElement scripts = doc.createElement("firstlist");
            node.appendChild(scripts);

            for (auto script_it = def->getFirstList().begin(); script_it != def->getFirstList().end(); script_it++)
            {
                //Add a script
                QDomElement script = doc.createElement("entry");
                scripts.appendChild(script);
                QDomText text = doc.createTextNode(*script_it);
                script.appendChild(text);
            }
        }

        //Parameters
        if (def->getSecondList().size() > 0)
        {
            QDomElement parameters = doc.createElement("secondlist");
            node.appendChild(parameters);

            for (auto param_it = def->getSecondList().begin(); param_it != def->getSecondList().end(); param_it++)
            {
                //Add a script
                QDomElement parameter = doc.createElement("entry");
                parameters.appendChild(parameter);
                QDomText text = doc.createTextNode(*param_it);
                parameter.appendChild(text);
            }
        }
    }

    return true;
}

bool SimulationManager::savePluginDefinitions(QDomDocument& doc, QDomElement& element, const SimulationSetup::TPluginDefinitions& source, const QString& msg, bool vague)
{
    bool errorHappened = false;
    for (SimulationSetup::TPluginDefinitions::const_iterator it = source.begin(); it != source.end(); it++)
    {
        const PluginDefinition& def = **it;

        //Plugin Type
        QString name;
        if (def.getPluginType() == PT_NORMAL)
            name = "plugin";
        if (def.getPluginType() == PT_SIGNAL)
            name = "signalplugin";
        if (def.getPluginType() == PT_FRAME)
            name = "frame";

        //Name
        QDomElement node = doc.createElement(name);
        if (def.getName().compare("") == 0)
        {
            if (!vague)
            {
                LOG_ERROR() << msg << "Cant save empty plugin name.";
                errorHappened = true;
                continue;
            }
        }
        else
            node.setAttribute("name", def.getName());

        //Caption
        if (def.getCaption().compare("") == 0)
        {
            if (!vague)
            {
                node.setAttribute("caption", def.getName());
            }
        }
        node.setAttribute("caption", def.getCaption());

        //Reload definition
        if (def.getReloadDefinition() != PluginDefinition::RD_INHERIT)
        if (!vague)
        {
            node.setAttribute("reload", def.getReloadDefinition() == PluginDefinition::RD_RELOAD ? "true" : "false");
        }

        //Attachments
        if (def.getPluginType() == PT_FRAME && !vague)
        {
            QDomElement attachments;
            if (def.getAttachments().size() > 1)
            {
                attachments = doc.createElement("attachments");
            }

            int i = 0;
            for (auto it = def.getAttachments().begin(); it != def.getAttachments().end(); it++)
            {
                if (i == 0)
                    node.setAttribute("attachment", *it);
                else
                {
                    QDomElement attachment;
                    attachment = doc.createElement("attachment");

                    QDomText text = doc.createTextNode(*it);
                    attachment.appendChild(text);
                    attachments.appendChild(attachment);
                }
                i++;
            }
            if (def.getAttachments().size() > 1)
            {
                node.appendChild(attachments);
            }
        }

        //Parameters
        if (!vague)
        {
            QDomElement parameters;
            if (def.getParameters().size() > 1)
            {
                parameters = doc.createElement("parameters");
            }

            int i = 0;
            for (auto it = def.getParameters().begin(); it != def.getParameters().end(); it++)
            {
                if (i == 0)
                    node.setAttribute("parameter", *it);
                else
                {
                    QDomElement parameter;
                    parameter = doc.createElement("parameter");

                    QDomText text = doc.createTextNode(*it);
                    parameter.appendChild(text);
                    parameters.appendChild(parameter);
                }
                i++;
            }
            if (def.getParameters().size() > 1)
            {
                node.appendChild(parameters);
            }
        }

        element.appendChild(node);
    }
    return true;
}

//Only called during initialization and by addIncludePath() which changes the settings object containing the include paths
bool SimulationManager::addIncludeDirectory(const QString& path)
{
    if (!mIncludePathsMutex.tryLock(1000))
        return false;

    if (mSetupIncludeDirectories.contains(path))
    {
        mIncludePathsMutex.unlock();
        return false;
    }

    mSetupIncludeDirectories.push_back(path);
    mIncludePathsMutex.unlock();

    return true;
}

//Only called by addIncludePath() which changes the settings object containing the include paths
bool SimulationManager::addIncludeFile(const QString& path)
{
    if (!mIncludePathsMutex.tryLock(1000))
        return false;

    if (mSetupIncludeFiles.contains(path))
    {
        mIncludePathsMutex.unlock();
        return false;
    }

     mSetupIncludeFiles.push_back(path);
    mIncludePathsMutex.unlock();

    return true;
}

bool SimulationManager::removeIncludeDirectory(const QString& path)
{
    if (!mIncludePathsMutex.tryLock(1000))
        return false;

    bool returning = false;
    for (QStringList::Iterator it = mSetupIncludeDirectories.begin(); it != mSetupIncludeDirectories.end(); it++)
    {
        if (it->compare(path) == 0)
        {
            mSetupIncludeDirectories.erase(it);
            returning = true;
            break;
        }
    }

    if (returning == true)
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mSettings->setValue("mSetupPaths", mSetupIncludeDirectories);
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access settings object.";

    mIncludePathsMutex.unlock();

    return returning;
}

bool SimulationManager::removeIncludeFile(const QString& path)
{
    mIncludePathsMutex.lock();
    bool returning = false;
    for (QStringList::Iterator it = mSetupIncludeFiles.begin(); it != mSetupIncludeFiles.end(); it++)
    {
        if (it->compare(path) == 0)
        {
            mSetupIncludeFiles.erase(it);
            returning = true;
            break;
        }
    }

    if (returning == true)
    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mSettings->setValue("mSetupFiles", mSetupIncludeFiles);
        mSettings->endGroup(true); //unlock
    }
    else
        LOG_ERROR() << "Could not access settings object.";

    mIncludePathsMutex.unlock();

    return false;
}

int SimulationManager::getNextTaskId()
{
    int returning = mNextTaskId;
    mNextTaskId++;
    return mNextTaskId;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool SimulationManager::chooseFile(SimulationSetup& setup, bool tryExistingFirst)
{
    bool repeat = true;
    while (repeat)
    {
        int returning = 0;

        //File not yet defined
        QFileDialog dialog((QWidget*)Carbon::get()->getWindowManager()->getMainWindow(), tr("Save Simulation Setup"),
            mDefaultSetupPath, mDefaultSetupExtensionText);
        dialog.setFileMode(QFileDialog::AnyFile);
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.setLabelText(QFileDialog::Accept, tr("Save"));
        dialog.setViewMode(QFileDialog::Detail);
        dialog.setDefaultSuffix(mDefaultSetupExtensionSuffix);
        dialog.selectFile("NewSetup.3s");
        dialog.setToolTip(tr("Save current simulation state as Simulation Setup file."));
        dialog.setWindowTitle(tr("Save Simulation Setup."));

        if (!tryExistingFirst)
        {
            returning = dialog.exec();
        }
        if (tryExistingFirst || returning == QFileDialog::AcceptSave)
        {
            QString file;
            if (tryExistingFirst)
                file = setup.getFileName();
            else
                file = dialog.selectedFiles().at(0);
            QFileInfo info(file);
            QString name = info.fileName();

            QString informative;
            bool targetOk = false;
            {
                int index = getSetupIndexByFilePath(file);
                if (index != -1)
                    if (setup.getName().compare(mSetupList.at(index)->getName()) == 0)
                    {
                        //File is already used by this setup, simply overwrite
                        return true;
                    }

                if (index == -1)
                {
                    LOG_DEBUG() << "Chose non-occupied file " << file << ".";
                    //continue with new filename
                    setup.setFileName(file);
                    return true;
                }
                else
                {
                    informative = QString(tr("The file contains the Simulation Setup %1.")).
                        arg(mSetupList.at(index)->getName());
                }
                informative.append(tr(" Press abort to choose another file."));
            }

            if (info.exists() && info.size() != 0) //size = 0: just created
            {
                QMessageBox messageBox;
                messageBox.setWindowTitle(QString(tr("Overwrite File")));
                messageBox.setText(QString(tr("Do you really want to overwrite file '%1'?")).arg(info.fileName()));
                messageBox.setInformativeText(informative);
                messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort);
                messageBox.setDefaultButton(QMessageBox::No);
                messageBox.setIcon(QMessageBox::Question);
                int ret = messageBox.exec();

                if (ret == QMessageBox::Yes)
                {
                    //continue with new filename
                    repeat = false;
                    setup.setFileName(file);
                }
                if (ret == QMessageBox::No)
                {
                    //Dont save, return
                    return false;
                }
                if (ret == QMessageBox::Abort)
                {
                    //Ask again
                }
            }
            else
            {
                //file either does not exist (error) or has size 0 (can be overwritten without asking)
                return info.exists();
            }
        }
        else
        {
            //dont save, return
            return false;
        }
    }

    return true;
}

void SimulationManager::clearSetupList()
{
    for (int i = mSetupList.size() -1; i >= 0; i--)
    {
        removeSimulationSetup(mSetupList.at(i)->getName(), false, false);
    }
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SimulationManager::updateSetting(QString name)
{
    //Single setting values that are changed in the SettingsWidget are updated here

    if (mSettings->beginGroup("gui_SimulationManager", true)) //lock
    {
        mIncludePathsMutex.lock();
        bool unknown = false;

        if      (name.compare("mExecutablePaths") == 0) mExecutableIncludeDirectories = mSettings->value(name, mExecutableIncludeDirectories).toStringList();
        else if (name.compare("mExecutableFiles") == 0) mExecutableIncludeFiles       = mSettings->value(name, mExecutableIncludeFiles).toStringList();
        else if (name.compare("mSetupPaths") == 0)      mSetupIncludeDirectories      = mSettings->value(name, mSetupIncludeDirectories).toStringList();
        else if (name.compare("mSetupFiles") == 0)      mSetupIncludeFiles            = mSettings->value(name, mSetupIncludeFiles).toStringList();
        else if (name.compare("mScriptPaths") == 0)     mScriptIncludeDirectories     = mSettings->value(name, mScriptIncludeDirectories).toStringList();
        else if (name.compare("mScriptFiles") == 0)     mScriptIncludeFiles           = mSettings->value(name, mScriptIncludeFiles).toStringList();
        else
        {
            unknown = true;
        }

        mSettings->endGroup(true); //unlock
        mIncludePathsMutex.unlock();

        if (!unknown)
            Carbon::get()->getMenuManager()->updateSimulationSetupList();
    }
    else
        LOG_ERROR() << "Could not access the Settings object to store the SimulationManagers include directories to.";
}

void SimulationManager::cleanup()
{
    static QMutex cleanupMutex;
    cleanupMutex.lock();

    if (mClosed)
    {
        cleanupMutex.unlock();
        return;
    }

    mClosed = true;

    stopSimulation(true); //stop all simulations, including main server
    saveSettings();
    saveAllSimulationSetups(true, true);

    cleanupMutex.unlock();
}
