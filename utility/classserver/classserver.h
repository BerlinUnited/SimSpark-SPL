/* -*- mode: c++ -*-
   
   this file is part of Soccer Server 3D
   Copyright (C) 2002 Soccer Server Maintainance Group

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#ifndef UTILITY_CLASSSERVER_H
#define UTILITY_CLASSSERVER_H

/*! \class ClassServer
    $Id: classserver.h,v 1.2 2002/08/21 08:47:13 fruit Exp $

    ClassServer - Class Creation Interface

    The class server is a system which manages factory classes for
    creating instances of classes. A facility like this makes
    extending functionality of the main program a breeze. It also
    allows scripting facilities to instantiate native classes easily.

    HISTORY: 
    The ClassServer subsystem was taken from a student project at the
    AI Research Group, Koblenz University. Original development by
    Marco Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/

// VC-Compiler specific ... disable those stupid warnings
#if defined(WIN32)
#pragma warning (disable : 4251)    // dll-interface for STL functions
#pragma warning (disable : 4503)    // name too long
#pragma warning (disable : 4786)    // truncation of debug information

#define SHARED_LIB_EXPORT    __declspec(dllexport)
#else
#define SHARED_LIB_EXPORT
#endif

// this macro only exports the class in this header, 
// if the shared library version is compiled
#ifdef SHARED_LIB_COMPILE
    #define CLASS_EXPORT    SHARED_LIB_EXPORT
#else
    #define CLASS_EXPORT
#endif

#include <map>
#include <string>
#include <list>

/*! Macros
    The following macros make exporting plugins from a shared library
    really simple.  You just have to create a factory for each class
    to be exported. Then follows a CS_Begin/CS_End block, containing a
    CS_Register or CS_RegisterToGroup call for each class. These
    macros basically define an entry point function through which the
    class server can access the classfactories.
*/

//! This macro creates a factory for the class 'CLASSNAME'
#define CS_CreateFactory(CLASSNAME)\
    class CS_Factory##CLASSNAME : public CS_Factory{\
    public:\
        CS_Factory##CLASSNAME(){\
            char* tmp_string = #CLASSNAME;\
            M_name = new char[strlen(tmp_string)+1];\
            strcpy(M_name, tmp_string);\
        }\
        virtual void* create()    { return new CLASSNAME();    }\
    };

//! This macro creates the preamble for a shared library export block
#define CS_Begin()\
    extern "C"{\
    SHARED_LIB_EXPORT void CS_RegisterClasses(ClassServer* CS_OurClassServer){

/*! This macro registers a class inside a CS_Begin/CS_End Block 
    to the "<null>" group
*/
#define CS_Register(CLASSNAME)\
    static CS_Factory##CLASSNAME theFactory##CLASSNAME;\
    CS_OurClassServer->doRegister(&theFactory##CLASSNAME);


/*! This macro registers a class inside a CS_Begin/CS_End Block
    to the specified group
//
#define CS_RegisterToGroup(GROUP, CLASSNAME)\
    static CS_Factory##CLASSNAME theFactory##CLASSNAME;\
    CS_OurClassServer->doRegister(#GROUP, &theFactory##CLASSNAME);

/!* This macro creates the end of a shared library export block. 
    One brace for the function and the other for the extern "C" block
*/
#define CS_End() }}

//! This macro begins an export block from within a function
#define CS_FunctionBegin()\
    {

/*! This macro registers a class inside a CS_Begin/CS_End Block 
    to the "<null>" group
*/
#define CS_FunctionRegister(CLASSNAME)\
    static CS_Factory##CLASSNAME theFactory##CLASSNAME;\
    ClassServer::instance().doRegister(&theFactory##CLASSNAME);

/*! This macro registers a class inside a CS_Begin/CS_End Block 
    to the specified group
*/
#define CS_FunctionRegisterToGroup(GROUP, CLASSNAME)\
    static CS_Factory##CLASSNAME theFactory##CLASSNAME;\
    ClassServer::instance().doRegister(#GROUP, &theFactory##CLASSNAME);

//! This macro terminates an export block
#define CS_FunctionEnd() }

/*! \class CS_Factory
    An interface which will be used for creating the automatically registering
    factory classes (see macros above)
*/
class CS_Factory
{
public:
    // virtual destructor
    virtual ~CS_Factory()    {    if (M_name) delete[] M_name;    }
    // Accessors
    char* getName()          {    return M_name;    }
    // the interface which has to be implemented
    virtual void* create() = 0;    
protected:
    //! name of the class which is created by this factory
    char*    M_name;        
};

/*! \class ClassServer
    A singleton object which manages a registry of classes and allows the
    instantiation via a 'string'. This is extremely useful and makes extending
    functionality very easy to maintain.
*/
class CLASS_EXPORT ClassServer
{
public:
    //! a string list
    typedef std::list<std::string> CommandNames;

    //! singleton implementation
    static ClassServer& instance();

    // register functions
    //! Register a class factory to the <null> group
    bool doRegister(CS_Factory* in_factory);
    //! Register a class factory to a group
    bool doRegister(const char* in_group, CS_Factory* in_factory);
    //! Register classes contained in a shared library
    bool registerLibrary(const char* library);
    void scan(const char* inPath);

    //! create the class 'in_name' in the group 'in_group'
    void* create(const char* in_group, const char* in_name);
    //! class creation
    void* create(const char* in_name);

    //! test if a class is registered
    bool probe(const char* in_group, const char* in_name);
    //! test if a class is registered
    bool probe(const char* in_name);

    //! dump status out to stdout
    void dumpStatus();

    //! list names of registered classes
    void listNames(CommandNames& class_list, const char* in_group = "<null>");


    //! destructor should only be called implicitly
    ~ClassServer();

protected:
    // declare a few types, so that we can save some 'typing' (pun intended)
    typedef std::map<std::string, CS_Factory*> FactoryMap;
    typedef std::map<std::string, FactoryMap> GroupMap;

    // for singleton
    ClassServer();

    //! registry of factories (categorized in groups)
    GroupMap M_registry;    
    //! list of loaded shared libraries
    std::list<void*> M_libraries;

private:
    ClassServer(const ClassServer&);
    ClassServer& operator=(const ClassServer&);
};
#endif // UTILITY_CLASSSERVER_H
