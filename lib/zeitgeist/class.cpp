#include "leaf.h"
#include "core.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace zeitgeist;

Class::Class(const std::string &name) :
Leaf(name)
{
}

Class::~Class()
{
	//cout << "~Class() '" << GetName() << "'\n";
	
	/*if (GetName().compare("ClassClass") == 0)
	{
		FILE *f = fopen("class.log", "wb");
		fclose(f);
	}*/

	if (mInstances.size() > 0)
	{
		cout << "  Leaked " << mInstances.size() << " instances..." << endl;
		for (TObjectList::iterator i = mInstances.begin(); i != mInstances.end(); ++i)
		{
			cout << "    " << (*i).get() << endl;
		}
	}
}

boost::shared_ptr<Object> Class::Create()
{
	//printf("Class %s: %s\n", GetName().c_str(), name.c_str());
	shared_ptr<Object> obj(CreateInstance());

	if (obj.get())
	{
		if (obj->Construct(obj, shared_static_cast<Class>(make_shared(GetSelf()))) == true)
		{
			// successfully constructed
			AttachInstance(obj);
		}
		else
		{
			obj.reset();
		}
	}

	return obj;
}

boost::shared_ptr<Core> Class::GetCore() const
{
	return make_shared(mCore);
}

void Class::AttachInstance(const boost::weak_ptr<Object> &instance)
{
	mInstances.push_back(instance);
}

void Class::DetachInstance(const boost::weak_ptr<Object> &instance)
{
	mInstances.remove(instance);
}

Object* Class::CreateInstance() const
{
	return NULL;
}

void Class::AttachTo(const boost::weak_ptr<Core>& core)
{
	mCore = core;
}

void Class::SetBundle(const boost::shared_ptr<salt::SharedLibrary> &bundle)
{
	mBundle = bundle;
}

Class::TCmdProc Class::GetCmdProc(const std::string &functionName)
{
	TCommandMap::iterator cmd = mFunctions.find(functionName);

	if (cmd != mFunctions.end())
	{
		return (*cmd).second;
	}

	shared_ptr<Leaf> classDir = GetCore()->Get("/classes");
	// ok, we don't have the requested function, so we'll try the base class
	// objects
	for (TStringList::iterator baseClass = mBaseClasses.begin(); baseClass != mBaseClasses.end(); ++baseClass)
	{
		// this should get the base class object (it has to live on the same level of the hierarchy
		// as this class object)
		shared_ptr<Class> theClass = shared_static_cast<Class>(GetCore()->Get(*baseClass, classDir));
		//printf("%x:'%s'.theClass: %s - %d\n", GetCore(), GetName().c_str(), (*baseClass).c_str(), theClass);
		if (theClass)
		{
			// now, we ask the class object, if it knows the command in question
			TCmdProc theCmd = theClass->GetCmdProc(functionName);
			//printf("theCmd: %s - %d\n", functionName.c_str(), theCmd);
			if (theCmd != NULL)
			{
				// here we have found the command and return it
				return theCmd;
			}
		}
	}

	return NULL;	
}

const Class::TStringList& Class::GetBaseClasses() const
{
	return mBaseClasses;
}

bool Class::Supports(const std::string &name) const
{
	if (GetName().compare(name) == 0) return true;

	// check base-classes
	shared_ptr<Leaf> classDir = GetCore()->Get("/classes");

	for (TStringList::const_iterator i = mBaseClasses.begin(); i != mBaseClasses.end(); ++i)
	{
		shared_ptr<Class> theClass = shared_static_cast<Class>(GetCore()->Get(*i, classDir));

		if (theClass)
		{
			if (theClass->Supports(name)) return true;
		}
		else
		{
			cout << "WARNING: Illegal BaseClass '" << (*i) << "' in Class '" << GetName() << "'" << endl;
		}
	}
	
	return false;
}

void CLASS(Class)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Leaf);	
}
