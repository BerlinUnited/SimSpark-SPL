/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: filesystem.h,v 1.4 2003/08/21 12:53:30 rollmark Exp $

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

#ifndef FILESYSTEM_H__
#define FILESYSTEM_H__

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <salt/fileclasses.h>


namespace zeitgeist
{
  /** 	This class defines the interface which derived filesystems
      	must implement in order to be used with the fileserver.
   */
class FileSystem : public Leaf
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
	FileSystem() : Leaf()	{}
	virtual ~FileSystem() {}

	/** tries to open the file named inName. Returns an instance
	    of a salt::RFile on success, NULL otherwise */
	virtual salt::RFile*	Open(const char* inName) = 0;

	/** sets the path all calls to Open are relative to. For a
	    standard file system this call maps directly to a
	    directory. For Filesystems providing access to an archive
	    it is used to select the archive, i.e. it is the filename
	    of an archive. Please refer to concrete Filesystems for an
	    example implementation. */
	virtual bool	SetPath(const char* inPath) = 0;


	/** iterates over all files managed by this filesystem.
	  * 'directory', 'name' and 'extension' give directory, name
	  * and extension a file must match. directory,name and
	  * extension may be NULL, in wich case every
	  * directory,extension and/or name matches. For each match
	  * the function callback is called with the name of the
	  * matched file and the additional user parameter
	  * 'param'. param is just passed through to the callback and
	  * has no meaning to the filesystem.
	  */
	virtual int ForEachFile(const char* expression, TCallback callback, void* param) = 0;

private:
	FileSystem(const FileSystem& obj);
	FileSystem& operator=(const FileSystem& obj);
};

DECLARE_ABSTRACTCLASS(FileSystem);

} //namespace zeitgeist

#endif //FILESYSTEM_H__
