#ifndef OBJECT_H__
#define OBJECT_H__

/*! \class Object
	$Id: object.h,v 1.1 2003/04/30 11:29:35 fruit Exp $
	
	Object
	
	Object is the base class of all objects in the Zeitgeist framework. An
	object is characterized by a single thing, the class which created it. The
	core it belongs to can be reached via the class object.

	Every object holds a shared_ptr to the class object, which created it.

	HISTORY:
		31.08.2002 MK
			- initial version
*/

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "core.h"

namespace zeitgeist
{

class Class;
class LogServer;
class ScriptServer;

class Object
{
	//
	// functions
	//
public:
	Object();
	virtual ~Object();

	virtual void Construct(const boost::shared_ptr<Object>& self, const boost::shared_ptr<Class>& creator);

	// class object
	boost::shared_ptr<Class>	GetClass() const;
	
	boost::weak_ptr<Object>&		GetSelf();
	const boost::weak_ptr<Object>&	GetSelf() const;

	boost::shared_ptr<Core>		GetCore() const;

	const boost::shared_ptr<FileServer>&	GetFile() const;
	const boost::shared_ptr<LogServer>&		GetLog() const;
	const boost::shared_ptr<ScriptServer>&	GetScript() const;

	virtual void				Dump() const;

	// function invocation used for scripting
	void	Invoke(const std::string &functionName);
	
private:

	//
	// members
	//
private:
	//! pointer to the class object which created this instance
	boost::shared_ptr<Class>		mClass;
	//! pointer to ourself
	boost::weak_ptr<Object>			mSelf;
};

} //namespace zeitgeist;

#endif //OBJECT_H__
