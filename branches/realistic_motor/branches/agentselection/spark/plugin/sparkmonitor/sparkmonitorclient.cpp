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
#include "sparkmonitorclient.h"
#include <oxygen/monitorserver/custommonitor.h>
#include <zeitgeist/logserver/logserver.h>
#include <rcssnet/exception.hpp>
#include <cerrno>

#ifndef WIN32
#include <netinet/in.h>
#endif

using namespace oxygen;
using namespace zeitgeist;
using namespace rcss::net;
using namespace salt;
using namespace boost;
using namespace std;

SparkMonitorClient::SparkMonitorClient() : NetClient()
{
}

SparkMonitorClient::~SparkMonitorClient()
{
}

void SparkMonitorClient::OnLink()
{
    // setup SceneServer reference
    mSceneServer = shared_dynamic_cast<SceneServer>
        (GetCore()->Get("/sys/server/scene"));

    if (mSceneServer.get() == 0)
        {
            GetLog()->Error()
                << "(SparkMonitorClient) ERROR: SceneServer not found\n";
        }
}

void SparkMonitorClient::OnUnlink()
{
    mSceneServer.reset();
    if (mManagedScene.get() != 0)
        {
            mManagedScene->UnlinkChildren();
            mManagedScene.reset();
        }
}

void SparkMonitorClient::InitSimulation()
{
    if (! Connect())
        {
            return;
        }

    // get the SceneImporter
    mSceneImporter = shared_dynamic_cast<SceneImporter>
        (GetCore()->Get("/sys/server/scene/RubySceneImporter"));

    if (mSceneImporter.get() == 0)
        {
            GetLog()->Error()
                << "(SparkMonitorClient) ERROR: cannot create"
                << "a RubySceneImporter instance\n";
        }

    // send the monitor init string
    SendMessage("(init)");
}

void SparkMonitorClient::DoneSimulation()
{
    mActiveScene.reset();
    mSceneImporter.reset();
    CloseConnection();
}

void SparkMonitorClient::StartCycle()
{
    ReadFragments();

    string msg;

    if (mNetMessage.get() != 0)
    {
        while (mNetMessage->Extract(mNetBuffer, msg))
        {
            ParseMessage(msg);
        }
    }
}

void SparkMonitorClient::ParseCustomPredicates(sexp_t* sexp, PredicateList& pList)
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

void SparkMonitorClient::ParseCustomPredicates(sexp_t* sexp)
{
    // ( (name param1 param2 ...) (name param1 param2 ...) ... )
    if (sexp == 0)
        {
            return;
        }

    // get list of registered CustomMonitor objects
    TLeafList customList;
    ListChildrenSupportingClass<CustomMonitor>(customList);

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
            shared_static_cast<CustomMonitor>((*iter))
                ->ParseCustomPredicates(pList);
        }
}

void SparkMonitorClient::ParseMessage(const string& msg)
{
    if (
        (mSceneServer.get() == 0) ||
        (mSceneImporter.get() == 0)
        )
        {
            return;
        }

    mActiveScene = mSceneServer->GetActiveScene();

    if (mActiveScene.get() == 0)
        {
            return;
        }

    if (mManagedScene.get() == 0)
        {
            mManagedScene = shared_dynamic_cast<BaseNode>
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
    sexp_t* sexp_custom = iparse_sexp(msgBuf,msg.size(),pcont);

    if (sexp_custom == 0)
        {
            destroy_sexp(sexp_custom);
            destroy_continuation(pcont);
            return;
        }

    ParseCustomPredicates(sexp_custom);

    mSceneImporter->ParseScene(string(pcont->lastPos),
                               mManagedScene,
                               boost::shared_ptr<ParameterList>());

    destroy_sexp(sexp_custom);
    destroy_continuation(pcont);
}
