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
#define SHARED_LIB_COMPILE

#include "classserver.h"

#include <iostream>

using namespace std;

// platformspecific stuff
#if defined(WIN32)
#include <windows.h>
#include <io.h>
// emulation layer for shared library access ... 
// this removes some #ifdefs from the code
#define CS_LoadLibrary(NAME)             LoadLibrary(NAME)
#define CS_GetProcAddress(LIB, FUNCTION) GetProcAddress((HMODULE)LIB, FUNCTION)
#define CS_FreeLibrary(LIB)              FreeLibrary((HMODULE)LIB)

#else

#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
// emulation layer for shared library access ... 
// this removes some #ifdefs from the code
#define CS_LoadLibrary(NAME)             dlopen(NAME, RTLD_LAZY)
#define CS_GetProcAddress(LIB, FUNCTION) dlsym(LIB, FUNCTION)
#define CS_FreeLibrary(LIB)              dlclose(LIB)
#endif

// this returns the instance of our ClassServer
ClassServer& 
ClassServer::instance()
{
    static ClassServer theInstance;

    return theInstance;
}

// constructor ... only called from Instance()
ClassServer::ClassServer()
{
    // we are being created ... so initialize ourself
    M_registry.clear();
    M_libraries.clear();
}

// destructor ... called implicitly at the end of the program
ClassServer::~ClassServer()
{
    // release all libraries
    while (M_libraries.size())
    {
        CS_FreeLibrary(M_libraries.back());
        M_libraries.pop_back();
    }

    M_libraries.clear();
    M_registry.clear();
}

// Register a class factory to the <null> group
bool 
ClassServer::doRegister(CS_Factory* in_factory)
{
    return doRegister("<null>", in_factory);
}


// Register a class factory to a group
bool 
ClassServer::doRegister(const char* in_group, CS_Factory* in_factory)
{
    // do some error checking
    if (in_factory == 0) return false;

    // register it only, if it doesn't exist
    if (probe(in_group, in_factory->getName())==false)
    {
        M_registry[in_group][in_factory->getName()] = in_factory;
        return true;
    }
    else
        return false;    // class exists already
}

// This function registers the classes contained in a shared library.
// It looks a bit messy because of the platform dependant nature of this
// procedure.
bool 
ClassServer::registerLibrary(const char* in_lib_name)
{
    // catch major errors
    if (in_lib_name == 0) return false;

    // open the shared library
    void* lib = CS_LoadLibrary(in_lib_name);

    if (lib)
    {
        // get our registration entry point
        void(*CS_RegisterClasses)(ClassServer*) = 0;
        CS_RegisterClasses = 
        (void(*)(ClassServer*))CS_GetProcAddress(lib, "CS_RegisterClasses");

        // check if we have the entry point function
        if (CS_RegisterClasses)
        {
            // register the classes in the shared library to ourself
            CS_RegisterClasses(this);

            // add the shared library to a list which is freed at the end
            M_libraries.push_back(lib);

            return true;
        }

        CS_FreeLibrary(lib);
    }

    // if we got here, then something went wrong
    return false;
}

// This function creates the class 'inName' in the group 'in_group'
void* 
ClassServer::create(const char* in_group, const char* in_name)
{
    // find the group
    GroupMap::iterator map_iter = M_registry.find(in_group);

    // did we get a valid group?
    if (map_iter == M_registry.end())
    {
        // guess not ... damn
        return 0;
    }

    // now find the class factory
    FactoryMap::iterator fac_iter = (*map_iter).second.find(in_name);

    // check if we found it
    if (fac_iter == (*map_iter).second.end())
    {
        // uh, oh ... something went wrong
        return 0;
    }

    // ok, create our object
    return (*fac_iter).second->create();
}


// This function creates the class specified by in_name.
// First, the "<null>"-Group is checked, then the others.
void* 
ClassServer::create(const char* in_name)
{
    //ok, first we'll give the "<null>" group a try
    void* ret = create("<null>", in_name);

    // if we got a class ... return it
    if (ret!=0) return ret;

    // ok, the "<null>" group didn't have it, so we'll try the other groups
    for (GroupMap::iterator i=M_registry.begin(); i!=M_registry.end(); i++)
    {
        // skip the "<null>" group
        if ((*i).first.c_str()[0]=='<' && (*i).first.compare("<null>")==0) 
            continue;

        // group != "<null>"
        for (FactoryMap::iterator j=(*i).second.begin(); 
             j!=(*i).second.end(); 
             j++)
        {
            if ((*j).first.compare(in_name)==0)
            {
                // we have found a matching classname, so we create the class;
                return (*j).second->create();
            }
        }
    }

    // we have tried everything, but didn't find a matching classname
    return 0;
}

// This tests if a class is already registered
bool 
ClassServer::probe(const char* in_group, const char* in_name)
{
    // find the group
    GroupMap::iterator map_iter = M_registry.find(in_group);

    // does the group exist?
    if (map_iter == M_registry.end())
    {
        // guess not ... so class can't exist either
        return false;
    }

    // now find the class factory
    FactoryMap::iterator fac_iter = (*map_iter).second.find(in_name);

    // check if we found it
    if (fac_iter == (*map_iter).second.end())
    {
        // ok .. don't have this class yet
        return false;
    }

    // we already have the same class
    return true;
}

// This tests, if a class is already registered
bool 
ClassServer::probe(const char* in_name)
{
    return probe("<null>", in_name);
}

// this returns a string list of all classes registered to a specific group
void 
ClassServer::listNames(CommandNames& class_list, const char* in_group)
{
    // find the group
    GroupMap::iterator map_iter = M_registry.find(in_group);

    // does the group exist?
    if (map_iter == M_registry.end())
    {
        // guess not ... so there are obviously no classes registered
        return;
    }

    // now get every class registered to this factory map 
    FactoryMap factory = (*map_iter).second;
    for (FactoryMap::iterator fac_iter = factory.begin(); 
         fac_iter != factory.end(); 
         fac_iter++)
    {
        class_list.push_back ((*fac_iter).first);
    }
}

// This scans a path for shared libraries to register. 
void 
ClassServer::scan(const char* in_path)
{
    char* search_pattern = 0;
    char* corrected_path = 0;
    int len;

    // correct in_path
    len = strlen(in_path) - 1;

    if (len>=0 && (in_path[len]=='/' || in_path[len]=='\\'))
    {
        // in_path has the form 'somepath/plugins/', so just copy it
        corrected_path = new char[len+2];
        strcpy(corrected_path, in_path);
    }
    else
    {
        // in_path has the form 'somepath/plugins', 
        // so just copy it and append the slash
        corrected_path = new char[len+3];
        strcpy(corrected_path, in_path);
        strcat(corrected_path, "/");
    }

    // here begins the platform specific part.
#if defined(WIN32)
    _finddata_t        dta;
    int                handle;

    // the search pattern has the form 'somepath/plugins/*'
    search_pattern = new char[strlen(corrected_path)+2];
    strcpy(search_pattern, corrected_path);
    strcat(search_pattern, "*");
    
    // check for a matching file
    handle = _findfirst(search_pattern, &dta);

    // if there's no matching file, we'll exit
    if (handle == -1)
        return;

    // iterate over all directories and files
    do
    {
        // ok, we have to generate a new path to look in 
        // (corrected_path + dta.name)
        char* new_path = new char[strlen(corrected_path)+strlen(dta.name)+1];
        strcpy(new_path, corrected_path);
        strcat(new_path, dta.name);

        // do we have a directory?
        if (dta.attrib&_A_SUBDIR)
        {
            // only recurse if we don't have a system directory
            if ((strcmp(dta.name,".") && strcmp(dta.name,"..")))
            {
                scan(new_path);
            }
        }
        else
        {
            // we have a file ... register it
            registerLibrary(new_path);
        }

        delete[] new_path;
    } while ((_findnext(handle, &dta)) == 0);

    _findclose(handle);
    delete[] search_pattern;

#else
    DIR* dir_stream;
    struct dirent* dir_entry;

    dir_stream = opendir(corrected_path);

    // if something went wrong, we'll just return silently
    if (dir_stream == 0)
        return;

    // now let's try and iterate over the entries
    dir_entry = readdir(dir_stream);

    while(dir_entry != 0)
    {
        char* new_path = new char[strlen(corrected_path)+
                                 strlen(dir_entry->d_name)+1];
        strcpy(new_path, corrected_path);
        strcat(new_path, dir_entry->d_name);

        struct stat stat_buf;
        if (stat(new_path, &stat_buf)==0)
        {
            if (stat_buf.st_mode&S_IFDIR)
            {    
                // only recurse if we don't have a system directory
                if ((strcmp(dir_entry->d_name,".") && 
                     strcmp(dir_entry->d_name,"..")))
                {
                    scan(new_path);
                }
            }
            else
            {
                // we have a file ... register it
                registerLibrary(new_path);
            }
        }
        dir_entry = readdir(dir_stream);
        delete[] new_path;
    }

    // close the directory stream
    closedir(dir_stream);
#endif

    delete[] corrected_path;
}

// Print out a list of all registered classfactories
void 
ClassServer::dumpStatus() 
{
    cout << "ClassServer::dumpStatus(" << this << ")\n";
    for (GroupMap::iterator i=M_registry.begin(); i!=M_registry.end(); i++)
    {
        cout << "'" << (*i).first << "':\n";
        for (FactoryMap::iterator j=(*i).second.begin(); 
         j!=(*i).second.end(); 
         j++)
        {
            cout << "  '" << (*j).first << "'\n";
        }
    }
}
