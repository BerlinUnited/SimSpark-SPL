/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: collisionperceptor.h,v 1.6.2.1 2004/01/09 13:13:08 rollmark Exp $

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

#include "perceptor.h"

namespace oxygen
{
#if 0
}
#endif

/*!
        \class CollisionPerceptor is used to store collisions as they
        occur within the scenegraph.
*/
class CollisionPerceptor : public Perceptor
{
public:
    /** \param predicate set "collidees" as a TLeafList as arguments of predicate
        \return true if data is available
     */
    bool Percept(Predicate& predicate);

    /** returns the stored list of collidees */
    const TLeafList& GetCollidees()  { return mCollidees; }

    /** stores a new collidee */
    void AddCollidee(boost::shared_ptr<Node> collidee);

protected:
    /** removes old collidees left over from the previous cycles */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

private:
    TLeafList mCollidees;
};

DECLARE_CLASS(CollisionPerceptor);

}
#endif //OXYGEN_COLLISIONPERCEPTOR_H
