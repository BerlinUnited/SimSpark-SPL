/* -*- mode: c++ -*-
   
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: class.h,v 1.6 2003/08/22 16:23:31 rollmark Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


   Class

   HISTORY:
		04.06.2002 MK
			- initial version

*/

#ifndef CLASS_H__
#define CLASS_H__

#include <string>
#include <vector>
#include <list>
#include <hash_map>
#include <boost/any.hpp>
#include <salt/defines.h>
#include <salt/sharedlibrary.h>
#include "leaf.h"

namespace zeitgeist
{

#define CLASS(className)	Class_##className

#define DECLARE_CLASS(className)\
	class CLASS(className) : public zeitgeist::Class\
	{\
	public:\
		CLASS(className)() : zeitgeist::Class(#className)	{	DefineClass();	}\
		zeitgeist::Object* CreateInstance() const\
		{\
			zeitgeist::Object *instance = new className();\
			return instance;\
		}\
	private:\
		void DefineClass();\
	};

#define DECLARE_ABSTRACTCLASS(className)\
	class CLASS(className) : public zeitgeist::Class\
	{\
	public:\
		CLASS(className)() : zeitgeist::Class(#className)	{	DefineClass();	}\
	private:\
		void DefineClass();\
	};

#define FUNCTION(functionName)\
	static void functionName(zeitgeist::Object *obj, const zeitgeist::Class::TParameterList &in)

#define DEFINE_FUNCTION(functionName)\
	mFunctions[#functionName] = functionName;

#define DEFINE_BASECLASS(baseClass)\
	mBaseClasses.push_back(#baseClass);

//
// Export stuff
//
#define ZEITGEIST_EXPORT_BEGIN()\
	using namespace boost;\
	using namespace salt;\
	using namespace zeitgeist;\
	extern "C"{\
	SHARED_LIB_EXPORT void Zeitgeist_RegisterBundle(std::list <shared_ptr<Class> > &classes){

#define ZEITGEIST_EXPORT_EX(className, path)\
	classes.push_back(shared_ptr<Class>(new CLASS(className)));

#define ZEITGEIST_EXPORT(className) ZEITGEIST_EXPORT_EX(className, "")

#define ZEITGEIST_EXPORT_END()\
	}}

// forward declarations
class Core;

/** This class is quite essential for the Zeitgeist Core. Every class
 * which wants to be managed by Zeitgeist will have to derive a class
 * object from this class and override the factory method. Only
 * decendants from Object are able to use Class properly and the
 * factory method returns a Object pointer.
 *
 * A Class object is characterized by several parameters:
 *
 * - the name of the class
 * - the version of the class
 *
 * A version is stored as an unsigned 32-bit integer, with each byte
 * representing a version number. So ABCD would be A.B.C.D. That way a
 * simple integer comparison can be used for newer version queries.
 * 
 * Class objects also are the key to providing Zeitgeist with a plugin
 * interface. The Core is responsible for managing all Class objects it
 * knows. It is possible to export Class objects from a shared library
 * through a unified interface, therefore enabling Class objects to be
 * added at runtime to the Core.
*/
class Class : public Leaf
{
	// friends
	friend class Object;
	friend class Core;

	//
	// types
	//
public:

	/** defines a parameter list as a list of objects of different
	 *  types. This is type safe using the boost:any
	 *  class. TParameterList is used to transfer parameters
	 *  between the C++ part and and the script representation of
	 *  a class (e.g. a ruby class).
	 */
	typedef std::vector<boost::any>		TParameterList;


	/** defines a signature for a function used as the c++
	    implementation of a member function exported to a script
	    language. It receives a pointer to an instance of the
	    class from which it is a member function along with a
	    list of paramters.
	 */
	typedef void (*TCmdProc)(Object *obj, const TParameterList &in);
	typedef std::list<std::string>					TStringList;

private:
	/** defines a list of pointers to object instances */
	typedef std::list< boost::weak_ptr<Object> >	TObjectList;

	/** defines a mapping from member names to command
	    procedures */
	typedef std::hash_map<std::string, TCmdProc>	TCommandMap;

	//
	// functions
	//
public:
	/** constructs a class object for the class 'name' */
	Class(const std::string &name);
	virtual ~Class();

	/** creates a new instance of the represented class */
	boost::shared_ptr<Object>			Create();

	/** returns a pointer to the core this class is attached to */
	boost::shared_ptr<Core>				GetCore() const;

	/** sets the bundle, this class was loaded from */
	void			SetBundle(const boost::shared_ptr<salt::SharedLibrary> &bundle);

	/** the command procedure for a function */
	TCmdProc		GetCmdProc(const std::string &functionName);

	/** returns a list of base class names */
	const TStringList&	GetBaseClasses() const;

	/** return true if the class supports a given 'interface',
	 *  i.e. the base class hierarchy contains the class 'name'
	 */
	bool			Supports(const std::string &name) const;

protected:
	/** adds an instance to the local list of instances */
	void	AttachInstance(const boost::weak_ptr<Object> &instance);

	/** removes an instance from the local list of instances */
	void	DetachInstance(const boost::weak_ptr<Object> &instance);

private:
	Class(const Class &obj);
	Class& operator=(const Class &obj);

	/** pure virtual function which creates instances */
	virtual Object*	CreateInstance() const;

	/**  pure virtual function which initializes the script callbacks and links to parent classes */
	virtual void	DefineClass() = 0;

	/** set the core, which this class belongs to */
	void AttachTo(const boost::weak_ptr<Core>& core);

	//
	// members
	//
protected:
	TCommandMap								mFunctions;
	TStringList								mBaseClasses;
private:
	boost::weak_ptr<Core>					mCore;

	/** a shared pointer to the bundle, this class object came
	 * from. So, if all references to the class object are
	 * deleted, the shared library will be freed.
	 */
	boost::shared_ptr<salt::SharedLibrary>	mBundle;

	/** a list of instances, which were created by this class object */
	TObjectList								mInstances;
};


/** this is the class object beloging to the class
    'zeitgeist::Class'. */
class CLASS(Class) : public Class
{
public:
	CLASS(Class)() : Class("ClassClass")	{	DefineClass();	}
private:
	void DefineClass();
};

/** declare the clss object for leaf. Put here to avoid a circular dependency
    between class inheriting from leaf and leaf needing class to inherit its
    corresponding class object
*/
DECLARE_CLASS(Leaf);


} // namespace zeitgeist

#endif //CLASS_H__
