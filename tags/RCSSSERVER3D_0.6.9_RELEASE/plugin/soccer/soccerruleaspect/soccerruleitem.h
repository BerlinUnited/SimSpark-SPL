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
#ifndef SOCCERRULEITEM_H
#define SOCCERRULEITEM_H

#include <oxygen/monitorserver/monitoritem.h>
#include "soccerruleaspect.h"

class SoccerRuleItem : public oxygen::MonitorItem
{
public:
    SoccerRuleItem();
    virtual ~SoccerRuleItem();

    /** This function is called once for every MonitorSystem each time
     *  a new client connects. It should append predicates to a list
     *  that is sent using the active monitor
     */
    virtual void GetInitialPredicates(oxygen::PredicateList& pList);

    /** This function will be called periodically to append predicates
        to a list that is sent using the active monitor
     */
    virtual void GetPredicates(oxygen::PredicateList& pList);

    /** sets the reference to the GameStateAspect */
    virtual void OnLink();

    /** resets the reference to the GameStateAspect */
    virtual void OnUnlink();

protected:
    boost::shared_ptr<SoccerRuleAspect> mSoccerRule;
    
    //! Last foul sent out to monitors
    unsigned mLastFoul;

    void AddFoul(SoccerRuleAspect::Foul const& foul, oxygen::PredicateList& pList);

};

DECLARE_CLASS(SoccerRuleItem)

#endif // SOCCERRULEITEM_H

