#include "object.h"
#include "class.h"
#include "core.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace zeitgeist;

Object::Object()
{
}

Object::~Object()
{
	if (mClass.get())
	{
		mClass->DetachInstance(GetSelf());
	}
}

bool Object::Construct(const boost::shared_ptr<Object>& self, const boost::shared_ptr<Class>& creator)
{
	mSelf	= self;
	mClass	= creator;

	// give each object a chance to do something
	return ConstructInternal();
}

boost::shared_ptr<Class> Object::GetClass() const
{
	return mClass;
}

boost::weak_ptr<Object>& Object::GetSelf()
{
	return mSelf;
}

const boost::weak_ptr<Object>& Object::GetSelf() const
{
	return mSelf;
}

boost::shared_ptr<Core> Object::GetCore() const
{
	assert(mClass.get() != NULL);
	return mClass->GetCore();
}

void Object::Dump() const
{
	cout << "Object: " << (void*) this << " " << (void*) mSelf.get() << " - use count = " << mSelf.use_count() << endl;
}

void Object::Invoke(const std::string &functionName)
{
	Class::TParameterList in;

	Class::TCmdProc cmd = mClass->GetCmdProc(functionName);

	if (cmd != NULL)
		cmd(GetSelf().get(), in);
}

const boost::shared_ptr<FileServer>& Object::GetFile() const
{
	return GetCore()->GetFileServer();
}

const boost::shared_ptr<LogServer>& Object::GetLog() const
{
	return GetCore()->GetLogServer();
}

const boost::shared_ptr<ScriptServer>& Object::GetScript() const
{
	return GetCore()->GetScriptServer();
}

bool Object::ConstructInternal()
{
	return true;
}
