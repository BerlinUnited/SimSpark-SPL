/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: agentaspect.h,v 1.7 2003/12/27 17:53:41 fruit Exp $

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
#ifndef OXYGEN_AGENTASPECT_H
#define OXYGEN_AGENTASPECT_H

// #ifdef HAVE_CONFIG_H
// #include <config.h>
// #endif

#include <oxygen/sceneserver/transform.h>
#include <oxygen/gamecontrolserver/actionobject.h>
#include <oxygen/gamecontrolserver/baseparser.h>
#include "effector.h"
#include "perceptor.h"

namespace oxygen
{
#if 0
}
#endif

class AgentAspect : public Transform
{
public:
    AgentAspect();
    virtual ~AgentAspect();

    /** Initializes the AgentAspect. Called immediately after the
        AgentAspect is created by the GameControlServer. \param
        createEffector is the name of the initial effector class that
        the agent uses to construct all remaining parts
     */
    virtual bool Init(const std::string& createEffector);

    /** RealizeActions realizes the actions described by \param
        actions using the corresponding effectors
    */
    virtual bool RealizeActions(boost::shared_ptr<ActionObject::TList> actions);

    /** QuerySensors collects data from all perceptors below this
        AgentAspect
     */
    virtual boost::shared_ptr<Predicate::TList> QueryPerceptors();

    /** updates the map of Effectors below this AgentAspect */
    virtual void UpdateEffectorMap();

    /** looks up the effector corresponding to a predicate */
    virtual boost::shared_ptr<Effector> GetEffector(const std::string predicate) const;

protected:
    typedef std::map<std::string, boost::shared_ptr<Effector> > TEffectorMap;

    //! the map of effectors below this AgentAspect
    TEffectorMap mEffectorMap;

private:
};

DECLARE_CLASS(AgentAspect);

} // namespace oxygen

#endif //OXYGEN_AGENTASPECT_H
