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
*/
#include "fileserver.h"
#include <salt/fileclasses.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/core.h>

using namespace boost;
using namespace salt;
using namespace zeitgeist;
using namespace std;

FileServer::FileServer() : Node(), mNextHandle(1)
{
}

FileServer::~FileServer()
{
}

void FileServer::AddResourceLocation(const std::string& path)
{
    mResourceLocations.push_back(path + salt::RFile::Sep());
}

bool FileServer::LocateResource(const std::string& inName, std::string& outName)
{
    if (Exist(inName))
        {
            outName = inName;
            return true;
        }

    for (unsigned i = 0; i < mResourceLocations.size(); ++i)
    {
        string fname = mResourceLocations[i] + inName;

        if (Exist(fname))
            {
                GetLog()->Debug() << "(FileServer::LocateResource) expanded filename to '"
                                  << fname << "'\n";

                outName = fname;
                return true;
            }
    }

    GetLog()->Debug()
        << "FileServer::LocateResource) unable to locate resource '"
        << inName << "'\n";

    return false;
}

shared_ptr<salt::RFile> FileServer::OpenResource(const std::string& inName)
{
    string fname;
    if (! LocateResource(inName, fname))
        {
            GetLog()->Error() << "(FileServer::OpenResource) Cannot locate file '"
                              << inName << "'\n";
            return shared_ptr<salt::RFile>();
        }

    return Open(fname);
}

shared_ptr<salt::RFile> FileServer::Open(const string& inName)
{
    for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
        {
            shared_ptr<FileSystem> fileSys = shared_static_cast<FileSystem>(*i);

            shared_ptr<salt::RFile> file(fileSys->Open(inName));

            //first successful is returned
            if(file.get() != 0)
                {
                    return file;
                }
        }

    // try to open it via the regular file system
    shared_ptr<salt::RFile> file(new StdFile());
    if (! file->Open(inName.c_str()))
        {
            file.reset();
        }

    return file;
}

FileServer::THandle FileServer::Register(const string& inName)
{
    shared_ptr<salt::RFile> file = Open(inName);

    if (file.get() == 0)
        {
            return 0;
        }

    mFileMap[mNextHandle] = file;
    THandle h = mNextHandle;
    ++mNextHandle;

    return h;
}

shared_ptr<salt::RFile> FileServer::Get(THandle handle) const
{
    TFileMap::const_iterator iter = mFileMap.find(handle);

    if (iter == mFileMap.end())
        {
            GetLog()->Warning()
                << "(FileServer::Get) Warning: Unknown file handle "
                << handle << "\n";
            return shared_ptr<salt::RFile>();
        }

    return (*iter).second;
}

void FileServer::Close(THandle handle)
{
    TFileMap::iterator iter = mFileMap.find(handle);

    if (iter == mFileMap.end())
        {
            GetLog()->Warning()
                << "(FileServer::Close) Warning: Unknown file handle "
                << handle << "\n";
            return;
        }

    mFileMap.erase(iter);

    if (mFileMap.empty())
        {
            // restart handle counting on empty FileMap
            mNextHandle = 1;
        }
}

void FileServer::OnUnlink()
{
    if (! mFileMap.empty())
        {
            GetLog()->Warning() << "(FileServer) There are "
                                << mFileMap.size()
                                << " files left in the registry\n";
        }
}

bool FileServer::Exist(const string& inName)
{
    return (Open(inName).get() != 0);
}

// this routine registers a new file system instance with the server
bool FileServer::Mount(const string& inFileSysName, const string& inPath)
{
    shared_ptr<FileSystem> fileSys =
        shared_static_cast<FileSystem>(GetChild(inPath));

    if (fileSys)
        {
            // we already have a file system which is bound to the same name
            if (fileSys->GetClass()->GetName().compare(inFileSysName) == 0)
                {
                    // as the file system has the same type, we can return true
                    return true;
                }
            else
                {
                    // already have a file system of a different type, so return false
                    GetLog()->Error()
                        << "(FileServer) ERROR: a FileSystem is already mounted a "
                        << inPath << "\n";
                    return false;
                }
        }

    // try to instantiate the file system
    fileSys = shared_static_cast<FileSystem>(GetCore()->New(inFileSysName));

    if (
        (fileSys.get() == 0) ||
        (! fileSys->SetPath(inPath))
        )
        {
            return false;
        }

    // link it into our hierarchy
    AddChildReference(fileSys);

    GetLog()->Normal() <<
        "(FileServer) successfully mounted a '"
                       << inFileSysName << "' at '"
                       << inPath << "'\n";
    return true;
}

bool FileServer::Unmount(const string& inPath)
{
    // try to remove a std file system first
    if (Unmount ("FileSystemSTD", inPath))
        {
            return true;
        }

    shared_ptr<Leaf> leaf = GetChild(inPath);

    if(leaf)
    {
        leaf->Unlink();
 return true;
    }

  return false;
}

bool FileServer::Unmount(const string& inFileSysName, const string& inPath)
{
  shared_ptr<FileSystem> fileSystem = shared_static_cast<FileSystem>(GetChild(inPath));

  if(fileSystem)
    {
      if (fileSystem->GetClass()->GetName().compare(inFileSysName) == 0)
        {
          fileSystem->Unlink();
          return true;
        }
    }

  return false;
}

int FileServer::ForEachFile(const string& /*directory*/, const string& /*name*/,
                            const string& /*extension*/,
                            FileSystem::TCallback /*callback*/, void* /*param*/)
{
  int count = 0;
  /*for (TDescriptionList::iterator iter = mFileSystems.begin (); iter != mFileSystems.end (); ++iter)
    {
    count += iter->fileSystem->ForEachFile(directory,name,extension,callback,param);
    }*/

  return count;
}
