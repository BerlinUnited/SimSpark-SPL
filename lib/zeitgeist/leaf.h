#ifndef LEAF_H__
#define LEAF_H__

/*! \class Leaf
	$Id: leaf.h,v 1.1 2003/04/30 14:21:50 fruit Exp $
	
	Leaf
	
	Leaf defines the beginning of the hierarchy. A Leaf object can reside
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
		22.02.2003 MK
			- renamed to Leaf

*/

#include <list>
#include <string>
#include "object.h"

namespace zeitgeist
{

class Leaf : public Object
{
	friend class Node;
	//
	// types
	//
public:
	typedef std::list< boost::shared_ptr<Leaf> >	TLeafList;

	//
	// functions
	//
public:
	Leaf(const std::string &name = "<unnamed>");
	virtual ~Leaf();

	std::string&		GetName()			{	return mName;	}
	const std::string&	GetName() const		{	return mName;	}

	// hierarchy
	boost::weak_ptr<Node>&			GetParent();
	const boost::weak_ptr<Node>&	GetParent() const;
	virtual boost::shared_ptr<Leaf>	GetChild(const std::string &name, bool recursive = false);
	virtual boost::shared_ptr<Leaf>	GetChildOfClass(const std::string &name, bool recursive = false);
	virtual boost::shared_ptr<Leaf>	GetChildSupportingClass(const std::string &name, bool recursive = false);
	virtual void					GetChildren(const std::string &name, TLeafList &baseList, bool recursive = false);
	virtual void					GetChildrenOfClass(const std::string &name, TLeafList &baseList, bool recursive = false);
	virtual void					GetChildrenSupportingClass(const std::string &name, TLeafList &baseList, bool recursive = false);
	virtual bool					IsLeaf() const;

	virtual void	RemoveChildReference(const boost::shared_ptr<Leaf> &base);
	virtual bool	AddChildReference(const boost::shared_ptr<Leaf> &base);

	void			Unlink();

	virtual	void	Dump() const;

	//virtual TLink::Iterator	GetIterator();

	const std::string&	GetFullPath() const;

	void				ClearCachedData() const;

	void	SetName(const std::string &name)	{	mName = name;	}

	// iterator helpers
	virtual TLeafList::iterator begin();
	virtual TLeafList::const_iterator begin() const;
	virtual TLeafList::iterator end();
	virtual TLeafList::const_iterator end() const;
protected:
	/*! This routine has to be implemented 'very carefully'. The parent object
		always holds a shared pointer reference to mSelf. What we have to do is
		'get' the shared reference, remove it from the old parent. Insert it
		into the new parent and change the parent pointer.
	*/
	void	SetParent(const boost::shared_ptr<Node> &parent);
	/*! This rountine is called, when the hierarchy object has been linked to
		a parent. At that point, traversal can commence. It can be overridden
		to support custom 'link' behavior.
	*/
	virtual void OnLink();
	/*! This rountine is called, before the hierarchy object is removed from the
		parent. It can be overridden to support custom 'link' behavior.
	*/
	virtual void OnUnlink();

private:
	Leaf(const Leaf &obj);
	Leaf& operator=(const Leaf &obj);

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

DECLARE_CLASS(Leaf);

}

#endif //LEAF_H__
