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
#ifndef ZEITGEIST_CORE_H
#define ZEITGEIST_CORE_H

#include <string>
#include <list>
#include <map>
#include <set>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif
#include "zeitgeist_defines.h"

namespace salt
{
class SharedLibrary;
}

namespace zeitgeist
{

class Class;
class Leaf;
class Node;
class FileServer;
class LogServer;
class ScriptServer;
class CoreContext;
class Object;
class RandomServer;

/** The Core is the hierarchy, i.e. it creates the root node, some
    basic class objects and installs fundamental server classes needed
    to run zeitgeist system. Further it provides a mechanism to import
    bundles- plugins containg new classes, that can be registered to
    the framework at runtime.
 */
class ZEITGEIST_API Core
{
public:
    //
    // types
    //

    /** CacheKey is a struct used as the key for the internal node
        lookup cache
    */
    struct CacheKey
    {
    public:
        //! node to which the path of this key is relative to
        boost::weak_ptr<Leaf> root;

        //! the path expression of this key
        std::string path;

    public:
        CacheKey()
        {}

        CacheKey(boost::weak_ptr<Leaf> r, const std::string& p) : root(r), path(p)
        {};

        bool operator == (const CacheKey& key) const;
        bool operator < (const CacheKey& key) const;
    };

    /** CachedLeafPath defines a pair of path key and it's
        corresponding weak reference. It acts like a smart pointer
        that is able to update the keep the managed reference up to
        date with the help of the core and the CacheKey
    */
    struct CachedLeafPath
    {
    protected:
        CacheKey key;
        boost::weak_ptr<Leaf> leaf;

    public:
        // need to have virtual destructor
        virtual ~CachedLeafPath() {}

        const CacheKey& GetKey()
        { return key; }

        void SetKey(const CacheKey& k)
        { key = k; }

        boost::weak_ptr<Leaf>& GetLeaf()
        { return leaf; }

        virtual void Cache(boost::shared_ptr<Core> core, const std::string& pathStr) = 0;
        virtual void Update(boost::shared_ptr<Core> core) = 0;

        bool operator == (const CachedLeafPath& p)
        {
            return (key == p.key);
        }

        bool operator < (const CachedLeafPath& p)
        {
            return (key < p.key);
        }

        bool expired()
        {
            return leaf.expired();
        }

        void reset()
        {
            leaf.reset();
        }
    };

    template<typename _CLASS>
    struct CachedPath : public CachedLeafPath
    {
    public:
        CachedPath() : CachedLeafPath()
        {}

        CachedPath(const CacheKey& k, const boost::weak_ptr<_CLASS> &l)
            : CachedLeafPath(k,l)
        {}

        virtual void Cache(boost::shared_ptr<Core> core, const std::string& pathStr)
        {
            if (core.get() == NULL)
                {
                    leaf.reset();
                    return;
                }

            key = CacheKey(core->mRoot, pathStr);
            Update(core);
        }

        virtual void Update(boost::shared_ptr<Core> core)
        {
            if (core.get() == NULL)
                {
                    leaf.reset();
                    return;
                }

            // lookup the path in the internal core cache
            boost::weak_ptr<Leaf> lookup = core->GetCachedInternal(key);
            if (! lookup.expired())
                {
                    leaf = boost::dynamic_pointer_cast<_CLASS>(lookup.lock());
                    return;
                }

            leaf = boost::dynamic_pointer_cast<_CLASS>
                (core->GetUncachedInternal(key));
        }

        boost::shared_ptr<_CLASS> get()
        {
            return boost::static_pointer_cast<_CLASS>(leaf.lock());
        }

        _CLASS* operator -> ()
        {
            return boost::static_pointer_cast<_CLASS>(leaf.lock()).get();
        }
    };

protected:
    /** TPathCache defines a mapping from a CacheKey to a weak
        reference of the corresponding Leaf
    */
    typedef std::map<CacheKey, boost::weak_ptr<Leaf> > TPathCache;

    /** TBundleMap defines a registry of loaded bundles */
    typedef std::pair<std::string, boost::shared_ptr<salt::SharedLibrary> > TBundlePair;
    typedef std::map<std::string, boost::shared_ptr<salt::SharedLibrary> > TBundleMap;

    //
    // functions
    //
public:
    Core();
    ~Core();

    /** Constructs the core, i.e. sets up the root object, registers
        some class objects for internal zeitgeist classes and creates
        servers for the basic services
    */
    void Construct(const boost::weak_ptr<Core>& self);

    /** Destructs the core, i.e. unlinks the hierarchy below the root
        object */
    void Desctruct();

    /** Resets the (strong) self pointer. */
    void Remove();

    /** creates a context for this Core */
    boost::shared_ptr<CoreContext> CreateContext();

    /** creates an instance of the class 'className' */
    boost::shared_ptr<Object> New(const std::string& className);

    /** returns true iff the given class exists */
    bool ExistsClass(const std::string& className);

    /** registers a class object
     * \param classObject is a reference to instance of the class object
     * \param subDir gives the directory below "/classes" where the
     * class object will be installed
     */
    bool RegisterClassObject(const boost::shared_ptr<Class> &classObject, const std::string &subDir);

    /** registers a class object
     * \param classObject is a reference to instance of the class object
     * \param subDir gives the directory below "/classes" where the
     * class object will be installed
     */
    bool RegisterClassObject(Class *classObject, const std::string &subDir);

    /** imports a bundle, i.e. registers classes contained inside
     *  a plugin to the framework.
     * \param bundleName is the filneName of the bundle
     */
    bool ImportBundle(const std::string& bundleName);

    /** returns a reference to the FileServer */
    const boost::shared_ptr<FileServer>& GetFileServer() const;

    /** returns a reference to the LogServer */
    const boost::shared_ptr<LogServer>& GetLogServer() const;

    /** returns a reference to the ScriptServer */
    const boost::shared_ptr<ScriptServer>& GetScriptServer() const;

    /** returns a reference to the root node */
    boost::shared_ptr<Leaf> GetRoot() const { return mRoot;  }

    /** returns a reference to the object denoted by the path
        expression 'pathStr'.
    */
    boost::shared_ptr<Leaf> Get(const std::string &pathStr);

    /** returns a reference to the object denoted by the path
        expression 'pathStr', relative to the node base.
    */
    boost::shared_ptr<Leaf> Get(const std::string &pathStr,
                                const boost::shared_ptr<Leaf>& base);

    /** returns true iff the object denoted by the path expression
        'pathStr', relative to the node base exists
     */
    bool Test(const std::string &pathStr,
              const boost::shared_ptr<Leaf>& base);

    /** returns true iff the object denoted by the path expression
        'pathStr' exists.
     */
    bool Test(const std::string &pathStr);

    /** returns a reference to a child node of 'parent' with the name
        'childName'
    */
    boost::shared_ptr<Leaf> GetChild(const boost::shared_ptr<Leaf> &parent,
                                     const std::string &childName);

    /** attaches the class 'newClass' to this core */
    void BindClass(const boost::shared_ptr<Class> &newClass) const;

    /** releases all unused bundles, i.e. bundles the core holds the
        only reference to */
    void GarbageCollectBundles();

protected:
    /** returns a cached reference to the Leaf corresponding to the
        given key. If the cached reference expired the entry is
        removed from the cache
    */
    boost::weak_ptr<Leaf> GetCachedInternal(const CacheKey& key);

    /** returns a reference to the object denoted by the path
        expression 'pathStr', relative to the node base. This method
        does not lookup for a cached reference but does update the
        cache accordingly
    */
    boost::shared_ptr<Leaf> GetUncachedInternal(const CacheKey& key);

    /** checks that the leaf reference isn't expired and inserts the
        given key,leaf pair into the cache
    */
    void PutCachedInternal(const CacheKey& key, const boost::weak_ptr<Leaf>& leaf);

    /** returns a reference to the object denoted by the path
        expression 'pathStr', relative to the node base.
    */
    boost::shared_ptr<Leaf> GetInternal(const std::string &pathStr,
                                        const boost::shared_ptr<Leaf>& base);

    /** signal handler */
    static void CatchSignal(int sig_num);

    //
    // members
    //
private:

    /** a reference to the root node */
    boost::shared_ptr<Leaf>                 mRoot;

    /** a reference to the class object of the node class */
    boost::shared_ptr<Class>                mNodeClass;

    /** a reference to the class object of the 'class' class */
    boost::shared_ptr<Class>                mClassClass;

    /** a reference to the file server */
    boost::shared_ptr<FileServer>   mFileServer;

    /** a reference to the logserver */
    boost::shared_ptr<LogServer>    mLogServer;

    /** a reference to the scriptserver */
    boost::shared_ptr<ScriptServer> mScriptServer;

    /** a reference to the randomserver */
    boost::shared_ptr<RandomServer> mRandomServer;

    /** a reference to the core, a smart this pointer */
    boost::weak_ptr<Core>                   mSelf;

    /** a registry of all registered bundles. Whenever we load a
        bundle, we add it to this map. This is necessary, because we
        have to keep the connection to each bundle open, until all the
        class objects belonging to it are completely destructed.

        This workaround is very ugly ... it cost me about 3-4
        hours of debugging to get this to work. Somehow, when I
        destroyed a class belonging to a bundle, it prevented the
        rest of the hierarchy deletion from working ... ouch. I
        have no idea, why this happened. Now, when I keep the
        bundles (DLLs,SOs) open for a while longer, then everything is
        nice and dandy.

        NOTE: I had this first as a member of Core, but I can't
        control the order in which the members are released and I
        would have to guarantee that gBundles gets deleted AFTER
        mRoot. Switching the order the variables are defined would
        probably take care of that, but sounds quite unportable,
        so I went with this option (making it global). I have
        added a 'GarbageCollectBundles' function, which erases
        'unique' references.

        WARNING: Just making this global is (apparently) not
        enough.
    */
    TBundleMap mBundles;

    //! the internal node lookup cache
    TPathCache mPathCache;
};

} //namespace zeitgeist

#endif //ZEITGEIST_CORE_H
