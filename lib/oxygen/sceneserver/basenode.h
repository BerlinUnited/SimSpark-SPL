/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: basenode.h,v 1.5 2003/11/14 14:05:53 fruit Exp $

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

   BaseNode

        NOTE:

        HISTORY:
                05.11.02 - MK
                        - Initial version

        TODO:

        TOFIX:
*/
#ifndef OXYGEN_BASENODE_H
#define OXYGEN_BASENODE_H

#include <salt/matrix.h>
#include <salt/bounds.h>
#include <zeitgeist/node.h>

namespace oxygen
{

class Scene;

/** BaseNode is the base class for all nodes which are part of the scene
    hierarchy.  It's Hierarchy functionality (children, naming, etc..) is
    inherited from zeitgeist. It does NOT have an explicit local and world
    transform.
*/

class BaseNode : public zeitgeist::Node
{
    //
    // Functions
    //
public:
    BaseNode();
    ~BaseNode();

    // transformation related

    /** return the local transform of this node (default: returns
     * identity) */
    virtual const salt::Matrix&     GetLocalTransform()     const;

    /** returns the world transform of this node (default: returns parents
     * world transform) */
    virtual const salt::Matrix&     GetWorldTransform()     const;

    /** sets the local transform of this node (default: ignored) */
    virtual void SetLocalTransform(const salt::Matrix &transform);

    /** sets the world transform of this node (default: ignored) */
    virtual void SetWorldTransform(const salt::Matrix &transform);

    // bounding box related

    /** computes the local bounding box of the node */
    virtual void    ComputeBoundingBox();

    /** returns the world bounding box of this node */
    const salt::AABB3&      GetWorldBoundingBox() const
    {       return mWorldBoundingBox;               }

    // scene graph update passes

    /** updates internal state before physics calculation */
    void PrePhysicsUpdate(float deltaTime);

    /** updates internal state after physics calculation */
    void PostPhysicsUpdate();

    /** update hierarchical date (position, bounding volumes, etc..) */
    void UpdateHierarchy();

    // scene graph rendering
    void Render();

    void RenderAmbient();

    /** moves up the hierarchy, until it finds a scene */
    boost::shared_ptr<Scene>        GetScene();

    /** enables debug mode controls */
    void EnableDebugMode();

    /** disabled debug mode controls */
    void DisableDebugMode();

    /** renders the node */
    virtual void RenderInternal();

    /** renders node */
    virtual void RenderAmbientInternal();

private:
    /** updates internal state before physics calculation */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    /** updates internal state after physics calculation */
    virtual void PostPhysicsUpdateInternal();

    /** updates hierarchical date (position, bounding volumes, etc..) */
    virtual void UpdateHierarchyInternal();

    //
    // Members
    //
protected:
    /** the identity matrix */
    static salt::Matrix             mIdentityMatrix;

    /** debug mode (for additional visualization) */
    bool                                    mDebugMode;

    /** local bounding box */
    salt::AABB3                             mLocalBoundingBox;

    /** world bounding box */
    salt::AABB3                             mWorldBoundingBox;
};

DECLARE_CLASS(BaseNode);

} //namespace oxygen
#endif //OXYGEN_BASENODE_H
