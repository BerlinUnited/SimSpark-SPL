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

#include "libraryloader.h"
#include "cutelogger/logger.h"
#include "pluginmanager.h"
#include "carbon.h"

using namespace boost;

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

LibraryLoader::LibraryLoader()
{
    mFilters << "*.dll";
    mFilters << "*.so";
}

LibraryLoader::~LibraryLoader()
{
    clearLibraries(); //Unlinks all libraries
    mDirectories.clear();
}

bool LibraryLoader::addDirectory(const QString& path)
{
    return addDirectory(QDir(path));
}

bool LibraryLoader::addDirectory(const QDir& path)
{
    //Add if not contained
    if (!containsDirectory(path))
        mDirectories.push_back(path.absolutePath());

    //Add libraries from this directory
    updateLibraries(path);
    return true;
}

bool LibraryLoader::removeDirectory(const QString& path)
{
    return removeDirectory(QDir(path));
}

bool LibraryLoader::removeDirectory(const QDir& path)
{
    if (!path.exists())
    {
        LOG_WARNING() << "Directory " << path << " does not exist.";
        return false;
    }
    QString normPath = path.absolutePath();

    //Search
    for (auto it = mDirectories.begin(); it != mDirectories.end(); it++)
    {
        if (it->compare(normPath) == 0)
        {
            //Erase
            mDirectories.erase(it);
            return true;
        }
    }

    //Not found
    return false;
}

bool LibraryLoader::containsDirectory(const QString& path) const
{
    return containsDirectory(QDir(path));
}

bool LibraryLoader::containsDirectory(const QDir& path) const
{
    if (!path.exists())
    {
        LOG_WARNING() << "Directory " << path << " does not exist.";
        return false;
    }
    QString normPath = path.absolutePath();

    //Search
    for (auto it = mDirectories.begin(); it != mDirectories.end(); it++)
    {
        if (it->compare(normPath) == 0)
            return true;
    }

    return false;
}

LibraryLoader::LibraryMap::ConstIterator LibraryLoader::librariesBegin() const
{
    return mLibraries.begin();
}

LibraryLoader::LibraryMap::ConstIterator LibraryLoader::librariesEnd() const
{
    return mLibraries.end();
}

QStringList::ConstIterator LibraryLoader::directoriesBegin() const
{
    return mDirectories.begin();
}

QStringList::ConstIterator LibraryLoader::directoriesEnd() const
{
    return mDirectories.end();
}

const LibraryLoader::LibraryMap& LibraryLoader::getLibraryMap() const
{
    return mLibraries;
}

const QStringList& LibraryLoader::getDirectories() const
{
    return mDirectories;
}

void LibraryLoader::addFiles(const QStringList& paths)
{
    for (auto it = paths.begin(); it != paths.end(); it++)
    {
        addFile(*it);
    }
}

int LibraryLoader::addFile(const QString& path, bool unique)
{
    if (unique && getLibrary(path) != 0)
        return 0;

    return addLibrary(path);
}

void LibraryLoader::load()
{
    //Load all libraries
    for (auto it = mLibraries.begin(); it != mLibraries.end(); it++)
    {
        load(*it);
    }
}

bool LibraryLoader::load(const QString& path)
{
    LibraryMap::Iterator lib(findLibraryDefinition(path));
    return (lib == mLibraries.end() ? false : load(*lib));
}

bool LibraryLoader::load(int id)
{
    LibraryMap::Iterator lib(findLibraryDefinition(id));
    return (lib == mLibraries.end() ? false : load(*lib));
}

bool LibraryLoader::load(const boost::shared_ptr<QLibrary>& search)
{
    LibraryMap::Iterator lib(findLibraryDefinition(search));
    return (lib == mLibraries.end() ? false : load(*lib));
}

bool LibraryLoader::unload(const QString& path)
{
    LibraryMap::Iterator lib(findLibraryDefinition(path));
    return (lib == mLibraries.end() ? false : unload(*lib));
}

bool LibraryLoader::unload(int id)
{
    LibraryMap::Iterator lib(findLibraryDefinition(id));
    return (lib == mLibraries.end() ? false : unload(*lib));
}

bool LibraryLoader::unload(const boost::shared_ptr<QLibrary>& search)
{
    LibraryMap::Iterator lib(findLibraryDefinition(search));
    return (lib == mLibraries.end() ? false : unload(*lib));
}

bool LibraryLoader::isLoaded(const QString& path) const
{
    LibraryMap::ConstIterator lib(findLibraryDefinition(path));
    return (lib == mLibraries.end() ? false : isLoaded(*lib));
}

bool LibraryLoader::isLoaded(int id) const
{
    LibraryMap::ConstIterator lib(findLibraryDefinition(id));
    return (lib == mLibraries.end() ? false : isLoaded(*lib));
}

bool LibraryLoader::isLoaded(const boost::shared_ptr<QLibrary>& search) const
{
    LibraryMap::ConstIterator lib(findLibraryDefinition(search));
    return (lib == mLibraries.end() ? false : isLoaded(*lib));
}

void LibraryLoader::exportPlugins()
{
    //Export all plugins from all libraries
    for (auto it = mLibraries.begin(); it != mLibraries.end(); it++)
    {
        exportPlugins(*it);
    }
}

bool LibraryLoader::exportPlugins(const QString& path)
{
    LibraryMap::Iterator lib(findLibraryDefinition(path));
    return (lib == mLibraries.end() ? false : exportPlugins(*lib));
}

bool LibraryLoader::exportPlugins(int id)
{
    LibraryMap::Iterator lib(findLibraryDefinition(id));
    return (lib == mLibraries.end() ? false : exportPlugins(*lib));
}

bool LibraryLoader::exportPlugins(const boost::shared_ptr<QLibrary>& search)
{
    LibraryMap::Iterator lib(findLibraryDefinition(search));
    return (lib == mLibraries.end() ? false : exportPlugins(*lib));
}

QMainWindow* LibraryLoader::exportLayout(const QString& path)
{
    LibraryMap::Iterator lib(findLibraryDefinition(path));
    return (lib == mLibraries.end() ? 0 : exportLayout(*lib));
}

QMainWindow* LibraryLoader::exportLayout(int id)
{
    LibraryMap::Iterator lib(findLibraryDefinition(id));
    return (lib == mLibraries.end() ? 0 : exportLayout(*lib));
}

QMainWindow* LibraryLoader::exportLayout(const boost::shared_ptr<QLibrary>& search)
{
    LibraryMap::Iterator lib(findLibraryDefinition(search));
    return (lib == mLibraries.end() ? 0 : exportLayout(*lib));
}

const shared_ptr<QLibrary> LibraryLoader::getLibrary(const QString& path) const
{
    return getLibraryNc(path);
}

const shared_ptr<QLibrary> LibraryLoader::getLibrary(int id) const
{
    return getLibraryNc(id);
}

const shared_ptr<QLibrary> LibraryLoader::getLibraryByFileName(const QString& path) const
{
    auto pos = findLibraryDefinitionByFileName(path);
    if (pos == mLibraries.end())
        return shared_ptr<QLibrary>();
    return pos->mLibrary;
}

QString LibraryLoader::getErrorString(const QString& path) const
{
    shared_ptr<QLibrary> lib(getLibraryNc(path));
    if (lib == 0)
        return QString("Library '" + path + "' not added to loader.");
    return lib->errorString();
}

QString LibraryLoader::getErrorString(int id) const
{
    shared_ptr<QLibrary> lib(getLibraryNc(id));
    if (lib == 0)
        return QString("Library with id '" + QString::number(id) + "' not added to loader.");
    return lib->errorString();
}

void LibraryLoader::updateLibraries()
{
    //Iterate directories
    for (auto dir_it = mDirectories.begin(); dir_it != mDirectories.end();)
    {
        //Check dir
        QDir dir(*dir_it);

        if (!dir.exists())
        {
            LOG_WARNING() << "Removing directory " << *dir_it << " from directory list, because directory does not exist.";
            dir_it = mDirectories.erase(dir_it);
            continue;
        }

        dir_it++;
    }
}

bool LibraryLoader::updateLibraries(const QDir& dir)
{
    //Exists?
    if (!dir.exists())
    {
        LOG_ERROR() << "Directory " << dir.absolutePath() << " does not exist.";
        return false;
    }

    //Add
    if (!containsDirectory(dir))
        addDirectory(dir);

    //Collect library file info
    QFileInfoList files = dir.entryInfoList(mFilters);

    //Iterate and add library files
    for (auto it = files.begin(); it != files.end(); it++)
    {
        addLibrary(it->absoluteFilePath());
    }

    return true;
}

int LibraryLoader::search(const QString& name) const
{
    for (auto it = mLibraries.begin(); it != mLibraries.end(); it++)
    {
        QString str = it->mLibrary->fileName();
        QFileInfo info(str);
        if (info.baseName().compare(name) == 0)
        {
            return it->mId;
        }
    }
    return 0;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

bool LibraryLoader::load(LibraryDefinition& lib)
{
    LOG_ASSERT(lib.mLibrary);

    //Load the library
    bool loaded = lib.mLibrary->load();
    if (!loaded)
    {
        LOG_WARNING() << "Loading library " << lib.mLibrary->fileName() << "failed: " << lib.mLibrary->errorString().toStdString().c_str();
        return false;
    }

    return true;
}

bool LibraryLoader::exportPlugins(LibraryDefinition& lib)
{
    LOG_ASSERT(lib.mLibrary);

    //Already exported? Skip
    if (lib.mExportedPlugins)
        return true;

    //Call export function
    QString baseName = QFileInfo(lib.mLibrary->fileName()).baseName();

    //signature: (PluginFactory pointer, next class id, library name, Logger::LoggerRegistration, Carbon*), returns class id + amount of exported plugins
    typedef int (*ExportFunc)(void*, int, const char*, void*, void*); 
    ExportFunc theExportFunc = (ExportFunc) lib.mLibrary->resolve("ExportPlugins");
    
    if (theExportFunc)
    {
        int errCount = PluginFactory::getFactory().mErrorMessages.size();

        //Export with the static factory, starting with the next free id and with the library name
        int nextId = PluginManager::getNextClassId();
        nextId = theExportFunc(&PluginFactory::getFactory(), nextId, baseName.toStdString().c_str(), 
            Logger::getStaticLoggerRegistration(), Carbon::get()); //export returns the next class id
        if (nextId == 0) //error
        {
            LOG_ERROR() << "Export function returned error. Factory pointer illegal.";
            return false;
        }
        PluginManager::setNextClassId(nextId);

        if (errCount < (int)PluginFactory::getFactory().mErrorMessages.size())
        {
            LOG_ERROR() << "Error in plugin factory while exporting plugin from library " << baseName << ": " << PluginFactory::getFactory().mErrorMessages.back();
            return false;
        }
    }
    else
    {
        LOG_INFO() << "Failed to access export plugin function in library " << lib.mLibrary->fileName() << ": " << lib.mLibrary->errorString().toStdString().c_str();
        return false;
    }

    //Set exported flag
    lib.mExportedPlugins = true;
    return true;
}

QMainWindow* LibraryLoader::exportLayout(LibraryDefinition& lib)
{
    LOG_ASSERT(lib.mLibrary);

    //Already exported? Return 0
    if (lib.mExportedLayout)
        return 0;

    //Call export function
    QString baseName = QFileInfo(lib.mLibrary->fileName()).baseName();

    //signature: (Logger::LoggerRegistration, Carbon*), returns QMainWindow*
    typedef void* (*ExportFunc)(void*, void*, void*); 
    ExportFunc theExportFunc = (ExportFunc) lib.mLibrary->resolve("ExportLayout");
    
    QMainWindow* newWindow = 0;
    if (theExportFunc)
    {
        //Export with the static carbon pointer, returns the new window
        newWindow = (QMainWindow*)theExportFunc(Logger::getStaticLoggerRegistration(), Carbon::get(), &PluginFactory::getFactory()); 
        if (newWindow == 0) //error
        {
            LOG_ERROR() << "Export function returned error.";
            return 0;
        }
    }
    else
    {
        LOG_INFO() << "Failed to access export plugin function in library " << lib.mLibrary->fileName() << ": " << lib.mLibrary->errorString().toStdString().c_str();
        return false;
    }

    //Set exported flag
    lib.mExportedLayout = true;
    return newWindow;
}

bool LibraryLoader::unload(LibraryDefinition& lib)
{
    LOG_ASSERT(lib.mLibrary);
    return lib.mLibrary->unload();
}

bool LibraryLoader::isLoaded(const LibraryDefinition& lib) const
{
    LOG_ASSERT(lib.mLibrary);
    return lib.mLibrary->isLoaded();
}

int LibraryLoader::addLibrary(const QString& path)
{
    //Valid library?
    if (!QLibrary::isLibrary(path))
    {
        LOG_ERROR() << "File at " << path << " is not a library.";
        return 0;
    }

    //Already contained?
    auto pos = mLibraries.find(path);

    //Does not exist -> create and assign id
    if (pos == mLibraries.end())
    {
        QLibrary* lib = new QLibrary(path);
        pos = mLibraries.insert(path, LibraryDefinition(getNextId(), shared_ptr<QLibrary>(lib)));
    }

    //Error
    if (pos == mLibraries.end())
    {
        LOG_ERROR() << "Error adding library " << path;
        return 0;
    }
    
    //Already contained, return its id
    return pos->mId;
}

void LibraryLoader::removeLibrary(const QString& path)
{
    auto pos = mLibraries.find(path);   
    if (pos != mLibraries.end())
    {
        //unload
        unload(*pos);

        //remove
        mLibraries.erase(pos);
    }
}

void LibraryLoader::clearLibraries()
{
    while (mLibraries.size() > 0)
    {
        //remove
        mLibraries.erase(mLibraries.begin());
    }
}

shared_ptr<QLibrary> LibraryLoader::getLibraryNc(const QString& path) const
{
    auto pos = mLibraries.find(path);
    if (pos == mLibraries.end())
        return shared_ptr<QLibrary>();
    return pos->mLibrary;
}

shared_ptr<QLibrary> LibraryLoader::getLibraryNc(int id) const
{
    for (auto it = mLibraries.begin(); it != mLibraries.end(); it++)
    {
        if (it->mId == id)
            return it->mLibrary;
    }
    return shared_ptr<QLibrary>();;
}

LibraryLoader::LibraryMap::ConstIterator LibraryLoader::findLibraryDefinition(const QString& path) const
{
    return mLibraries.find(path);
}

LibraryLoader::LibraryMap::ConstIterator LibraryLoader::findLibraryDefinition(int id) const
{
    for (auto it = mLibraries.begin(); it != mLibraries.end(); it++)
    {
        if (it->mId == id)
            return it;
    }
    return mLibraries.end();
}

LibraryLoader::LibraryMap::ConstIterator LibraryLoader::findLibraryDefinition(const boost::shared_ptr<QLibrary>& lib) const
{
    for (auto it = mLibraries.begin(); it != mLibraries.end(); it++)
    {
        if (it->mLibrary.get() == lib.get())
            return it;
    }
    return mLibraries.end();
}

LibraryLoader::LibraryMap::ConstIterator LibraryLoader::findLibraryDefinitionByFileName(const QString& name) const
{
    QList<QString> keys = mLibraries.keys();
    for (auto it = keys.begin(); it != keys.end(); it++)
    {
        QFileInfo info(*it);
        if (info.baseName().compare(name) == 0)
            return mLibraries.find(*it);
    }
    return mLibraries.end();
}

int LibraryLoader::getNextId()
{
    int id = mNextId;
    mNextId++;
    return id;
}
