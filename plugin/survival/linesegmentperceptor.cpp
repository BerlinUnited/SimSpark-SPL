/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: linesegmentperceptor.cpp,v 1.2.2.1 2003/11/19 19:07:38 rollmark Exp $

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
#include "linesegmentperceptor.h"
#include <oxygen/sceneserver/scene.h>
#include <kerosin/sceneserver/staticmesh.h>

using namespace boost;
using namespace kerosin;
using namespace salt;

LineSegmentPerceptor::LineSegmentPerceptor()
{
        mStart.Set(0,0,0);
        mEnd.Set(0,0,0);
}

void LineSegmentPerceptor::SetLineSegment(const salt::Vector3f& start, const salt::Vector3f& end)
{
        mStart  = start;
        mEnd    = end;
}

bool LineSegmentPerceptor::Percept(TDictionary &dictionary)
{
        // update control aspects
        TLeafList meshes;
        TLeafList::iterator i;

        GetScene()->GetChildrenSupportingClass("StaticMesh", meshes, true);
        shared_ptr<StaticMesh> self = shared_static_cast<StaticMesh>(make_shared(make_shared(GetParent())->GetParent())->GetChildOfClass("StaticMesh"));

        Vector3f start, end, ret;

        dictionary["hit"] = false;
        for (i = meshes.begin(); i != meshes.end(); ++i)
        {
                if (self.get() != (*i).get())
                {
                        if (shared_static_cast<StaticMesh>(*i)->Intersect(mStart, mEnd, ret))
                        {
                                dictionary["hit"] = true;
                                dictionary["p"] = ret;
                        }
                }
        }
        return true;
}
