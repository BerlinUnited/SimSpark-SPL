/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: scenedict.cpp,v 1.1 2008/02/22 07:52:15 hedayat Exp $

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
#include <oxygen/sceneserver/scenedict.h>

using namespace boost;
using namespace oxygen;
using namespace zeitgeist;

SceneDict::SceneDict()
{
}

SceneDict& SceneDict::GetInstance()
{
    static SceneDict theInstance;
    return theInstance;
}

const SceneDict::FileRef* SceneDict::Lookup(boost::weak_ptr<zeitgeist::Leaf> leaf)
{
    if (leaf.expired())
        {
            return 0;
        }

    TDictionary::const_iterator iter = mDictionary.find(leaf);
    if (iter == mDictionary.end())
        {
            return 0;
        }

    return &((*iter).second);
}

void SceneDict::Insert(boost::weak_ptr<zeitgeist::Leaf> leaf, const FileRef& ref)
{
    if (leaf.expired())
        {
            return;
        }

    mDictionary[leaf] = ref;
}

void SceneDict::Clear()
{
    mDictionary.clear();
}
