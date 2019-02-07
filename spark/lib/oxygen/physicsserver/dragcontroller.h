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
#ifndef OXYGEN_DRAGCONTROLLER_H
#define OXYGEN_DRAGCONTROLLER_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/bodycontroller.h>

namespace oxygen
{

/** \class DragController is a BodyController that applies a linear
    and angular drag the controlled body.
 */
class OXYGEN_API DragController : public BodyController
{
    //
    // Functions
    //
public:
    DragController();
    virtual ~DragController();

    // returns the applied linear drag
    float GetLinearDrag() const;

    // sets the applied linear drag
    void SetLinearDrag(float d);

    // returns the applied angular drag
    float GetAngularDrag() const;

    // sets the applied angular drag
    void SetAngularDrag(float d);

protected:
    /** calculates and applies the force needed to perfom the drag */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

    //
    // Members
    //
protected:
    // the applied linear drag acceleration
    float mLinearDrag;

    // the applied angual drag acceleration
    float mAngularDrag;
};

DECLARE_CLASS(DragController);

} // namespace oxygen


#endif // OXYGEN_DRAGCONTROLLER_H
