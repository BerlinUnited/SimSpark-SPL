/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: controlaspect.h,v 1.3 2003/11/14 14:05:53 fruit Exp $

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
#ifndef OXYGEN_CONTROLASPECT_H
#define OXYGEN_CONTROLASPECT_H

#include "../sceneserver/basenode.h"
#include "../agentaspect/effector.h"
#include "../agentaspect/perceptor.h"
#include "../agentaspect/agentaspect.h"

namespace oxygen
{

class ControlAspect : public BaseNode
{
public:
    //! called by scene server, before the world is updated
    void        PerformKills();

    //! function used by agents to request effectors
    boost::shared_ptr<Effector> RequestEffector(boost::shared_ptr<AgentAspect>& agent, const std::string& effectorName);
    //! function used by agents to request perceptors
    boost::shared_ptr<Perceptor> RequestPerceptor(boost::shared_ptr<AgentAspect>& agent, const std::string& perceptorName);
protected:
    //! function used by agents to request effectors (this must be implemented to provide custom behavior)
    virtual boost::shared_ptr<Effector> RequestEffectorInternal(boost::shared_ptr<AgentAspect>& agent, const std::string& effectorName) = 0;
    //! function used by agents to request perceptors (this must be implemented to provide custom behavior)
    virtual boost::shared_ptr<Perceptor> RequestPerceptorInternal(boost::shared_ptr<AgentAspect>& agent, const std::string& perceptorName) = 0;

    //! creates an instance of an effector class (used in RequestEffector)
    boost::shared_ptr<Effector> CreateEffector(const std::string& effectorName);
    //! creates an instance of a perceptor class (used in RequestPerceptor)
    boost::shared_ptr<Perceptor> CreatePerceptor(const std::string& perceptorName);

private:
    // this list holds references to agents, which need to be killed in the next frame
    TLeafList   mKillList;
};

DECLARE_ABSTRACTCLASS(ControlAspect);

} // namespace oxygen

#endif //OXYGEN_CONTROLASPECT_H
