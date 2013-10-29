/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id$

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
#ifndef ZEITGEIST_CLASS_H
#define ZEITGEIST_CLASS_H

#include <zeitgeist/scriptserver/gcvalue.h>

#ifdef HAVE_CONFIG_H
#include <sparkconfig.h>
#endif

#include <map>
#include <string>
#include <vector>
#include <list>
#include <salt/defines.h>
#include <salt/sharedlibrary.h>
#include "leaf.h"
#include "parameterlist.h"

namespace zeitgeist
{

#define CLASS(className)        Class_##className

#define DECLARE_CLASS(className)\
    class CLASS(className) : public zeitgeist::Class\
    {\
    public:\
        CLASS(className)() : zeitgeist::Class(#className) { DefineClass(); }\
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
        CLASS(className)() : zeitgeist::Class(#className) { DefineClass();  }\
    private:\
        void DefineClass();\
    };


// note the 'unused' attribute suppresses compiler warnings if the
// 'in' parameter is not accessed in the function implementation
#define FUNCTION(className,functionName)\
    static zeitgeist::GCValue functionName(className *obj, \
         __attribute__((unused)) const zeitgeist::ParameterList &in)

#define DEFINE_FUNCTION(functionName)\
    mFunctions[#functionName] = (TCmdProc) &functionName;

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
    SHARED_LIB_EXPORT void Zeitgeist_RegisterBundle(std::list <boost::shared_ptr<Class> > &classes){

#define ZEITGEIST_EXPORT_EX(className, path)\
    classes.push_back(boost::shared_ptr<Class>(new CLASS(className)));

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
class ZEITGEIST_API Class : public Leaf
{
    // friends
    friend class Object;
    friend class Core;

    //
    // types
    //
public:
    /** defines a signature for a function used as the c++
        implementation of a member function exported to a script
        language. It receives a pointer to an instance of the
        class from which it is a member function along with a
        list of paramters.
    */
    typedef GCValue (*TCmdProc)(Object* , const zeitgeist::ParameterList &in);
    typedef std::list<std::string> TStringList;

    /** defines a mapping from member names to command procedures */
    typedef std::map<std::string, TCmdProc> TCommandMap;

private:
    /** defines a list of pointers to object instances */
    typedef std::list< boost::weak_ptr<Object> > TObjectList;


    //
    // functions
    //
public:
    /** constructs a class object for the class 'name' */
    Class(const std::string &name);
    virtual ~Class();

    /** creates a new instance of the represented class */
    boost::shared_ptr<Object> Create();

    /** returns a pointer to the core this class is attached to */
    boost::shared_ptr<Core> GetCore() const;

    /** sets the bundle, this class was loaded from */
    void SetBundle(const boost::shared_ptr<salt::SharedLibrary> &bundle);

    /** the command procedure for a function */
    TCmdProc GetCmdProc(const std::string &functionName) const;

    /** returns a list of base class names */
    const TStringList& GetBaseClasses() const;

    /** returns true iff the class supports a given 'interface',
     *  i.e. the base class hierarchy contains the class 'name'
     */
    bool SupportsClass(const std::string &name) const;

    /** returns true iff the class supports a given command, i.e. to
     *  this class or to one of its base classes the given command
     *  procedure is registered
     */
    bool SupportsCommand(const std::string &name) const;

    /** returns the command map of this class */
    const TCommandMap& GetCommandMap() const;

    /** a shared pointer to the bundle, this class object came from */
    boost::shared_ptr<salt::SharedLibrary>  GetBundle() const;

protected:
    /** adds an instance to the local list of instances */
    void AttachInstance(const boost::weak_ptr<Object> &instance);

    /** removes an instance from the local list of instances */
    void DetachInstance(const boost::weak_ptr<Object> &instance);

private:
    Class(const Class &obj);
    Class& operator=(const Class &obj);

    /** pure virtual function which creates instances */
    virtual Object* CreateInstance() const;

    /**  pure virtual function which initializes the script callbacks and links to parent classes */
    virtual void DefineClass() = 0;

    /** set the core, which this class belongs to */
    void AttachTo(const boost::weak_ptr<Core>& core);

    //
    // members
    //
protected:
    TCommandMap mFunctions;
    TStringList mBaseClasses;
private:
    boost::weak_ptr<Core> mCore;

    /** a shared pointer to the bundle, this class object came
     * from. So, if all references to the class object are
     * deleted, the shared library will be freed.
     */
    boost::shared_ptr<salt::SharedLibrary>  mBundle;

    /** a list of instances, which were created by this class object */
    TObjectList mInstances;
};


/** this is the class object beloging to the class
    'zeitgeist::Class'. */
class CLASS(Class) : public Class
{
public:
    CLASS(Class)() : Class("ClassClass")    { DefineClass();  }
private:
    void DefineClass();
};

/** declare the clss object for leaf. Put here to avoid a circular dependency
    between class inheriting from leaf and leaf needing class to inherit its
    corresponding class object
*/
DECLARE_CLASS(Leaf);

} // namespace zeitgeist

#endif //ZEITGEIST_CLASS_H
