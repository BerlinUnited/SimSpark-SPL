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

#ifndef LIBRARYLOADER_H
#define LIBRARYLOADER_H
/**
 * \file libraryloader.h
 * \brief Contains LibraryLoader class.
 */

#include <QStringList>
#include <QLibrary>
#include <QMap>
#include <QVector>
#include <QPair>
#include <QDir>
#include <QMainWindow>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

/*!
 \brief Class that manages include directories and files for plugin and main window shared libraries.

 The library loader contains a list for include directories and single include files.
 All shared library files (*.dll, *.so) contained in the file list or one of the include directories are added to a library map.

 Managed libraries can be linked to the application.
 With exportPlugins() and exportLayout() the loader calls the export functions for Plugins or QMainWindows that are defined as external
 C function in the shared library using EXPORT_PLUGINS_BEGIN and EXPORT_PLUGINS_END of abstractplugin.h as well as EXPORT_LAYOUT of windowmanager.h.

 The LibraryLoader is used by the PluginManager to import plugins and the Carbon to import a main window.
*/
class LibraryLoader
{

protected:
    //protected types

    /*!
     \brief Struct containing information on a managed QLibrary.
    */
    struct LibraryDefinition
    {
        /*!
         \brief Constructor.

         \param id id of the library
         \param library pointer to the library
        */
        LibraryDefinition(int id, boost::shared_ptr<QLibrary> library) :
            mId(id),
            mLibrary(library),
            mExportedPlugins(false),
            mExportedLayout(false)
        {}
        /*!
         \brief Destructor.
        */
        ~LibraryDefinition()
        {}

        int mId;                              /*!< Id of the library. Given in order of creation. */
        boost::shared_ptr<QLibrary> mLibrary; /*!< Shared pointer to the library handler. */
        bool mExportedPlugins;                /*!< True if plugins were exported from this library. */
        bool mExportedLayout;                 /*!< True if a main window was exported from this library. */
    };

    /*!
     \brief Defines a library map as map of the absolute path to the library to the library definition.
    */
    typedef QMap<QString, LibraryDefinition> LibraryMap;

public:
    //public functions

    /*!
     \brief Constructor.
    */
    LibraryLoader();
    /*!
     \brief Destructor.
    */
    ~LibraryLoader();

    /*!
     \brief Add a directory to include libraries from.

     \param path absolute path to search libraries in
     \return true if adding was successful or directory was already contained
    */
    bool addDirectory(const QString& path);
    /*!
     \brief Add a directory to include libraries from.

     \param path directory to search libraries in
     \return true if adding was successful or directory was already contained
    */
    bool addDirectory(const QDir& path);
    /*!
     \brief Remove a library include directory.

     Libraries already linked wont be removed.
     \param path absolute path of the directory to remove
     \return true if removing was successful
    */
    bool removeDirectory(const QString& path);
    /*!
     \brief Remove a library include directory.

     Libraries already linked wont be removed.
     \param path directory to remove
     \return true if removing was successful
    */
    bool removeDirectory(const QDir& path);
    /*!
     \brief Returns true if the include directory with the given absolute path is already contained in the directory list.

     \param path absolute path to directory
    */
    bool containsDirectory(const QString& path) const;
    /*!
     \brief Returns true if the given include directory is already contained in the directory list.

     \param path directory to check
    */
    bool containsDirectory(const QDir& path) const;

    /*!
     \brief Returns begin iterator to the library definition map.
    */
    LibraryMap::ConstIterator librariesBegin() const;
    /*!
     \brief Returns end iterator to the library definition map.
    */
    LibraryMap::ConstIterator librariesEnd() const;
    /*!
     \brief Returns begin iterator to the include directory list.
    */
    QStringList::ConstIterator directoriesBegin() const;
    /*!
     \brief Returns end iterator to the include directory list.
    */
    QStringList::ConstIterator directoriesEnd() const;

    /*!
     \brief Returns the library definition map.
    */
    const LibraryMap& getLibraryMap() const;
    /*!
     \brief Returns the include directory list.
    */
    const QStringList& getDirectories() const;

    /*!
     \brief Add a list of libraries given by a list of absolute include paths using addFile().

     \param list of file paths
    */
    void addFiles(const QStringList& list);
    /*!
     \brief Add a library to the library definition map.

     \param path absolute path to the library to add
     \param unique if true, it will only be added if it is not already contained (recommended)
     \return id of the library. can be used to access it. 0 in case the librar could not be added.
    */
    int addFile(const QString& path, bool unique = true);

    /*!
     \brief Loads all libraries and links them to the application.
    */
    void load();
    /*!
     \brief Loads the specified library and links it to the application.

     \param path absolute path to the library
     \return true if loading was successful
    */
    bool load(const QString& path);
    /*!
     \brief Loads the specified library and links it to the application.

     \param id id of the library as returned by addFile()
     \return true if loading was successful
    */
    bool load(int id);
    /*!
     \brief Loads the specified library and links it to the application.

     \param lib pointer to the library as found in the library map accessible by getLibraryMap()
     \return true if loading was successful
    */
    bool load(const boost::shared_ptr<QLibrary>& lib);

    /*!
     \brief Unloads the specified library and unlinks it from the application.

     This will NOT be called automatically when the LibraryLoader is destroyed or include directories are removed.
     \param path absolute path to the library
     \return true if unloading was successful
    */
    bool unload(const QString& path);
    /*!
     \brief Unloads the specified library and unlinks it from the application.

     This will NOT be called automatically when the LibraryLoader is destroyed or include directories are removed.
     \param id id of the library as returned by addFile()
     \return true if unloading was successful
    */
    bool unload(int id);
    /*!
     \brief Unloads the specified library and unlinks it from the application.

     This will NOT be called automatically when the LibraryLoader is destroyed or include directories are removed.
     \param lib pointer to the library as found in the library map accessible by getLibraryMap()
     \return true if unloading was successful
    */
    bool unload(const boost::shared_ptr<QLibrary>& lib);

    /*!
     \brief Returns true if the specified library was already loaded and linked to the application.

     \param path absolute path to the library
     \return true if the library was found and is loaded
    */
    bool isLoaded(const QString& path) const;
    /*!
     \brief Returns true if the specified library was already loaded and linked to the application.

     \param id id of the library as returned by addFile()
     \return true if the library was found and is loaded
    */
    bool isLoaded(int id) const;
    /*!
     \brief Returns true if the specified library was already loaded and linked to the application.

     \param lib pointer to the library as found in the library map accessible by getLibraryMap()
     \return true if the library was found and is loaded
    */
    bool isLoaded(const boost::shared_ptr<QLibrary>& lib) const;

    /*!
     \brief Exports plugins from all all libraries.
    */
    void exportPlugins();
    /*!
     \brief Exports plugins from the specified library.

     This will only work for libraries which offer the external C function 'ExportPlugins' as defined by the EXPORT_PLUGINS_BEGIN and EXPORT_PLUGINS_END
     macros in abstractplugin.h.
     \param path absolute path to the library
     \return true if export function was found and plugins were exported successfully
    */
    bool exportPlugins(const QString& path);
    /*!
     \brief Exports plugins from the specified library.

     This will only work for libraries which offer the external C function 'ExportPlugins' as defined by the EXPORT_PLUGINS_BEGIN and EXPORT_PLUGINS_END
     macros in abstractplugin.h.
     \param id id of the library as returned by addFile()
     \return true if export function was found and plugins were exported successfully
    */
    bool exportPlugins(int id);
    /*!
     \brief Exports plugins from the specified library.

     This will only work for libraries which offer the external C function 'ExportPlugins' as defined by the EXPORT_PLUGINS_BEGIN and EXPORT_PLUGINS_END
     macros in abstractplugin.h.
     \param lib pointer to the library as found in the library map accessible by getLibraryMap()
     \return true if export function was found and plugins were exported successfully
    */
    bool exportPlugins(const boost::shared_ptr<QLibrary>& lib);

    /*!
     \brief Exports a gui layout (as QMainWindow) from the specified library.

     This will only work for libraries which offer the external C function 'ExportLayout' as defined by the EXPORT_LAYOUT macro in windowmanager.h.
     \param path absolute path to the library
     \return true if export function was found and main window was exported successfully
    */
    QMainWindow* exportLayout(const QString& path);
    /*!
     \brief Exports a gui layout (as QMainWindow) from the specified library.

     This will only work for libraries which offer the external C function 'ExportLayout' as defined by the EXPORT_LAYOUT macro in windowmanager.h.
     \param id id of the library as returned by addFile()
     \return true if export function was found and main window was exported successfully
    */
    QMainWindow* exportLayout(int id);
    /*!
     \brief Exports a gui layout (as QMainWindow) from the specified library.

     This will only work for libraries which offer the external C function 'ExportLayout' as defined by the EXPORT_LAYOUT macro in windowmanager.h.
     \param lib pointer to the library as found in the library map accessible by getLibraryMap()
     \return true if export function was found and main window was exported successfully
    */
    QMainWindow* exportLayout(const boost::shared_ptr<QLibrary>& lib);

    /*!
     \brief Returns a shared pointer to the specified library.

     \param path absolute path to the library
     \return pointer to the library, or null pointer if nothing was found
    */
    const boost::shared_ptr<QLibrary> getLibrary(const QString& path) const;
    /*!
     \brief Returns a shared pointer to the specified library.

     \param id id of the library as returned by addFile()
     \return pointer to the library, or null pointer if nothing was found
    */
    const boost::shared_ptr<QLibrary> getLibrary(int id) const;

    /*!
     \brief Searches and returns a shared pointer to a library specified by name.

     \param name base file name (without path and extension) of the library to search for
     \return pointer to the library, or null pointer if nothing was found
    */
    const boost::shared_ptr<QLibrary> getLibraryByFileName(const QString& name) const;

    /*!
     \brief Returns the error string of a specified library.

     Errors may happen after load or export function calls.
     \param path absolute path to the library
     \return the error string
    */
    QString getErrorString(const QString& path) const;
    /*!
     \brief Returns the error string of a specified library.

     Errors may happen after load or export function calls.
     \param id id of the library as returned by addFile()
     \return the error string
    */
    QString getErrorString(int id) const;

    /*!
     \brief Updates the whole library list by adding all libraries included in all library include paths.

     New libraries found will only be added to the library list.
     They wont be loaded and linked to the application yet.
    */
    void updateLibraries();
    /*!
     \brief Updates the library list by loading all libraries included in the specified path

     New libraries found will only be added to the library list.
     They wont be loaded and linked to the application yet.
     \param path directory to include libraries from
     \return true if the directory was found and no error happened while loading
    */
    bool updateLibraries(const QDir& path);

    /*!
     \brief Searches and returns the id to a library specified by name.

     \param name base file name (without path and extension) of the library to search for
     \return id of the library or 0 if nothing was found
    */
    int search(const QString& name) const;

private:
    //private functions

    /*!
     \brief Loads the specified library and links it to the application.

     \param lib the library definition of the library map
     \return true if loading was successful
    */
    bool load(LibraryDefinition& lib);
    /*!
     \brief Unloads the specified library and unlinks it from the application.

     This will NOT be called automatically when the LibraryLoader is destroyed or include directories are removed.
     \param lib the library definition of the library map
     \return true if unloading was successful
    */
    bool unload(LibraryDefinition& lib);
    /*!
     \brief Returns true if the specified library was already loaded and linked to the application.

     \param lib the library definition of the library map
     \return true if the library was found and is loaded
    */
    bool isLoaded(const LibraryDefinition& lib) const;
    /*!
     \brief Exports plugins from the specified library.

     This will only work for libraries which offer the external C function 'ExportPlugins' as defined by the EXPORT_PLUGINS_BEGIN and EXPORT_PLUGINS_END
     macros in abstractplugin.h.
     \param lib the library definition of the library map
     \return true if export function was found and plugins were exported successfully
    */
    bool exportPlugins(LibraryDefinition& lib);
    /*!
     \brief Exports a gui layout (as QMainWindow) from the specified library.

     This will only work for libraries which offer the external C function 'ExportLayout' as defined by the EXPORT_LAYOUT macro in windowmanager.h.
     \param lib the library definition of the library map
     \return true if export function was found and main window was exported successfully
    */
    QMainWindow* exportLayout(LibraryDefinition& lib);

    /*!
     \brief Returns shared pointer to the specified library.

     \param path absolute path to the library
    */
    boost::shared_ptr<QLibrary> getLibraryNc(const QString& path) const;
    /*!
     \brief Returns shared pointer to the specified library.

     \param id id of the library as returned by addFile()
    */
    boost::shared_ptr<QLibrary> getLibraryNc(int id) const;
    /*!
     \brief Returns iterator in the library map to the specified library.

     \param path absolute path to the library
     \return iterator to the found position or end() if not found
    */
    LibraryMap::ConstIterator findLibraryDefinition(const QString& path) const;
    /*!
     \brief Returns iterator in the library map to the specified library.

     \param id id of the library as returned by addFile()
     \return iterator to the found position or end() if not found
    */
    LibraryMap::ConstIterator findLibraryDefinition(int id) const;
    /*!
     \brief Returns iterator in the library map to the specified library.

     \param lib pointer to the library as contained in the library map accessible by getLibraryMap()
     \return iterator to the found position or end() if not found
    */
    LibraryMap::ConstIterator findLibraryDefinition(const boost::shared_ptr<QLibrary>& lib) const;
    /*!
     \brief Searches and returns iterator to a library specified by name.

     \param name base file name (without path and extension) of the library to search for
     \return iterator to the found position, or end() if not found
    */
    LibraryMap::ConstIterator findLibraryDefinitionByFileName(const QString& name) const;

    /*!
     \brief Adds a single library at the given path to the library map.

     \param path absolute path to the library
     \return int new id of the library, or the old id if already contained
    */
    int addLibrary(const QString& path);
    /*!
     \brief Removes a single library with the given path from the library map.

     This will NOT unlink the library but only remove the handle.
     Call unload() of the library to unlink it.
     \param path absolute path to the library
    */
    void removeLibrary(const QString& path);
    /*!
     \brief Removes all libraries from the library map.

     This will NOT unlink the libraries but only remove the handles.
     Call unload() to unlink them.
     \param path absolute path to the library
    */
    void clearLibraries();

    /*!
     \brief Returns the next library id, starting with 1.
    */
    int getNextId();

private:
    //private members

    LibraryMap mLibraries;    /*!< Map of all library definitions, using their absolute filepath as key. */
    QStringList mDirectories; /*!< List of all library include directories. */
    QStringList mFilters;     /*!< List of library file filters. */

    int mNextId; /*!< Next library id. */
};

#endif
