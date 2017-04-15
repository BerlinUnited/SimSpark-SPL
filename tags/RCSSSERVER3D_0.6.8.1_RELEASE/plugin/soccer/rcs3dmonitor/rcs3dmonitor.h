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
#ifndef RCS3DMONITOR_H__
#define RCS3DMONITOR_H__

#include <oxygen/monitorserver/monitorsystem.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <kerosin/sceneserver/staticmesh.h>
#include <kerosin/sceneserver/light.h>
#include <oxygen/physicsserver/body.h>
#include "../ball/ball.h"

class RCS3DMonitor : public oxygen::MonitorSystem
{
public:
    enum ENodeType
        {
            NT_BASE = 0,
            NT_TRANSFORM,
            NT_STATICMESH,
            NT_LIGHT,
            NT_BALL
        };

    struct NodeCache
    {
    public:
        ENodeType type;

        /** the last local transform matrix sent to the client */
        salt::Matrix transform;

    public:
        NodeCache(ENodeType nt = NT_BASE)
            : type(nt)
        {
        }

        NodeCache(ENodeType nt, const salt::Matrix& t)
            : type(nt), transform(t)
        {
        }
    };

    typedef std::map<boost::shared_ptr<oxygen::BaseNode>, NodeCache> TNodeCacheMap;

public:
    RCS3DMonitor();
    virtual ~RCS3DMonitor();

    /** If a monitor sends information to the world model, this
     * function is called to process it.
     * @param data data sent from monitor to monitorsystem via SPADES.
     */
    void ParseMonitorMessage(const std::string& data);

    /** This function will be called periodically to get information
     * about the current state of the world.
     * \param items holds a list of additional name value pairs. These
     * predicates are collected from MonitorItem objects registered to
     * the MonitorServer. The monitor should transfer them to the
     * client if possible.
     */
    virtual std::string GetMonitorInformation(const oxygen::PredicateList& pList);

    /** This function is called once for every MonitorSystem each time
     *  a new client connects. It should return any header/setup
     *  information that is needed.
     *  \param items holds a list of additional name value
     *  pairs. These predicates are collected from MonitorItem objects
     *  registered to the MonitorServer. The monitor should transfer
     *  them to the client if possible.
     */
    virtual std::string GetMonitorHeaderInfo(const oxygen::PredicateList& pList);

    /** update variables from a script */
    virtual void UpdateCached();

protected:
    virtual void OnLink();
    virtual void OnUnlink();

    void ClearNodeCache();

    /** This function looks the cached node entry in the node
        cache. The entry is added to the cache if it does not exist
    */
    NodeCache* LookupNode(boost::shared_ptr<oxygen::BaseNode> node);

    void DescribeCustomPredicates(std::stringstream& ss,const oxygen::PredicateList& pList);
    void DescribeActiveScene(std::stringstream& ss);
    void DescribeScene(std::stringstream& ss,
                       boost::shared_ptr<oxygen::BaseNode> node);

    /** This function writes the s-expression for the given node to
        the given strinstream, omitting the closing parentheses. It
        returns false if the node is skipped and no description is
        created. Skipped nodes are invisible scene graph nodes.
    */
    bool DescribeNode(std::stringstream& ss,
                      boost::shared_ptr<oxygen::BaseNode> node);
    void DescribeBaseNode(std::stringstream& ss);
    void DescribeBall(std::stringstream& ss, NodeCache& entry,
                           boost::shared_ptr<Ball> ball);
    void DescribeTransform(std::stringstream& ss, NodeCache& entry,
                           boost::shared_ptr<oxygen::Transform> transform, bool prefix = true);
    void DescribeMesh(std::stringstream& ss,
                      boost::shared_ptr<kerosin::StaticMesh> mesh);
    void DescribeLight(std::stringstream& ss,
                       boost::shared_ptr<kerosin::Light> light);

protected:
    /** cached reference to the SceneServer */
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;

    /** cached reference to the current active scene */
    boost::shared_ptr<oxygen::Scene> mActiveScene;

    /** true, if the full state is generated */
    bool mFullState;

    /** cached node type and state */
    TNodeCacheMap mNodeCache;
};

DECLARE_CLASS(RCS3DMonitor);

#endif // RCS3DMONITOR_H__

