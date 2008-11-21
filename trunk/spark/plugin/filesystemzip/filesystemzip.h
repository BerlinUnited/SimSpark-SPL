/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: filesystemzip.h,v 1.6 2007/05/29 09:45:38 jboedeck Exp $

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
#ifndef FILESYSTEMZIP_H__
#define FILESYSTEMZIP_H__

/*      \class FileSystemZIP
        $Id: filesystemzip.h,v 1.6 2007/05/29 09:45:38 jboedeck Exp $

        FileSystemZIP - The ZIP File System

        This is a bit more tricky than the STD file system, since it uses a ZIP
        file as a virtual file system. Decompression is handled with ZLIB whereas
        the file format was decoded using custom ZIP-file reading routines ...
        handwritten.

        NOTE:

        HISTORY:
        30.08.01 - MK
        - Initial version
        26.01.02 - MR
        - added ForEachFile
        25.02.02 - MR
        - now using a hash map to search for contained files
        28.06.02 - MK
        - moved to Zeitgeist

        TODO:

        TOFIX:
*/
#ifdef HAVE_CONFIG_H
#include <sparkconfig.h>
#endif

#ifdef HAVE_HASH_MAP
#include <hash_map>
#else
#include <map>
#endif

#include <string>
#include <zeitgeist/fileserver/filesystem.h>

//Forward declarations needed
namespace salt
{
  class StdFile;
  class RFile;
}

class FileSystemZIP : public zeitgeist::FileSystem
{
    // types
protected:
    struct TLocalHeader
    {
        long    signature;
        short   version_needed;
        short   general_purpose;
        short   compression_method;
        short   last_mod_time;
        short   last_mod_date;
        long    crc32;
        unsigned long compressed_size;
        unsigned long uncompressed_size;
        short   filename_length;
        short   extra_field_length;
        char    *filename;
        char    *extra_field;
    };

    struct TFileHeader
    {
        long    signature;
        short   version_made;
        short   version_needed;
        short   general_purpose;
        short   compression_method;
        short   last_mod_time;
        short   last_mod_date;
        long    crc32;
        long    compressed_size;
        long    uncompressed_size;
        short   filename_length;
        short   extra_field_length;
        short   file_comment_length;
        short   disk_number_start;
        short   internal_file_attributes;
        long    external_file_attributes;
        long    relative_offset;
        char    *filename;
        char    *extra_field;
        char    *file_comment;
    };

    struct TCentralDirectoryEnd
    {
        long    signature;
        short   this_disk_no;
        short   cds_disk_no;
        short   num_entries_this_disk;
        short   num_entries_total;
        long    cd_size;
        long    offset_start_cd;
        short   zipfile_comment_length;
        char    *zipfile_comment;
    };

    struct TArchiveEntry
    {
        char                    *filename;
        long                    attr;
        long                    data;
    };

#ifdef HAVE_HASH_MAP
    typedef std::hash_map<std::string, TArchiveEntry*> TEntryMap;
#else
    typedef std::map<std::string, TArchiveEntry*> TEntryMap;
#endif

    // functions
public:
    FileSystemZIP();
    virtual ~FileSystemZIP();

    virtual void Clear();
    virtual boost::shared_ptr<salt::RFile>  Open(const std::string& inName);
    virtual bool SetPath(const std::string& inPath);
    virtual int ForEachFile(const std::string& expression, TCallback callback, void* param);

protected:
    void AddArchiveEntry(TArchiveEntry *ae);
    void ZIPGetLocalHeader(TLocalHeader *lh);
    void ZIPGetFileHeader(TFileHeader *fh);
    void ZIPGetCentralDirectoryEnd(TCentralDirectoryEnd *cde);
    void ZIPGetCentralDirectoryEndFirst(TCentralDirectoryEnd *cde);
    void ZIPSkipData(TLocalHeader *lh);

    // members
protected:
    boost::shared_ptr<salt::StdFile> mHandle;
    std::string mArchiveName;
    TEntryMap  mEntryMap;
};

DECLARE_CLASS(FileSystemZIP);

#endif // FILESYSTEMZIP_H__
