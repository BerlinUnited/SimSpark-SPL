#ifndef NODE_H__
#define NODE_H__

/*! \class Node
	$Id: node.h,v 1.2 2003/04/30 14:21:50 fruit Exp $
	
	Node
	:TODO: Class description for Node

	HISTORY:
		15.06.2002 MK
			- initial version
		02.09.2002 MK
			- removed quad-link
*/

#include <salt/defines.h>
#include "leaf.h"

namespace zeitgeist
{

class Node : public Leaf
{
	//
	// functions
	//
public:
	Node(const std::string &name = "<Unnamed>");
	virtual ~Node();

	// hierarchy
	virtual boost::shared_ptr<Leaf>	GetChild(const std::string &name, bool recursive = false);
	virtual boost::shared_ptr<Leaf>	GetChildOfClass(const std::string &name, bool recursive = false);
	virtual boost::shared_ptr<Leaf>	GetChildSupportingClass(const std::string &name, bool recursive = false);
	virtual void					GetChildren(const std::string &name, TLeafList &baseList, bool recursive = false);
	virtual void					GetChildrenOfClass(const std::string &name, TLeafList &baseList, bool recursive = false);
	virtual void					GetChildrenSupportingClass(const std::string &name, TLeafList &baseList, bool recursive = false);
	virtual bool					IsLeaf() const;
	virtual void RemoveChildReference(const boost::shared_ptr<Leaf> &base);
	virtual bool AddChildReference(const boost::shared_ptr<Leaf> &base);

	// hierarchy navigation
	virtual void	Dump() const;
	
	// iterator helpers
	virtual TLeafList::iterator begin();
	virtual TLeafList::const_iterator begin() const;
	virtual TLeafList::iterator end();
	virtual TLeafList::const_iterator end() const;
private:
	Node(const Node &obj);
	Node& operator=(const Node &obj);

	//
	// members
	//
protected:
	// object hierarchy related stuff
	TLeafList		mChildren;
};

DECLARE_CLASS(Node);

} //namespace zeitgeist;

#endif //NODE_H__
