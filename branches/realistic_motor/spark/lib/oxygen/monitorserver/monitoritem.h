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
#ifndef OXYGEN_MONITORITEM_H
#define OXYGEN_MONITORITEM_H

#include <zeitgeist/leaf.h>
#include <oxygen/gamecontrolserver/predicate.h>

namespace oxygen
{

class MonitorItem : public zeitgeist::Leaf
{
public:
    MonitorItem() : Leaf() {}
    virtual ~MonitorItem() {}

    /** This function is called once for every MonitorSystem each time
     *  a new client connects. It should append predicates to a list
     *  that is sent using the active monitor
     */
    virtual void GetInitialPredicates(PredicateList& pList) = 0;

    /** This function will be called periodically to append predicates
        to a list that is sent using the active monitor
     */
    virtual void GetPredicates(PredicateList& pList) = 0;
};

DECLARE_ABSTRACTCLASS(MonitorItem);

} // namespace oxygen

#endif // OXYGEN_MONITORITEM_H
