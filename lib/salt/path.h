/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: path.h,v 1.3 2003/05/19 21:32:40 fruit Exp $

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
#ifndef PATH_H__
#define PATH_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <list>

/*! \class Path
	$Id: path.h,v 1.3 2003/05/19 21:32:40 fruit Exp $
	
	Path

	This class serves as a helper for paths within the object hierarchy. It is
	capable of cleaning paths and separating a path into path components.

	HISTORY:
		13.06.2002 MK
			- initial version
		31.08.2002 MK
			- moved to salt

*/

namespace salt
{

class Path
{
	//
	// types
	//
public:
	typedef std::list<std::string>	TStringList;

	//
	// functions
	//
public:
	Path(const std::string &path = "");

	void Set(const std::string &path);

	std::string GetCleanPath(const std::string &sep = "/") const;

	bool IsAbsolute() const;
	bool IsEmpty() const;

	const std::string& Front() const;
	void PopFront();

	const std::string& Back() const;
	void PopBack();
	
private:
	Path(const Path &obj);
	Path& operator=(const Path &obj);

	void Tokenize(const std::string &path);

	//
	// members
	//
public:
protected:
private:
	TStringList mPathComponents;
	bool		mIsAbsolute;
};

} //namespace salt

#endif //PATH_H__
