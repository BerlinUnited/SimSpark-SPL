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
#ifndef SPARK_SPARKMONITORCLIENT_H
#define SPARK_SPARKMONITORCLIENT_H

#include <sfsexp/sexp.h>
#include <zeitgeist/class.h>
#include <oxygen/simulationserver/netclient.h>
#include <oxygen/sceneserver/sceneserver.h>
#include <oxygen/sceneserver/scene.h>
#include <oxygen/sceneserver/sceneimporter.h>
#include <oxygen/gamecontrolserver/predicate.h>

class SparkMonitorClient : public oxygen::NetClient
{
public:
    SparkMonitorClient();
    virtual ~SparkMonitorClient();

    /** called once when the simulation is started */
    virtual void InitSimulation();

    /** called once before the simulation is shut down */
    virtual void DoneSimulation();

    /** called when a new simulation cycle starts, before the
        simulation is stepped */
    virtual void StartCycle();

protected:
    /** parses a received message */
    void ParseMessage(const std::string& msg);

    void ParseCustomPredicates(sexp_t* sexp, oxygen::PredicateList& pList);

    /** parses the given s-expression into a ParameterList and
        transfers it to a registered user monitor
     */
    void ParseCustomPredicates(sexp_t* sexp);

    virtual void OnLink();
    virtual void OnUnlink();

protected:
    /** cached reference to the SceneServer */
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;

    /** cached reference to the current active scene */
    boost::shared_ptr<oxygen::Scene> mActiveScene;

    /** chached reference to the scene importer */
    boost::shared_ptr<oxygen::SceneImporter> mSceneImporter;

    /** the root node of the managed scene */
    boost::shared_ptr<oxygen::BaseNode> mManagedScene;
};

DECLARE_CLASS(SparkMonitorClient);

#endif // SPARKMONITORCLIENT_H
