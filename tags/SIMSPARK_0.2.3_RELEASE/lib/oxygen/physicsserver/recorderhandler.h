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
#ifndef OXYGEN_RECORDERHANDLER_H
#define OXYGEN_COLLISIONRECORDER_H

#include <oxygen/oxygen_defines.h>
#include <oxygen/physicsserver/collisionhandler.h>
#include <oxygen/physicsserver/collider.h>
#include <set>

namespace oxygen
{
/** \class RecorderHandler is a CollisionHandler that accumulates
     collision information of the Collider it belongs to. It is the
     resonsibility of the user to reset the recorder.
*/
class OXYGEN_API RecorderHandler : public CollisionHandler
{
public:
    typedef std::set<boost::weak_ptr<Collider> > TCollisionSet;
    typedef std::list<boost::weak_ptr<Node> > TParentList;

public:
    RecorderHandler() : CollisionHandler() {};
    virtual ~RecorderHandler() {};

    /** stores the collidee into the internal CollisionSet

    \param collidee is the geom ID of the colliders collision
    partner

    \param holds the contact points between the two affected geoms
    as returned from the ODE dCollide function
    */
    virtual void HandleCollision
    (boost::shared_ptr<Collider> collidee, GenericContact& contact);

    /** removes all stored collisions from the recorder */
    void Clear();

    /** constructs a list of nodes that are parent nodes of the stored
        collidee nodes and are an instance of or are derived from the
        given class.

        \param name is the name of the class the parent nodes must
        support

        \param list is the list that receives the parent nodes
     */
    void GetParentsSupportingClass(const std::string &name, TParentList &list);

    /** same functionality as GetParentsSupportingClass, but using the
        C++ type system instead of string comparisons for improved
        performance

        \param CLASS is the template parameter for the name of the class
        the parent nodes must support

        \param list is the list that receives the parent nodes
     */
    template <class CLASS>
    void FindParentsSupportingClass(TParentList &list)
    {
        for (
            RecorderHandler::TCollisionSet::const_iterator iter = mCollisionSet.begin();
            iter != mCollisionSet.end();
            ++iter
            )
        {
            boost::shared_ptr<oxygen::Collider> collidee = (*iter).lock();
            if (collidee.get() == 0)
                {
                    continue;
                }

            boost::weak_ptr<zeitgeist::Node> parent =
                collidee->FindParentSupportingClass<CLASS>();

            if (! parent.expired())
                {
                    list.push_back(parent);
                }
        }
    }

protected:
    TCollisionSet mCollisionSet;
};

    DECLARE_CLASS(RecorderHandler);

} //namespace oxygen

#endif // OXYGEN_COLLISIONRECORDER_H
