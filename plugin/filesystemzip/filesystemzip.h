#ifndef FILESYSTEMZIP_H__
#define FILESYSTEMZIP_H__

/*	\class FileSystemZIP
	$Id: filesystemzip.h,v 1.1 2003/04/30 11:06:17 fruit Exp $

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

#include <hash_map>
#include <string>
#include <kerosin/fileserver/filesystem.h>

//Forward declarations needed
namespace salt
{
class StdFile;
class RFile;
}

class FileSystemZIP : public kerosin::FileSystem
{
	//=========================================================================
	// types
	//=========================================================================
private:
	// there's no reason to 'pollute' the global namespace with these, since the
	// user can't access them anyways
	struct TLocalHeader
	{
		long	signature;
		short	version_needed;
		short	general_purpose;
		short	compression_method;
		short	last_mod_time;
		short	last_mod_date;
		long	crc32;
		unsigned long compressed_size;
		unsigned long uncompressed_size;
		short	filename_length;
		short	extra_field_length;
		char	*filename;
		char	*extra_field;
	};

	struct TFileHeader
	{
		long	signature;
		short	version_made;
		short	version_needed;
		short	general_purpose;
		short	compression_method;
		short	last_mod_time;
		short	last_mod_date;
		long	crc32;
		long	compressed_size;
		long	uncompressed_size;
		short	filename_length;
		short	extra_field_length;
		short	file_comment_length;
		short	disk_number_start;
		short	internal_file_attributes;
		long	external_file_attributes;
		long	relative_offset;
		char	*filename;
		char	*extra_field;
		char	*file_comment;
	};

	struct TCentralDirectoryEnd
	{
		long	signature;
		short	this_disk_no;
		short	cds_disk_no;
		short	num_entries_this_disk;
		short	num_entries_total;
		long	cd_size;
		long	offset_start_cd;
		short	zipfile_comment_length;
		char	*zipfile_comment;
	};

	struct TArchiveEntry
	{
		char			*filename;
		long			attr;
		long			data;
	};

	typedef std::hash_map<std::string, TArchiveEntry*> TEntryMap;

	//=========================================================================
	// functions
	//=========================================================================
public:
	FileSystemZIP();
	virtual ~FileSystemZIP();
  
	virtual void			Clear();
	virtual salt::RFile*	Open(const char *inName);
	virtual bool			SetPath(const char *inPath);
	virtual int				ForEachFile(const char* expression, TCallback callback, void* param);
	
private:
	void AddArchiveEntry(TArchiveEntry *ae);

	void ZIPGetLocalHeader(TLocalHeader *lh);
	void ZIPGetFileHeader(TFileHeader *fh);
	void ZIPGetCentralDirectoryEnd(TCentralDirectoryEnd *cde);
	void ZIPGetCentralDirectoryEndFirst(TCentralDirectoryEnd *cde);
	void ZIPSkipData(TLocalHeader *lh);

	//=========================================================================
	// members
	//=========================================================================
private:
	salt::StdFile						*mHandle;
	char								*mArchiveName;
	TEntryMap							mEntryMap;
};

DECLARE_CLASS(FileSystemZIP);

#endif // FILESYSTEMZIP_H__
