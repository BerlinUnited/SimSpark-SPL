#ifndef INDEXBUFFER_H__
#define INDEXBUFFER_H__

#include "../openglserver/openglserver.h"

class IndexBuffer
{
public:
	IndexBuffer() : mMaxIndex(0), mNumIndex(0), mIndex(0)	{}
	virtual ~IndexBuffer();

	//! Cache indices in indexbuffer
	void	Cache(unsigned int numIndex, unsigned int *index);

	//! Append a single index
	void	Cache(unsigned int newIndex);

	void	Flush()				{	mNumIndex = 0;	}

	//! Render indices in indexbuffer with the current GL state
	void	Render();

	int GetNumIndex() const			{	return mNumIndex;	}
	unsigned int* GetIndex() const	{	return mIndex;		}

protected:
	//! Ensures that the index buffer can hold the additional (!) number of elements (creates new storage on demand, copys old data)
	void	EnsureFit(unsigned int count);

	//! Maximum number of indices we can cache with the allocated memory
	unsigned int	mMaxIndex;
	//! Number of indices currently cached
	unsigned int	mNumIndex;
	//! Pointer to the memory, where we cache the indices
	unsigned int	*mIndex;
};
#endif //INDEXBUFFER_H__