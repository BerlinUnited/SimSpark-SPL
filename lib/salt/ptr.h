#ifndef PTR_H__
#define PTR_H__

#include <cassert>

namespace salt
{

template <typename TYPE>
class PtrManager;

// TYPE is the managed resource by this handle. 
template <typename T>
class Ptr
{
	friend class PtrManager<T>;
public:
	enum
	{
		eMaxBitsIndex	= 16,
		eMaxBitsSalt	= 16,
		eMaxIndex	= (1 << eMaxBitsIndex) - 1,
		eMaxSalt	= (1 << eMaxBitsSalt)  - 1,
	};

public:
	/*! The constructor can only be invoked with a reference to a PtrManager.
		This ensures that mManager is always in a good state (unless you
		manually kill the manager, which would be 'not good'.
	*/
	Ptr(PtrManager<T> *mgr = NULL) : mManager(mgr), mHandle(0)	{}

	unsigned int	GetIndex() const	{	return mIndex;		}
	unsigned int	GetSalt() const		{	return mSalt;		}
	unsigned int	GetHandle() const	{	return mHandle;		}
	bool			IsNull() const		{	return (!mHandle);	}
	bool			IsValid() const		{	return mManager.Validate(*this);	}
	
	void			Release()			{	mManager.Release(*this);	}

	operator unsigned int () const	{	return mHandle;	}
	
	/*!	This operator allows 'natural' pointer like semantics to be used with
		Ptr<T>. It is internally rerouted to the PtrManager to perform the
		lookup of the handle.
	*/
	T & operator*() const; // never throws
	/*!	This operator allows 'natural' pointer like semantics to be used with
		Ptr<T>. It is internally rerouted to the PtrManager to perform the
		lookup of the handle.
	*/
	T * operator->() const; // never throws
	/*!	This operator allows assignment of POPs (plain old pointers) to a Ptr<T>.
		This call is internally rerouted to the PtrManager associated with the
		handle.
	*/
	Ptr<T>& operator=(T* rhs);
private:
	/*!	This function is used by PtrManager::Assign() to initialize the Ptr<T>.
	*/
	void Init(unsigned int index, unsigned int salt);

private:
	//! the union makes it easier to access the index and salt part of the handle
	union
	{
		struct
		{
			unsigned int mIndex	: eMaxBitsIndex;
			unsigned int mSalt	: eMaxBitsSalt;
		};
		unsigned int mHandle;
	};

	PtrManager<T>	*mManager;
};

template <typename T>
void Ptr<T>::Init(unsigned int index, unsigned int salt)
{
	assert(IsNull());				// no reassignment

	mIndex	= index;
	mSalt	= salt;
}

template <typename T>
inline bool operator != (Ptr<T> l, Ptr<T> r)
{
	return (l.GetHandle() != r.GetHandle());
}

template <typename T>
inline bool operator == (Ptr<T> l, Ptr<T> r)
{
	return (l.GetHandle() == r.GetHandle());
}

template <typename T>
T & Ptr<T>::operator*() const
{
	assert (mManager);

	return *mManager->Lookup(*this);
}

template <typename T>
T * Ptr<T>::operator->() const
{
	assert (mManager);

	if (mManager)
		return mManager->Lookup(*this);
	else
		return NULL;
}

template <typename T>
inline Ptr<T>& Ptr<T>::operator=(T* rhs)
{
	assert (mManager);
	mManager->Assign(*this, rhs);
	return *this;
}

} // namespace salt

#endif //PTR_H__
