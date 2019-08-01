/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef SPARK_SPARK_H
#define SPARK_SPARK_H

#if HAVE_CONFIG_H
#include <sparkconfig.h>
#endif

#include <string>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <salt/defines.h>

#ifdef spark_EXPORTS
#define SPARK_API API_EXPORT
#else
#define SPARK_API API_IMPORT
#endif


namespace zeitgeist
{
    class Zeitgeist;
    class LogServer;
    class ScriptServer;
    class Core;
}

namespace oxygen
{
    class Oxygen;
    class SceneServer;
    class SimulationServer;
    class Scene;
}

#if HAVE_KEROSIN_KEROSIN_H
namespace kerosin
{
    class Kerosin;
    class InputControl;
    class RenderControl;
    class InputServer;
}
#endif

namespace spark
{

/** \class Spark is an application framework for apps that use oxygen
    and kerosin classes. It provides a default setup of and easy
    access to all relevant classes. Applications that want to use
    spark should subclass it and override the given callbacks.
 */
class SPARK_API Spark
{
public:
    
    /** Constructor*/
    Spark(const std::string& relPathPrefix = std::string(""));

    /** Destructor. */
    virtual ~Spark();

    /** inits the Spark lib, returns true on success, has to be called once
        before any other spark methods */
    bool Init(int argc, char** argv);

    /** resets all cached object references */
    void ResetCached();

    /** updated all cached object references */
    bool UpdateCached();

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

#if HAVE_KEROSIN_KEROSIN_H
    /** returns the InputServer */
    boost::shared_ptr<kerosin::InputServer> GetInputServer();

    /** returns the input control node */
    boost::shared_ptr<kerosin::InputControl> GetInputControl();

    /** returns the render control node */
    boost::shared_ptr<kerosin::RenderControl> GetRenderControl();
#else
    #warning "Compiling spark with no I/O"
#endif

protected:
    boost::shared_ptr<zeitgeist::LogServer> mLogServer;
    boost::shared_ptr<zeitgeist::ScriptServer> mScriptServer;
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;
    boost::shared_ptr<oxygen::SimulationServer> mSimulationServer;

    boost::shared_ptr<zeitgeist::Zeitgeist> mZeitgeist;
    boost::shared_ptr<oxygen::Oxygen> mOxygen;
#if HAVE_KEROSIN_KEROSIN_H
    boost::shared_ptr<kerosin::Kerosin> mKerosin;
#endif

    std::string mRelPathPrefix;
};

} // namespace kerosin

#endif // SPARK_SPARK_H
