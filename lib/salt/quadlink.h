/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: quadlink.h,v 1.3 2003/05/19 21:32:39 fruit Exp $

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
#ifndef QUADLINK_H__
#define QUADLINK_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <boost/weak_ptr.hpp>

/*! \class QuadLink
	$Id: quadlink.h,v 1.3 2003/05/19 21:32:39 fruit Exp $
	
	QuadLink
	:TODO: Class description for QuadLink

	HISTORY:
		15.06.2002 MK
			- initial version
		12.08.2002 MK
			- moved to salt
		01.09.2002 MK
			- using boost::weak_ptr
*/

namespace salt
{

template <class T>
class QuadLink
{
	//
	// types
	//
public:
	class Iterator
	{
	public:
		Iterator(QuadLink<T> *link = NULL) : mLink(link)	{ if (mLink) mLink = mLink->GetNext(); }
		void Next()	{	if (mLink) mLink = mLink->GetNext();	}
		bool Done()	{	if (mLink == NULL) return true; else return (mLink->GetChild().get()==NULL);	}
		boost::shared_ptr<T> Current()	{	if (mLink) return boost::make_shared(mLink->GetChild()); return boost::shared_ptr<T>();	}
	private:
		QuadLink<T>	*mLink;
	};

	//
	// functions
	//
public:
	QuadLink() {	mNext = this; mPrevious = this;	}

	void SetChild(const boost::weak_ptr<T> &node)	{	mChild	= node;	}
	void SetParent(const boost::weak_ptr<T> &node)	{	mParent	= node;	}

	boost::weak_ptr<T>& GetChild() 					{	return mChild;	}
	const boost::weak_ptr<T>& GetChild() const		{	return mChild;	}

	boost::weak_ptr<T>& GetParent()					{	return mParent;	}
	const boost::weak_ptr<T>& GetParent() const		{	return mParent;	}

	QuadLink* GetNext() const		{	return mNext;		}
	QuadLink* GetPrevious() const	{	return mPrevious;	}

	void Link(QuadLink *link)
	{
		link->mPrevious = this;
		link->mNext = mNext;
		mNext = link;
		link->mNext->mPrevious = link;
	}
	
	void Unlink() const
	{
		mPrevious->mNext = mNext;
		mNext->mPrevious = mPrevious;
	}
private:
	QuadLink(const QuadLink& obj);
	QuadLink& operator=(const QuadLink& obj);

	//
	// members
	//
private:
	QuadLink			*mNext;
	QuadLink			*mPrevious;
	boost::weak_ptr<T>	mChild;
	boost::weak_ptr<T>	mParent;
};

} //namespace salt

#endif //QUADLINK_H__
