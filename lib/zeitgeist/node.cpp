#include <boost/tokenizer.hpp>
#include "node.h"
#include <salt/path.h>
#include <iostream>

/*
std::ostream& zeitgeist::operator<<(std::ostream& output, const Node& node)
{
	output << (int)&node << " '" << node.GetName() << "'" << std::endl;

	//for (Node::TBaseList::const_iterator i=base.mChildren.begin(); i!=base.mChildren.end(); ++i)
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


Node::Node(const std::string& name) : Base(name)
{
}

Node::~Node()
{
	cout << "~Node() '" << mName << "' " << mChildren.size() <<" -> " << (void*)this << endl;
}

void Node::Construct(const boost::shared_ptr<Object>& self, const boost::shared_ptr<Class>& creator)
{
	Base::Construct(self, creator);
}

boost::shared_ptr<Base> Node::GetChild(const std::string &name, bool recursive)
{
	shared_ptr<Base> base	= Base::GetChild(name, recursive);

	if (base.get() != NULL) return base;

	for (TBaseList::iterator i = mChildren.begin(); i != mChildren.end(); ++i)
	{
		// check if we have found a match and return it
		if ((*i)->GetName().compare(name) == 0)
			return (*i);

		if (recursive)
			return (*i)->GetChild(name, recursive);
	}

	return boost::shared_ptr<Base>();
}

bool Node::IsLeaf() const
{
	return false;
}

void Node::RemoveChildReference(const boost::shared_ptr<Base> &base)
{
	mChildren.remove(base);
}

bool Node::AddChildReference(const boost::shared_ptr<Base> &base)
{
	mChildren.push_back(base);
	//cout << "Node: self " << (void*) GetSelf().get() << endl;
	base->SetParent(shared_static_cast<Node>(make_shared(GetSelf())));

	return true;
}

void Node::Dump() const
{
	Base::Dump();
	cout << "Node: numChildren = " << mChildren.size() << endl;

	for (TBaseList::const_iterator i = mChildren.begin(); i!=mChildren.end(); ++i)
	{
		(*i)->Dump();
		cout << endl;
	}

	cout << "End Node" << endl;
}

Base::TBaseList::iterator Node::begin()
{
	return mChildren.begin();
}

Base::TBaseList::const_iterator Node::begin() const
{
	return mChildren.begin();
}

Base::TBaseList::iterator Node::end()
{
	return mChildren.end();
}

Base::TBaseList::const_iterator Node::end() const
{
	return mChildren.end();
}

/*void Node::Status()
{
	Base::Status();

	for (Iterator i = GetIterator(); !i.Done(); i.Next())
	{
		Base *base = i.Current();
		cout << base->GetName();
		if (!base->IsLeaf())
		{
			cout << "/";
		}
		cout << endl;
	}
}*/

/*
Base::TLink::Iterator	Node::GetIterator()
{
	TLink::Iterator i(&mChildLink);
	return i;
}

*/