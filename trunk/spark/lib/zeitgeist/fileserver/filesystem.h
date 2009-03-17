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

   FileSystem

        HISTORY:
                27.06.2002 MK
                        - initial version

*/
#ifndef ZEITGEIST_FILESYSTEM_H
#define ZEITGEIST_FILESYSTEM_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <salt/fileclasses.h>


namespace zeitgeist
{
  /** this class defines the interface which derived filesystems must
      implement in order to be used with the fileserver.
   */
class ZEITGEIST_API FileSystem : public Leaf
{
    //
    // types
    //
public:
    /** this function defines an interface for a callback, used to
        iterate over files. It used in conjunction with ForEachFile().
    */
    typedef void (*TCallback) (char *filename, void *param);

    //
    // functions
    //
public:
    /** constructs a filesystem */
    FileSystem() : Leaf()       {}
    virtual ~FileSystem() {}

    /** tries to open the file named inName. Returns an instance of a
        salt::RFile on success, NULL otherwise */
    virtual boost::shared_ptr<salt::RFile> Open(const std::string& inName) = 0;

    /** sets the path all calls to Open are relative to. For a
        standard file system this call maps directly to a
        directory. For Filesystems providing access to an archive it
        is used to select the archive, i.e. it is the filename of an
        archive. Please refer to concrete Filesystems for an example
        implementation. */
    virtual bool SetPath(const std::string& inPath) = 0;

    /** iterates over all files managed by this filesystem.
     * 'directory', 'name' and 'extension' give directory, name and
     * extension a file must match. directory,name and extension may
     * be NULL, in wich case every directory,extension and/or name
     * matches. For each match the function callback is called with
     * the name of the matched file and the additional user parameter
     * 'param'. param is just passed through to the callback and has
     * no meaning to the filesystem.
     */
    virtual int ForEachFile(const std::string&, TCallback callback,
                            void* param) = 0;

private:
    FileSystem(const FileSystem& obj);
    FileSystem& operator=(const FileSystem& obj);
};

DECLARE_ABSTRACTCLASS(FileSystem);

} //namespace zeitgeist

#endif //ZEITGEIST_FILESYSTEM_H
