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
#include "soccerbase.h"
#include <oxygen/physicsserver/rigidbody.h>
#include <oxygen/physicsserver/spherecollider.h>
#include <oxygen/agentaspect/perceptor.h>
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <oxygen/controlaspect/controlaspect.h>
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <gamestateaspect/gamestateaspect.h>
#include <soccerruleaspect/soccerruleaspect.h>
#include <agentstate/agentstate.h>
#include <ball/ball.h>
#include <oxygen/physicsserver/space.h>
#include <zeitgeist/leaf.h>

using namespace boost;
using namespace zeitgeist;
using namespace oxygen;
using namespace std;
using namespace salt;

bool
SoccerBase::GetSceneServer(const Leaf& base,
                           boost::shared_ptr<SceneServer>& scene_server)
{
    scene_server = static_pointer_cast<SceneServer>
        (base.GetCore()->Get("/sys/server/scene"));

    if (scene_server.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << ") scene server not found.\n";
        return false;
    }

    return true;
}

bool
SoccerBase::GetTransformParent(const Leaf& base,
                               boost::shared_ptr<Transform>& transform_parent)
{
    transform_parent = dynamic_pointer_cast<Transform>
        ((base.FindParentSupportingClass<Transform>()).lock());

    if (transform_parent.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << ") parent node is not derived from TransformNode\n";
        return false;
    }
    return true;
}

bool
SoccerBase::GetAgentState(const boost::shared_ptr<Transform> transform,
                          boost::shared_ptr<AgentState>& agent_state)
{
    agent_state =
        dynamic_pointer_cast<AgentState>(transform->GetChild("AgentState", true));

    if (agent_state.get() == 0)
    {
        return false;
    }

    return true;
}

bool
SoccerBase::GetAgentBody(const boost::shared_ptr<Transform> transform,
                          boost::shared_ptr<RigidBody>& agent_body)

{
    agent_body = transform->FindChildSupportingClass<RigidBody>(true);

    if (agent_body.get() == 0)
    {
        transform->GetLog()->Error()
            << "(SoccerBase) ERROR: " << transform->GetName()
            << " node has no Body child\n";
        return false;
    }

    return true;
}

bool
SoccerBase::GetAgentBody(const Leaf& base, TTeamIndex idx,
                         int unum, boost::shared_ptr<RigidBody>& agent_body)
{
    boost::shared_ptr<AgentState> agentState;
    boost::shared_ptr<Transform>  parent;

    // get matching AgentState
    if (!GetAgentState(base, idx, unum, agentState))
        return false;

    // get AgentAspect
    if (!GetTransformParent(*agentState, parent))
        return false;

    // call GetAgentBody with matching AgentAspect
    return GetAgentBody(parent, agent_body);
}

bool
SoccerBase::GetAgentState(const Leaf& base,
                          boost::shared_ptr<AgentState>& agent_state)
{
    boost::shared_ptr<Transform> parent;
    if (! GetTransformParent(base,parent))
    {
        return false;
    }

    return GetAgentState(parent,agent_state);
}

bool
SoccerBase::GetAgentState(const Leaf& base, TTeamIndex idx,
                          int unum, boost::shared_ptr<AgentState>& agentState)
{
    static TAgentStateMap mAgentStateMapLeft;
    static TAgentStateMap mAgentStateMapRight;

    if (idx == TI_NONE)
        return false;

    // do we have a cached reference?
    if (
        idx == TI_LEFT &&
        !mAgentStateMapLeft.empty()
        )
        {
            TAgentStateMap::iterator iter = mAgentStateMapLeft.find(unum);

            if (iter != mAgentStateMapLeft.end())
                {
                    // is the pointer to the parent (AgentAspect) still valid
                    // (maybe the agent disconnected)?
                    if (!(iter->second)->GetParent().lock().get())
                        {
                            base.GetLog()->Error() << "(SoccerBase) WARNING: "
                                                   << "AgentState has invalid parent! "
                                                   << "The agent probably disconnected, removing from map."
                                                   << "\n";

                            mAgentStateMapLeft.erase(iter);
                        }
                    else
                        {
                            agentState = (*iter).second;
                            return true;
                        }
                }
        }
    else if (
             idx == TI_RIGHT &&
             !mAgentStateMapRight.empty()
             )
        {
            TAgentStateMap::iterator iter = mAgentStateMapRight.find(unum);

            if (iter != mAgentStateMapRight.end())
                {
                    // is the pointer to the parent (AgentAspect) still valid
                    // (maybe the agent disconnected)?
                    if ((iter->second)->GetParent().lock().get() == 0)
                        {
                            base.GetLog()->Error() << "(SoccerBase) WARNING: "
                                                   << "AgentState has invalid parent! "
                                                   << "The agent probably disconnected, removing from map."
                                                   << "\n";

                            mAgentStateMapRight.erase(iter);
                        }
                    else
                        {
                            agentState = (*iter).second;
                            return true;
                        }
                }
        }

    // we have to get all agent states for this team
    TAgentStateList agentStates;
    GetAgentStates(base, agentStates, idx);

    for (TAgentStateList::iterator iter = agentStates.begin();
         iter != agentStates.end();
         ++iter)
        {
            if ((*iter)->GetUniformNumber() == unum)
            {
                agentState = *iter;

                if (idx == TI_LEFT)
                    {
                        mAgentStateMapLeft[unum] = agentState;
                    }
                else
                    {
                        mAgentStateMapRight[unum] = agentState;
                    }

                return true;
            }
        }

    return false;
}

bool
SoccerBase::GetAgentStates(const zeitgeist::Leaf& base,
                           TAgentStateList& agentStates,
                           TTeamIndex idx)
{
    static boost::shared_ptr<GameControlServer> gameCtrl;

    if (gameCtrl.get() == 0)
    {
        GetGameControlServer(base, gameCtrl);

        if (gameCtrl.get() == 0)
        {
            base.GetLog()->Error() << "(SoccerBase) ERROR: can't get "
                                   << "GameControlServer\n";
            return false;
        }
    }

    GameControlServer::TAgentAspectList aspectList;
    gameCtrl->GetAgentAspectList(aspectList);

    GameControlServer::TAgentAspectList::iterator iter;
    boost::shared_ptr<AgentState> agentState;

    for (
         iter = aspectList.begin();
         iter != aspectList.end();
         ++iter
         )
        {
            agentState = dynamic_pointer_cast<AgentState>((*iter)->GetChild("AgentState", true));

            if (
                agentState.get() != 0 &&
                (
                 agentState->GetTeamIndex() == idx ||
                 idx == TI_NONE
                 )
                )
                {
                    agentStates.push_back(agentState);
                }
        }

    return true;
}

bool
SoccerBase::GetGameState(const Leaf& base,
                         boost::shared_ptr<GameStateAspect>& game_state)
{
    game_state = dynamic_pointer_cast<GameStateAspect>
        (base.GetCore()->Get("/sys/server/gamecontrol/GameStateAspect"));

    if (game_state.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << ") found no GameStateAspect\n";
        return false;
    }

    return true;
}

bool
SoccerBase::GetSoccerRuleAspect(const Leaf& base,
                                boost::shared_ptr<SoccerRuleAspect> & soccer_rule_aspect)
{
    soccer_rule_aspect = dynamic_pointer_cast<SoccerRuleAspect>
        (base.GetCore()->Get("/sys/server/gamecontrol/SoccerRuleAspect"));

    if (soccer_rule_aspect.get() == 0)
    {
        base.GetLog()->Error()
            << "Error: (SoccerBase: " << base.GetName()
            << " found no SoccerRuleAspect\n";

        return false;
    }

    return true;
}

bool
SoccerBase::GetGameControlServer(const Leaf& base,
                                boost::shared_ptr<GameControlServer> & game_control_server)
{
    static boost::shared_ptr<GameControlServer> gameControlServer;

    if (gameControlServer.get() == 0)
    {
        gameControlServer = dynamic_pointer_cast<GameControlServer>
            (base.GetCore()->Get("/sys/server/gamecontrol"));

        if (gameControlServer.get() == 0)
        {
            base.GetLog()->Error()
                << "Error: (SoccerBase: " << base.GetName()
                << " found no GameControlServer\n";

            return false;
        }
    }

    game_control_server = gameControlServer;

    return true;
}

bool
SoccerBase::GetActiveScene(const Leaf& base,
                           boost::shared_ptr<Scene>& active_scene)
{
    static boost::shared_ptr<SceneServer> sceneServer;

    if (sceneServer.get() == 0)
    {
        if (! GetSceneServer(base,sceneServer))
        {
            base.GetLog()->Error()
                << "(SoccerBase) ERROR: " << base.GetName()
                << ", could not get SceneServer\n";

            return false;
        }
    }

    active_scene = sceneServer->GetActiveScene();

    if (active_scene.get() == 0)
    {
        base.GetLog()->Error()
            << "ERROR: (SoccerBase: " << base.GetName()
            << ", SceneServer reports no active scene\n";

        return false;
    }

    return true;
}

bool
SoccerBase::GetBody(const Leaf& base, boost::shared_ptr<RigidBody>& body)
{
    boost::shared_ptr<Transform> parent;
    if (! GetTransformParent(base,parent))
    {
        base.GetLog()->Error() << "(SoccerBase) ERROR: no transform parent "
                          << "found in GetBody()\n";
        return false;
    }

  body = dynamic_pointer_cast<RigidBody>(parent->FindChildSupportingClass<RigidBody>());

  if (body.get() == 0)
    {
        base.GetLog()->Error()
            << "ERROR: (SoccerBase: " << base.GetName()
            << ") parent node has no Body child.";

        return false;
    }

  return true;
}

bool
SoccerBase::GetBall(const Leaf& base, boost::shared_ptr<Ball>& ball)
{
    static boost::shared_ptr<Scene> scene;
    static boost::shared_ptr<Ball> ballRef;

    if (scene.get() == 0)
    {
        if (! GetActiveScene(base,scene))
        {
            base.GetLog()->Error()
                << "(SoccerBase) ERROR: " << base.GetName()
                << ", could not get active scene.\n";

            return false;
        }
    }

    if (ballRef.get() == 0)
    {
        ballRef = dynamic_pointer_cast<Ball>
            (base.GetCore()->Get(scene->GetFullPath() + "Ball"));

        if (ballRef.get() == 0)
        {
            base.GetLog()->Error()
                << "(SoccerBase) ERROR: " << base.GetName()
                << ", found no ball node\n";

            return false;
        }
    }

    ball = ballRef;

    return true;
}

bool
SoccerBase::GetBallBody(const Leaf& base, boost::shared_ptr<RigidBody>& body)
{
    static boost::shared_ptr<Scene> scene;
    static boost::shared_ptr<RigidBody> bodyRef;

    if (scene.get() == 0)
    {
        if (! GetActiveScene(base,scene))
        {
            base.GetLog()->Error()
                << "(SoccerBase) ERROR: " << base.GetName()
                << ", could not get active scene.\n";

            return false;
        }
    }

    if (bodyRef.get() == 0)
    {
        bodyRef = dynamic_pointer_cast<RigidBody>
            (base.GetCore()->Get(scene->GetFullPath() + "Ball/physics"));

        if (bodyRef.get() == 0)
        {
            base.GetLog()->Error()
                << "(SoccerBase) ERROR: " << base.GetName()
                << ", found no ball body node\n";

            return false;
        }
    }

    body = bodyRef;

    return true;
}

bool
SoccerBase::GetBallCollider(const zeitgeist::Leaf& base,
                boost::shared_ptr<oxygen::SphereCollider>& sphere)
{
    static boost::shared_ptr<Scene> scene;
    static boost::shared_ptr<SphereCollider> sphereRef;

    if (scene.get() == 0)
    {
        if (! GetActiveScene(base,scene))
        {
            base.GetLog()->Error()
                << "(SoccerBase) ERROR: " << base.GetName()
                << ", could not get active scene.\n";

            return false;
        }
    }

    if (sphereRef.get() == 0)
    {
        sphereRef = dynamic_pointer_cast<SphereCollider>
            (base.GetCore()->Get(scene->GetFullPath() + "Ball/geometry"));

        if (sphereRef.get() == 0)
        {
            base.GetLog()->Error()
                << "(SoccerBase) ERROR:" << base.GetName()
                << ", Ball got no SphereCollider node\n";

            return false;
        }
    }

   sphere = sphereRef;

   return true;
}

salt::Vector3f
SoccerBase::FlipView(const salt::Vector3f& pos, TTeamIndex ti)
{
    salt::Vector3f newPos;
    switch (ti)
    {
    case TI_RIGHT:
        newPos[0] = -pos[0];
        newPos[1] = -pos[1];
        newPos[2] = pos[2];
        break;
    case TI_NONE:
    case TI_LEFT:
        newPos = pos;
        break;
    }
    return newPos;
}

TTeamIndex
SoccerBase::OpponentTeam(TTeamIndex ti)
{
    switch (ti)
    {
    case TI_RIGHT:
        return TI_LEFT;
    case TI_LEFT:
        return TI_RIGHT;
    default:
        return TI_NONE;
    }
}

string
SoccerBase::PlayMode2Str(const TPlayMode mode)
{
    switch (mode)
    {
    case PM_BeforeKickOff:
        return STR_PM_BeforeKickOff;

    case PM_KickOff_Left:
        return STR_PM_KickOff_Left;

    case PM_KickOff_Right:
        return STR_PM_KickOff_Right;

    case PM_PlayOn:
        return STR_PM_PlayOn;

    case PM_KickIn_Left:
        return STR_PM_KickIn_Left;

    case PM_KickIn_Right:
        return STR_PM_KickIn_Right;

    case PM_CORNER_KICK_LEFT:
        return STR_PM_CORNER_KICK_LEFT;

    case PM_CORNER_KICK_RIGHT:
        return STR_PM_CORNER_KICK_RIGHT;

    case PM_GOAL_KICK_LEFT:
        return STR_PM_GOAL_KICK_LEFT;

    case PM_GOAL_KICK_RIGHT:
        return STR_PM_GOAL_KICK_RIGHT;

    case PM_OFFSIDE_LEFT:
        return STR_PM_OFFSIDE_LEFT;

    case PM_OFFSIDE_RIGHT:
        return STR_PM_OFFSIDE_RIGHT;

    case PM_GameOver:
        return STR_PM_GameOver;

    case PM_Goal_Left:
        return STR_PM_Goal_Left;

    case PM_Goal_Right:
        return STR_PM_Goal_Right;

    case PM_FREE_KICK_LEFT:
        return STR_PM_FREE_KICK_LEFT;

    case PM_FREE_KICK_RIGHT:
        return STR_PM_FREE_KICK_RIGHT;

    case PM_DIRECT_FREE_KICK_LEFT:
        return STR_PM_DIRECT_FREE_KICK_LEFT;

    case PM_DIRECT_FREE_KICK_RIGHT:
        return STR_PM_DIRECT_FREE_KICK_RIGHT;

    default:
        return STR_PM_Unknown;
    };
}

boost::shared_ptr<ControlAspect>
SoccerBase::GetControlAspect(const zeitgeist::Leaf& base,const string& name)
{
  static const string gcsPath = "/sys/server/gamecontrol/";

  boost::shared_ptr<ControlAspect> aspect = dynamic_pointer_cast<ControlAspect>
    (base.GetCore()->Get(gcsPath + name));

  if (aspect.get() == 0)
    {
        base.GetLog()->Error()
            << "ERROR: (SoccerBase: " << base.GetName()
            << ") found no ControlAspect " << name << "\n";
    }

  return aspect;
}

bool
SoccerBase::MoveAgent(boost::shared_ptr<Transform> agent_aspect, const Vector3f& pos)
{
    Vector3f agentPos = agent_aspect->GetWorldTransform().Pos();

    boost::shared_ptr<Transform> parent = dynamic_pointer_cast<Transform>
            (agent_aspect->FindParentSupportingClass<Transform>().lock());

    if (parent.get() == 0)
    {
        agent_aspect->GetLog()->Error() << "(MoveAgent) ERROR: can't get parent node.\n";
        return false;
    }

    Leaf::TLeafList leafList;

    parent->ListChildrenSupportingClass<RigidBody>(leafList, true);

    if (leafList.size() == 0)
    {
        agent_aspect->GetLog()->Error()
            << "(MoveAgent) ERROR: agent aspect doesn't have "
            << "children of type Body\n";

        return false;
    }

    Leaf::TLeafList::iterator iter = leafList.begin();

    // move all child bodies
    for (; iter != leafList.end(); ++iter)
    {
        boost::shared_ptr<RigidBody> childBody =
            dynamic_pointer_cast<RigidBody>(*iter);

        Vector3f childPos = childBody->GetPosition();

        childBody->SetPosition(pos + (childPos-agentPos));
        childBody->SetVelocity(Vector3f(0,0,0));
        childBody->SetAngularVelocity(Vector3f(0,0,0));
    }

    return true;
}

bool
SoccerBase::MoveAndRotateAgent(boost::shared_ptr<Transform> agent_aspect, const Vector3f& pos, float angle)
{
    boost::shared_ptr<Transform> parent = dynamic_pointer_cast<Transform>
            (agent_aspect->FindParentSupportingClass<Transform>().lock());

    if (parent.get() == 0)
    {
        agent_aspect->GetLog()->Error() << "(MoveAndRotateAgent) ERROR: can't get parent node.\n";
        return false;
    }

    Leaf::TLeafList leafList;

    parent->ListChildrenSupportingClass<RigidBody>(leafList, true);

    if (leafList.size() == 0)
    {
        agent_aspect->GetLog()->Error()
            << "(MoveAndRotateAgent) ERROR: agent aspect doesn't have "
            << "children of type Body\n";

        return false;
    }

    boost::shared_ptr<RigidBody> body;
    GetAgentBody(agent_aspect, body);
    const Vector3f& agentPos = body->GetPosition();
    Matrix bodyR = body->GetRotation();
    bodyR.InvertRotationMatrix();
    Matrix mat;
    mat.RotationZ(gDegToRad(angle));
    mat *= bodyR;

    Leaf::TLeafList::iterator iter = leafList.begin();

    // move all child bodies
    for (;
         iter != leafList.end();
         ++iter
         )
        {
	       boost::shared_ptr<RigidBody> childBody =
                dynamic_pointer_cast<RigidBody>(*iter);

    	    Vector3f childPos = childBody->GetPosition();
            Matrix childR = childBody->GetRotation();
            childR = mat*childR;
    	    childBody->SetPosition(pos + mat.Rotate(childPos-agentPos));
    	    childBody->SetVelocity(Vector3f(0,0,0));
    	    childBody->SetAngularVelocity(Vector3f(0,0,0));
            childBody->SetRotation(childR);
    	}

    return true;
}

AABB3 SoccerBase::GetAgentBoundingBox(const Leaf& base)
{
    AABB3 boundingBox;

    boost::shared_ptr<Space> parent = base.FindParentSupportingClass<Space>().lock();

    if (!parent)
    {
        base.GetLog()->Error()
                << "(GetAgentBoundingBox) ERROR: can't get parent node.\n";
        return boundingBox;
    }

    /* We can't simply use the GetWorldBoundingBox of the space node, becuase
     * (at least currently) it'll return a wrong answer. Currently, the space
     * object is always at (0,0,0) which is encapsulated in the result of it's
     * GetWorldBoundingBox method call.
     */

    Leaf::TLeafList baseNodes;
    parent->ListChildrenSupportingClass<BaseNode>(baseNodes);

    if (baseNodes.empty())
        {
            base.GetLog()->Error()
                    << "(GetAgentBoundingBox) ERROR: space object doesn't have any"
                    << " children of type BaseNode.\n";
        }

    for (Leaf::TLeafList::iterator i = baseNodes.begin(); i!= baseNodes.end(); ++i)
    {
        boost::shared_ptr<BaseNode> node = static_pointer_cast<BaseNode>(*i);
        boundingBox.Encapsulate(node->GetWorldBoundingBox());
    }

    return boundingBox;
}

AABB2 SoccerBase::GetAgentBoundingRect(const Leaf& base)
{
    AABB2 boundingRect;

    boost::shared_ptr<Space> parent = base.FindParentSupportingClass<Space>().lock();

    if (!parent)
    {
        base.GetLog()->Error()
                << "(GetAgentBoundingBox) ERROR: can't get parent node.\n";
        return boundingRect;
    }

    /* We can't simply use the GetWorldBoundingBox of the space node, becuase
     * (at least currently) it'll return a wrong answer. Currently, the space
     * object is always at (0,0,0) which is encapsulated in the result of it's
     * GetWorldBoundingBox method call.
     */

    Leaf::TLeafList baseNodes;
    parent->ListChildrenSupportingClass<Collider>(baseNodes,true);

    if (baseNodes.empty())
        {
            base.GetLog()->Error()
                    << "(GetAgentBoundingBox) ERROR: space object doesn't have any"
                    << " children of type BaseNode.\n";
        }

    for (Leaf::TLeafList::iterator i = baseNodes.begin(); i!= baseNodes.end(); ++i)
    {
        boost::shared_ptr<BaseNode> node = static_pointer_cast<BaseNode>(*i);
        const AABB3 &box = node->GetWorldBoundingBox();
        boundingRect.Encapsulate(box.minVec.x(), box.minVec.y());
        boundingRect.Encapsulate(box.maxVec.x(), box.maxVec.y());
    }

    return boundingRect;
}
