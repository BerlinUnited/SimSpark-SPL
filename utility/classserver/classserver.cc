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

#include <iostream>
#include "classserver.h"

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
ClassServer& ClassServer::instance()
{
    static ClassServer theInstance;

    return theInstance;
}

// constructor ... only called from Instance()
ClassServer::ClassServer()
{
    // we are being created ... so initialize ourself
    mRegistry.clear();
    mLibraries.clear();
}

// destructor ... called implicitly at the end of the program
ClassServer::~ClassServer()
{
    // release all libraries
    while (mLibraries.size())
    {
        CS_FreeLibrary(mLibraries.back());
        mLibraries.pop_back();
    }

    mLibraries.clear();
    mRegistry.clear();
}

// Register a class factory to the <null> group
bool 
ClassServer::registerFactory(CS_Factory* inFactory)
{
    return registerFactory("<null>", inFactory);
}


// Register a class factory to a group
bool 
ClassServer::registerFactory(const char* inGroup, CS_Factory* inFactory)
{
    // do some error checking
    if (inFactory == NULL) return false;

    // register it only, if it doesn't exist
    if (probe(inGroup, inFactory->getName())==false)
    {
        mRegistry[inGroup][inFactory->getName()] = inFactory;
        return true;
    }
    else
        return false;    // class exists already
}

// This function registers the classes contained in a shared library.
// It looks a bit messy because of the platform dependant nature of this
// procedure.
bool 
ClassServer::registerLibrary(const char* inLibName)
{
    // catch major errors
    if (inLibName == NULL) return false;

    // open the shared library
    void* lib = CS_LoadLibrary(inLibName);

    if (lib)
    {
        // get our registration entry point
        void(*CS_RegisterClasses)(ClassServer*) = NULL;
        CS_RegisterClasses = 
        (void(*)(ClassServer*))CS_GetProcAddress(lib, "CS_RegisterClasses");

        // check if we have the entry point function
        if (CS_RegisterClasses)
        {
            // register the classes in the shared library to ourself
            CS_RegisterClasses(this);

            // add the shared library to a list which is freed at the end
            mLibraries.push_back(lib);

            return true;
        }

        CS_FreeLibrary(lib);
    }

    // if we got here, then something went wrong
    return false;
}

// This function creates the class 'inName' in the group 'inGroup'
void* 
ClassServer::create(const char* inGroup, const char* inName)
{
    // find the group
    tGroupMap::iterator mapIter = mRegistry.find(inGroup);

    // did we get a valid group?
    if (mapIter == mRegistry.end())
    {
        // guess not ... damn
        return NULL;
    }

    // now find the class factory
    tFactoryMap::iterator facIter = (*mapIter).second.find(inName);

    // check if we found it
    if (facIter == (*mapIter).second.end())
    {
        // uh, oh ... something went wrong
        return NULL;
    }

    // ok, create our object
    return (*facIter).second->create();
}


// This function creates the class specified by inName.
// First, the "<null>"-Group is checked, then the others.
void* 
ClassServer::create(const char* inName)
{
    //ok, first we'll give the "<null>" group a try
    void* ret = create("<null>", inName);

    // if we got a class ... return it
    if (ret!=NULL) return ret;

    // ok, the "<null>" group didn't have it, so we'll try the other groups
    for (tGroupMap::iterator i=mRegistry.begin(); i!=mRegistry.end(); i++)
    {
        // skip the "<null>" group
        if ((*i).first.c_str()[0]=='<' && (*i).first.compare("<null>")==0) 
            continue;

        // group != "<null>"
        for (tFactoryMap::iterator j=(*i).second.begin(); 
             j!=(*i).second.end(); 
             j++)
        {
            if ((*j).first.compare(inName)==0)
            {
                // we have found a matching classname, so we create the class;
                return (*j).second->create();
            }
        }
    }

    // we have tried everything, but didn't find a matching classname
    return NULL;
}

// This tests if a class is already registered
bool 
ClassServer::probe(const char* inGroup, const char* inName)
{
    // find the group
    tGroupMap::iterator mapIter = mRegistry.find(inGroup);

    // does the group exist?
    if (mapIter == mRegistry.end())
    {
        // guess not ... so class can't exist either
        return false;
    }

    // now find the class factory
    tFactoryMap::iterator facIter = (*mapIter).second.find(inName);

    // check if we found it
    if (facIter == (*mapIter).second.end())
    {
        // ok .. don't have this class yet
        return false;
    }

    // we already have the same class
    return true;
}

// This tests, if a class is already registered
bool 
ClassServer::probe(const char* inName)
{
    return probe("<null>", inName);
}

// this returns a string list of all classes registered to a specific group
void 
ClassServer::listAll(tCommandNames& classList, const char* inGroup)
{
    // find the group
    tGroupMap::iterator mapIter = mRegistry.find(inGroup);

    // does the group exist?
    if (mapIter == mRegistry.end())
    {
        // guess not ... so there are obviously no classes registered
        return;
    }

    // now get every class registered to this factory map 
    tFactoryMap factory = (*mapIter).second;
    for (tFactoryMap::iterator facIter = factory.begin(); 
         facIter != factory.end(); 
         facIter++)
    {
        classList.push_back ((*facIter).first);
    }
}

// This scans a path for shared libraries to register. 
void 
ClassServer::scan(const char* inPath)
{
    char* searchPattern = NULL;
    char* correctedPath = NULL;
    int   len;

    // correct inPath
    len = strlen(inPath) - 1;

    if (len>=0 && (inPath[len]=='/' || inPath[len]=='\\'))
    {
        // inPath has the form 'somepath/plugins/', so just copy it
        correctedPath = new char[len+2];
        strcpy(correctedPath, inPath);
    }
    else
    {
        // inPath has the form 'somepath/plugins', 
        // so just copy it and append the slash
        correctedPath = new char[len+3];
        strcpy(correctedPath, inPath);
        strcat(correctedPath, "/");
    }

    // here begins the platform specific part.
#if defined(WIN32)
    _finddata_t        dta;
    int                handle;

    // the search pattern has the form 'somepath/plugins/*'
    searchPattern = new char[strlen(correctedPath)+2];
    strcpy(searchPattern, correctedPath);
    strcat(searchPattern, "*");
    
    // check for a matching file
    handle = _findfirst(searchPattern, &dta);

    // if there's no matching file, we'll exit
    if (handle == -1)
        return;

    // iterate over all directories and files
    do
    {
        // ok, we have to generate a new path to look in (correctedPath + dta.name)
        char* newPath = new char[strlen(correctedPath)+strlen(dta.name)+1];
        strcpy(newPath, correctedPath);
        strcat(newPath, dta.name);

        // do we have a directory?
        if (dta.attrib&_A_SUBDIR)
        {
            // only recurse if we don't have a system directory
            if ((strcmp(dta.name,".") && strcmp(dta.name,"..")))
            {
                scan(newPath);
            }
        }
        else
        {
            // we have a file ... register it
            registerLibrary(newPath);
        }

        delete[] newPath;
    } while ((_findnext(handle, &dta)) == 0);

    _findclose(handle);
    delete[] searchPattern;

#else
    DIR*            dirStream;
    struct dirent*    dirEntry;

    dirStream = opendir(correctedPath);

    // if something went wrong, we'll just return silently
    if (dirStream == NULL)
        return;

    // now let's try and iterate over the entries
    dirEntry = readdir(dirStream);

    while(dirEntry != NULL)
    {
        char* newPath = new char[strlen(correctedPath)+
                                 strlen(dirEntry->d_name)+1];
        strcpy(newPath, correctedPath);
        strcat(newPath, dirEntry->d_name);

        struct stat statBuf;
        if (stat(newPath, &statBuf)==0)
        {
            if (statBuf.st_mode&S_IFDIR)
            {    
                // only recurse if we don't have a system directory
                if ((strcmp(dirEntry->d_name,".") && 
                     strcmp(dirEntry->d_name,"..")))
                {
                    scan(newPath);
                }
            }
            else
            {
                // we have a file ... register it
                registerLibrary(newPath);
            }
        }
        dirEntry = readdir(dirStream);
        delete[] newPath;
    }

    // close the directory stream
    closedir(dirStream);
#endif

    delete[] correctedPath;
}

// Print out a list of all registered classfactories
void 
ClassServer::dumpStatus() 
{
    cout << "ClassServer::dumpStatus(" << this << ")\n";
    for (tGroupMap::iterator i=mRegistry.begin(); i!=mRegistry.end(); i++)
    {
    cout << "'" << (*i).first << "':\n";
        for (tFactoryMap::iterator j=(*i).second.begin(); 
         j!=(*i).second.end(); 
         j++)
        {
            cout << "  '" << (*j).first << "'\n";
        }
    }
}
