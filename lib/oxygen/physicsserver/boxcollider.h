/* -*- mode: c++ -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: boxcollider.h,v 1.1 2003/08/31 12:16:49 rollmark Exp $

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

   BoxCollider
*/

#ifndef BOXCOLLIDER_H__
#define BOXCOLLIDER_H__

#include "collider.h"

namespace kerosin
{

/** BoxCollider encapsulates an ODE box geometry object.
 */
class BoxCollider : public Collider
{
        //
        // Functions
        //
public:
        BoxCollider();

        /** Sets the side lengths of the box geom */
        void SetBoxLengths(float x, float y, float z);

protected:
    /** constructs a default box with side lengths of 1 */
    virtual bool ConstructInternal();
};

DECLARE_CLASS(BoxCollider);

} //namespace kerosin

#endif //BOXCOLLIDER_H__
