/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sexpmonitor.cpp,v 1.1.2.1 2004/01/08 16:01:10 rollmark Exp $

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
#include "sexpmonitor.h"
#include <sstream>
#include <zeitgeist/logserver/logserver.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <netinet/in.h>

using namespace oxygen;
using namespace std;
using namespace boost;

SexpMonitor::SexpMonitor() : MonitorSystem()
{
}

SexpMonitor::~SexpMonitor()
{
}

bool
SexpMonitor::ConstructInternal()
{
  return true;
}

void
SexpMonitor::ParseMonitorMessage(std::string data)
{
    GetLog()->Debug() << "SexpMonitor received " << data << " from monitor\n";
}

std::string SexpMonitor::GenerateSexp(string type, TLeafList& list)
{
    stringstream ss;

    for (TLeafList::iterator i = list.begin();i != list.end();++i)
        {
            shared_ptr<Transform> j = shared_static_cast<Transform>(*i);
            const salt::Vector3f& pos = j->GetWorldTransform().Pos();

            ss << "(" << type << " " << pos[0] << " " << pos[1] << " " << pos[2] << ")";
        }

    return ss.str();
}

string
SexpMonitor::GetMonitorInfo()
{
    // map from class types to be recognized to their expression type
    const struct ExpressionType
    {
        string className;
        string typeName;
    } typeMap[] =
        {
            {"AgentAspect","agent"},
            {"FieldFlag","flag"}
        };

    shared_ptr<SceneServer> sceneServer =
        shared_dynamic_cast<SceneServer>(GetCore()->Get("/sys/server/scene"));

    if (sceneServer.get() == 0)
        {
            GetLog()->Error() << "(SexpMonitor) cannot get SceneServer\n";
            return "";
        }

    shared_ptr<Scene> activeScene = sceneServer->GetActiveScene();
    stringstream expression;

    int n = sizeof(typeMap) / sizeof(ExpressionType);
    for (int i=0;i<n;++i)
        {
            const ExpressionType& entry = typeMap[i];

            TLeafList nodes;
            activeScene->GetChildrenSupportingClass(entry.className, nodes, true);
            expression << GenerateSexp(entry.typeName, nodes);
        }

    expression << endl;
    return expression.str();
}

string
SexpMonitor::GetMonitorHeaderInfo()
{
    return "";
}


