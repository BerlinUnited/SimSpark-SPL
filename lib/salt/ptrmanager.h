/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: ptrmanager.h,v 1.2 2003/05/19 21:32:39 fruit Exp $

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
#ifndef PTRMANAGER_H__
#define PTRMANAGER_H__

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ptr.h"
#include <vector>

namespace salt
{

template <typename T>
class PtrManager
{
private:
	struct HandleEntry
	{
		T				*data;	// the actual pointer we are working with
		unsigned int	salt;	// the salt associated with the entry
	};
	//! an array of handles, we are actually managing
	typedef std::vector<HandleEntry>	TDataVec;
	//! the type of the free store
	typedef std::vector<unsigned int>	TFreeVec;

public:
	PtrManager();
	/*! Here we create a Ptr<T>, which is bound to the PtrManager<T> instance.
	*/
	Ptr<T>	CreatePointer();
	/*! Assign a regular pointer to a Ptr<T>. This registers the regular pointer
		with the PtrManager.
	*/
	void	Assign(Ptr<T> &handle, T *value);
	/*!	Look up a managed pointer using a Ptr<T>
	*/
	T*		Lookup(const Ptr<T> &handle) const;
	void	Release(Ptr<T> &handle);
	/*! Validate that a Ptr<T> points to the correct object. This can detect
		cases, where the pointer has been deleted by other Ptr<T>s.
	*/
	bool	Validate(const Ptr<T> &handle) const;

	// stats
	unsigned int	GetActivePtrCount() const;
private:
	//! the actual pointers and salt numbers
	TDataVec		mData;
	//! a vector of free stores
	TFreeVec		mFreeStore;
	//! our 'salt generator'
	unsigned int	mCurrentSalt;
};

template <typename T>
PtrManager<T>::PtrManager() :
mCurrentSalt(0)
{
}

template <typename T>
Ptr<T> PtrManager<T>::CreatePointer()
{
	Ptr<T> blah(*this);
	return blah;
}

template <typename T>
void PtrManager<T>::Assign(Ptr<T> &handle, T *value)
{
	// wrap around salt and skip 0
	if (++mCurrentSalt > Ptr<T>::eMaxSalt) mCurrentSalt = 1;

	unsigned int index;
	if (mFreeStore.empty())
	{
		// no indices to reuse, so create a new one
		index = mData.size();
		HandleEntry entry;
		entry.data	= value;
		entry.salt	= mCurrentSalt;
		handle.Init(index, mCurrentSalt);
		mData.push_back(entry);
	}
	else
	{
		// we have a free index
		index = mFreeStore.back();
		mFreeStore.pop_back();
		handle.Init(index, mCurrentSalt);
		mData[index].data	= value;
		mData[index].salt	= mCurrentSalt;
	}
}

template <typename T>
T* PtrManager<T>::Lookup(const Ptr<T> &handle) const
{
	unsigned int index = handle.GetIndex();

	// do we have an addressable index
	if (mData.size() > index)
	{
		// now, only when the salt numbers match, the index is valid
		if (mData[index].salt == handle.GetSalt())
		{
			return mData[index].data;
		}
	}

	return NULL;
}

template <typename T>
void PtrManager<T>::Release(Ptr<T> &handle)
{
	assert (handle.IsValid());
	
	unsigned int index = handle.GetIndex();
	// this invalidates all the handles referring to this index
	mData[index].salt = 0;
	// delete the object the handle referred to
	delete mData[index].data;
	// place the index on the free store, so it will be reused
	mFreeStore.push_back(index);
}

template <typename T>
bool PtrManager<T>::Validate(const Ptr<T> &handle) const
{
	unsigned int index = handle.GetIndex();

	// do we have an addressable index
	if (mData.size() > index)
	{
		// now, only when the salt numbers match, the index is valid
		if (mData[index].salt == handle.GetSalt())
		{
			return true;
		}
	}
	return false;
}

template <typename T>
unsigned int PtrManager<T>::GetActivePtrCount() const
{
	return (mData.size() - mFreeStore.size());
}

}//namespace salt

#endif //PTRMANAGER_H__
