/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: indexbuffer.h,v 1.2 2003/08/31 13:08:42 rollmark Exp $

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

#ifndef INDEXBUFFER_H__
#define INDEXBUFFER_H__

#include "../openglserver/openglserver.h"

/** IndexBuffer inteded as a cache for indeces pointing into a vertexbuffer to
 * describe a set of triangles. It is used together with the StaticMesh class
 */
class IndexBuffer
{
public:
        IndexBuffer() : mMaxIndex(0), mNumIndex(0), mIndex(0)   {}
        virtual ~IndexBuffer();

        /** caches indices in indexbuffer */
        void    Cache(unsigned int numIndex, unsigned int *index);

        /** appends a single index */
        void    Cache(unsigned int newIndex);

        /** empties the index buffer */
        void    Flush()                         {       mNumIndex = 0;  }

        /** renders indices in the indexbuffer with the current GL state */
        void    Render();

        /** returns the number of cached indeces */
        int GetNumIndex() const                 {       return mNumIndex;       }

        /** returns a cached index */
        unsigned int* GetIndex() const  {       return mIndex;          }

protected:

        /** ensures that the index buffer can hold the additional (!) number
            of elements (creates new storage on demand, copys old data)
        */
        void    EnsureFit(unsigned int count);

        /** Maximum number of indices we can cache with the allocated memory */
        unsigned int    mMaxIndex;

        /** Number of indices currently cached */
        unsigned int    mNumIndex;

        /** Pointer to the memory, where we cache the indices */
        unsigned int    *mIndex;
};
#endif //INDEXBUFFER_H__
