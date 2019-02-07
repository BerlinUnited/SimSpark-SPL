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
#ifndef OXYGEN_AGENTASPECT_H
#define OXYGEN_AGENTASPECT_H

#include <oxygen/oxygen_defines.h>
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

class OXYGEN_API AgentAspect : public Transform
{
public:
    AgentAspect();
    virtual ~AgentAspect();

    /** Initializes the AgentAspect. Called immediately after the
        AgentAspect is created by the GameControlServer.
       \param createEffector is the name of the initial effector class that
              the agent uses to construct all remaining parts
       \param id a unique ID to set
     */
    virtual bool Init(const std::string& createEffector, int id);

    /** RealizeActions realizes the actions described by \param
        actions using the corresponding effectors
    */
    virtual bool RealizeActions(boost::shared_ptr<ActionObject::TList> actions);

    /** QuerySensors collects data from all perceptors below this
        AgentAspect
     */
    virtual boost::shared_ptr<PredicateList> QueryPerceptors();

    /** updates the map of Effectors below this AgentAspect */
    virtual void UpdateEffectorMap();

    /** looks up the effector corresponding to a predicate */
    virtual boost::shared_ptr<Effector> GetEffector(const std::string predicate) const;

    void UpdateCacheInternal();
    
    //! @return the unique ID for the agent aspect
    inline int ID() const { return mID; }

    /** @return if the agent is in sync with the server in Sync mode*/
    bool IsSynced() const;

    /** sets the synchronization status of the agent */
    void SetSynced(bool synced) { mIsSynced = synced; }

protected:
    typedef std::map<std::string, boost::shared_ptr<Effector> > TEffectorMap;

    //! the map of effectors below this AgentAspect
    TEffectorMap mEffectorMap;

private:
    int mID;

    /** cache of the agent's effectors */
    TLeafList mPerceptors;
    
    /** indicates how many times the QueryPerceptors be called */
    unsigned int mPerceptorCycle;

    /** show if the agent is in sync with the server (in Sync mode) */
    bool mIsSynced;
};

DECLARE_CLASS(AgentAspect);

} // namespace oxygen

#endif //OXYGEN_AGENTASPECT_H
