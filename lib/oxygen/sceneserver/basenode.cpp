/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: basenode.cpp,v 1.4.6.1 2003/12/26 11:31:51 rollmark Exp $

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

#include "basenode.h"
#include "scene.h"

using namespace boost;
using namespace oxygen;
using namespace salt;
using namespace zeitgeist;

salt::Matrix BaseNode::mIdentityMatrix;

BaseNode::BaseNode() :
zeitgeist::Node(), mDebugMode(false)
{
        // a bit redundant, but somehow we have to ensure that this is ALWAYS set
        mIdentityMatrix.Identity();
}

BaseNode::~BaseNode()
{
}

const salt::Matrix& BaseNode::GetLocalTransform() const
{
        return mIdentityMatrix;
}

const salt::Matrix& BaseNode::GetWorldTransform() const
{
        shared_ptr<BaseNode> parent = shared_static_cast<BaseNode>(make_shared(mParent));

        // no parent, return identity
        if (parent.get() == NULL)
                return mIdentityMatrix;
        else
                return parent->GetWorldTransform();
}

void BaseNode::SetLocalTransform(const salt::Matrix& /*transform*/)
{
}

void BaseNode::SetWorldTransform(const salt::Matrix& /*transform*/)
{
}

/*!
        This routine is responsible for updating the local bounding box of the
        node. The default behavior is to treat the node as a point!

        Please note that this is always called from Compute()!
*/
void BaseNode::ComputeBoundingBox()
{
        mLocalBoundingBox.minVec.Set(0,0,0);
        mLocalBoundingBox.maxVec.Set(0,0,0);
}

void BaseNode::PrePhysicsUpdate(float deltaTime)
{
        // perform update on hierarchy
        for (TLeafList::iterator i = begin(); i!= end(); ++i)
        {
                if ((*i)->GetClass()->Supports("BaseNode"))
                        (shared_static_cast<BaseNode>(*i))->PrePhysicsUpdate(deltaTime);
        }

        // do the internal update ... can be overridden by derived classes to add
        // custom behavior
        PrePhysicsUpdateInternal(deltaTime);

        ComputeBoundingBox();
}

void BaseNode::PostPhysicsUpdate()
{
        // perform update on hierarchy
        for (TLeafList::iterator i = begin(); i!= end(); ++i)
        {
                if ((*i)->GetClass()->Supports("BaseNode"))
                        (shared_static_cast<BaseNode>(*i))->PostPhysicsUpdate();
        }

        // do the internal update ... can be overridden by derived classes to add
        // custom behavior
        PostPhysicsUpdateInternal();
}

void BaseNode::UpdateHierarchy()
{
        // do the internal update ... can be overridden by derived classes to add
        // custom behavior
        UpdateHierarchyInternal();

        // generate the bounding volume of this node
        Matrix worldTransform = GetWorldTransform();
        mWorldBoundingBox = mLocalBoundingBox;
        mWorldBoundingBox.TransformBy(worldTransform);

        // perform update on hierarchy
        for (TLeafList::iterator i = begin(); i!= end(); ++i)
        {
                if ((*i)->GetClass()->Supports("BaseNode"))
                {
                        shared_ptr<BaseNode> node = shared_static_cast<BaseNode>(*i);
                        node->UpdateHierarchy();
                        // here we merge our world bounding volume with the child volumes
                        mWorldBoundingBox.Encapsulate(node->GetWorldBoundingBox());
                }
        }
}

void BaseNode::Render()
{
#if 0
        // do the internal update ... can be overridden by derived classes to add
        // custom behavior
        glPushMatrix();
        glMultMatrixf(GetWorldTransform().m);
#endif
        RenderInternal();
#if 0
        glPopMatrix();
#endif
        // perform update on hierarchy
        for (TLeafList::iterator i = begin(); i!= end(); ++i)
        {
                if ((*i)->GetClass()->Supports("BaseNode"))
                        (shared_static_cast<BaseNode>(*i))->Render();
        }
}

void BaseNode::RenderAmbient()
{
#if 0
        // do the internal update ... can be overridden by derived classes to add
        // custom behavior
        glPushMatrix();
        glMultMatrixf(GetWorldTransform().m);
#endif
        RenderAmbientInternal();
#if 0
        glPopMatrix();
#endif
        // perform update on hierarchy
        for (TLeafList::iterator i = begin(); i!= end(); ++i)
        {
                if ((*i)->GetClass()->Supports("BaseNode"))
                        (shared_static_cast<BaseNode>(*i))->RenderAmbient();
        }
}

boost::shared_ptr<Scene> BaseNode::GetScene()
{
  return shared_dynamic_cast<Scene>(GetParentSupportingClass("Scene"));
}

void BaseNode::EnableDebugMode()
{
        mDebugMode = true;
}

void BaseNode::DisableDebugMode()
{
        mDebugMode = false;
}

void BaseNode::PrePhysicsUpdateInternal(float /*deltaTime*/)
{
}

void BaseNode::PostPhysicsUpdateInternal()
{
}

void BaseNode::UpdateHierarchyInternal()
{
}

void BaseNode::RenderInternal()
{
}

void BaseNode::RenderAmbientInternal()
{
}
