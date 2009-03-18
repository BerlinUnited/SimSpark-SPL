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
#ifndef OXYGEN_SCENEDICT_H
#define OXYGEN_SCENEDICT_H

#include <oxygen/oxygen_defines.h>
#include <zeitgeist/leaf.h>
#include <map>

namespace oxygen
{

class OXYGEN_API SceneDict
{
public:
    struct FileRef
    {
    public:
        std::string fname;
        unsigned int line;

    public:
        FileRef(const std::string& f = std::string(), unsigned int l = 0)
            : fname(f), line(l)
        {
        }
    };

    typedef std::map<boost::weak_ptr<zeitgeist::Leaf>, FileRef> TDictionary;

public:
    static SceneDict& GetInstance();

    const FileRef* Lookup(boost::weak_ptr<zeitgeist::Leaf> leaf);
    void Insert(boost::weak_ptr<zeitgeist::Leaf> leaf, const FileRef& ref);
    void Clear();

private:
    SceneDict();

protected:
    TDictionary mDictionary;
};

} // namespace oxygen

#endif // OXYGEN_SCENEDICT_H
