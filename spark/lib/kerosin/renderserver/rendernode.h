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

   RenderNode
*/
#ifndef KEROSIN_RENDERNODE_H
#define KEROSIN_RENDERNODE_H

#include <oxygen/sceneserver/basenode.h>
#include <kerosin/kerosin_defines.h>

namespace kerosin
{
/** RenderNode is the base class for all nodes which are part of the
    scene hierarchy and are able to render itself.
*/
class KEROSIN_API RenderNode : public oxygen::BaseNode
{
    //
    // Functions
    //
public:
    RenderNode();
    virtual ~RenderNode();

    /** renders the node */
    virtual void RenderInternal() = 0;

    bool IsTransparent() { return mTransparent; }
    void SetTransparent();
    
    void SetVisible(bool visible);
    bool IsVisible() const { return mVisible; }
    bool VisibleToggled();
    
private:
    bool mTransparent;
    bool mVisible;
    bool mVisibleToggled;
};

DECLARE_ABSTRACTCLASS(RenderNode);

} //namespace kerosin

#endif //KEROSIN_RENDERNODE_H
