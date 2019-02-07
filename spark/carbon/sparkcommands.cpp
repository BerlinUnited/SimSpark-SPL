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

#include "sparkcommands.h"
#include "sparkcommandqueue.h"
#include "sparkcontroller.h"
#include "simspark.h"
#include "cutelogger/logger.h"

#include <oxygen/sceneserver/sceneimporter.h>
#include <oxygen/sceneserver/basenode.h>
#include <oxygen/sceneserver/scenedict.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/leaf.h>
#include <zeitgeist/node.h>

using namespace boost;
using namespace zeitgeist;

namespace SparkCommand
{
//--------------------------------------------------------------
// Command base class
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

Command::~Command()
{
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

Command::Command(int type) 
    : AbstractCommand(type)
{
}

SparkController* Command::getController() 
{
    return static_cast<CommandQueue*>(mParentQueue)->mController;
}

//--------------------------------------------------------------
// Derived classes
//--------------------------------------------------------------
// EvalScript
//--------------------------------------------------------------

bool EvalScript::execute()
{
    if (!getController())
    {
        LOG_ERROR() << "No SparkController specified.";
        return false;
    }

    shared_ptr<ScriptServer> server = shared_ptr<ScriptServer>(getController()->getSpark()->GetScriptServer());
    if (!server.get())
        server = shared_ptr<ScriptServer>(getController()->getSpark()->GetCore()->GetScriptServer());

    if (!server.get())
    {
        LOG_ERROR() << "ScriptServer not found.";
        return false;
    }

    return server->Eval(mCommandString.toStdString());
}

//--------------------------------------------------------------
// ExecuteScriptFile
//--------------------------------------------------------------

bool ExecuteScriptFile::execute()
{
    if (!getController())
    {
        LOG_ERROR() << "No SparkController specified.";
        return false;
    }

    return getController()->runScript(mFile);
}

//--------------------------------------------------------------
// ImportScene
//--------------------------------------------------------------

bool ImportScene::execute()
{
    if (!getController())
    {
        LOG_ERROR() << "No SparkController specified.";
        return false;
    }

    if (mNode.get() == 0)
    {
        LOG_ERROR() << "Empty base node pointer.";
        return false;
    }

    boost::shared_ptr<oxygen::SceneImporter> sceneImporter = 
        boost::static_pointer_cast<oxygen::SceneImporter>(getController()->getSpark()->GetCore()->Get("sys/server/scene/RubySceneImporter"));
    sceneImporter->SetSceneDict(&oxygen::SceneDict::GetInstance());
    if (sceneImporter.get() == 0)
    {
        LOG_ERROR() << "RubySceneImporter not found. Expected it to be at: 'sys/server/scene/RubySceneImporter'";
        return false;
    }

    LOG_INFO() << "Importing scene " << mFile << " to node " << QString(mNode->GetName().c_str());
    sceneImporter->ImportScene(mFile.toStdString(), mNode, mParameters);  
    return true;
}

//--------------------------------------------------------------
// RemoveLeaf
//--------------------------------------------------------------

bool RemoveLeaf::execute()
{
    mLeaf->Unlink();
    return true;
}

//--------------------------------------------------------------
// ReleaseRuntimeAccessDelayed
//--------------------------------------------------------------

bool ReleaseRuntimeAccessDelayed::execute()
{
    if (!getController())
        return false;

    if (mWaited < mWaitCycles)
    {
        mWaited++;
        prolong(); //check again in next cycle
        return true;
    }
    else
    {
        getController()->releaseRuntimeAccess(mExclusive);
    }
    return true;
}

//--------------------------------------------------------------
// ChangeLogLevel
//--------------------------------------------------------------

bool ChangeLogLevel::execute()
{
    if (!getController())
        return false;

    getController()->setLogPropertiesProtected(mLevel, mSource);
    return true;
}


} //namespace SparkCommands
