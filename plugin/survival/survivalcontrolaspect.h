/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: survivalcontrolaspect.h,v 1.3.2.1 2003/11/19 19:07:39 rollmark Exp $

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
#ifndef SURVIVALCONTROLASPECT_H__
#define SURVIVALCONTROLASPECT_H__

#include <oxygen/controlaspect/controlaspect.h>
#include <kerosin/fontserver/font.h>
#include "survivalagentaspect.h"
#include <list>

class SurvivalControlAspect : public oxygen::ControlAspect
{
protected:
        //! function used by agents to request effectors
        boost::shared_ptr<oxygen::Effector> RequestEffectorInternal(boost::shared_ptr<oxygen::AgentAspect>& agent, const std::string& effectorName);
        //! function used by agents to request perceptors
        boost::shared_ptr<oxygen::Perceptor> RequestPerceptorInternal(boost::shared_ptr<oxygen::AgentAspect>& agent, const std::string& perceptorName);
private:
        //! update internal state before physics calculation
        virtual void PrePhysicsUpdateInternal(float deltaTime);

        //! init state of simulation
        virtual void OnLink();

        //! displays current health and score
        virtual void RenderInternal();

        //! register an agent to this control aspect
        bool RegisterAgent(boost::shared_ptr<oxygen::AgentAspect>& agent);

private:
        struct AgentControlState
        {
                boost::shared_ptr<SurvivalAgentAspect>  agent;

                float   healthInit;     // the amount of health this agent
                                                        // receives with each live

                float   health;         // the current health
                int             death;          // number of its deaths

                bool    hasFood;        // (internal) true, if the agent
                                                        // touched the food in the current cycle
        };

        typedef std::list<AgentControlState> ControlledAgentList;

private:
        boost::shared_ptr<kerosin::Font>        mFont;
        ControlledAgentList                                     mControlledAgents;

        //! rate at which health decreses (units/second)
        float   mSickRate;

        //! minimum distance for an agent to get the food
        float   mFoodMinDist;

        //! the health increase an agent gets if it touches the food.
        //! this value is divided among the agents touching the food
        //! in simulatinously in one cycle
        float   mFoodValue;
};

DECLARE_CLASS(SurvivalControlAspect);

#endif //SURVIVALCONTROLASPECT_H__
