/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: collisionperceptor.h,v 1.2 2003/08/31 21:53:45 fruit Exp $

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
#ifndef OXYGEN_COLLISIONPERCEPTOR_H
#define OXYGEN_COLLISIONPERCEPTOR_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "perceptor.h"

namespace oxygen
{
#if 0
}
#endif

/*!
        The CollisionPerceptor class is used to filter collisions as they occur
        within the scenegraph.
*/
class CollisionPerceptor : public Perceptor
{
    //
    // functions
    //
public:
    /*
      Returns:
      "collidees" as a TLeafList
    */
    virtual bool Percept(TDictionary &dictionary);

    TLeafList& GetCollidees() {     return mCollidees;      }

private:
    TLeafList mCollidees;
};

DECLARE_CLASS(CollisionPerceptor);

}
#endif //OXYGEN_COLLISIONPERCEPTOR_H
