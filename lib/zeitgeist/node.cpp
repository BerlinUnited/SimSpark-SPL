#include <boost/tokenizer.hpp>
#include "node.h"
#include <salt/path.h>
#include <iostream>

/*
std::ostream& zeitgeist::operator<<(std::ostream& output, const Node& node)
{
	output << (int)&node << " '" << node.GetName() << "'" << std::endl;

	//for (Node::TLeafList::const_iterator i=leaf.mChildren.begin(); i!=leaf.mChildren.end(); ++i)
	//{
	//	output << *(*i);
	//}
	return output;
}*/

//=============================================================================
using namespace boost;
using namespace salt;
using namespace std;
using namespace zeitgeist;


Node::Node(const std::string& name) : Leaf(name)
{
}

Node::~Node()
{
	//cout << "~Node() '" << mName << "' " << mChildren.size() <<" -> " << (void*)this << endl;
}

boost::shared_ptr<Leaf> Node::GetChild(const std::string &name, bool recursive)
{
	shared_ptr<Leaf> leaf	= Leaf::GetChild(name, recursive);

	if (leaf.get() != NULL) return leaf;

	for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		// check if we have found a match and return it
		if ((*i)->GetName().compare(name) == 0)
			return (*i);

		if (recursive)
		{
			shared_ptr<Leaf> ret = (*i)->GetChild(name, recursive);
			if (ret.get() != NULL) return ret;
		}
	}

	return boost::shared_ptr<Leaf>();
}

boost::shared_ptr<Leaf> Node::GetChildOfClass(const std::string &name, bool recursive)
{
	for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		// check if we have found a match and add it
		shared_ptr<Class> theClass = (*i)->GetClass();
		if (theClass.get() != NULL && theClass->GetName().compare(name) == 0)
		{
			return (*i);
		}

		if (recursive)
		{
			shared_ptr<Leaf> ret = (*i)->GetChildOfClass(name, recursive);
			if (ret.get() != NULL) return ret;
		}
	}

	return shared_ptr<Leaf>();
}

boost::shared_ptr<Leaf> Node::GetChildSupportingClass(const std::string &name, bool recursive)
{
	for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		// check if we have found a match and add it
		shared_ptr<Class> theClass = (*i)->GetClass();
		if (theClass.get() != NULL && theClass->Supports(name))
		{
			return (*i);
		}

		if (recursive)
		{
			shared_ptr<Leaf> ret = (*i)->GetChildOfClass(name, recursive);
			if (ret.get() != NULL) return ret;
		}
	}

	return shared_ptr<Leaf>();
}

void Node::GetChildren(const std::string &name, TLeafList &baseList, bool recursive)
{
	Leaf::GetChildren(name, baseList, recursive);

	for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		// check if we have found a match and add it
		if ((*i)->GetName().compare(name) == 0)
		{
			baseList.push_back(*i);
		}

		if (recursive)
			(*i)->GetChildren(name, baseList, recursive);
	}
}

void Node:::GetChildrenOfClass(const std::string &name, TLeafList &baseList, bool recursive)
{
	Leaf::GetChildrenOfClass(name, baseList, recursive);

	for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		// check if we have found a match and add it
		shared_ptr<Class> theClass = (*i)->GetClass();
		if (theClass.get() != NULL && theClass->GetName().compare(name) == 0)
		{
			baseList.push_back(*i);
		}

		if (recursive)
			(*i)->GetChildrenOfClass(name, baseList, recursive);
	}
}

void Node::GetChildrenSupportingClass(const std::string &name, TLeafList &baseList, bool recursive)
{
	Leaf::GetChildrenOfClass(name, baseList, recursive);

	for (TLeafList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		// check if we have found a match and add it
		shared_ptr<Class> theClass = (*i)->GetClass();
		if (theClass.get() != NULL && theClass->Supports(name))
		{
			baseList.push_back(*i);
		}

		if (recursive)
			(*i)->GetChildrenOfClass(name, baseList, recursive);
	}
}

bool Node::IsLeaf() const
{
	return false;
}

void Node::RemoveChildReference(const boost::shared_ptr<Leaf> &leaf)
{
	mChildren.remove(leaf);
}

bool Node::AddChildReference(const boost::shared_ptr<Leaf> &leaf)
{
	mChildren.push_back(leaf);
	//cout << "Node: self " << (void*) GetSelf().get() << endl;
	leaf->SetParent(shared_static_cast<Node>(make_shared(GetSelf())));

	return true;
}

void Node::Dump() const
{
	Leaf::Dump();
	cout << "Node: numChildren = " << mChildren.size() << endl;

	for (TLeafList::const_iterator i = mChildren.begin(); i!=mChildren.end(); ++i)
	{
		(*i)->Dump();
		cout << endl;
	}

	cout << "End Node" << endl;
}

Leaf::TLeafList::iterator Node::begin()
{
	return mChildren.begin();
}

Leaf::TLeafList::const_iterator Node::begin() const
{
	return mChildren.begin();
}

Leaf::TLeafList::iterator Node::end()
{
	return mChildren.end();
}

Leaf::TLeafList::const_iterator Node::end() const
{
	return mChildren.end();
}
