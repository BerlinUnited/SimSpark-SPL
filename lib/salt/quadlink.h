#ifndef QUADLINK_H__
#define QUADLINK_H__

/*! \class QuadLink
	$Id: quadlink.h,v 1.2 2003/04/30 14:21:49 fruit Exp $
	
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

#include <boost/weak_ptr.hpp>

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
