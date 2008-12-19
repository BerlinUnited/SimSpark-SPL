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
*/
#include "core.h"
#include "leaf.h"
#include "object.h"
#include "object_c.h"
#include "node.h"
#include "corecontext.h"
#include "fileserver/fileserver.h"
#include "fileserver/filesystem.h"
#include "logserver/logserver.h"
#include "scriptserver/scriptserver.h"
#include "randomserver/randomserver.h"

#include <salt/path.h>
#include <salt/sharedlibrary.h>
#include <signal.h>
#include <iostream>
#include <sstream>

#if HAVE_CONFIG_H
#include <sparkconfig.h>
#endif

#ifdef HAVE_EXECINFO_H
#include <execinfo.h>
#endif

using namespace boost;
using namespace salt;
using namespace std;
using namespace zeitgeist;

// -------- struct Core::CacheKey

bool Core::CacheKey::operator == (const CacheKey& key) const
{
    return (
            (root.expired() == key.root.expired()) &&
            (
             (root.expired()) ||
             (root.lock() == key.root.lock())
             ) &&
            (path == key.path)
            );
}

bool Core::CacheKey::operator < (const CacheKey& key) const
{
    if (root.expired())
        {
            if (! key.root.expired())
                {
                    return true;
                }
        } else
        {
            if (key.root.expired())
                {
                    return false;
                }

        }

    shared_ptr<Leaf> myRoot = root.lock();
    shared_ptr<Leaf> keyRoot = key.root.lock();

    if (myRoot != keyRoot)
        {
            return (myRoot.get() < keyRoot.get());
        }

    return (path < key.path);
}

// -------- class Core

/** Create the fundamental classes needed for the hierarchy to operate
 */
Core::Core()
{
    mClassClass = shared_ptr<Class>(new CLASS(Class));
    mClassClass->Construct(mClassClass, shared_ptr<Class>());
}

Core::~Core()
{
    mBundles.clear();
}

void Core::Construct(const boost::weak_ptr<Core>& self)
{
    mSelf = self;

    // setup the node class
    mNodeClass.reset(new CLASS(Node));
    BindClass(mNodeClass);

    // create the root node
    mRoot = shared_static_cast<Leaf>(mNodeClass->Create());
    mRoot->SetName("");

    shared_ptr<CoreContext> context = CreateContext();

    // register the internal zeitgeist classes
    RegisterClassObject(mNodeClass, "zeitgeist/");
    RegisterClassObject(mClassClass, "zeitgeist/");
    RegisterClassObject(new CLASS(Leaf), "zeitgeist/");
    RegisterClassObject(new CLASS(FileServer), "zeitgeist/");
    RegisterClassObject(new CLASS(FileSystem), "zeitgeist/");
    RegisterClassObject(new CLASS(LogServer), "zeitgeist/");
    RegisterClassObject(new CLASS(RandomServer), "zeitgeist/");
    RegisterClassObject(new CLASS(ScriptServer), "zeitgeist/");
#ifdef  THE_TELNETSERVER_COMPILES_FOR_OLIVER
    RegisterClassObject(new CLASS(TelnetServer), "zeitgeist/");
#endif
    // create the log server
    mLogServer = shared_static_cast<LogServer>
        (context->New("zeitgeist/LogServer", "/sys/server/log"));

    // create an initial useful loggin setup. This can be modified
    // from scripts later on
    mLogServer->AddStream(&cerr, LogServer::eError);

    mLogServer->AddStream(&cout,
                          LogServer::eNormal |
                          LogServer::eWarning
                          );

#if INIT_DEBUG
      mLogServer->AddStream(&cout,
                            LogServer::eDebug
                            );
#endif

    mFileServer = shared_static_cast<FileServer>
        (context->New("zeitgeist/FileServer", "/sys/server/file"));

    // create the script server
    mScriptServer = shared_static_cast<ScriptServer>
        (context->New("zeitgeist/ScriptServer", "/sys/server/script"));

    // create the random server
    mRandomServer = shared_static_cast<RandomServer>
        (context->New("zeitgeist/RandomServer", "/sys/server/random"));

    // install fault handler
    signal(SIGSEGV, CatchSignal);
    signal(SIGABRT, CatchSignal);
}

void Core::CatchSignal(int sig_num)
{
    cerr << "(Core) caught signal " << sig_num << endl;

#ifdef __linux__
    // retrieve the name of our executable without access to argc and
    // argv (this works only with linux)

    const int exeNameSz = 4096;
    char exeName[exeNameSz];
    memset(exeName, 0, sizeof(exeName));

    readlink("/proc/self/exe",exeName,exeNameSz);

    // print stack trace
    const int arSize = 200;
    void *addresses[arSize];
    memset(addresses, 0, sizeof(addresses));

    int depth = backtrace (addresses, arSize);
    char** strings = backtrace_symbols (addresses, depth);

    cerr << "(Core) dumping " << depth << " stack frames.\n";

    for (int i=0; i<depth; ++i)
        {
            stringstream ss;
            ss << "echo \"[" << i << "] " << strings[i] << "\" | c++filt;";

            // use the addr2line tool from binutils to retrieve the
            // source line from the frame adress
            // -C : decode, demangle low-level symbol names
            // -e : specify executable name
            // -f : display function names, as well as file and line number info

            ss << "addr2line -C -f -e \"" << exeName << "\" " << addresses[i];
            system(ss.str().c_str());

            cerr << "\n";
        }

    free (strings);
#endif

    cerr << "(Core) exit" << endl;
    exit(1);
}

void Core::Desctruct()
{
    // destruct hierarchy
    mRoot.reset();

    // reset server references
    mRandomServer.reset();
    mClassClass.reset();
    mNodeClass.reset();
    mFileServer.reset();
    mLogServer.reset();
    mScriptServer.reset();
}

boost::shared_ptr<CoreContext> Core::CreateContext()
{
    return shared_ptr<CoreContext>(new CoreContext(mSelf.lock(), GetRoot()));
}

boost::shared_ptr<Object>
Core::New(const std::string& className)
{
    // select the correct class to create our instance
    shared_ptr<CoreContext> context = CreateContext();
    shared_ptr<Class> theClass = shared_dynamic_cast<Class>
        (context->Get("/classes/"+className));

    // here we will store our created instance
    shared_ptr<Object> instance;

    if (theClass.get() == 0)
        {
            mLogServer->Error() << "(Core::New) unkown class '"
                                << className << "'\n";
        } else
            {
                instance = theClass->Create();
            }

    return instance;
}

bool
Core::ExistsClass(const std::string& className)
{
    return (Test("/classes/"+className));
}

bool Core::RegisterClassObject(const boost::shared_ptr<Class> &classObject,
                               const std::string &subDir)
{
    shared_ptr<CoreContext> context = CreateContext();
    BindClass(classObject);

    return context->Install(classObject, "/classes/" + subDir, true);
}

bool Core::RegisterClassObject(Class *classObject, const std::string &subDir)
{
    return RegisterClassObject(shared_ptr<Class>(classObject), subDir);
}

bool Core::ImportBundle(const std::string& bundleName)
{
    TBundleMap::const_iterator iter = mBundles.find(bundleName);
    if (iter != mBundles.end())
        {
            // already imported
            return true;
        }

    shared_ptr<SharedLibrary> bundle(new SharedLibrary());
    if (!bundle->Open(bundleName))
        {
            mLogServer->Error() << "(Core) ERROR: Could not open '"
                                << bundleName << "'" << endl;
            return false;
        }

    std::list <shared_ptr<Class> > classes;
    void(*Zeitgeist_RegisterBundle)(std::list <shared_ptr<Class> > &) = NULL;

    Zeitgeist_RegisterBundle = (void(*)(std::list <shared_ptr<Class> > &))
        bundle->GetProcAddress("Zeitgeist_RegisterBundle");

    if (Zeitgeist_RegisterBundle == NULL)
        {
            mLogServer->Error() << "ERROR: (Core) No entry point!" << std::endl;
            return false;
        }

    Zeitgeist_RegisterBundle(classes);

    bool usingClass = false;
    while(!classes.empty())
        {
            if (RegisterClassObject(classes.back(), ""))
                {
                    classes.back()->SetBundle(bundle);
                    usingClass = true;
                }

            classes.pop_back();
        }

    // we only add the bundle, if it contained a class which was
    // registered
    if (usingClass)
        mBundles[bundleName] = bundle;

    return true;
}

const boost::shared_ptr<FileServer>& Core::GetFileServer() const
{
    return mFileServer;
}

const boost::shared_ptr<LogServer>& Core::GetLogServer() const
{
    return mLogServer;
}

const boost::shared_ptr<ScriptServer>& Core::GetScriptServer() const
{
    return mScriptServer;
}

boost::weak_ptr<Leaf> Core::GetCachedInternal(const CacheKey& key)
{
    // lookup the key in the internal cache
    TPathCache::iterator iter = mPathCache.find(key);
    if (iter != mPathCache.end())
        {
            boost::weak_ptr<Leaf>& entry = (*iter).second;
            if (! entry.expired())
                {
                    return entry;
                }

            // remove entry as it points to an expired node
            mPathCache.erase(key);
        }

    return shared_ptr<Leaf>();
}

void Core::PutCachedInternal(const CacheKey& key, const boost::weak_ptr<Leaf>& leaf)
{
    // update cache; note that we can't cache the fact, that a node is
    // not present as it may be created later on

    if (leaf.expired())
        {
            return;
        }

    mPathCache[key] = leaf;
}

boost::shared_ptr<Leaf> Core::GetUncachedInternal(const CacheKey& key)
{
    // walk the hierarchy
    shared_ptr<Leaf> current;
    Path path(key.path);

    if (
        (path.IsAbsolute()) ||
        (key.root.expired())
        )
        {
            current = mRoot;
        } else
            {
                current = key.root.lock();
            }

    while (
           (! path.IsEmpty()) &&
           (current.get()!=NULL)
           )
        {
            //printf("%s\n", path.Front().c_str());
            current = current->GetChild(path.Front());
            path.PopFront();
        }

    // update cache
    PutCachedInternal(key, current);

    return current;
}

boost::shared_ptr<Leaf> Core::GetInternal(const std::string &pathStr,
                                          const boost::shared_ptr<Leaf>& leaf)
{
    // lookup the path in the internal cache
    CacheKey key(leaf, pathStr);

    boost::weak_ptr<Leaf> cached(GetCachedInternal(key));
    if (! cached.expired())
        {
            return cached.lock();
        }

    return GetUncachedInternal(key);
}

boost::shared_ptr<Leaf> Core::Get(const std::string &pathStr)
{
    return Get(pathStr, mRoot);
}

boost::shared_ptr<Leaf> Core::Get(const std::string &pathStr,
                                  const boost::shared_ptr<Leaf>& leaf)
{
    boost::shared_ptr<Leaf> current = GetInternal(pathStr,leaf);

    if (current.get() == NULL)
        {
            mLogServer->Debug() << "(Core::Get) Could not find object '"
                                << pathStr << "'" << std::endl;
        }

    return current;
}

bool Core::Test(const std::string &pathStr)
{
    return Test(pathStr, mRoot);
}

bool Core::Test(const std::string &pathStr,
                const boost::shared_ptr<Leaf>& leaf)
{
    return (GetInternal(pathStr,leaf).get() != 0);
}


boost::shared_ptr<Leaf> Core::GetChild(const boost::shared_ptr<Leaf> &parent,
                                       const std::string &childName)
{
    // if we have a leaf, then we can't get a child ... ever!!
    if (parent->IsLeaf()) return shared_ptr<Leaf>();

    // ok, no leaf, so let's try to get an existing child
    boost::shared_ptr<Leaf> child = parent->GetChild(childName);

    if (child.get() == NULL)
        {
            // if we hit this branch, then no child with the desired
            // name exists, but we will change this :)
            child = shared_static_cast<Leaf>(mNodeClass->Create());
            child->SetName(childName);

            if (parent->AddChildReference(child) == false)
                {
                    return shared_ptr<Leaf>();
                }
        }

    return child;
}

void Core::BindClass(const boost::shared_ptr<Class> &newClass) const
{
    /* The following condition (true || ...) is always true.
     * The condition right of the || was the original condition, but I had to
     * change it since this caused ClassClass to have no core attached on my system
     * (which in turn made the program crash) -- oliver */
    if (true || newClass != mClassClass)
    {
        newClass->Construct(newClass, mClassClass);
        newClass->AttachTo(mSelf);
    }
}

void Core::GarbageCollectBundles()
{
    TBundleMap::iterator iter = mBundles.begin();

    while (iter != mBundles.end())
        {
            const TBundlePair& entry = (*iter);
            if (entry.second.unique())
                {
                    mBundles.erase(iter);
                    iter = mBundles.begin();
                    continue;
                }

            ++iter;
        }
}
