/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
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
#include <sstream>
#include "gamecontrolserver.h"
#include "baseparser.h"
#include <oxygen/agentaspect/agentaspect.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/controlaspect/controlaspect.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/corecontext.h>

using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace std;

GameControlServer::GameControlServer() : zeitgeist::Node()
{
    mExit = false;
}

GameControlServer::~GameControlServer()
{
}

bool
GameControlServer::InitParser(const std::string& parserName)
{
    mParser = shared_dynamic_cast<BaseParser>(GetCore()->New(parserName));

    if (mParser.get() == 0)
    {
        GetLog()->Error() << "ERROR: (GameControlServer::InitParser) Unable to create "
                          << parserName << "\n";
        return false;
    }

    return true;
}

void
GameControlServer::InitEffector(const std::string& effectorName)
{
   mCreateEffector = effectorName;
}

bool
GameControlServer::InitControlAspect(const string& aspectName)
{
    boost::shared_ptr<ControlAspect> aspect
        = shared_dynamic_cast<ControlAspect>(GetCore()->New(aspectName));

    if (aspect.get() == 0)
    {
        GetLog()->Error() << "ERROR: (GameControlServer::InitControlAspect) "
                          << "Unable to create " << aspectName << "\n";
        return false;
    }

    aspect->SetName(aspectName);
    AddChildReference(aspect);

    return true;
}

boost::shared_ptr<BaseParser>
GameControlServer::GetParser()
{
    return mParser;
}

boost::shared_ptr<Scene>
GameControlServer::GetActiveScene()
{
    boost::shared_ptr<SceneServer> sceneServer =
        shared_dynamic_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (GameControlServer) SceneServer not found.\n";
        return boost::shared_ptr<Scene>();
    }

    boost::shared_ptr<Scene> scene = sceneServer->GetActiveScene();

    if (scene.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (GameControlServer) SceneServer "
            << "reports no active scene\n";
    }

    return scene;
}

bool
GameControlServer::AgentConnect(int id)
{
    // for map::insert(Elem), the test here is not required. map::insert does not
    // overwrite existing elements. The test is required to report if the agent
    // was already connected or not.
    if (mAgentMap.find(id) != mAgentMap.end())
    {
        return false;
    }

    GetLog()->Normal()
        << "(GameControlServer) a new agent connected (id: " << id << ")\n";

    boost::shared_ptr<Scene> scene = GetActiveScene();
    if (scene.get() == 0)
    {
        GetLog()->Error()
            << "(GameControlServer) ERROR: Got no active scene from the "
            << "SceneServer to create the AgentAspect in.\n";
        return false;
    }

    // create a new AgentAspect for the ID in the scene and add it to
    // our map of AgentAspects
    boost::shared_ptr<AgentAspect> aspect = shared_dynamic_cast<AgentAspect>
        (GetCore()->New("oxygen/AgentAspect"));

    if (aspect.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (GameControlServer) cannot create new AgentAspect\n";
        return false;
    }

    stringstream name;
    name << "AgentAspect" << id;
    aspect->SetName(name.str());

    scene->AddChildReference(aspect);

    mAgentMap[id] = aspect;

    bool ok = aspect->Init(mCreateEffector,id);
    if (ok)
    {
      aspect->UpdateCache();
      scene->UpdateCache(false);
    }
    
    // mark the scene as modified
    scene->SetModified(true);

    return ok;
}

bool GameControlServer::AgentDisappear(int id)
{
    TAgentMap::iterator iter = mAgentMap.find(id);

    if (iter == mAgentMap.end())
    {
        GetLog()->Error()
            << "ERROR: (GameControlServer) AgentDisappear called for "
            << "unknown agent id " << id << "\n";
        return false;
    }

    // remove the AgentAspect from the Scene and our map. The
    // AgentAspect does all the necessary cleanup
    boost::shared_ptr<Scene> scene = GetActiveScene();

    if (scene.get() != 0)
    {
        (*iter).second->UnlinkChildren();
        (*iter).second->Unlink();

    } else
        {
            GetLog()->Error()
                << "ERROR: (GameControlServer) failed to remove AgentAspect "
                << "for agent id " << id << "\n";
        }

    mAgentMap.erase(id);

    // mark the scene as modified
    scene->SetModified(true);

    GetLog()->Debug() << "(GameControlServer) An agent disconnected (id: "
                      << id << ")\n";

    return true;
}

float
GameControlServer::GetSenseInterval(int /*id*/)
{
    // the real thing should query the AgentAspect corresponding to
    // the agent.
    return 0.2f;
}

float
GameControlServer::GetSenseLatency(int /*id*/)
{
    // the real thing should query the AgentAspect corresponding to
    // the agent
    return 0.1f;
}

float
GameControlServer::GetActionLatency(int /*id*/)
{
    // the real thing should query the AgentAspect corresponding to
    // the agent.
    return 0.1f;
}

int
GameControlServer::GetAgentCount() const
{
    return mAgentMap.size();
}

boost::shared_ptr<ActionObject::TList>
GameControlServer::Parse(int id, const string& str) const
{
    TAgentMap::const_iterator iter = mAgentMap.find(id);

    if (iter == mAgentMap.end())
    {
        GetLog()->Error()
            << "ERROR: (GameControlServer::Parse) Parse "
            << "called with unknown agent id "
            << id << "\n";
        return boost::shared_ptr<ActionObject::TList>();
    }

    if (mParser.get() == 0)
    {
        GetLog()->Error()
            << "ERROR: (GameControlServer::Parse) No parser registered.\n";
        return boost::shared_ptr<ActionObject::TList>();
    }

    // use the parser to create a PredicateList
    boost::shared_ptr<PredicateList> predicates(mParser->Parse(str));

    // construct an ActionList using the registered effectors
    boost::shared_ptr<ActionObject::TList> actionList(new ActionObject::TList());

    boost::shared_ptr<AgentAspect> aspect = (*iter).second;

    for
        (
            PredicateList::TList::const_iterator iter = predicates->begin();
            iter != predicates->end();
            ++iter
            )
    {
        const Predicate& predicate = (*iter);

        boost::shared_ptr<Effector> effector = aspect->GetEffector(predicate.name);
        if (effector.get() == 0)
        {
            GetLog()->Warning()
                << "(GameControlServer::Parse) No effector"
                << " registered for predicate "
                << predicate.name << "\n";
            continue;
        }

        boost::shared_ptr<ActionObject> action(effector->GetActionObject(predicate));

        if (action.get() == 0)
        {
            continue;
        }

        actionList->push_back(action);
    }

    return actionList;
}

boost::shared_ptr<AgentAspect>
GameControlServer::GetAgentAspect(int id)
{
    TAgentMap::iterator iter = mAgentMap.find(id);
    if (iter == mAgentMap.end())
    {
        return boost::shared_ptr<AgentAspect>();
    }

    return (*iter).second;
}

void 
GameControlServer::GetAgentAspectList(TAgentAspectList & list)
{
    for (
         TAgentMap::iterator iter = mAgentMap.begin();
         iter != mAgentMap.end();
         ++iter
         )
        {
            list.push_back(iter->second);
        }
}

void
GameControlServer::Update(float deltaTime)
{
    // remove disappeared agent
    for(
        vector<int>::iterator iter = mDisappearedAgent.begin();
        iter != mDisappearedAgent.end();
        ++iter
        )
    {
        AgentDisappear(*iter);
    }

    mDisappearedAgent.clear();

    // build list of ControlAspects, NOT searching recursively
    TLeafList control;
    ListChildrenSupportingClass<ControlAspect>(control,false);

    // update all ControlAspects found
    for (
        TLeafList::iterator iter = control.begin();
        iter != control.end();
        ++iter
        )
    {
        boost::shared_ptr<ControlAspect> aspect =
            shared_static_cast<ControlAspect>(*iter);

        aspect->Update(deltaTime);
    }
}

void
GameControlServer::Quit()
{
    mExit = true;
}

bool
GameControlServer::IsFinished() const
{
    return mExit;
}

void
GameControlServer::pushDisappearedAgent(int id)
{
    mDisappearedAgent.push_back(id);
}
