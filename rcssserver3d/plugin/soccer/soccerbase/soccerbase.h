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
#ifndef SOCCERBASE_H
#define SOCCERBASE_H

#include <soccertypes.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/leaf.h>
#include <zeitgeist/logserver/logserver.h>
#include <boost/shared_ptr.hpp>
//#include <map>

//namespace zeitgeist
//{
//    class Leaf;
//}

namespace oxygen
{
    class SceneServer;
    class Scene;
    class Transform;
    class Perceptor;
    class RigidBody;
    class SphereCollider;
    class ControlAspect;
    class AgentAspect;
    class GameControlServer;
}

namespace salt
{
    class Vector3f;
    class AABB3;
    class AABB2;
}

class AgentState;
class GameStateAspect;
class SoccerRuleAspect;
class Ball;

class SoccerBase
{
public:
    typedef std::vector<boost::shared_ptr<AgentState> > TAgentStateList;
    typedef std::map<int, boost::shared_ptr<AgentState> > TAgentStateMap;

public:
    SoccerBase() {}

    virtual ~SoccerBase() {}

    /** returns a reference to the SceneServer */
    static bool
    GetSceneServer(const zeitgeist::Leaf& base,
                   boost::shared_ptr<oxygen::SceneServer>& scene_server);

    /** returns a reference to the closest parent supporting
        Transform */
    static bool
    GetTransformParent(const zeitgeist::Leaf& base,
                       boost::shared_ptr<oxygen::Transform>& transform_parent);

    /** returns a reference to the Body node below the closest
        transform parent */
    static bool
    GetBody(const zeitgeist::Leaf& base, boost::shared_ptr<oxygen::RigidBody>& body);

    /** returns a reference to the Body node below the given Transform
        node */
    static bool
    GetAgentBody(const boost::shared_ptr<oxygen::Transform> transform,
                 boost::shared_ptr<oxygen::RigidBody>& agent_body);

    /** returns a reference to the Body node below the given Transform
        node based on parameters team index and uniform number */
    static bool
    GetAgentBody(const zeitgeist::Leaf& base, TTeamIndex idx,
                 int unum, boost::shared_ptr<oxygen::RigidBody>& agent_body);

    /** returns a reference to the AgentState node below the closest
        Transform parent */
    static bool
    GetAgentState(const zeitgeist::Leaf& base,
                  boost::shared_ptr<AgentState>& agent_state);

    /** returns a reference to the AgentState node below the given
        transform node */
    static bool
    GetAgentState(const boost::shared_ptr<oxygen::Transform> transform,
                  boost::shared_ptr<AgentState>& agentState);

    /** returns a reference to the AgentState node below the given Transform
        node based on parameters team index and uniform number */
    static bool
    GetAgentState(const zeitgeist::Leaf& base, TTeamIndex idx,
                 int unum, boost::shared_ptr<AgentState>& agent_state);

    static bool
    GetAgentStates(const zeitgeist::Leaf& base,
                   TAgentStateList& agentStates,
                   TTeamIndex idx = TI_NONE);

    /** return a reference to the GameStateAspect node */
    static bool
    GetGameState(const zeitgeist::Leaf& base,
                 boost::shared_ptr<GameStateAspect>& game_state);

    /** return a reference to the SoccerRuleAspect node */
    static bool
    GetSoccerRuleAspect(const zeitgeist::Leaf& base,
                        boost::shared_ptr<SoccerRuleAspect>& soccer_rule_aspect);

    /** return a reference to the GameControlServer */
    static bool
    GetGameControlServer(const zeitgeist::Leaf& base,
                         boost::shared_ptr<oxygen::GameControlServer>& game_control_server);

    /** returns a reference to the active scene from the SceneServer */
    static bool
    GetActiveScene(const zeitgeist::Leaf& base,
                   boost::shared_ptr<oxygen::Scene>& active_scene);

    /** returns a reference to the Ball node */
    static bool
    GetBall(const zeitgeist::Leaf& base, boost::shared_ptr<Ball>& ball);

    /** returns a reference to the Body node below the Ball */
    static bool
    GetBallBody(const zeitgeist::Leaf& base,
                boost::shared_ptr<oxygen::RigidBody>& body);

    /** returns a reference to the Body SphereCollider node below the Ball */
    static bool
    GetBallCollider(const zeitgeist::Leaf& base,
                boost::shared_ptr<oxygen::SphereCollider>& sphere);

    /** flips horizontal coordinates according to the side of the agent */
    static salt::Vector3f FlipView(const salt::Vector3f& pos, TTeamIndex ti);

    /** Get the team index of the opponent team */
    static TTeamIndex OpponentTeam(TTeamIndex ti);

    /** returns a reference to a ControlAspect registered to the
        GameControlServer */
    static boost::shared_ptr<oxygen::ControlAspect>
    GetControlAspect(const zeitgeist::Leaf& base, const std::string& name);

    /** looks up a ruby variable in the Soccer namespace */
    template<typename TYPE>
    static bool GetSoccerVar(const zeitgeist::Leaf& base, const std::string& name, TYPE& value)
    {
        static const std::string nSpace = "Soccer.";
        bool ok = base.GetCore()->GetScriptServer()->GetVariable(std::string(nSpace + name),value);

        if (! ok)
        {
            base.GetLog()->Error()
                    << "ERROR: (SoccerBase: " << base.GetName()
                    << ") soccer variable '" << name << "' not found\n";
            return false;
        }

        return ok;
    }

    /** returns a string representing a play mode */
    static std::string PlayMode2Str(const TPlayMode mode);

    /** returns a string representing a SPL state */
    static std::string SPLState2Str(const spl::TSPLState state);

    /** returns a string representing a SPL penalty */
    static std::string SPLPenalty2Str(const spl::TSPLPenalty penalty);

    /* move an agent including all its connected bodies */
    static bool
    MoveAgent(boost::shared_ptr<oxygen::Transform> agent_aspect, const salt::Vector3f& pos);

    /* move an agent including all its connected bodies and rotate
       specified angle (DEG) around the z-axis
    */
    static bool
    MoveAndRotateAgent(boost::shared_ptr<oxygen::Transform> agent_aspect,
                       const salt::Vector3f& pos,
                       float angle);

    /** returns the bounding box around the agent below the closest Space parent */
    static salt::AABB3
    GetAgentBoundingBox(const zeitgeist::Leaf& base);

    /** returns the bounding rect(z=0) around the agent below the closest Space parent */
    static salt::AABB2
    GetAgentBoundingRect(const zeitgeist::Leaf& base);
};

#endif
