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
#ifndef OXYGEN_INDEXBUFFER_H
#define OXYGEN_INDEXBUFFER_H

#include <oxygen/oxygen_defines.h>
#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif

namespace oxygen
{

/** IndexBuffer is intended as a cache for indeces pointing into a
    vertexbuffer to describe a set of triangles. It is used together
    with the StaticMesh class
 */
class OXYGEN_API IndexBuffer
{
public:
    IndexBuffer();
    virtual ~IndexBuffer();

    /** caches (i.e. copies) an index list for rendering at a later
        point in time, growing the index cache if necessary.

        \param numIndex number of indices to add to the cache
        \param index    pointer to index data
    */
    void Cache(unsigned int numIndex, unsigned int *index);

    /** appends a single index */
    void Cache(unsigned int newIndex);

    /** empties the index buffer */
    void Flush();

    /** returns the number of cached indeces */
    int GetNumIndex() const;

    /** returns a cached index */
    boost::shared_array<unsigned int> GetIndex() const;

    /** ensures that the index buffer can hold the additional (!)
        number of elements (creates new storage on demand, copys old
        data)
    */
    void EnsureFit(unsigned int count);

protected:
    /** the maximum number of indices we can cache with the allocated
        memory */
    unsigned int mMaxIndex;

    /** the number of indices currently cached */
    unsigned int mNumIndex;

    /** pointer to the memory, where we cache the indices */
    boost::shared_array<unsigned int> mIndex;
};

}
#endif //OXYGEN_INDEXBUFFER_H
