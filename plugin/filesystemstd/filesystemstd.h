/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: filesystemstd.h,v 1.5 2004/04/18 16:20:23 rollmark Exp $

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

   FileSystemSTD
*/

#ifndef FILESYSTEMSTD_H__
#define FILESYSTEMSTD_H__

#include <string>
#include <zeitgeist/fileserver/filesystem.h>

/**     \class FileSystemSTD

        FileSystemSTD - The 'Standard' File System

        OK, this one is really simple. It just loads a file from a
        given base directory ... no fancy conversions, just a simple
        Open("blah").
*/
class FileSystemSTD : public zeitgeist::FileSystem
{
        //
        // Methods
        //
public:
        FileSystemSTD();
        virtual ~FileSystemSTD();

        virtual boost::shared_ptr<salt::RFile> Open(const std::string& inName);
        virtual bool SetPath(const std::string& inPath);
        virtual int ForEachFile(const std::string& expression,
                                TCallback callback, void* param);

        //
        // members
        //
protected:
        std::string     mPath;
};

DECLARE_CLASS(FileSystemSTD);

#endif //__FILESYS_STD_H__
