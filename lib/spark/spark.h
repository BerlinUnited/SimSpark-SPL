/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: spark.h,v 1.4 2004/12/31 11:27:04 rollmark Exp $

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
#ifndef SPARK_SPARK_H
#define SPARK_SPARK_H

#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>
#include <oxygen/oxygen.h>
#include <kerosin/renderserver/rendercontrol.h>
#include <kerosin/inputserver/inputcontrol.h>

namespace spark
{

/** \class Spark is an application framework for apps that use oxygen
    and kerosin classes. It provides a default setup of and easy
    access to all relevant classes. Applications that want to use
    spark should subclass it and override the given callbacks.
 */
class Spark
{
public:
    Spark(const std::string& relPathPrefix);
    virtual ~Spark();

    /** inits the Spark lib, returns true on success, has to be called once
        before any other spark methods */
    bool Init(int argc, char** argv);

    //
    // user callbacks

    /** called once after Spark finished it's init */
    virtual bool InitApp(int argc, char** argv);

    //
    // set/get methods

    /** returns the zeitgeist instance */
    zeitgeist::Zeitgeist& GetZeitgeist();

    /** returns the zeitgeist core instance */
    boost::shared_ptr<zeitgeist::Core> GetCore();

    /** returns the LogServer */
    boost::shared_ptr<zeitgeist::LogServer> GetLog();

    /** returns the ScriptServer */
    boost::shared_ptr<zeitgeist::ScriptServer> GetScriptServer();

    /** returns the SceneServer */
    boost::shared_ptr<oxygen::SceneServer> GetSceneServer();

    /** returns the current active Scene */
    boost::shared_ptr<oxygen::Scene> GetActiveScene();

    /** returns the SimulationServer */
    boost::shared_ptr<oxygen::SimulationServer> GetSimulationServer();

    /** returns the input control node */
    boost::shared_ptr<kerosin::InputControl> GetInputControl();

    /** returns the render control node */
    boost::shared_ptr<kerosin::RenderControl> GetRenderControl();

protected:
    boost::shared_ptr<zeitgeist::LogServer> mLogServer;
    boost::shared_ptr<zeitgeist::ScriptServer> mScriptServer;
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;
    boost::shared_ptr<oxygen::SimulationServer> mSimulationServer;

    zeitgeist::Zeitgeist mZeitgeist;
    oxygen::Oxygen mOxygen;
    kerosin::Kerosin mKerosin;
};

} // namespace kerosin

#endif // SPARK_SPARK_H
