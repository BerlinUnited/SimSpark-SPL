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

#include "spark.h"
#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

#if HAVE_KEROSIN_KEROSIN_H
#include <kerosin/kerosin.h>
#include <kerosin/renderserver/rendercontrol.h>
#include <kerosin/inputserver/inputserver.h>
#include <kerosin/inputserver/inputcontrol.h>

using namespace kerosin;
#endif

using namespace spark;
using namespace oxygen;
using namespace zeitgeist;
using namespace salt;
using namespace std;
using namespace boost;

Spark::Spark()
{
}

Spark::~Spark()
{
    // reset shared ptr to objects in the zeitgeist hierarchy before
    // the zeitgeist core is shutdown
    ResetCached();
}

void Spark::ResetCached()
{
    mLogServer.reset();
    mScriptServer.reset();
    mSceneServer.reset();
    mSimulationServer.reset();
}

bool Spark::UpdateCached()
{
    bool ok = true;

    mLogServer = mZeitgeist->GetCore()->GetLogServer();
    if (mLogServer.get() == 0)
    {
        cerr << "(Spark) ERROR: LogServer not found\n";
        ok = false;
    }

    mScriptServer = mZeitgeist->GetCore()->GetScriptServer();
    if (mScriptServer.get() == 0)
    {
        mLogServer->Error() << "(Spark) ERROR: ScriptServer not found\n";
        ok = false;
    }

    mSceneServer = shared_dynamic_cast<SceneServer>
        (mZeitgeist->GetCore()->Get("/sys/server/scene"));

    if (mSceneServer.get() == 0)
    {
        mLogServer->Error() << "(Spark) ERROR: SceneServer not found\n";
        ok = false;
    }

    mSimulationServer = shared_dynamic_cast<SimulationServer>
        (mZeitgeist->GetCore()->Get("/sys/server/simulation"));

    if (mSimulationServer.get() == 0)
    {
        mLogServer->Error() << "(Spark) ERROR: SimulationServer not found\n";
        ok = false;
    }

    return ok;
}

bool
Spark::Init(int argc, char** argv)
{
    // See if user gave path prefix for init scripts
    string relPathPrefix = "";
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "--init-script-prefix") == 0)
        {
            relPathPrefix = argv[i + 1];
        }
    }

    mZeitgeist = shared_ptr<Zeitgeist>(new Zeitgeist("." PACKAGE_NAME, relPathPrefix));
    mOxygen = shared_ptr<Oxygen>(new Oxygen(*mZeitgeist));
#if HAVE_KEROSIN_KEROSIN_H
    mKerosin = shared_ptr<Kerosin>(new Kerosin(*mZeitgeist));
#endif

    // run the spark init script
    mZeitgeist->GetCore()->GetScriptServer()->RunInitScript
        (
         "spark.rb",
         "lib/spark",
         ScriptServer::IS_USERLOCAL
         );

    UpdateCached();

    // run the app defined init
    return InitApp(argc,argv);
}

bool
Spark::InitApp(int /*argc*/, char** /*argv*/)
{
    return true;
}

Zeitgeist&
Spark::GetZeitgeist()
{
    return (*mZeitgeist);
}

boost::shared_ptr<Core>
Spark::GetCore()
{
    return mZeitgeist->GetCore();
}

boost::shared_ptr<zeitgeist::LogServer>
Spark::GetLog()
{
    return mZeitgeist->GetCore()->GetLogServer();
}

boost::shared_ptr<SceneServer>
Spark::GetSceneServer()
{
    return mSceneServer;
}

boost::shared_ptr<SimulationServer>
Spark::GetSimulationServer()
{
    return mSimulationServer;
}

#if HAVE_KEROSIN_KEROSIN_H
boost::shared_ptr<InputControl>
Spark::GetInputControl()
{
    if (mSimulationServer.get() == 0)
    {
        return boost::shared_ptr<InputControl>();
    }

    return shared_dynamic_cast<InputControl>
        (mSimulationServer->GetControlNode("InputControl"));
}

boost::shared_ptr<InputServer>
Spark::GetInputServer()
{
    return shared_dynamic_cast<kerosin::InputServer>
        (mZeitgeist->GetCore()->Get("/sys/server/input"));
}

boost::shared_ptr<RenderControl>
Spark::GetRenderControl()
{
    if (mSimulationServer.get() == 0)
    {
        return boost::shared_ptr<RenderControl>();
    }

    return shared_dynamic_cast<RenderControl>
        (mSimulationServer->GetControlNode("RenderControl"));
}
#endif // HAVE_KEROSIN_KEROSIN_H

boost::shared_ptr<ScriptServer>
Spark::GetScriptServer()
{
    return mScriptServer;
}

boost::shared_ptr<Scene>
Spark::GetActiveScene()
{
    boost::shared_ptr<Scene> scene = mSceneServer->GetActiveScene();

    if (scene.get() == 0)
    {
        mLogServer->Warning()
            << "(Spark) Warning: no active scene registered\n";
    }

    return scene;
}

