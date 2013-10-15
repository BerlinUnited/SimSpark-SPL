/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: collisionperceptor.h 3 2008-11-21 02:38:08Z hedayat $

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
#ifndef COLLISIONPERCEPTOR_H
#define COLLISIONPERCEPTOR_H

#include <oxygen/agentaspect/perceptor.h>

/** \class CollisionPerceptor is used to store collisions as they
    occur within the scenegraph. It is inteded to be used together
    with the PerceptorHandler that passes collision information from
    the physics system on to this perceptor.
*/
class CollisionPerceptor : public oxygen::Perceptor
{
public:
    /** \param predicate set "collidees" as a TLeafList as arguments of predicate
        \return true if data is available
     */
    virtual bool Percept(boost::shared_ptr<oxygen::PredicateList> predList);

    /** returns the stored list of collidees */
    const zeitgeist::Leaf::TLeafList& GetCollidees()  { return mCollidees; }

    /** stores a new collidee */
    void AddCollidee(boost::shared_ptr<zeitgeist::Node> collidee);

protected:
    /** removes old collidees left over from the previous cycles */
    virtual void PrePhysicsUpdateInternal(float deltaTime);

protected:
    zeitgeist::Leaf::TLeafList mCollidees;

};

DECLARE_CLASS(CollisionPerceptor);

#endif //COLLISIONPERCEPTOR_H
