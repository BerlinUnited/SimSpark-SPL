#include "base.h"
#include "class.h"
#include "node.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace zeitgeist;

Base::Base(const std::string &name) : mName(name), mCachedFullPath(NULL)
{
}

Base::~Base()
{
	cout << "~Base() " << mName << " -> " << (void*)this << endl;
}

void Base::Construct(const boost::shared_ptr<Object>& self, const boost::shared_ptr<Class>& creator)
{
	Object::Construct(self, creator);
}

void Base::SetParent(const boost::shared_ptr<Node> &newParent)
{
	if (shared_ptr<Node> oldParent = make_shared(GetParent()))
	{
		// we have a parent, so update our state
                // changed "Base" to "Leaf" -- 17.04.03 Oliver
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
}

boost::weak_ptr<Node>& Base::GetParent()
{
	return mParent;	
}

const boost::weak_ptr<Node>& Base::GetParent() const
{
	return mParent;	
}

boost::shared_ptr<Base> Base::GetChild(const std::string &name, bool recursive)
{
	if (name.compare("..") == 0)
	{
            // added shared_dynamic_cast<Base> 17.04.03 Oliver
		return shared_dynamic_cast<Base>(make_shared(GetParent()));
	}
	
	if (name.compare(".") == 0)
	{

		return shared_static_cast<Base>(make_shared(GetSelf()));
	}
	
	return boost::shared_ptr<Base>();
}

bool Base::IsLeaf() const
{
	return true;
}

void Base::RemoveChildReference(const boost::shared_ptr<Base> &base)
{
}

bool Base::AddChildReference(const boost::shared_ptr<Base> &base)
{
	return false;
}

void Base::Unlink()
{
	// here we lose our reference to the parent
	SetParent(boost::shared_ptr<Node>());

	ClearCachedData();
}

void Base::Dump() const
{
	Object::Dump();
	cout << "Base: '" << GetName() << "'" << endl;
}

/*void Base::Status()
{
	cout << "Name: '" << mName << "'" << endl;
	cout << "FullPath: '" << GetFullPath() << "'" << endl;
}*/

/*void Base::Invoke(const std::string &functionName)
{
	Class::TParameterList in;

	Class::TCmdProc cmd = mClass->GetCmdProc(functionName);

	if (cmd != NULL)
		cmd(this, in);
}

Base* Base::OnSelect()
{
	return this;
}*/

const std::string& Base::GetFullPath() const
{
	//printf("this:getfullpath %x - %s - %x %x\n", this, GetName().c_str(), GetParent(), mCachedFullPath);
	if (mCachedFullPath == NULL)
	{
		std::string parentPath;

		if (GetParent().get() != NULL)
		{
                    // added shared_dynamic_cast<Base>() -- 17.04.03 Oliver
			shared_ptr<Base> blah = shared_dynamic_cast<Base>(make_shared(GetParent()));
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

void Base::ClearCachedData() const
{
	delete mCachedFullPath;
	mCachedFullPath = NULL;
}

Base::TBaseList gFakeChildren;

Base::TBaseList::iterator Base::begin()
{
	return gFakeChildren.begin();
}

Base::TBaseList::const_iterator Base::begin() const
{
	return gFakeChildren.begin();
}

Base::TBaseList::iterator Base::end()
{
	return gFakeChildren.end();
}

Base::TBaseList::const_iterator Base::end() const
{
	return gFakeChildren.end();
}

