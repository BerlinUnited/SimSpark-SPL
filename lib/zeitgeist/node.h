#ifndef NODE_H__
#define NODE_H__

/*! \class Node
	$Id: node.h,v 1.1 2003/04/30 11:29:35 fruit Exp $
	
	Node
	:TODO: Class description for Node

	HISTORY:
		15.06.2002 MK
			- initial version
		02.09.2002 MK
			- removed quad-link
*/

#include <salt/defines.h>
#include "base.h"

namespace zeitgeist
{

class Node : public Base
{
	//
	// functions
	//
public:
	Node(const std::string &name = "<Unnamed>");
	virtual ~Node();

	virtual void Construct(const boost::shared_ptr<Object>& self, const boost::shared_ptr<Class>& creator);

	// hierarchy
	virtual boost::shared_ptr<Base>	GetChild(const std::string &name, bool recursive = false);
	virtual bool					IsLeaf() const;
	virtual void RemoveChildReference(const boost::shared_ptr<Base> &base);
	virtual bool AddChildReference(const boost::shared_ptr<Base> &base);

	// hierarchy navigation
	virtual void	Dump() const;
	
	// iterator helpers
	virtual TBaseList::iterator begin();
	virtual TBaseList::const_iterator begin() const;
	virtual TBaseList::iterator end();
	virtual TBaseList::const_iterator end() const;
private:
	Node(const Node &obj);
	Node& operator=(const Node &obj);

	//
	// members
	//
protected:
	// object hierarchy related stuff
	TBaseList		mChildren;
};

DECLARE_CLASS(Node);

} //namespace zeitgeist;

#endif //NODE_H__
