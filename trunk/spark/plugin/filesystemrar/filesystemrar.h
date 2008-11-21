/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: filesystemrar.h,v 1.5 2004/04/18 16:20:53 rollmark Exp $

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

   FileSystemRAR

        HISTORY:
                10.08.02 - MK
                        - Initial version

        TODO:
                - implement ForEachFile platform independent

        TOFIX:
*/
#ifndef FILESYSTEMRAR_H__
#define FILESYSTEMRAR_H__

#include <string>
#include <zeitgeist/fileserver/filesystem.h>

/**     \class FileSystemRAR

        FileSystemRAR

        This class is a wrapper to the unrar.dll capable of handling
        RAR-compressed archives. Unfortunately, the unrar.dll is only
        available for Windows platforms :(

        Another issue to note for this is the way this filesystem
        works. It does not keep the archive open over subsequent
        Open() calls. Instead it has to reopen the archive again and
        again. This is a limitation of the API provided by the
        unrar.dll and very annoying.
*/
class FileSystemRAR : public zeitgeist::FileSystem
{
        //
        // functions
        //
public:
        FileSystemRAR();
        virtual ~FileSystemRAR();

        virtual boost::shared_ptr<salt::RFile> Open(const std::string& inName);
        virtual bool SetPath(const std::string& inPath);
        virtual int ForEachFile(const std::string& expression, TCallback callback, void* param);

        //
        // members
        //
protected:
        std::string     mArchiveName;
};

DECLARE_CLASS(FileSystemRAR);

#endif //FILESYSTEMRAR_H__
