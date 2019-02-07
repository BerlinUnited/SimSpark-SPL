/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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

   FileServer - global interface for file system access

        HISTORY:
                12.07.01 - MK
                        - Initial version
                14.07.01 - MK
                        - ClassServer was added, so the FileServer doesn't manage the
                          file system factories anymore (only concrete file system instances)
                11.10.01 - MK
                        - Made singleton functionality more secure
                20.11.01 - AF
                        - added the priority of the file systems : LIFO
                        - added the information of the file system id and its path to every mounted file system
                        - prevented the odditity to mount a file system more than once
                        - added GetFirstFileSystem and
                        GetNextFileSystem to iterate through the
                        mounted file systems
                26.01.02 - MR
                        - added ForEachFile
                27.06.02 - MK
                        - converted this to the Zeitgeist framework
                09.07.02 - MK
                        - converted this to the Kerosin framework
                10.10.02 - MK
                        - back to Zeitgeist framework

        TODO:

        TOFIX:
*/
#ifndef ZEITGEIST_FILESERVER_H
#define ZEITGEIST_FILESERVER_H

#include <vector>
#include <zeitgeist/node.h>
#include "filesystem.h"

namespace zeitgeist
{

/** FileServer - the global interface for file system access

    What the FileServer does:
    - Manage different file systems
    - Load files from a FileSystem

    The file server is an extremely useful subsystem, since it allows
    to access various implemented file systems. The regular file
    system is a standard directory-based implementation. The ZIP file
    system can load files from within ZIP-files. Before actually being
    able to open files, the FileServer is initialized with several
    FileSystems, which are associated with different paths. For each
    path, you have to pass in a file system. Now, when trying to open
    a file, each registered file system is asked to open the
    file. Filesystems are searched in the inverse order in wich they
    were registered- think of a filesystem stack. The first succesful
    opened file wins. This allows for some nice effects. File systems
    are associated with id strings like 'STD' or 'ZIP'.
  */
class ZEITGEIST_API FileServer : public Node
{
    //
    // functions
    //
public:
    typedef long THandle;

protected:
    typedef std::map<THandle, boost::shared_ptr<salt::RFile> > TFileMap;

public:
    /** constructs the fileserver */
    FileServer();
    ~FileServer();

    /**
     * adds a new location to the list of resource locations
     */
    void AddResourceLocation(const std::string& path);

    /** tests for the existence of the given file at well known
        locations. On success the function returns true. In this case
        outName is set to the full path to the file.
    */
    bool LocateResource(const std::string& inName, std::string& outName);

    /** searches each registered file system for a file with this name
        and tries well known resource locations if that fails
    */
    boost::shared_ptr<salt::RFile> OpenResource(const std::string& inName);

    /** searchs each registered file system for a file with this
        name. Filesystems are searched in the inverse order in
        which they are registered to the fileserve, i.e. a
        filesystem stack. The first succesful opened file is
        returned.
    */
    boost::shared_ptr<salt::RFile> Open(const std::string& inName);

    /** tries to open the requested file and registers it. On success
        it returns a non 0 handle assiociated with the file object.
     */
    THandle Register(const std::string& inName);

    /** returns the file corresponding to the given handle.
     */
    boost::shared_ptr<salt::RFile> Get(THandle handle) const;

    /** closes the file corresponding to the given handle
     */
    void Close(THandle handle);

    /** returns true if the file 'inName' exists. */
    bool Exist(const std::string& inName);

    /** registers a filesystem to the fileserver. A file system
        may be registered only once, on each further try nothing
        is done and false returned

        \param inFileSysName is the class name of the File system
        \param inPath is the mount point in the virtual file
        system provided by the fileserver
    */
    bool Mount(const std::string& inFileSysName, const std::string& inPath);

    /** unmounts a file system at the mount point inPath. if no
        file system id is given, for a first try FileSystemSTD is
        assumed, then the type is ignored. Returns true on success.
    */
    bool Unmount(const std::string& inPath);

    /** unmounts a file system at the mount point inPath. Returns
        true on success.
    */
    bool Unmount(const std::string& inClass, const std::string& inPath);

    /** iterates through files. 'directory', 'name' and
     * 'extension' give directory, name and extension a file must
     * match.  directory,name and extension may be NULL, in which
     * case every directory,extension and/or name matches. For
     * each match the function callback is called with the name
     * of the matched file and the additional user parameter
     * 'param'. param is just passed through to the callback and
     * has no meaning to the filesystem.
     */
    int ForEachFile(const std::string& directory, const std::string& name,
                    const std::string& extension,
                    FileSystem::TCallback callback, void* param);

protected:
    /** This rountine is called, before the FileServer hierarchy
        object is removed from the parent.
    */
    virtual void OnUnlink();

private:
    FileServer(const FileServer&);
    FileServer& operator=(const FileServer&);

protected:
    /** registry of opened files using the handle base system */
    TFileMap mFileMap;

    /** the next free handle */
    THandle mNextHandle;

    /** list of resource locations */
    std::vector<std::string> mResourceLocations;
};

DECLARE_CLASS(FileServer)

} //namespace zeitgeist

#endif //ZEITGEIST_FILESERVER_H
