/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sparkmonitorclient.cpp,v 1.7 2004/05/05 09:10:52 rollmark Exp $

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
#include <zeitgeist/logserver/logserver.h>
#include <netinet/in.h>
#include <rcssnet/exception.hpp>
#include <cerrno>

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
                << "(SparkMonitor) ERROR: SceneServer not found\n";
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
    while (mNetMessage->Extract(mNetBuffer, msg))
        {
            ParseMessage(msg);
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

    mSceneImporter->ParseScene(msg, mManagedScene, shared_ptr<ParameterList>());
}
