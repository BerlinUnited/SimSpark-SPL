#ifndef BASE_H__
#define BASE_H__

/*! \class Base
	$Id: base.h,v 1.1 2003/04/30 11:29:35 fruit Exp $
	
	Base
	
	Base defines the beginning of the hierarchy. A Base object can reside
	within the hierarchy, but cannot have children!

	HISTORY:
		04.06.2002 MK
			- initial version
		13.06.2002 MK
			- reworked Base to use a quad-linked tree
		15.06.2002 MK
			- split Base up into Base and Node classes
		02.09.2002 MK
			- removed quad-link

*/

#include <list>
#include <string>
#include "object.h"

namespace zeitgeist
{

class Base : public Object
{
	friend class Node;
	//
	// types
	//
public:
	typedef std::list< boost::shared_ptr<Base> >	TBaseList;

	//
	// functions
	//
public:
	Base(const std::string &name = "<unnamed>");
	virtual ~Base();

	virtual void Construct(const boost::shared_ptr<Object>& self, const boost::shared_ptr<Class>& creator);

	std::string&		GetName()			{	return mName;	}
	const std::string&	GetName() const		{	return mName;	}

	// hierarchy
	boost::weak_ptr<Node>&			GetParent();
	const boost::weak_ptr<Node>&	GetParent() const;
	virtual boost::shared_ptr<Base>	GetChild(const std::string &name, bool recursive = false);
	virtual bool					IsLeaf() const;

	virtual void	RemoveChildReference(const boost::shared_ptr<Base> &base);
	virtual bool	AddChildReference(const boost::shared_ptr<Base> &base);

	void			Unlink();

	virtual	void	Dump() const;

	//virtual TLink::Iterator	GetIterator();

	const std::string&	GetFullPath() const;

	void				ClearCachedData() const;

	void	SetName(const std::string &name)	{	mName = name;	}

	// iterator helpers
	virtual TBaseList::iterator begin();
	virtual TBaseList::const_iterator begin() const;
	virtual TBaseList::iterator end();
	virtual TBaseList::const_iterator end() const;
protected:
	/*! This routine has to be implemented 'very carefully'. The parent object
		always holds a shared pointer reference to mSelf. What we have to do is
		'get' the shared reference, remove it from the old parent. Insert it
		into the new parent and change the parent pointer.
	*/
	void	SetParent(const boost::shared_ptr<Node> &parent);

private:
	Base(const Base &obj);
	Base& operator=(const Base &obj);

	//
	// members
	//
protected:
	/*!	This smart pointer holds a link to the parent of this node. It has to
		be at least a Node, as that is the first class, which can hold children.
		We use a weak pointer to break the cyclic dependency. */
	boost::weak_ptr<Node>	mParent;

private:
	//! the name of the object
	std::string	mName;

	//! temporary cached data
	mutable std::string *mCachedFullPath;
};

} //namespace zeitgeist;

#include "class.h"

namespace zeitgeist
{

DECLARE_CLASS(Base);

}

#endif //BASE_H__
