/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: soccerbase.h,v 1.1.2.1 2004/02/05 09:53:05 fruit Exp $

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
#ifndef SOCCERBASE_H
#define SOCCERBASE_H

#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/transform.h>
#include <soccer/agentstate/agentstate.h>
#include <soccer/soccertypes.h>

class SoccerBase : public zeitgeist::Leaf
{
public:
    SoccerBase() {}

    virtual ~SoccerBase() {}

    static bool
    GetSceneServer(const oxygen::BaseNode& base,
                   boost::shared_ptr<oxygen::SceneServer>& scene_server);

    static bool
    GetTransformParent(const oxygen::BaseNode& base,
                       boost::shared_ptr<oxygen::Transform>& transform_parent);

    static bool
    GetAgentState(const oxygen::BaseNode& base,
                  boost::shared_ptr<oxygen::Transform> parent,
                  boost::shared_ptr<AgentState>& agent_state);

    //! return a reference to the active scene from SceneServer
    static bool
    GetActiveScene(const oxygen::BaseNode& base,
                   boost::shared_ptr<oxygen::SceneServer> scene_server,
                   boost::shared_ptr<oxygen::Scene>& active_scene);

    //! flip horizontal coordinates according to the side of the agent
    static salt::Vector3f FlipView(const salt::Vector3f& pos, TTeamIndex ti);

};

DECLARE_CLASS(SoccerBase);

#endif
