/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: recorderhandler.cpp,v 1.3 2007/06/15 09:47:29 jboedeck Exp $

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
#include "recorderhandler.h"

using namespace oxygen;
using namespace boost;

void
RecorderHandler::HandleCollision
(boost::shared_ptr<Collider> collidee, dContact& /*contact*/)
{
    mCollisionSet.insert(weak_ptr<Collider>(collidee));
}

void
RecorderHandler::Clear()
{
    mCollisionSet.clear();
}

void
RecorderHandler::GetParentsSupportingClass
(const std::string &name, TParentList &list)
{
    for (
         RecorderHandler::TCollisionSet::const_iterator iter = mCollisionSet.begin();
         iter != mCollisionSet.end();
         ++iter
         )
        {
            shared_ptr<Collider> collidee = (*iter).lock();
            if (collidee.get() == 0)
                {
                    continue;
                }

            weak_ptr<Node> parent =
                collidee->GetParentSupportingClass(name);

            if (! parent.expired())
                {
                    list.push_back(parent);
                }
        }
}
