#include "class.h"
#include "node.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace zeitgeist;

Leaf::Leaf(const std::string &name) : mName(name), mCachedFullPath(NULL)
{
}

Leaf::~Leaf()
{
	//cout << "~Leaf() " << mName << " -> " << (void*)this << endl;
}

boost::weak_ptr<Node>& Leaf::GetParent()
{
	return mParent;	
}

const boost::weak_ptr<Node>& Leaf::GetParent() const
{
	return mParent;	
}

boost::shared_ptr<Leaf> Leaf::GetChild(const std::string &name, bool recursive)
{
	if (name.compare("..") == 0)
	{
		return make_shared(GetParent());
	}
	
	if (name.compare(".") == 0)
	{

		return shared_static_cast<Leaf>(make_shared(GetSelf()));
	}
	
	return boost::shared_ptr<Leaf>();
}

boost::shared_ptr<Leaf> Leaf::GetChildOfClass(const std::string &name, bool recursive)
{
	return boost::shared_ptr<Leaf>();
}

boost::shared_ptr<Leaf> Leaf::GetChildSupportingClass(const std::string &name, bool recursive)
{
	return boost::shared_ptr<Leaf>();
}

void Leaf::GetChildren(const std::string &name, TLeafList &baseList, bool recursive)
{
	if (name.compare("..") == 0)
	{
		baseList.push_back(make_shared(GetParent()));
	}
	
	if (name.compare(".") == 0)
	{
		baseList.push_back(shared_static_cast<Leaf>(make_shared(GetSelf())));
	}
}

void Leaf::GetChildrenOfClass(const std::string &name, TLeafList &baseList, bool recursive)
{
}

void Leaf::GetChildrenSupportingClass(const std::string &name, TLeafList &baseList, bool recursive)
{
}

bool Leaf::IsLeaf() const
{
	return true;
}

void Leaf::RemoveChildReference(const boost::shared_ptr<Leaf> &base)
{
}

bool Leaf::AddChildReference(const boost::shared_ptr<Leaf> &base)
{
	return false;
}

void Leaf::Unlink()
{
	// give us a chance to update our state
	OnUnlink();

	// here we lose our reference to the parent
	SetParent(boost::shared_ptr<Node>());

	ClearCachedData();
}

void Leaf::Dump() const
{
	Object::Dump();
	cout << "Leaf: '" << GetName() << "'" << endl;
}

const std::string& Leaf::GetFullPath() const
{
	//printf("this:getfullpath %x - %s - %x %x\n", this, GetName().c_str(), GetParent(), mCachedFullPath);
	if (mCachedFullPath == NULL)
	{
		std::string parentPath;

		if (GetParent().get() != NULL)
		{
			shared_ptr<Leaf> blah = make_shared(GetParent());
			parentPath = blah->GetFullPath();
		}

		// no cached data available
		if (IsLeaf())
			mCachedFullPath = new std::string(parentPath + mName);
		else
			mCachedFullPath = new std::string(parentPath + mName + "/");
	}

	return *mCachedFullPath;
}

void Leaf::ClearCachedData() const
{
	delete mCachedFullPath;
	mCachedFullPath = NULL;
}

Leaf::TLeafList gFakeChildren;

Leaf::TLeafList::iterator Leaf::begin()
{
	return gFakeChildren.begin();
}

Leaf::TLeafList::const_iterator Leaf::begin() const
{
	return gFakeChildren.begin();
}

Leaf::TLeafList::iterator Leaf::end()
{
	return gFakeChildren.end();
}

Leaf::TLeafList::const_iterator Leaf::end() const
{
	return gFakeChildren.end();
}

void Leaf::SetParent(const boost::shared_ptr<Node> &newParent)
{
	if (shared_ptr<Node> oldParent = make_shared(GetParent()))
	{
		// we have a parent, so update our state
		shared_ptr<Leaf> self = shared_static_cast<Leaf>(make_shared(GetSelf()));
		// here reference count should be > 1 (at least one in the parent, and one in this routine)
		//cout << "Have parent - use count " << self.use_count() << "\n";
		assert(self.use_count() > 1);
		// we remove ourself from the old parent's list of children
		oldParent->RemoveChildReference(self);
		//cout << "Have parent2 - use count " << self.use_count() << "\n";
		// we add ourself to the new parent's list of children
		if (newParent)
			newParent->AddChildReference(self);
		
		//cout << "Have parent3 - use count " << self.use_count() << "\n";
	}
	
	mParent = newParent;

	//cout << "set parent " << (void*)GetParent().get() << "\n";

	// we have been linked, so now we can do something :)
	OnLink();
}

void Leaf::OnLink()
{
}

void Leaf::OnUnlink()
{
}
