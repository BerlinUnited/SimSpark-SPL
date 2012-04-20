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

void SparkMonitor::ClearNodeCache()
{
    mNodeCache.clear();
}

void SparkMonitor::UpdateCached()
{
    MonitorSystem::UpdateCached();
    ClearNodeCache();
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
    ClearNodeCache();
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

string SparkMonitor::GetMonitorInfomation(const PredicateList& pList)
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
    ClearNodeCache();
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

void SparkMonitor::DescribeBaseNode(stringstream& ss)
{
    if (mFullState)
        {
            ss << "(nd BN";
        } else
            {
                ss << "(nd";
            }
}

void SparkMonitor::DescribeLight(stringstream& ss, boost::shared_ptr<Light> light)
{
    if (! mFullState)
        {
            return DescribeBaseNode(ss);
        }

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

void SparkMonitor::DescribeTransform(stringstream& ss, NodeCache& entry, boost::shared_ptr<Transform> transform)
{
    if (mFullState)
        {
            ss << "(nd TRF";
        } else
            {
                ss << "(nd";
            }

    // include transform data only for fullstate or a modified
    // transform node
    const float precision = 0.005f;
    const Matrix& mat = transform->GetLocalTransform();

    bool update = false;

    if (mFullState)
        {
            update = true;
        } else
            {
                const salt::Matrix& old = entry.transform;

                for (int i=0;i<16;++i)
                    {
                        const float d = fabs(old.m[i] - mat.m[i]);

                        if (d > precision)
                            {
                                update = true;
                                break;
                            }
                    }
            }

    if (update)
        {
            ss << " (SLT";

            for (int i=0;i<16;++i)
                {
                    ss << " " << mat.m[i];
                }

            ss << ")";

            // update cache
            entry.transform = mat;
        }
}

void SparkMonitor::DescribeMesh(stringstream& ss, boost::shared_ptr<StaticMesh> mesh)
{
    boost::shared_ptr<SingleMatNode> singleMat =
        shared_dynamic_cast<SingleMatNode>(mesh);

    if (singleMat.get() != 0)
        {
            ss << "(nd SMN";
        } else
            {
                ss << "(nd StaticMesh";
            }

    if (mFullState || mesh->VisibleToggled())
        if (mesh->IsVisible())
            ss << " (setVisible 1)";
        else
            ss << " (setVisible 0)";
        
    if (! mFullState)
      return;

    if (mesh->IsTransparent())
        {
            ss << " (setTransparent)";
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

    ss << " (sSc "
       << scale[0] << " "
       << scale[1] << " "
       << scale[2] << ")";
    
    if (singleMat.get() != 0)
        {
            boost::shared_ptr<Material> mat = singleMat->GetMaterial();
            if (mat.get() != 0)
                {
                    ss << " (sMat " << mat->GetName() << ")";
                }
        }
    else{
        std::vector<std::string> mats = mesh->GetMaterialNames();
        if ( !mats.empty() ){
            ss<<"(resetMaterials";
            for(std::vector<std::string>::const_iterator iter = mats.begin();
                mats.end() != iter; ++iter){
                ss<<' '<<*iter;
            }
            ss<<')';
        }
    }
}

SparkMonitor::NodeCache* SparkMonitor::LookupNode(boost::shared_ptr<BaseNode> node)
{
    if (node.get() == 0)
        {
            assert(false);
            return 0;
        }

    TNodeCacheMap::iterator iter = mNodeCache.find(node);
    if (iter != mNodeCache.end())
        {
            return &((*iter).second);
        }

    boost::shared_ptr<Transform> transform
        = shared_dynamic_cast<Transform>(node);

    if (transform.get() != 0)
        {
            mNodeCache[node]
                = NodeCache(NT_TRANSFORM, transform->GetLocalTransform());

            return &(mNodeCache[node]);
        }

    boost::shared_ptr<StaticMesh> mesh
        = shared_dynamic_cast<StaticMesh>(node);
    if (mesh.get() != 0)
        {
            mNodeCache[node] = NodeCache(NT_STATICMESH);
            return &(mNodeCache[node]);
        }

    boost::shared_ptr<Light> light
        = shared_dynamic_cast<Light>(node);
    if (light.get() != 0)
        {
            mNodeCache[node] = NodeCache(NT_LIGHT);
            return &(mNodeCache[node]);
        }

    // treat every other node type as a BaseNode
    mNodeCache[node] = NodeCache(NT_BASE);

    return &(mNodeCache[node]);
}

bool SparkMonitor::DescribeNode(stringstream& ss, boost::shared_ptr<BaseNode> node)
{
    NodeCache* entry = LookupNode(node);
    if (entry == 0)
        {
            // skip node
            assert(false);
            return false;
        }

    switch (entry->type)
        {
        default:
            assert(false);
            // fall through

        case NT_BASE:
            // skip node
            return false;

        case NT_TRANSFORM:
            DescribeTransform
                (ss, (*entry), shared_static_cast<Transform>(node));
            return true;

        case NT_STATICMESH:
            DescribeMesh
                (ss, shared_static_cast<StaticMesh>(node));
            return true;

        case NT_LIGHT:
            DescribeLight
                (ss, shared_static_cast<Light>(node));
            return true;
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
            if (mFullState)
            {
                ss << "(RSG 0 1)";
            } else
            {
                ss << "(RDS 0 1)";
            }

            ss << "(";
            DescribeScene(ss,mActiveScene);
            ss << ")";
        }
}

void SparkMonitor::DescribeScene(stringstream& ss, boost::shared_ptr<BaseNode> node)
{
    bool closeParen = DescribeNode(ss, node);

    TLeafList baseNodes = node->GetBaseNodeChildren();
    for (TLeafList::iterator i = baseNodes.begin(); i!= baseNodes.end(); ++i)
        {
            boost::shared_ptr<BaseNode> baseNode = shared_dynamic_cast<BaseNode>(*i);
            DescribeScene(ss,baseNode);
        }
        
    if (closeParen)
        {
            ss << ")";
        }
}
