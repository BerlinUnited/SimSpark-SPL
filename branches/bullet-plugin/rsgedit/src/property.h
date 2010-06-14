/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef PROPERTY_H__
#define PROPERTY_H__

#ifdef WIN32
#include <winsock2.h>
#endif

#include <list>
#include <map>
#include <boost/weak_ptr.hpp>
#include <wx/string.h>

namespace zeitgeist
{
    class Class;
    class Leaf;
}

class Property
{
public:
    enum EClass
        {
            CL_LEAF,
            CL_CLASS,
            CL_BASENODE,
            CL_TRANSFORM,
            CL_RIGIDBODY,
            CL_JOINT,
            CL_HINGEJOINT,
            CL_UNIVERSALJOINT,
            CL_WORLD,
            CL_COLLISIONHANDLER,
            CL_CONTACTJOINTHANDLER,
            CL_DRAGCONTROLLER,
            CL_MATERIALSOLID
        };

    //! mapping from path of zeitgeist class to EClass value
    typedef std::map<wxString, EClass> TClassMap;

    //! list of base class types
    typedef std::list<EClass> TClassList;

    struct Entry
    {
    public:
        wxString name;
        wxString value;

    public:
        Entry(const wxString& n, const wxString& v)
            : name(n), value(v) {}
    };

    typedef std::list<Entry> TEntryList;

public:
    Property();

    void GetClassList(boost::shared_ptr<zeitgeist::Leaf> leaf, TClassList& clList) const;
    void GenEntries(boost::shared_ptr<zeitgeist::Leaf> leaf,
                    const TClassList& clList, TEntryList& entries) const;

protected:
    void GenLeafEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenClassEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenBaseNodeEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenTransformEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenBodyEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenHingeEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenUniversalEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenJointEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenWorldEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenCollisionHandlerEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenContactJointEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenDragControllerEntries(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;
    void GenSolidMaterialProperty(boost::shared_ptr<zeitgeist::Leaf> leaf, TEntryList& entries) const;

    void GetClassList(boost::shared_ptr<zeitgeist::Class> cl, TClassList& clList) const;
    void Init();

protected:
    TClassMap mClassMap;
};

#endif // PROPERTY_H__
