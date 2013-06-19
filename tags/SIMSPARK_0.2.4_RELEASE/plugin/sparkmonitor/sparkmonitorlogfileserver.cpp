/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group

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
#include "sparkmonitorlogfileserver.h"
#include <oxygen/monitorserver/custommonitor.h>
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <rcssnet/exception.hpp>
#include <cerrno>

#ifdef WIN32
#include <windows.h>
#else
#include <netinet/in.h>
#endif

using namespace oxygen;
using namespace zeitgeist;
using namespace rcss::net;
using namespace salt;
using namespace boost;
using namespace std;

SparkMonitorLogFileServer::SparkMonitorLogFileServer() : SimControlNode()
{
    mPause = false;
    mForwardStep = false;
    mBackwardPlayback = false;
    mSexpMemory = init_sexp_memory();
}

SparkMonitorLogFileServer::~SparkMonitorLogFileServer()
{
    destroy_sexp_memory(mSexpMemory);
}

void SparkMonitorLogFileServer::OnLink()
{
    mScriptServer = GetCore()->GetScriptServer();

    // setup SceneServer reference
    mSceneServer = dynamic_pointer_cast<SceneServer>
        (GetCore()->Get("/sys/server/scene"));

    if (mSceneServer.get() == 0)
        {
            GetLog()->Error()
                << "(SparkMonitor) ERROR: SceneServer not found\n";
        }

}

void SparkMonitorLogFileServer::OnUnlink()
{
    mSceneServer.reset();
    if (mManagedScene.get() != 0)
        {
            mManagedScene->UnlinkChildren();
            mManagedScene.reset();
        }
}

void SparkMonitorLogFileServer::InitSimulation()
{
    // get the SceneImporter
    mSceneImporter = dynamic_pointer_cast<SceneImporter>
        (GetCore()->Get("/sys/server/scene/RubySceneImporter"));

    if (mSceneImporter.get() == 0)
        {
            GetLog()->Error()
                << "(SparkMonitorLogFileServer) ERROR: cannot create"
                << " a RubySceneImporter instance\n";
        }

    mLog.open(mLogfileName.c_str());

    if (! mLog.is_open())
        {
            GetLog()->Error()
                << "(SparkMonitorLogFileServer) ERROR: cannot open"
                << " the log file\n";

            exit(1);
        }
}

void SparkMonitorLogFileServer::DoneSimulation()
{
    mActiveScene.reset();
    mSceneImporter.reset();
    mLog.close();
}

void SparkMonitorLogFileServer::StartCycle()
{
    if (mPause && !mForwardStep)
        {
            return;
        }

    if (mBackwardPlayback)
        {
            if (linePositions.size() < 3)
                return;

            linePositions.pop();
            linePositions.pop();

            mLog.seekg( linePositions.top() );
        }

    string msg;

    linePositions.push( mLog.tellg() );
    std::getline(mLog,msg);

    if (msg.size() != 0)
        {
            ParseMessage(msg);
        }

    mForwardStep = false;
}

void SparkMonitorLogFileServer::ParseCustomPredicates(sexp_t* sexp, PredicateList& pList)
{
    if (
        (sexp == 0) ||
        (sexp->ty != SEXP_VALUE)
        )
        {
            return;
        }

    Predicate& pred = pList.AddPredicate();
    pred.name = sexp->val;

    sexp = sexp->next;
    while (sexp != 0)
        {
            if (sexp->ty == SEXP_VALUE)
                {
                    pred.parameter.AddValue(sexp->val);
                }

            sexp = sexp->next;
        }
}

void SparkMonitorLogFileServer::ParseCustomPredicates(sexp_t* sexp)
{
    // ( (name param1 param2 ...) (name param1 param2 ...) ... )
    if (sexp == 0)
        {
            return;
        }

    // get list of registered CustomMonitor objects
    TLeafList customList;
    ListChildrenSupportingClass<CustomMonitor>(customList);
    customList.push_back(GetCore()->Get("/sys/server/simulation/SparkMonitorClient/SoccerMonitor"));

    if (customList.empty())
        {
            return;
        }

    // parse predicates
    PredicateList pList;

    sexp = sexp->list;
    while (sexp != 0)
        {
            if (sexp->ty == SEXP_LIST)
                {
                    sexp_t* sPred = sexp->list;
                    ParseCustomPredicates(sPred,pList);
                }

            sexp = sexp->next;
        }

    // pass predicates to all registered CustomMonitor objects
    for (
         TLeafList::iterator iter = customList.begin();
         iter != customList.end();
         ++iter
         )
        {
            static_pointer_cast<CustomMonitor>((*iter))
                ->ParseCustomPredicates(pList);
        }
}

void SparkMonitorLogFileServer::ParseMessage(const string& msg)
{
    if (
        (mSceneServer.get() == 0) ||
        (mSceneImporter.get() == 0)
        )
        {
            return;
        }

    mActiveScene = mSceneServer->GetActiveScene();
    mActiveScene->UpdateCache();

    if (mActiveScene.get() == 0)
        {
            return;
        }

    if (mManagedScene.get() == 0)
        {
            mManagedScene = dynamic_pointer_cast<BaseNode>
                (GetCore()->New("oxygen/BaseNode"));
            mActiveScene->AddChildReference(mManagedScene);
        }

    // parse s-expressions; we expect a leading list of custom
    // predicates followed by the RubySceneGraph expressions

    /** (
        (predicate parameter ...)
        (predicate parameter ...)
        ...
        )
        (RubySceneGraph Header)
        (RubySceneGraph RigidBody)
        )
    **/

    char* msgBuf        = const_cast<char*>(msg.c_str());
    pcont_t* pcont      = init_continuation(msgBuf);
    sexp_t* sexp_custom = iparse_sexp(mSexpMemory, msgBuf, msg.size(), pcont);

    if (sexp_custom == 0)
        {
            destroy_sexp(mSexpMemory, sexp_custom);
            destroy_continuation(mSexpMemory, pcont);
            return;
        }

    ParseCustomPredicates(sexp_custom);

    mSceneImporter->ParseScene(string(pcont->lastPos),
                               mManagedScene,
                               boost::shared_ptr<ParameterList>());
    mActiveScene->SetModified(true);

    destroy_sexp(mSexpMemory, sexp_custom);
    destroy_continuation(mSexpMemory, pcont);
}

void
SparkMonitorLogFileServer::BackwardStep()
{
    if (linePositions.size() < 3)
        return;

    linePositions.pop();
    linePositions.pop();

    mLog.seekg( linePositions.top() );

    mForwardStep = true;
}

void
SparkMonitorLogFileServer::BackwardPlayback()
{
    mBackwardPlayback = !mBackwardPlayback;
    mPause = false;
}
