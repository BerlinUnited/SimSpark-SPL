/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitortest.cpp,v 1.3 2003/12/27 17:53:42 fruit Exp $

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
#include "monitortest.h"

#include <sstream>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <netinet/in.h>

using namespace oxygen;
using namespace std;
using namespace boost;

MonitorTest::MonitorTest() : MonitorSystem()
{
}

MonitorTest::~MonitorTest()
{
}

bool MonitorTest::ConstructInternal()
{
  GetLog()->Normal() << "a monitortest was created." << endl;
  return true;
}

void
MonitorTest::ParseMonitorMessage(std::string data)
{
    GetLog()->Debug() << "MonitorTest received " << data << " from monitor\n";
}


string MonitorTest::GetMonitorInfo()
{
    /*    GetLog()->Debug() << "**** (MonitorTest::GetMonitorInfo)\n";
          return ("(player 0.3 1.4 0.2)");*/

    shared_ptr<SceneServer> sceneServer =
        shared_dynamic_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
        {
            GetLog()->Error() << "(MonitorTest) cannot get SceneServer\n";
            return "";
        }

    TLeafList transformList;
    shared_ptr<Scene> activeScene = sceneServer->GetActiveScene();
    activeScene->GetChildrenSupportingClass("AgentAspect", transformList, true);

    stringstream ss;

    for (TLeafList::iterator i = transformList.begin();
         i != transformList.end(); ++i)
        {
            shared_ptr<Transform> j = shared_static_cast<Transform>(*i);
            const salt::Vector3f& pos = j->GetWorldTransform().Pos();

            ss << "(player " << pos[0] << " " << pos[1] << " " << pos[2] << ")";
        }

    ss << endl;

    return ss.str();
}

string MonitorTest::GetMonitorHeaderInfo()
{
    return "";
}


