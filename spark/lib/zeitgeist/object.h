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

   Object

        HISTORY:
                31.08.2002 MK
                        - initial version
*/
#ifndef ZEITGEIST_OBJECT_H
#define ZEITGEIST_OBJECT_H

#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include "core.h"
#include "parameterlist.h"

namespace zeitgeist
{

class Class;
class LogServer;
class ScriptServer;

/** Object is the base class of all objects in the Zeitgeist
 *  framework. An object is characterized by a single thing, the class
 *  which created it. The core it belongs to can be reached via the
 *  class object. Every object holds a boost::shared_ptr to the class object,
 *  which created it.
*/

class ZEITGEIST_API Object
{
    //
    // functions
    //
public:
    /** constructs an object */
    Object();
    virtual ~Object();

    /** sets up the internal state of the object, used by Class::Create().
     * \param self is a pointer to the instance of the object, i.e. a smart this pointer
     * \param creator is a pointer to the class object that created this object
     */
    bool Construct(const boost::shared_ptr<Object>& self, const boost::shared_ptr<Class>& creator);

    // class object
    /** returns the corresponding class object */
    boost::shared_ptr<Class>        GetClass() const;

    /** returns a pointer to the object */
    boost::weak_ptr<Object>&                GetSelf();

    /** returns a constant pointer to the object */
    const boost::weak_ptr<Object>&  GetSelf() const;

    /** return a pointer to the core, this object belongs to */
    boost::shared_ptr<Core>         GetCore() const;

    /** helper function to get a reference to the FileServer */
    const boost::shared_ptr<FileServer>&    GetFile() const;

    /** helper function to get a reference to the LogServer */
    const boost::shared_ptr<LogServer>&             GetLog() const;

    /** helper function to get a reference to the ScriptServer */
    const boost::shared_ptr<ScriptServer>&  GetScript() const;

    /** prints debug output to stdout */
    virtual void                            Dump() const;

    /** function invocation used for scripting */
    void Invoke(const std::string &functionName, const ParameterList& parameter);

protected:
    /** called after the object has been created and attached to a
        core to allow for object dependent internal
        construction */
    virtual bool ConstructInternal();

    //
    // members
    //
private:
    /** pointer to the class object which created this instance */
    boost::shared_ptr<Class>                mClass;

    /** pointer to the instance of the object */
    boost::weak_ptr<Object>                 mSelf;
};

} //namespace zeitgeist;

#endif //ZEITGEIST_OBJECT_H
