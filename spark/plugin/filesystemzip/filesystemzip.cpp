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
#include "filesystemzip.h"
#include <salt/fileclasses.h>
#include <zeitgeist/logserver/logserver.h>
#include <string.h>
#include <zlib.h>
#ifndef Q_MOC_RUN
#include <boost/regex.hpp>
#endif

#ifndef HAVE_STRUPR
#include <ctype.h>
char*
strupr( char* s1 )
{
    int i;
    for (i=0; i<strlen(s1); ++i)
        {
            s1[i] = toupper(s1[i]);
        }
    return s1;
}
#endif

//ZIP Format Definitions
#define SIG_LOCAL_HEADER          0x04034b50
#define SIG_FILE_HEADER           0x02014b50
#define SIG_CENTRAL_DIRECTORY_END 0x06054b50

#define ZIP_STORE   0
#define ZIP_DEFLATE 8

using namespace salt;
using namespace std;
using namespace boost;

FileSystemZIP::FileSystemZIP()
    : FileSystem()
{
}

FileSystemZIP::~FileSystemZIP()
{
    Clear();
}

int FileSystemZIP::ForEachFile(const string& expression, TCallback callback, void* param)
{
    if (!callback)
        {
            return 0;
        }

    int count = 0;
    boost::regex regExpression(expression);

    for (TEntryMap::iterator i = mEntryMap.begin(); i != mEntryMap.end(); ++i)
        {
            TArchiveEntry *cur = (*i).second;
            // check if we have a file
            if (!(cur->attr&16))
                {
                    if (boost::regex_match(cur->filename, regExpression))
                        {
                            callback(cur->filename,param);
                            ++count;
                        }
                }
        }

    return count;
}

//
// This sets the path to the archive this object will be associated with.
// The behavior is quite simple. First it will try 'inName' then 'inName.zip'
//
bool FileSystemZIP::SetPath(const string& inName)
{
    if(mHandle.get() != 0)
        {
            //Already selected an archive so unselect it
            Clear();
        }

    // get a stdio file
    mHandle = boost::shared_ptr<StdFile>(new StdFile());

    // try inName directly
    if(! mHandle->Open(inName.c_str()))
        {
            // ok, let's try it with a '.zip' extension
            string tryName = inName + ".zip";
            if(! mHandle->Open(tryName.c_str()))
                {
                    GetLog()->Error() << "(FileSystemZIP) ERROR: unable to open '"
                                      << inName << "'\n";
                    return false;
                }
        }

    //The open operation succeeded, so we can store the name of the
    //archive file
    mArchiveName = inName;

    //Read Central Directory End
    TCentralDirectoryEnd    cde;
    ZIPGetCentralDirectoryEndFirst(&cde);

    //Move to beginning of the Central Directory
    mHandle->Seek(cde.offset_start_cd, SEEK_SET);

    //Free memory
    delete[] cde.zipfile_comment;

    //Start reading data
    TLocalHeader lh;
    TFileHeader fh;
    TArchiveEntry* ae;

    int sig = mHandle->Igetl();

    while(! mHandle->Eof())
        {
            switch(sig){
            case SIG_LOCAL_HEADER:
                //We should actually never get here, but this is left in for completeness
                ZIPGetLocalHeader(&lh);
                ZIPSkipData(&lh);

                delete[] lh.filename;
                delete[] lh.extra_field;
                break;
            case SIG_FILE_HEADER:
                //The data for this block is added to our internal database
                ZIPGetFileHeader(&fh);
                ae = new TArchiveEntry;
                ae->filename    = strupr(fh.filename);
                ae->data                = fh.relative_offset+4;
                ae->attr                = (fh.external_file_attributes)&0xff;
                AddArchiveEntry(ae);

                delete[] fh.extra_field;
                delete[] fh.file_comment;
                break;
            case SIG_CENTRAL_DIRECTORY_END:
                //Again useless reading of data, but we need to eat up the stuff, so here goes
                ZIPGetCentralDirectoryEnd(&cde);

                delete[] cde.zipfile_comment;
                break;
            default:
                // an error has occured
                return false;
        }

        //Get next signature and update EOF flag
        sig = mHandle->Igetl();
    }
    return true;
}

boost::shared_ptr<salt::RFile> FileSystemZIP::Open(const string& inName)
{
    char            *fileName;
    TArchiveEntry   *cur;
    TLocalHeader    lh;
    unsigned int    bytes_left=0;
    size_t                  size;
    unsigned int    buf_size=4096;
    unsigned char   *compr, *uncompr;
    unsigned int    err;
    z_stream        d_stream; /* decompression stream */

    // if no file is open, return 0
    if(mHandle.get() == 0)
        {
            return boost::shared_ptr<salt::RFile>();
        }

    // we have an archive, now get a local copy of inName
    fileName = new char[inName.size()+1];
    strcpy(fileName, inName.c_str());

    // search for file
    TEntryMap::iterator i = mEntryMap.find(std::string(strupr(fileName)));
    delete[]fileName;

    if (i == mEntryMap.end())
        {
            return boost::shared_ptr<salt::RFile>();
        }

    // we have found a file
    cur = (*i).second;

    //Move to beginning of data
    mHandle->Seek(cur->data,SEEK_SET);

    //Get the local header
    ZIPGetLocalHeader(&lh);

    //Allocate memory for file
    uncompr = new unsigned char[lh.uncompressed_size];

    //Get data based on compression method used
    switch(lh.compression_method)
        {
        case ZIP_STORE:
            //Data is not compressed, so just read it from the file
            mHandle->Read(uncompr, lh.uncompressed_size);
            break;
        case ZIP_DEFLATE:
            //Data is compressed, so decompress it
            bytes_left                      = lh.uncompressed_size;
            d_stream.zalloc         = (alloc_func)0;
            d_stream.zfree          = (free_func)0;
            d_stream.opaque         = (voidpf)0;
            d_stream.avail_out      = lh.uncompressed_size;
            d_stream.next_out       = uncompr;

            // get memory for buffer
            compr                   = new unsigned char[buf_size];

            //This makes ZLIB able to decompress ZIP File streams
            inflateInit2(&d_stream, -15);

            while(bytes_left)
                {
                    d_stream.next_in = compr;
                    if(bytes_left > buf_size)
                        size = buf_size;
                    else
                        size = bytes_left;

                    d_stream.avail_in = mHandle->Read(compr, size);
                    err = inflate(&d_stream,bytes_left>size ? Z_PARTIAL_FLUSH : Z_FINISH);
                    bytes_left -= size;
                }

            //Finished decompression
            inflateEnd(&d_stream);
            //Free decompression buffer
            delete[]compr;
            break;
        default:
            //Mode not supported
            delete []uncompr;
            uncompr=0;
            break;
        }

    boost::shared_ptr<salt::RFile> f;
    if(uncompr != 0)
        {
            MemFile* mf = new MemFile();
            mf->Open(uncompr, lh.uncompressed_size);
            f.reset(mf);
        }

    delete[] lh.filename;
    delete[] lh.extra_field;
    return f;
}

void FileSystemZIP::Clear()
{
    mArchiveName = "";
    mHandle.reset();

    for (TEntryMap::iterator i = mEntryMap.begin(); i != mEntryMap.end(); ++i)
        {
            TArchiveEntry *cur = (*i).second;
            delete[] cur->filename;
            delete cur;
        }


    mEntryMap.clear();
}

void FileSystemZIP::ZIPGetLocalHeader(TLocalHeader *lh)
{
    int i;

    lh->signature                   = SIG_LOCAL_HEADER;
    lh->version_needed              = mHandle->Igetw();
    lh->general_purpose             = mHandle->Igetw();
    lh->compression_method  = mHandle->Igetw();
    lh->last_mod_time               = mHandle->Igetw();
    lh->last_mod_date               = mHandle->Igetw();
    lh->crc32                               = mHandle->Igetl();
    lh->compressed_size             = mHandle->Igetl();
    lh->uncompressed_size   = mHandle->Igetl();
    lh->filename_length             = mHandle->Igetw();
    lh->extra_field_length  = mHandle->Igetw();

    lh->filename = new char[lh->filename_length+1];
    for(i=0;i<lh->filename_length;i++)
        lh->filename[i]=mHandle->Getc();

    lh->filename[i] = 0;

    lh->extra_field = new char[lh->extra_field_length+1];
    for(i=0;i<lh->extra_field_length;i++)
        lh->extra_field[i]=mHandle->Getc();

    lh->extra_field[i] = 0;
}

void FileSystemZIP::ZIPSkipData(TLocalHeader *lh)
{
    mHandle->Seek(lh->compressed_size,SEEK_CUR);
}

void FileSystemZIP::ZIPGetFileHeader(TFileHeader *fh)
{
    int i;

    fh->signature              = SIG_FILE_HEADER;
    fh->version_made           = mHandle->Igetw();
    fh->version_needed         = mHandle->Igetw();
    fh->general_purpose        = mHandle->Igetw();
    fh->compression_method     = mHandle->Igetw();
    fh->last_mod_time          = mHandle->Igetw();
    fh->last_mod_date          = mHandle->Igetw();
    fh->crc32                  = mHandle->Igetl();
    fh->compressed_size        = mHandle->Igetl();
    fh->uncompressed_size      = mHandle->Igetl();
    fh->filename_length        = mHandle->Igetw();
    fh->extra_field_length     = mHandle->Igetw();
    fh->file_comment_length    = mHandle->Igetw();
    fh->disk_number_start      = mHandle->Igetw();
    fh->internal_file_attributes = mHandle->Igetw();
    fh->external_file_attributes = mHandle->Igetl();
    fh->relative_offset           = mHandle->Igetl();

    fh->filename = new char[fh->filename_length+1];
    for(i=0;i<fh->filename_length;i++)
        fh->filename[i]=mHandle->Getc();

    fh->filename[i] = 0;

    fh->extra_field = new char[fh->extra_field_length+1];
    for(i=0;i<fh->extra_field_length;i++)
        fh->extra_field[i]=mHandle->Getc();

    fh->extra_field[i] = 0;

    fh->file_comment = new char[fh->file_comment_length+1];
    for(i=0;i<fh->file_comment_length;i++)
        fh->file_comment[i]=mHandle->Getc();

    fh->file_comment[i] = 0;
}

void FileSystemZIP::ZIPGetCentralDirectoryEnd(TCentralDirectoryEnd *cde)
{
    int i;

    cde->signature                  = SIG_CENTRAL_DIRECTORY_END;
    cde->this_disk_no               = mHandle->Igetw();
    cde->cds_disk_no                = mHandle->Igetw();
    cde->num_entries_this_disk      = mHandle->Igetw();
    cde->num_entries_total          = mHandle->Igetw();
    cde->cd_size                    = mHandle->Igetl();
    cde->offset_start_cd            = mHandle->Igetl();
    cde->zipfile_comment_length     = mHandle->Igetw();

    cde->zipfile_comment = new char[cde->zipfile_comment_length+1];
    for(i=0;i<cde->zipfile_comment_length;i++)
        cde->zipfile_comment[i]=mHandle->Getc();

    cde->zipfile_comment[i] = 0;
}

void FileSystemZIP::ZIPGetCentralDirectoryEndFirst(TCentralDirectoryEnd *cde)
{
    int save_pos = mHandle->Tell();
    long sig = 0;

    //position fpointer to end of file
    //fseek(f,0,SEEK_END) did not work!
    if(mHandle->Seek(mHandle->Size()-21,SEEK_SET)) return;
    sig = mHandle->Igetl();
    while(sig!=SIG_CENTRAL_DIRECTORY_END)
        {
            if(mHandle->Seek(-5,SEEK_CUR)) return;
            sig = mHandle->Igetl();
        }

    ZIPGetCentralDirectoryEnd(cde);
    mHandle->Seek(save_pos, SEEK_SET);
}

void FileSystemZIP::AddArchiveEntry(TArchiveEntry *ae)
{
    mEntryMap[std::string(ae->filename)] = ae;
}
