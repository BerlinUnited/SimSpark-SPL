#include "indexbuffer.h"
#include <stdio.h>

IndexBuffer::~IndexBuffer()
{
	delete[] mIndex;
}

/*!
	This routine caches an index list for rendering at a later point in
	time. It also grows the index cache if necessary.

	\param numIndex	number of indices to add to the cache
	\param index	pointer to index data
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

void IndexBuffer::Render()
{	
	if(mNumIndex>=0)
		glDrawElements(GL_TRIANGLES, mNumIndex, GL_UNSIGNED_INT, mIndex);
}

void IndexBuffer::EnsureFit(unsigned int count)
{
	if(mIndex == NULL)
	{
		mIndex		= new unsigned int[count];
		mMaxIndex	= count;
		mNumIndex	= 0;
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

			mIndex		= newIndex;
			mMaxIndex	= newSize;
		}
	}	
}