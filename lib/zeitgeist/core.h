/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: core.h,v 1.5 2004/02/12 14:07:23 fruit Exp $

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
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

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
class Core
{
    //
    // functions
    //
public:
    Core();
    ~Core();

    /** Constructs the core, i.e. sets up the root object, registers
        some class objects for internal zeitgeist classes and
        creates servers for the basic services
    */
    void Construct(const boost::weak_ptr<Core>& self);

    /** creates a context for this Core */
    boost::shared_ptr<CoreContext> CreateContext();

    /** creates an instance of the class 'className' */
    boost::shared_ptr<Object> New(const std::string& className);

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
    const boost::shared_ptr<FileServer>&    GetFileServer() const;

    /** returns a reference to the LogServer */
    const boost::shared_ptr<LogServer>&             GetLogServer() const;

    /** returns a reference to the ScriptServer */
    const boost::shared_ptr<ScriptServer>&  GetScriptServer() const;

    /** returns a reference to the root node */
    boost::shared_ptr<Leaf> GetRoot() const {       return mRoot;   }

    /** returns a reference to the object denoted by the path expression 'pathStr'. */
    boost::shared_ptr<Leaf> Get(const std::string &pathStr);

    /** returns a reference to the object denoted by the path
        expression 'pathStr', relative to the node base.
    */
    boost::shared_ptr<Leaf> Get(const std::string &pathStr, const boost::shared_ptr<Leaf>& base);

    /** returns a reference to a child node of 'parent' with the
        name 'childName'
    */
    boost::shared_ptr<Leaf> GetChild(const boost::shared_ptr<Leaf> &parent, const std::string &childName);

    /** attaches the class 'newClass' to this core */
    void BindClass(const boost::shared_ptr<Class> &newClass) const;

    /** releases all unused bundles, i.e. bundles the core holds the only reference to */
    void GarbageCollectBundles();

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

    /** a list of all registered bundles. Whenever we load a
        bundle, we add it to this list. This is necessary, because
        we have to keep the connection to each bundle open, until
        all the class objects belonging to it are completely
        destructed.

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
    std::list<boost::shared_ptr<salt::SharedLibrary> >      mBundles;
};

} //namespace zeitgeist

#endif //ZEITGEIST_CORE_H
