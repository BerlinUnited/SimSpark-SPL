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

#include "transform.h"
#include "sceneserver.h"

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace zeitgeist;

Transform::Transform()
    : BaseNode()
{
    mChangedMark = -1;
    mLocalTransform.Identity();
    mWorldTransform.Identity();

    SetName("transform");
}

Transform::~Transform()
{
}

int Transform::GetChangedMark() const
{
    return mChangedMark;
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
    mChangedMark = SceneServer::GetTransformMark();
    mLocalTransform = transform;
}

void Transform::SetWorldTransform(const salt::Matrix &transform)
{
    boost::shared_ptr<BaseNode> parent = shared_static_cast<BaseNode>
        (mParent.lock());

    if (parent.get() == 0)
        {
            return;
        }

    mChangedMark = SceneServer::GetTransformMark();
    mLocalTransform = (parent->GetWorldTransform());
    mLocalTransform.InvertMatrix();
    mLocalTransform = mLocalTransform * transform;
}

void Transform::SetLocalPos(const salt::Vector3f &pos)
{
    mChangedMark = SceneServer::GetTransformMark();

    mLocalTransform.Pos() = pos;
    UpdateHierarchyInternal();
}

const salt::Vector3f& Transform::GetLocalPos()
{
    return mLocalTransform.Pos();
}

void Transform::SetLocalRotationRad(const salt::Vector3f &rot)
{
    mChangedMark = SceneServer::GetTransformMark();

    Vector3f pos = mLocalTransform.Pos();
    mLocalTransform.RotationX(rot[0]);
    mLocalTransform.RotateY(rot[1]);
    mLocalTransform.RotateZ(rot[2]);
    mLocalTransform.Pos() = pos;

    UpdateHierarchyInternal();
}

void Transform::SetLocalRotationDeg(const salt::Vector3f &rot)
{
    SetLocalRotationRad(Vector3f(
                                 gDegToRad(rot[0]),
                                 gDegToRad(rot[1]),
                                 gDegToRad(rot[2])
                                 )
                        );
}

void Transform::OnLink()
{
    UpdateHierarchyInternal();
}

void Transform::UpdateHierarchyInternal()
{
    boost::shared_ptr<BaseNode> parent = shared_static_cast<BaseNode>(mParent.lock());

    // no parent, return local transform
    if (parent.get() == NULL)
        {
            mWorldTransform = mLocalTransform;
        } else
            {
                mWorldTransform = parent->GetWorldTransform() * mLocalTransform;
            }
}
