/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: indexbuffer.cpp,v 1.3 2003/09/09 16:04:20 rollmark Exp $

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

#include "indexbuffer.h"
#include <string.h>

using namespace oxygen;

IndexBuffer::~IndexBuffer()
{
        delete[] mIndex;
}

/*!
        This routine caches an index list for rendering at a later point in
        time. It also grows the index cache if necessary.

        \param numIndex number of indices to add to the cache
        \param index    pointer to index data
*/
void IndexBuffer::Cache(unsigned int numIndex, unsigned int *index)
{
        EnsureFit(numIndex);

        // append new index data to our cache
        memcpy(&mIndex[mNumIndex], index, numIndex*sizeof(unsigned int));
        mNumIndex += numIndex;
}

void IndexBuffer::Cache(unsigned int newIndex)
{
        Cache(1, &newIndex);
}

void IndexBuffer::EnsureFit(unsigned int count)
{
        if(mIndex == NULL)
        {
                mIndex          = new unsigned int[count];
                mMaxIndex       = count;
                mNumIndex       = 0;
        }
        else
        {
                // check if there's enough room in our index cache
                const unsigned int reqSize = mNumIndex+count;

                if(mMaxIndex < reqSize)
                {
                        // we don't have enough room ... so make room
                        unsigned int newSize = mMaxIndex*2;

                        while(newSize<reqSize)
                                newSize *= 2;

                        // allocate new memory
                        unsigned int *newIndex = new unsigned int[newSize];

                        // copy old data
                        memcpy(newIndex, mIndex, mNumIndex*sizeof(unsigned int));
                        // free old memory
                        delete[] mIndex;

                        mIndex          = newIndex;
                        mMaxIndex       = newSize;
                }
        }
}
