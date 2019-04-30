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

        CoreContext


        HISTORY:
                05.07.2002 MK
                        - initial version

*/
#ifndef ZEITGEIST_CORECONTEXT_H
#define ZEITGEIST_CORECONTEXT_H

#include <string>
#include <list>
#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include "zeitgeist_defines.h"

namespace zeitgeist
{

class Core;
class Leaf;

/**     This class is responsible for representing a browsing context
 *      within the object hierarchy. It is usually created by the Core
 *      object.
 *
 *      Why was the browsing context separated from the Core?
 *
 *      Well, the usage scenario of the framework envisions multiple clients
 *      browsing the same object hierarchy. Therefore it was necessary to
 *      place the current browsing context into a distinct object.
 */
class ZEITGEIST_API CoreContext
{
    //
    // types
    //
private:
    typedef std::list< boost::shared_ptr<Leaf> > TObjectStack;

    //
    // functions
    //
public:
    /** constructs a core context
     * \param 'core' is the core this context belongs to
     * \param 'root' is the root of the core
     */
    CoreContext(const boost::shared_ptr<Core> &core,
                const boost::shared_ptr<Leaf> &root);
    virtual ~CoreContext();

    /** constructs a new class of type 'className' below the node
     * described by 'pathStr'. The function assumes that only
     * compatible classes are created this way. Compatible means
     * 'starting at Leaf' in the hierarchy. It returns a reference to
     * the constructed object.
     */
    boost::shared_ptr<Leaf> New(const std::string& className,
                                const std::string& pathStr);
    bool Delete(const std::string& name);

    /** selects the currenlty active object to be the one described
        the by path expression pathStr. It returns a reference to the
        selected object*/
    boost::shared_ptr<Leaf> Select(const std::string& pathStr);

    /** inserts the object 'leaf' below the object described by the
        path expression 'pathStr' into the hierarchy and returns true
        on success */
    bool Install(const boost::shared_ptr<Leaf>& leaf, const std::string& pathStr,
                 bool isNamed = false);

    /** returns a reference to the object described by the path
        expression pathStr */
    boost::shared_ptr<Leaf> Get(const std::string& pathStr);

    /** returns true if the object referenced by the path expression
        pathStr exists */
    bool Test(const std::string& pathStr);

    /** returns the currently selected object */
    boost::shared_ptr<Leaf> GetSelection() const { return mObject; }

    /** returns the core this context belongs to */
    boost::shared_ptr<Core> GetCore() const { return mCore; }

    /** prints the children of the currently selected object to
        stdout */
    void ListObjects() const;

    /** pushs the current active object on the object stack */
    void Push();

    /** makes the top of the object stack the current object, no
        change if stack is empty */
    void Pop();

    /** prints the objects on the stack to stdout */
    void Dir() const;

private:
    CoreContext(const CoreContext& obj);
    CoreContext& operator=(const CoreContext& obj);

    //
    // members
    //
private:
    /** pointer to the core object this context belongs to */
    boost::shared_ptr<Core> mCore;

    /** the current active object */
    boost::shared_ptr<Leaf> mObject;

    /** the current working path */
    std::string mPath;

    /** the object stack */
    TObjectStack mObjectStack;
};

} // namespace zeitgeist

#endif //ZEITGEIST_CORECONTEXT_H
