/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sparkmonitor.cpp,v 1.9 2007/05/29 21:27:42 jamu Exp $

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
#include "sparkmonitor.h"
#include <zeitgeist/logserver/logserver.h>
#include <kerosin/sceneserver/singlematnode.h>
#include <kerosin/materialserver/material.h>
#include <oxygen/monitorserver/monitorcmdparser.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <sstream>

using namespace kerosin;
using namespace oxygen;
using namespace zeitgeist;
using namespace boost;
using namespace salt;
using namespace std;

SparkMonitor::SparkMonitor() : oxygen::MonitorSystem()
{
    mFullState = true;
}

SparkMonitor::~SparkMonitor()
{
}

void SparkMonitor::OnLink()
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

void SparkMonitor::OnUnlink()
{
    mSceneServer.reset();
    mActiveScene.reset();
}

void SparkMonitor::ParseMonitorMessage(const std::string& data)
{
    // pass the received string on to all installed CommandParsers
    TLeafList items;
    ListChildrenSupportingClass<MonitorCmdParser>(items);

    for (
         TLeafList::iterator iter = items.begin();
         iter != items.end();
         ++iter
         )
        {
            shared_static_cast<MonitorCmdParser>(*iter)
                ->ParseMonitorMessage(data);
        }
}

string SparkMonitor::GetMonitorInfo(const PredicateList& pList)
{
    stringstream ss;
    mFullState = false;
    DescribeCustomPredicates(ss,pList);
    DescribeActiveScene(ss);
    return ss.str();
}

string SparkMonitor::GetMonitorHeaderInfo(const PredicateList& pList)
{
    stringstream ss;
    mFullState = true;
    DescribeCustomPredicates(ss,pList);
    DescribeActiveScene(ss);
    return ss.str();
}

void SparkMonitor::DescribeCustomPredicates(stringstream& ss,const PredicateList& pList)
{
    ss << "(";

    for (
         PredicateList::TList::const_iterator iter = pList.begin();
         iter != pList.end();
         ++iter
         )
        {
            const Predicate& pred = (*iter);

            ss << "(";
            ss << pred.name;

            const ParameterList& paramList = pred.parameter;
            ParameterList::TVector::const_iterator pIter = paramList.begin();

            std::string param;
            while (
                   (pIter != paramList.end()) &&
                   (paramList.AdvanceValue(pIter, param))
                   )
                {
                    ss << " ";
                    ss << param;
                }

            ss << ")";
        }

    ss << ")";
}

void SparkMonitor::DescribeLight(stringstream& ss, shared_ptr<Light> light)
{
    ss << "(nd Light ";

    const RGBA& diff = light->GetDiffuse();
    ss << "(setDiffuse " << diff.r() << " " << diff.g() << " "
       << diff.b() << " " << diff.a() << ") ";

    const RGBA& amb = light->GetAmbient();
    ss << "(setAmbient " << amb.r() << " " << amb.g() << " "
       << amb.b() << " " << amb.a() << ") ";

    const RGBA& spec = light->GetSpecular();
    ss << "(setSpecular " << spec.r() << " " << spec.g() << " "
       << spec.b() << " " << spec.a() << ")";
}

void SparkMonitor::DescribeTransform(stringstream& ss, shared_ptr<Transform> transform)
{
    const float precision = 0.005;
    const Matrix& mat = transform->GetLocalTransform();

    if (mFullState)
        {
            ss << "(nd Transform";
        } else
            {
                ss << "(nd";
            }

    // include transform data only for fullstate or a modified
    // transform node
    bool update = false;
    if (mFullState)
        {
            update = true;
        } else
            {
                if (SceneServer::GetTransformMark() == transform->GetChangedMark())
                {
                    const salt::Matrix& current = transform->GetLocalTransform();
                    const salt::Matrix& old = transform->GetOldLocalTransform();
                    for (int i=0;i<16;++i)
                        {
                            const float d = fabs(current.m[i] - old.m[i]);

                            if (d > precision)
                                {
                                    update = true;
                                    break;
                                }
                        }
                }
            }


    if (update)
        {
            ss << " (setLocalTransform ";

            for (int i=0;i<16;++i)
                {
                    ss << mat.m[i] << " ";
                }

            ss << ")";
        }
}

void SparkMonitor::DescribeMesh(stringstream& ss, boost::shared_ptr<StaticMesh> mesh)
{
    shared_ptr<SingleMatNode> singleMat =
        shared_dynamic_cast<SingleMatNode>(mesh);

    if (singleMat.get() != 0)
        {
            ss << "(nd SingleMatNode";
        } else
            {
                ss << "(nd StaticMesh";
            }

    ss << " (load " << mesh->GetMeshName();

    const ParameterList& params = mesh->GetMeshParameter();
    for (
         ParameterList::TVector::const_iterator iter = params.begin();
         iter != params.end();
         ++iter
         )
        {
            string str;
            params.GetValue(iter,str);
            ss << " " << str;
        }

    ss << ")";

    const Vector3f& scale = mesh->GetScale();

    ss << " (setScale "
       << scale[0] << " "
       << scale[1] << " "
       << scale[2] << ")";

    if (singleMat.get() != 0)
        {
            shared_ptr<Material> mat = singleMat->GetMaterial();
            if (mat.get() != 0)
                {
                    ss << " (setMaterial " << mat->GetName() << ")";
                }
        }
}

void SparkMonitor::DescribeNode(stringstream& ss, shared_ptr<BaseNode> node)
{
    // Transform node
    shared_ptr<Transform> transform =
        shared_dynamic_cast<Transform>(node);

    if (transform.get() != 0)
        {
            DescribeTransform(ss,transform);
            return;
        }

    if (mFullState)
        {
            // StaticMesh node
            shared_ptr<StaticMesh> mesh =
                shared_dynamic_cast<StaticMesh>(node);

            if (mesh.get() != 0)
                {
                    DescribeMesh(ss,mesh);
                    return;
                }

            // Light node
            shared_ptr<Light> light =
                shared_dynamic_cast<Light>(node);

            if (light.get() != 0)
                {
                    DescribeLight(ss,light);
                    return;
                }
        }

    if (mFullState)
        {
            // default to BaseNode
            ss << "(nd BaseNode";
        } else
            {
                ss << "(nd";
            }
}

void SparkMonitor::DescribeActiveScene(stringstream& ss)
{
    if (mSceneServer.get() == 0)
        {
            return;
        }

    mActiveScene = mSceneServer->GetActiveScene();

    if (mActiveScene.get() != 0)
        {
            if (mActiveScene->GetModified())
                {
                    // send a full scene update if nodes have been
                    // added or removed (on scene import, agent
                    // connect/disconnect etc.)
                    mFullState = true;
                }

            if (mFullState)
                {
                    ss << "(RSG 0 1)";
                } else
                    {
                        ss << "(RDS 0 1)";
                    }

            DescribeScene(ss,mActiveScene);
        }

    ss << endl;
}

void SparkMonitor::DescribeScene(stringstream& ss, shared_ptr<BaseNode> node)
{
    DescribeNode(ss, node);

    for (TLeafList::iterator i = node->begin(); i!= node->end(); ++i)
        {
            shared_ptr<BaseNode> baseNode = shared_dynamic_cast<BaseNode>(*i);
            if (baseNode.get() == 0)
                {
                    continue;
                }

            DescribeScene(ss,baseNode);
        }

    ss << ")";
}

