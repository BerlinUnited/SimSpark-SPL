/* -*- mode: c++; c-basic-indent: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: controlaspect.cpp,v 1.2 2003/08/31 21:53:45 fruit Exp $

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

#include "controlaspect.h"

using namespace boost;
using namespace oxygen;
using namespace zeitgeist;

void ControlAspect::PerformKills()
{
    while (mKillList.size())
    {
        // get the transform the agent to kill was attached to and unlink it
        mKillList.back()->Unlink();
        // lose our reference to it ... bye, bye
        mKillList.pop_back();
    }
}

shared_ptr<Effector> ControlAspect::RequestEffector(shared_ptr<AgentAspect>& agent, const std::string& effectorName)
{
    // try internal function to see if we allow the creation of the effector
    shared_ptr<Effector> effector = RequestEffectorInternal(agent, effectorName);

    if (effector)
    {
        // we were allowed to create the effector, now we attach it to the agent aspect
        agent->AddChildReference(effector);
    }
    else
    {
        printf("EKill\n");
        // the agent tried to create an effector, which is illegal, we will remember its
        // name, so we can unlink it (which destroys it!)
        mKillList.push_back(make_shared(agent->GetParent()));
    }

    return effector;
}

shared_ptr<Perceptor> ControlAspect::RequestPerceptor(shared_ptr<AgentAspect>& agent, const std::string& perceptorName)
{
    // try internal function to see if we allow the creation of the effector
    shared_ptr<Perceptor> perceptor = RequestPerceptorInternal(agent, perceptorName);

    if (perceptor)
    {
        // we were allowed to create the effector, now we attach it to the agent aspect
        agent->AddChildReference(perceptor);
    }
    else
    {
        // the agent tried to create an effector, which is illegal, we will remember its
        // name, so we can unlink it (which destroys it!)
        mKillList.push_back(make_shared(agent->GetParent()));
    }

    return perceptor;
}

shared_ptr<Effector> ControlAspect::CreateEffector(const std::string& effectorName)
{
    // try to create an instance
    shared_ptr<Object> instance = GetCore()->New(effectorName);

    // check if we got a valid instance
    if (instance && instance->GetClass()->Supports("Effector"))
    {
        return shared_static_cast<Effector>(instance);
    }
    else
    {
        return shared_ptr<Effector>();
    }
}

shared_ptr<Perceptor> ControlAspect::CreatePerceptor(const std::string& perceptorName)
{
    // try to create an instance
    shared_ptr<Object> instance = GetCore()->New(perceptorName);

    // check if we got a valid instance
    if (instance && instance->GetClass()->Supports("Perceptor"))
    {
        return shared_static_cast<Perceptor>(instance);
    }
    else
    {
        return shared_ptr<Perceptor>();
    }
}
