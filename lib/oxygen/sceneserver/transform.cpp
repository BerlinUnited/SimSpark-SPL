/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: transform.cpp,v 1.4 2004/03/22 11:02:32 rollmark Exp $

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

#include "transform.h"

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace zeitgeist;

Transform::Transform() :
BaseNode()
{
        mLocalTransform.Identity();
        mWorldTransform.Identity();

        SetName("transform");
}

Transform::~Transform()
{
}

const salt::Matrix& Transform::GetLocalTransform() const
{
        return mLocalTransform;
}

const salt::Matrix& Transform::GetWorldTransform() const
{
        return mWorldTransform;
}

void Transform::SetLocalTransform(const salt::Matrix &transform)
{
        mLocalTransform = transform;
}

void Transform::SetWorldTransform(const salt::Matrix &transform)
{
        mWorldTransform = transform;
}

void Transform::SetLocalPos(const salt::Vector3f &pos)
{
        mLocalTransform.Pos() = pos;
        UpdateHierarchyInternal();
}

void Transform::OnLink()
{
        UpdateHierarchyInternal();
}

void Transform::UpdateHierarchyInternal()
{
        shared_ptr<BaseNode> parent = shared_static_cast<BaseNode>(make_shared(mParent));

        // no parent, return identity
        if (parent.get() == NULL)
                mWorldTransform = mLocalTransform;
        else
                mWorldTransform = parent->GetWorldTransform() * mLocalTransform;
}
