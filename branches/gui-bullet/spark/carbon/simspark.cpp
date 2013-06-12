/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
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

#include "simspark.h"

#include "carbon.h"
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/zeitgeist.h>
#include <oxygen/simulationserver/simulationserver.h>
#include <oxygen/physicsserver/world.h>
#include <oxygen/physicsserver/space.h>
#include <oxygen/oxygen.h>
#include <kerosin/renderserver/rendercontrol.h>
#include <kerosin/inputserver/inputcontrol.h>
#include <kerosin/kerosin.h>

#include "openglmanager.h"
#include "guisimcontrol.h"

#include "cutelogger/logstream.h"

#include <QFileInfo>

#if __APPLE__
#include <SDL.h>
#endif


//--------------------------------------------------------------
// Struct SimSpark::RubyValue
//--------------------------------------------------------------

SimSpark::RubyValue::RubyValue() :
    mType (RVT_NIL), mString(""), mInt(0), mFloat(0), mBool(false){}
SimSpark::RubyValue::RubyValue(std::string val) :
    mType (RVT_STRING), mString(val), mInt(0), mFloat(0), mBool(false){}
SimSpark::RubyValue::RubyValue(int val) :
    mType (RVT_INT), mString(""), mInt(val), mFloat(0), mBool(false){}
SimSpark::RubyValue::RubyValue(float val) :
    mType (RVT_FLOAT), mString(""), mInt(0), mFloat(val), mBool(false){}
SimSpark::RubyValue::RubyValue(bool val) :
    mType (RVT_BOOL), mString(""), mInt(0), mFloat(0), mBool(val){}

std::string SimSpark::RubyValue::toString() const
{
    std::stringstream ss("");
    switch (mType)
    {
    case RVT_BOOL:   return mBool ? "true" : "false"; break;
    case RVT_INT:    ss << mInt; return ss.str(); break;
    case RVT_FLOAT:  ss << mFloat; return ss.str(); break;
    case RVT_STRING: ss << '"' << mString << '"'; return ss.str(); break;
    case RVT_NIL:    return "nil"; break;
    default:
        LOG_ERROR() << "Unknown RubyValue type " << mType;
        break;
    }
    return "";
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SimSpark::SimSpark(const std::string& sparkScriptsPath, const std::string& sparkScript, const std::string& scriptPath, const std::vector<std::string>& additionalScripts) :
        mState(SSS_PREINIT),
        Spark(sparkScriptsPath),
        mScriptPath(scriptPath),
        mSparkScript(sparkScript),
        mSparkScriptsPrefix(sparkScriptsPath),
        mAdditionalScripts(additionalScripts)
{
    mReset = false;
    mRunExecuted = false;
    mReady = false;
}

SimSpark::~SimSpark()
{
}

bool SimSpark::HasScriptValue(const std::string& name) const
{
    for (auto it = mScriptValues.begin(); it != mScriptValues.end(); it++)
    {
        if (it->first.compare(name) == 0)
        {
            return true;
        }
    }
    return false;
}

SimSpark::RubyValue SimSpark::GetScriptValue(const std::string& name) const
{
    for (auto it = mScriptValues.begin(); it != mScriptValues.end(); it++)
    {
        if (it->first.compare(name) == 0)
        {
            return it->first;
        }
    }
    return RubyValue();
}

void SimSpark::AddScriptValue(const std::string& name, const RubyValue& value)
{
    for (auto it = mScriptValues.begin(); it != mScriptValues.end(); it++)
    {
        if (it->first.compare(name) == 0)
        {
            it->second = value;
            return;
        }
    }

    mScriptValues.push_back(std::make_pair(name, value));
}

void SimSpark::RemoveScriptValue(const std::string& name)
{
    for (auto it = mScriptValues.begin(); it != mScriptValues.end(); it++)
    {
        if (it->first.compare(name) == 0)
        {
            mScriptValues.erase(it);
            return;
        }
    }
}

void SimSpark::ClearScriptValues()
{
    mScriptValues.clear();
}

bool SimSpark::Init(int argc, char** argv)
{
    if (mState != SSS_PREINIT)
    {
        LOG_INFO() << "Simspark already initialized.";
        return true;
    }

    // init library cores
    mZeitgeist = boost::shared_ptr<zeitgeist::Zeitgeist>(new zeitgeist::Zeitgeist("." PACKAGE_NAME, mRelPathPrefix));
    mOxygen = boost::shared_ptr<oxygen::Oxygen>(new oxygen::Oxygen(*mZeitgeist));
    mKerosin = boost::shared_ptr<kerosin::Kerosin>(new kerosin::Kerosin(*mZeitgeist));

    // add log streams from preinit list

    for (auto it = mLogStreams.begin(); it != mLogStreams.end(); it++)
    {
        GetLog()->AddStream(&*(it->mStream), it->mLevel, it->mSync);
    }
    mLogStreams.clear();

    // run the spark init script
    mZeitgeist->GetCore()->GetScriptServer()->RunInitScript(
         mSparkScript,
         "lib/spark",
         zeitgeist::ScriptServer::IS_USERLOCAL
         );

    UpdateCached();

    // install additional gui classes
    mZeitgeist->GetCore()->RegisterClassObject(new CLASS(GuiSimControl), "gui/");

    // run the app defined init
    return InitApp(argc,argv);
}

bool SimSpark::Reinit(const std::string& sparkScriptsPath, const std::string& sparkScript, const std::string& scriptPath, const std::vector<std::string>& additionalScripts)
{
    if (!mReset)
    {
        LOG_ERROR() << "Cant reinitialize spark. It has to be Reset() after execution.";
        return false;
    }

    LOG_INFO() << "Reinitializing SimSpark...";

    //Change script paths
    if (mSparkScriptsPrefix.compare(sparkScriptsPath) != 0)
    {
        LOG_ERROR() << "Tried to reinitialize spark with a different init script path. This is not possible at the moment.";
        return false;
    }
    if (mScriptPath.compare(scriptPath) != 0)
    {
        LOG_ERROR() << "Tried to reinitialize spark with a different main script. This is not possible at the moment.";
        return false;
    }

    mScriptPath = scriptPath;
    mSparkScript = sparkScript;
    mAdditionalScripts = additionalScripts;

    //Reinitialize zeitgeist

    /*
    // {#Spark-Reset#}
    // When resetting the core, do initialization again: add resource locations, update the cache, run initialization scripts
    // (or even better: do not use this function but create a new SimSpark object)

    //Add resources for current simulation
    for (int i = 0; i < mResourceLocations.size(); i++)
    {
        GetCore()->GetFileServer()->AddResourceLocation(mResourceLocations.at(i));
    }

    UpdateCached();
    */

    //Remove all log streams (they might be reinitialized)
    if (GetCore().get() && GetCore()->GetLogServer().get())
    {
        GetCore()->GetLogServer()->RemoveAllStreams();
    }

    UpdateCached();

    return true;
}

bool SimSpark::Reset()
{
    if (mReset)
    {
        LOG_INFO() << "Already resetted.";
        return true;
    }

    if (mState == SSS_INITIALIZED)
    {
        LOG_INFO() << "Can only reset after running.";
        return true;
    }

    if (mState == SSS_PREINIT)
    {
        LOG_INFO() << "Can only reset after running.";
        return true;
    }

    //Pause simulation first
    //if (mState == SSS_RUNNING)
        //Pause();

    //Initialized or stopped -> reset to initialized
    //{#Spark-Reset#}: If it should ever be possible to completely reset SimSpark, resetting to PreInitialized and re-running the init-script in Reinit() would be a better choice here
    mState = SSS_INITIALIZED;
    mReset = true;

    LOG_INFO() << "Resetting SimSpark...";

    /* {#Spark-Reset#}
    // Tried to reset, but the internal connections are nearly impossible to find. Removing anything crashes something else.
    // The removal functions have to be implemented in soccer, soccermonitor and zeitgeist!

    zeitgeist::GCValue path;
    GetCore()->GetScriptServer()->Eval("$scenePath", path);
    std::string pathStr;
    path.GetString(pathStr);
    boost::shared_ptr<zeitgeist::Leaf> leaf(GetCore()->Get(pathStr));
    boost::shared_ptr<oxygen::Scene> scene = boost::shared_ptr<oxygen::Scene>(boost::shared_static_cast<oxygen::Scene>(leaf));
    boost::shared_ptr<oxygen::World> world;
    boost::shared_ptr<oxygen::Space> space;

    for (auto it = scene->begin(); it != scene->end(); it++)
    {
        if (it->get()->GetName().compare("world") == 0)
        {
            world = boost::shared_static_cast<oxygen::World>(*it);
            scene->RemoveChildReference(*it);
            break;
        }
    }
    for (auto it = scene->begin(); it != scene->end(); it++)
    {
        if (it->get()->GetName().compare("space") == 0)
        {
             space = boost::shared_static_cast<oxygen::Space>(*it);
             scene->RemoveChildReference(*it);
             break;
        }
    }

    //boost::shared_ptr<oxygen::GameControlServer> gc = boost::shared_static_cast<oxygen::GameControlServer>(GetCore()->Get("/sys/server/gamecontrol/"));
    //boost::shared_ptr<zeitgeist::Leaf> gc_state = gc->GetChild("GameStateAspect");
    //boost::shared_ptr<zeitgeist::Leaf> gc_ball = gc->GetChild("BallStateAspect");
    //boost::shared_ptr<zeitgeist::Leaf> gc_rule = gc->GetChild("SoccerRuleAspect");
    //
    //gc->UnlinkChildren();
    //gc->Unlink();
    //gc_state.reset();
    //gc_ball.reset();
    //gc_rule.reset();
    //gc.reset();

    boost::shared_ptr<zeitgeist::Node> root = boost::shared_static_cast<zeitgeist::Node>(GetCore()->GetRoot());
    boost::shared_ptr<zeitgeist::Leaf> root_leaf(GetCore()->GetRoot());

    scene->UnlinkChildren();
    GetCore()->GetScriptServer()->Eval("sparkResetEnvironment()");
    world->Unlink();
    space->Unlink();
    world.reset();
    space.reset();
    root->UpdateCached();
    scene->UpdateCache(true);
    scene.reset();
    UpdateCached();
    GetCore()->GetScriptServer()->Eval("sparkResetEnvironment()");
    //root->UpdateCached();
    //UpdateCached();*/

    //GetCore()->GetScriptServer()->Eval("sparkResetEnvironment()");

    return true;
}

bool SimSpark::Quit()
{
    //Quits simulation loop and resets simspark
    if (GetSimulationServer().get())
        GetSimulationServer()->Quit();
    else
    {
        LOG_ERROR() << "Cant quit. SimulationServer not found.";
        return false;
    }
    return true;
}

bool SimSpark::IsInitialized()
{
    return mState == SSS_INITIALIZED || IsRunning() || IsPaused();
}

bool SimSpark::IsRunning()
{
    return mState == SSS_RUNNING;
}

bool SimSpark::IsPaused()
{
    return mState == SSS_PAUSED;
}

void SimSpark::AddResourceLocation(const std::string& location)
{
    if (mState != SSS_PREINIT)
        GetCore()->GetFileServer()->AddResourceLocation(location);

    mResourceLocations.push_back(location);
}

void SimSpark::AddLogStream(boost::shared_ptr<LogStream> stream, zeitgeist::LogServer::EPriorityLevel level, bool syncStream)
{
    if (mState == SSS_PREINIT)
        mLogStreams.push_back(LogStreamDefinition(stream, level, syncStream)); //keep stream and add while initializing
    else
        GetLog()->AddStream(&*stream, level, true); //add stream directly
}

int SimSpark::ExecuteRunScripts()
{
    if (mState == SSS_PREINIT)
    {
        LOG_INFO() << "Cant run simspark. It has to be initialized using Init() first.";
        return 1;
    }
    if (mState == SSS_RUNNING)
    {
        LOG_INFO() << "SimSpark already running.";
        return 0;
    }
    if (mState == SSS_PAUSED)
    {
        LOG_INFO() << "Unpause SimSpark with Continue()!";
        return 1;
    }

    if (mRunExecuted && !mReset)
    {
        LOG_WARNING() << "Spark has not been reset after last execution.";
    }

    //Init ruby variables from script value list
    boost::shared_ptr<zeitgeist::ScriptServer> scriptServer = GetCore()->GetScriptServer();
    if (scriptServer.get() == 0)
    {
        LOG_ERROR() << "Could not find ScriptServer. Aborting execution.";
        return 1;
    }
    for (auto it = mScriptValues.begin(); it != mScriptValues.end(); it++)
    {
        std::stringstream command;
        bool isString = (it->second.mType == RVT_STRING);
        command << "$" << it->first << " = " << it->second.toString();
        LOG_INFO() << "Setting script variable: " << command.str().c_str();
        scriptServer->Eval(command.str());
    }

    if (mRunExecuted == false)
    {
        //{#Spark-Reset#}
        //Can only execute main script once. When complete reset is possible, reset simspark in the "Reset()" function, and call the main script here every time.

        //Run main simulation initialization script
        GetCore()->GetScriptServer()->Run(mScriptPath);
    }
    bool ok = UpdateCached();

    if (!ok)
    {
        LOG_ERROR() << "Error in spark initialization (main script). Aborting execution.";
        return 1;
    }

    //Run additional scripts (even after restarting)
    for (auto it = mAdditionalScripts.begin(); it != mAdditionalScripts.end(); it++)
    {
        ok = RunScript(*it);

        if (!ok)
        {
            LOG_ERROR() << "Script execution failed. Aborting run.";
            return 1;
        }

        ok = UpdateCached();
        if (!ok)
        {
            LOG_ERROR() << "Error in spark initialization (updating cache after script '" << it->c_str() << "'). Aborting execution.";
            return 1;
        }
    }

    mReady = true; //Ready to call Run()
    return 0;
}

int SimSpark::Run()
{
    if (!mReady)
    {
        LOG_INFO() << "Cant run simspark without executing run scripts.";
        return 1;
    }
    mReady = false;

    // Run simulation server
    mState = SSS_RUNNING;
    GetSimulationServer()->SetSimStep(0.02f);
    GetSimulationServer()->Run();
    mRunExecuted = true;

    bool ok = Reset();
    mState = SSS_INITIALIZED; //State is "re-runnable"

    if (!ok)
    {
        LOG_ERROR() << "Spark reset failed.";
    }

    return 0;
}

bool SimSpark::Pause()
{
    if (mState == SSS_PREINIT)
    {
        LOG_INFO() << "Cant pause simspark. It has to be initialized using Init() first.";
        return false;
    }
    if (mState == SSS_INITIALIZED)
    {
        LOG_INFO() << "SimSpark is not running.";
        return false;
    }
    if (mState == SSS_PAUSED)
    {
        LOG_INFO() << "SimSpark already paused.";
        return false;
    }

    //pause simulation server
    GetSimulationServer()->PauseCycle(true);

    mState = SSS_PAUSED;

    return true;
}

bool SimSpark::Continue()
{
    if (mState == SSS_PREINIT)
    {
        LOG_INFO() << "Cant unpause simspark. It has to be initialized using Init() first.";
        return false;
    }
    if (mState == SSS_INITIALIZED)
    {
        LOG_INFO() << "SimSpark is not running.";
        return false;
    }
    if (mState == SSS_RUNNING)
    {
        LOG_INFO() << "SimSpark is not paused.";
        return false;
    }

    //unpause simulation server
    GetSimulationServer()->PauseCycle(false);

    mState = SSS_RUNNING;

    return true;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

bool SimSpark::InitApp(int argc, char** argv)
{
    //Add resources for current simulation
    for (int i = 0; i < (int)mResourceLocations.size(); i++)
    {
        GetCore()->GetFileServer()->AddResourceLocation(mResourceLocations.at(i));
    }

    // process command line
    /*if (! ProcessCmdLine(argc, argv))
    {
        return false;
    }*/

    mState = SSS_INITIALIZED;

    return true;
}

bool SimSpark::RunScript(const std::string& path)
{
    LOG_INFO() << "Running script '" << path.c_str() << "'...";
    bool ok = GetScriptServer()->Run(path);

    // Error logging using ruby was not possible at this point. Including ruby.h or zeitgeist/scriptserver/rubywrapper.h
    // caused some unresolvable compile-errors in combination with Qt. The project probably has to be upgraded to ruby 1.9 to
    // make it possible. Therefore, the log info about error scripts cant be read out. The user has to check on the console instead.

    if (!ok)
    {
        LOG_ERROR() << "Error while executing script '" << path.c_str() << "."; //"':\n" << zeitgeist::QRbGetError().c_str();
        return false;
    }

    return true;
}
