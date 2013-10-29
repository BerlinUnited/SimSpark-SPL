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
#include "rcs3dmonitor.h"
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

RCS3DMonitor::RCS3DMonitor() : oxygen::MonitorSystem()
{
    mFullState = true;
}

RCS3DMonitor::~RCS3DMonitor()
{
}

void RCS3DMonitor::ClearNodeCache()
{
    mNodeCache.clear();
}

void RCS3DMonitor::UpdateCached()
{
    MonitorSystem::UpdateCached();
    ClearNodeCache();
}

void RCS3DMonitor::OnLink()
{
    // setup SceneServer reference
    mSceneServer = dynamic_pointer_cast<SceneServer>
        (GetCore()->Get("/sys/server/scene"));

    if (mSceneServer.get() == 0)
        {
            GetLog()->Error()
                << "(RCS3DMonitor) ERROR: SceneServer not found\n";
        }
}

void RCS3DMonitor::OnUnlink()
{
    mSceneServer.reset();
    mActiveScene.reset();
    ClearNodeCache();
}

void RCS3DMonitor::ParseMonitorMessage(const std::string& data)
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
            static_pointer_cast<MonitorCmdParser>(*iter)
                ->ParseMonitorMessage(data);
        }
}

string RCS3DMonitor::GetMonitorInformation(const PredicateList& pList)
{
    stringstream ss;
    mFullState = false;
    DescribeCustomPredicates(ss,pList);
    DescribeActiveScene(ss);

    return ss.str();
}

string RCS3DMonitor::GetMonitorHeaderInfo(const PredicateList& pList)
{
    stringstream ss;
    mFullState = true;
    ClearNodeCache();
    DescribeCustomPredicates(ss,pList);
    DescribeActiveScene(ss);

    return ss.str();
}

void RCS3DMonitor::DescribeCustomPredicates(stringstream& ss,const PredicateList& pList)
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

void RCS3DMonitor::DescribeBall(stringstream& ss, NodeCache& entry, boost::shared_ptr<Ball> ball)
{
      if (mFullState)
          {
              ss << "(nd Ball";
          } else
              {
                  ss << "(nd";
              }

  DescribeTransform(ss, entry, boost::static_pointer_cast<Transform>(ball), false);
}

void RCS3DMonitor::DescribeBaseNode(stringstream& ss)
{
    if (mFullState)
        {
            ss << "(nd BN";
        } else
            {
                ss << "(nd";
            }
}

void RCS3DMonitor::DescribeLight(stringstream& ss, boost::shared_ptr<Light> light)
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

void RCS3DMonitor::DescribeTransform(stringstream& ss, NodeCache& entry, boost::shared_ptr<Transform> transform, bool prefix)
{
    if (prefix)
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

void RCS3DMonitor::DescribeMesh(stringstream& ss, boost::shared_ptr<StaticMesh> mesh)
{
    boost::shared_ptr<SingleMatNode> singleMat =
        dynamic_pointer_cast<SingleMatNode>(mesh);

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

RCS3DMonitor::NodeCache* RCS3DMonitor::LookupNode(boost::shared_ptr<BaseNode> node)
{
    if (node.get() == 0)
        {
            assert(false);
            return 0;
        }

    // Check whether we already have this node in cache
    TNodeCacheMap::iterator iter = mNodeCache.find(node);
    if (iter != mNodeCache.end())
        {
            return &((*iter).second);
        }

    // Ball
    boost::shared_ptr<Ball> ball
        = dynamic_pointer_cast<Ball>(node);
    if (ball.get() != 0)
        {
            mNodeCache[node]
                = NodeCache(NT_BALL, ball->GetLocalTransform());

            return &(mNodeCache[node]);
        }
    
    // Transform
    boost::shared_ptr<Transform> transform
        = dynamic_pointer_cast<Transform>(node);
    if (transform.get() != 0)
        {
            mNodeCache[node]
                = NodeCache(NT_TRANSFORM, transform->GetLocalTransform());

            return &(mNodeCache[node]);
        }

    boost::shared_ptr<StaticMesh> mesh
        = dynamic_pointer_cast<StaticMesh>(node);
    if (mesh.get() != 0)
        {
            mNodeCache[node] = NodeCache(NT_STATICMESH);
            return &(mNodeCache[node]);
        }

    boost::shared_ptr<Light> light
        = dynamic_pointer_cast<Light>(node);
    if (light.get() != 0)
        {
            mNodeCache[node] = NodeCache(NT_LIGHT);
            return &(mNodeCache[node]);
        }

    // treat every other node type as a BaseNode
    mNodeCache[node] = NodeCache(NT_BASE);

    return &(mNodeCache[node]);
}

bool RCS3DMonitor::DescribeNode(stringstream& ss, boost::shared_ptr<BaseNode> node)
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

        case NT_BALL:
            DescribeBall
                (ss, (*entry), static_pointer_cast<Ball>(node));
        
        case NT_TRANSFORM:
            DescribeTransform
                (ss, (*entry), static_pointer_cast<Transform>(node));
            return true;

        case NT_STATICMESH:
            DescribeMesh
                (ss, static_pointer_cast<StaticMesh>(node));
            return true;

        case NT_LIGHT:
            DescribeLight
                (ss, static_pointer_cast<Light>(node));
            return true;
        }
}

void RCS3DMonitor::DescribeActiveScene(stringstream& ss)
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

void RCS3DMonitor::DescribeScene(stringstream& ss, boost::shared_ptr<BaseNode> node)
{
    bool closeParen = DescribeNode(ss, node);

    TLeafList baseNodes = node->GetBaseNodeChildren();
    for (TLeafList::iterator i = baseNodes.begin(); i!= baseNodes.end(); ++i)
        {
            boost::shared_ptr<BaseNode> baseNode = dynamic_pointer_cast<BaseNode>(*i);
            DescribeScene(ss,baseNode);
        }
        
    if (closeParen)
        {
            ss << ")";
        }
}
