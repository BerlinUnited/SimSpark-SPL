/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sparkmonitor.h,v 1.2 2004/04/29 15:23:00 rollmark Exp $

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
#ifndef SPARKMONITOR_H__
#define SPARKMONITOR_H__

#include <oxygen/monitorserver/monitorsystem.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/transform.h>
#include <kerosin/sceneserver/staticmesh.h>
#include <kerosin/sceneserver/light.h>
#include <oxygen/physicsserver/body.h>

class SparkMonitor : public oxygen::MonitorSystem
{
public:
    SparkMonitor();
    virtual ~SparkMonitor();

    /** If a monitor sends information to the world model, this
     * function is called to process it.
     * @param data data sent from monitor to monitorsystem via SPADES.
     */
    void ParseMonitorMessage(const std::string& data);

    /** This function will be called periodically to get information
     * about the current state of the world.
     */
    virtual std::string GetMonitorInfo();

    /** This function is called once for every MonitorSystem. It
     *  should return any header/setup information that is needed.
     */
    virtual std::string GetMonitorHeaderInfo();

protected:
    virtual void OnLink();
    virtual void OnUnlink();

    void DescribeActiveScene(std::stringstream& ss);
    void DescribeScene(std::stringstream& ss,
                       boost::shared_ptr<oxygen::BaseNode> node);

    void DescribeNode(std::stringstream& ss,
                      boost::shared_ptr<oxygen::BaseNode> node);
    void DescribeTransform(std::stringstream& ss,
                           boost::shared_ptr<oxygen::Transform> transform);
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
};

DECLARE_CLASS(SparkMonitor);

#endif // SPARKMONITOR_H__

