#ifndef CLASS_H__
#define CLASS_H__

/*! \class Class
	$Id: class.h,v 1.3 2003/06/03 11:58:26 fruit Exp $
	
	Class

	This class is quite essential for the Zeitgeist Core. Every class which
	wants to be managed by Zeitgeist will have to derive a class object
	from this class and override the factory method. Only decendants from
	Object are able to use Class properly and the factory method returns
	a Object pointer.

	A Class object is characterized by several parameters:

		- the name of the class
		- the version of the class

	A version is stored as an unsigned 32-bit integer, with each byte
	representing a version number. So ABCD would be A.B.C.D. That way a
	simple integer comparison can be used for newer version queries.

	Class objects also are the key to providing Zeitgeist with a plugin
	interface. The Core is responsible for managing all Class objects it
	knows. It is possible to export Class objects from a shared library
	through a unified interface, therefore enabling Class objects to be
	added at runtime to the Core.

	HISTORY:
		04.06.2002 MK
			- initial version

*/

#include <list>
#include <string>
#include <vector>

#ifdef HAVE_HASH_MAP
#include <hash_map>
#else
#include <map>

namespace std
{
    template <class _Key, class _Tp, class _Compare = std::less<_Key>,
        class _Alloc = std::allocator<std::pair<const _Key, _Tp> > >
        class hash_map : public std::map<_Key,_Tp, _Compare, _Alloc>
        {};
}

#endif

#include <boost/any.hpp>
#include <salt/defines.h>
#include <salt/sharedlibrary.h>

#include "class_m.h"
#include "leaf.h"

namespace zeitgeist
{

// forward declarations
class Core;

class Class : public Leaf
{
	// friends
	friend class Object;
	friend class Core;

	//
	// types
	//
public:
	typedef std::vector<boost::any>		TParameterList;
	typedef void (*TCmdProc)(Object *obj, const TParameterList &in);
	typedef std::list<std::string>					TStringList;

private:
	typedef std::list< boost::weak_ptr<Object> >	TObjectList;
	typedef std::hash_map<std::string, TCmdProc>	TCommandMap;

	//
	// functions
	//
public:
	Class(const std::string &name);
	virtual ~Class();

	boost::shared_ptr<Object>			Create();

	boost::shared_ptr<Core>				GetCore() const;

	//! set the bundle, this class was loaded from
	void			SetBundle(const boost::shared_ptr<salt::SharedLibrary> &bundle);

	//! returns the command procedure
	TCmdProc		GetCmdProc(const std::string &functionName);

	//! retrieve the list of base class names
	const TStringList&	GetBaseClasses() const;

	//! check if the class supports a given 'interface' (if the base class hierarchy contains the class)
	bool			Supports(const std::string &name) const;

protected:
	//! add an instance to our local list of instances
	void	AttachInstance(const boost::weak_ptr<Object> &instance);
	//! remove an instance from our local list of instances
	void	DetachInstance(const boost::weak_ptr<Object> &instance);

private:
	Class(const Class &obj);
	Class& operator=(const Class &obj);
	//! pure virtual function which creates instances
	virtual Object*	CreateInstance() const;
	//! pure virtual function which initializes the script callbacks and links to parent classes
	virtual void	DefineClass() = 0;
	//! set the core, which this class belongs to
	void AttachTo(const boost::weak_ptr<Core>& core);

	//
	// members
	//
protected:
	TCommandMap								mFunctions;
	TStringList								mBaseClasses;
private:
	boost::weak_ptr<Core>					mCore;
	/*! a shared pointer to the bundle, this class object came from. So, if all
		references to the class object are deleted, the shared library will be
		freed.*/
	boost::shared_ptr<salt::SharedLibrary>	mBundle;
	//! list of instances, which were created by this class object
	TObjectList								mInstances;
};


class CLASS(Class) : public Class
{
public:
	CLASS(Class)() : Class("ClassClass")	{	DefineClass();	}
private:
	void DefineClass();
};

// moved to here from leaf.h to avoid cyclic dependencies
DECLARE_CLASS(Leaf);

} // namespace zeitgeist

#endif //CLASS_H__
