/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Author: Patrick Geib
   Copyright (C) 2012 RoboCup Soccer Server 3D Maintenance Group
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

//Local
#include "soccercontrolframe.h"
#include "soccercontrolframe_soccercommands.h"
#include "export.h"

//Carbon
#include <pluginmanager.h>
#include <carbon.h>
#include <simulationmanager.h>
#include <simulation.h>
#include <sparkcontroller.h>
#include <guisimcontrol.h>
#include <serverthread.h>
#include <simspark.h>
#include <settings.h>

//SimSpark
#include <oxygen/gamecontrolserver/gamecontrolserver.h>
#include <zeitgeist/leaf.h>

//Soccer
#include "soccer/gamestateaspect/gamestateaspect.h"
#include "soccer/soccerruleaspect/soccerruleaspect.h"
#include "soccer/ballstateaspect/ballstateaspect.h"
//#include <agentstate/agentstate.h>

//Boost
#include <boost/make_shared.hpp>

//Qt
#include <QValidator>
 
using namespace boost;
using namespace oxygen;
using namespace SoccerControlFrameUtil;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//Factory registration
REGISTER_EXPORT_FRAME(SoccerControlFrame, "soccercontrolframe", "Soccer", 1000)

//Static class information
void SoccerControlFrame::saveClassInfo()
{
    //Description
    setClassInfo(mClassId, AbstractPlugin::PI_DESCRIPTION, 
        tr("The soccer control frame can be used to monitor and control the state of a simspark soccer simulation."));

    //Icon
    setClassInfo(mClassId, AbstractPlugin::PI_ICON_NAME, ":soccer");

    //Tags
    QStringList tags;
    tags << "soccer" << "spark" << "rcssserver3d";
    setClassInfo(mClassId, AbstractPlugin::PI_TAGS, tags);
}

//--------------------------------------------------------------
// SoccerFrameUpdateControl
//--------------------------------------------------------------

SoccerFrameUpdateControl::SoccerFrameUpdateControl(SoccerControlFrame* parent)
    : mParent(parent), CommandQueue(0)
{
}
SoccerFrameUpdateControl::~SoccerFrameUpdateControl()
{
}

void SoccerFrameUpdateControl::startCycle()
{
    emit stateStartCycle();
}

void SoccerFrameUpdateControl::endCycle()
{
    SparkCommand::CommandQueue::endCycle();
    emit stateEndCycle();
}

void SoccerFrameUpdateControl::waitCycle()
{
    SparkCommand::CommandQueue::waitCycle();
    emit stateWaitCycle();
}

//--------------------------------------------------------------
// SoccerFrame
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SoccerControlFrame::SoccerControlFrame() :
    mSettings(getCarbon()->getSettings())
{
    //----------------
    //Layout
    ui.setupUi(this);
    loadFrameDesign();

    //----------------
    //Variables
    mInit = false;
    mLastInitState = true; //--> 1 display update during initialization
    mReadyUpdate = true;
    mSelectedPlayer = -1;
    mSelectedPlayMode = PM_NONE;
    mUpdateTimer.setInterval(100);

    //----------------
    //Gui-Field init
    ui.gameTimeEdit->setValidator(new QDoubleValidator(0.0, INT_MAX, 4, ui.gameTimeEdit));
    ui.team1GoalsEdit->setValidator(new QIntValidator(0, INT_MAX, ui.team1GoalsEdit));
    ui.team2GoalsEdit->setValidator(new QIntValidator(0, INT_MAX, ui.team2GoalsEdit));
    ui.team1PenaltiesEdit->setValidator(new QIntValidator(0, INT_MAX, ui.team1PenaltiesEdit));
    ui.team2PenaltiesEdit->setValidator(new QIntValidator(0, INT_MAX, ui.team2PenaltiesEdit));
    ui.batteryEdit->setValidator(new QDoubleValidator(0.0, INT_MAX, 4, ui.batteryEdit));

    //----------------
    //Settings

    loadDefaultSettings();
    loadSettings();

    //----------------
    //Menu Entries

    //----------------
    //Connections
    Simulation* simulation = &*getCarbon()->getSimulationManager()->getSimulation();
    connect(simulation, SIGNAL(serverCreated(boost::shared_ptr<ServerThread>)), this, SLOT(updateServerCreated(boost::shared_ptr<ServerThread>)));

    connect(&mUpdateTimer, SIGNAL(timeout()), this, SLOT(readyUpdate()));
}

SoccerControlFrame::~SoccerControlFrame()
{

}

void SoccerControlFrame::init(const QStringList& parameters)
{
    //Combo box entries
    mGameModes.clear();
    ui.playModeComboBox->clear();

    mGameModes.insert(PM_BeforeKickOff,     GameMode(0, PM_BeforeKickOff,     true, tr("Before Kickoff")));
    mGameModes.insert(PM_KickOff_Left,      GameMode(0, PM_KickOff_Left,      true, tr("KickOff Left")));
    mGameModes.insert(PM_KickOff_Right,     GameMode(0, PM_KickOff_Right,     true, tr("KickOff Right")));
    mGameModes.insert(PM_PlayOn,            GameMode(0, PM_PlayOn,            true, tr("Play On")));
    mGameModes.insert(PM_KickIn_Left,       GameMode(0, PM_KickIn_Left,       true, tr("Kick-In Left")));
    mGameModes.insert(PM_KickIn_Right,      GameMode(0, PM_KickIn_Right,      true, tr("Kick-In Right")));
    mGameModes.insert(PM_CORNER_KICK_LEFT,  GameMode(0, PM_CORNER_KICK_LEFT,  true, tr("Corner Kick Left")));
    mGameModes.insert(PM_CORNER_KICK_RIGHT, GameMode(0, PM_CORNER_KICK_RIGHT, true, tr("Corner Kick Right")));
    mGameModes.insert(PM_GOAL_KICK_LEFT,    GameMode(0, PM_GOAL_KICK_LEFT,    true, tr("Goal Kick Left")));
    mGameModes.insert(PM_GOAL_KICK_RIGHT,   GameMode(0, PM_GOAL_KICK_RIGHT,   true, tr("Goal Kick Right")));
    mGameModes.insert(PM_OFFSIDE_LEFT,      GameMode(0, PM_OFFSIDE_LEFT,      true, tr("Offside Left")));
    mGameModes.insert(PM_OFFSIDE_RIGHT,     GameMode(0, PM_OFFSIDE_RIGHT,     true, tr("Offside Right")));
    mGameModes.insert(PM_GameOver,          GameMode(0, PM_GameOver,          true, tr("Game Over")));
    mGameModes.insert(PM_Goal_Left,         GameMode(0, PM_Goal_Left,         true, tr("Goal Left")));
    mGameModes.insert(PM_Goal_Right,        GameMode(0, PM_Goal_Right,        true, tr("Goal Right")));
    mGameModes.insert(PM_FREE_KICK_LEFT,    GameMode(0, PM_FREE_KICK_LEFT,    true, tr("Free Kick Left")));
    mGameModes.insert(PM_FREE_KICK_RIGHT,   GameMode(0, PM_FREE_KICK_RIGHT,   true, tr("Free Kick Right")));

    int run = 0;
    for (auto it = mGameModes.begin(); it != mGameModes.end(); it++)
    {
        it->mIndex = run;
        ui.playModeComboBox->addItem(it->mName);
        run++;
    }
    mSelectedPlayMode = PM_BeforeKickOff;

    tryInitSpark();
    updateDisplay();
}

bool SoccerControlFrame::initialized()
{
    return mInit;
}

void SoccerControlFrame::loadSettings()
{

}

void SoccerControlFrame::saveSettings()
{

}

void SoccerControlFrame::loadDefaultSettings()
{
    mGameControlServerPath = "/sys/server/gamecontrol/";
    mGameStateAspectPath   = "/sys/server/gamecontrol/GameStateAspect/";
    mSoccerRuleAspectPath  = "/sys/server/gamecontrol/SoccerRuleAspect/";
    mBallStateAspectPath   = "/sys/server/gamecontrol/BallStateAspect/";
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

bool SoccerControlFrame::tryInitSpark()
{
    //If already initialized, simply update
    if (mInit)
        updateDisplay();

    //If there is no spark controller, try to get it
    if (mSparkController.get() == 0)
    {
        mServerThread = getCarbon()->getSimulationManager()->getSparkServerThread();

        if (mServerThread.get() == 0)
            return false;
        else
        {
            mSparkController = mServerThread->getSparkController();
            connect(&*mSparkController, SIGNAL(sparkRunning(SparkController*)), this, SLOT(updateSparkRunning(SparkController*)));
        }
    }

    //If there is a spark controller, check for SimSpark
    if (mSparkController->getSpark().get() != 0)
    {
        //There is a controller and simspark-> initialize

        QString gameControlServer = mGameControlServerPath;
        QString gameStateAspect   = mGameStateAspectPath;
        QString soccerRuleAspect  = mSoccerRuleAspectPath;
        QString ballStateAspect   = mBallStateAspectPath;

        //Try to find soccer aspects
        if (mSettings->beginGroup(this, true, 1000)) //lock
        {
            gameControlServer = mSettings->value("mGameControlServerPath", mGameControlServerPath).toString();
            gameStateAspect   = mSettings->value("mGameStateAspectPath", mGameStateAspectPath).toString();
            soccerRuleAspect  = mSettings->value("mSoccerRuleAspectPath", mSoccerRuleAspectPath).toString();
            ballStateAspect   = mSettings->value("mBallStateAspectPath", mBallStateAspectPath).toString();
            mSettings->endGroup(true); //unlock
        }
        else
        {
            LOG_WARNING() << "Could not read Settings object in SoccerControlFrame. Using default values for control node paths.";
        }

        SimSpark* spark = &*mSparkController->getSpark();
        boost::shared_ptr<GameControlServer> gameControl(shared_dynamic_cast<GameControlServer>(spark->GetCore()->Get(gameControlServer.toStdString())));
        boost::shared_ptr<GameStateAspect>   gameState  (shared_dynamic_cast<GameStateAspect>  (spark->GetCore()->Get(gameStateAspect.toStdString())));
        boost::shared_ptr<SoccerRuleAspect>  soccerRule (shared_dynamic_cast<SoccerRuleAspect> (spark->GetCore()->Get(soccerRuleAspect.toStdString())));
        boost::shared_ptr<BallStateAspect>   ballState  (shared_dynamic_cast<BallStateAspect>  (spark->GetCore()->Get(ballStateAspect.toStdString())));

        bool success = true;
        if (!gameControl.get())
        {
            LOG_ERROR() << "Soccer GameControlServer not found. Expected it to be at " << gameControlServer << ".";
            success = false;
        }
        if (!gameState.get())
        {
            LOG_ERROR() << "Soccer GameStateAspect not found. Expected it to be at " << gameStateAspect << ".";
            success = false;
        }
        if (!soccerRule.get())
        {
            LOG_ERROR() << "Soccer SoccerRuleAspect not found. Expected it to be at " << soccerRuleAspect << ".";
            success = false;
        }
        if (!ballState.get())
        {
            LOG_ERROR() << "Soccer BallStateAspect not found. Expected it to be at " << ballStateAspect << ".";
            success = false;
        }

        if (success == false)
        {
            //Init failed, return
            mGameControlServer.reset();
            mRuleAspect.reset();
            mBallAspect.reset();
            mGameAspect.reset();
            return false;
        }
        
        //All nodes found, init successful
        mGameControlServer = gameControl;
        mRuleAspect        = soccerRule;
        mBallAspect        = ballState;
        mGameAspect        = gameState;

        //Register control node that polls an update every time a cycle ended
        registerUpdater();

        mInit = true;
        updateDisplay();
        return true;
    }
    return false;
}

void SoccerControlFrame::registerUpdater()
{
    shared_ptr<GuiSimControl> simControl = mSparkController->getSimulationControl().lock();
    if (simControl.get() == 0)
    {
        LOG_ERROR() << "No gui simulation control node found to register updater in.";
        return;
    }

    //Remove old updater (should happen automatically, just to be safe)
    if (mUpdater.get() != 0)
    {
        simControl->unregisterSimControl(mUpdater);
        mUpdater.reset();
    }

    //Create new updater
    mUpdater = shared_ptr<SoccerFrameUpdateControl>(new SoccerFrameUpdateControl(this));
    simControl->registerSimControl(mUpdater);
    connect(&*mUpdater, SIGNAL(stateEndCycle()), this, SLOT(onCycleFinished())); //Queued connection
    connect(&*mUpdater, SIGNAL(stateWaitCycle()), this, SLOT(onCycleFinished())); //Queued connection

    //Start update timer
    mUpdateTimer.start();
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Protected Slots
//--------------------------------------------------------------

void SoccerControlFrame::updateServerCreated(boost::shared_ptr<ServerThread> server)
{
    mSparkController = server->getSparkController();
    connect(&*mSparkController, SIGNAL(sparkRunning(SparkController*)), this, SLOT(updateSparkRunning(SparkController*)));
    tryInitSpark();
}

void SoccerControlFrame::updateSparkRunning(SparkController* controller)
{
    if (mSparkController.get() != 0)
    {
        if (mSparkController->getSpark().get() == controller->getSpark().get())
        {
            tryInitSpark();
        }
    }
}

void SoccerControlFrame::onCycleFinished()
{
    updateDisplay();
    mReadyUpdate = false;
}

//--------------------------------------------------------------
// Public Slots
//--------------------------------------------------------------

void SoccerControlFrame::readyUpdate()
{
    mReadyUpdate = true;
}

void SoccerControlFrame::updateDisplay()
{
    if (mLastInitState != mInit)
    {
        mLastInitState = mInit;
        mUpdateCount = 0;

        ui.batteryEdit->setEnabled(mInit);
        ui.temperatureEdit->setEnabled(mInit);
        ui.gameTimeEdit->setEnabled(mInit);
        ui.team1GoalsEdit->setEnabled(mInit);
        ui.team2GoalsEdit->setEnabled(mInit);
        ui.team1PenaltiesEdit->setEnabled(mInit);
        ui.team2PenaltiesEdit->setEnabled(mInit);

        ui.gameTimeResetButton->setEnabled(mInit);
        ui.resetBatteryButton->setEnabled(mInit);
        ui.resetTemperatureButton->setEnabled(mInit);
        ui.kickOffButton->setEnabled(mInit);
        ui.playModeButton->setEnabled(mInit);
        ui.playModeComboBox->setEnabled(mInit);

        mSelectedPlayer = -1;
    }

    if (!mInit)
    {
        ui.batteryEdit->setText("");
        ui.temperatureEdit->setText("");
        ui.gameTimeEdit->setText("");
        ui.team1GoalsEdit->setText("");
        ui.team2GoalsEdit->setText("");
        ui.team1PenaltiesEdit->setText("");
        ui.team2PenaltiesEdit->setText("");
        ui.team1NameLabel->setText(tr("Left"));
        ui.team2NameLabel->setText(tr("Right"));
        ui.agentNameLabel->setText("");
        ui.agentTeamLabel->setText("");
        ui.playModeLabel->setText(tr("Not initialized."));
    }

    if (mInit && mReadyUpdate)
    {
        //Read and display non-changing values
        if (mUpdateCount == 0)
        {
        }

        //Read and update changing values
        ui.team1NameLabel->setText(mGameAspect->GetTeamName(TI_LEFT).c_str());
        ui.team2NameLabel->setText(mGameAspect->GetTeamName(TI_RIGHT).c_str());
        if (!ui.team1GoalsEdit->hasFocus()) ui.team1GoalsEdit->setText(QString::number(mGameAspect->GetScore(TI_LEFT)));
        if (!ui.team2GoalsEdit->hasFocus()) ui.team2GoalsEdit->setText(QString::number(mGameAspect->GetScore(TI_RIGHT)));
        if (!ui.team1PenaltiesEdit->hasFocus()) ui.team1PenaltiesEdit->setText("0");
        if (!ui.team2PenaltiesEdit->hasFocus()) ui.team2PenaltiesEdit->setText("0");

        if (!ui.gameTimeEdit->hasFocus()) ui.gameTimeEdit->setText(QString::number(mGameAspect->GetTime()));

        auto pos = mGameModes.find((int)mGameAspect->GetPlayMode());
        if (pos == mGameModes.end())
        {
            LOG_WARNING() << "Current game mode " << mGameAspect->GetPlayMode() << " is unknown.";
        }
        else
        {
            if (mUpdateCount == 0)
                ui.playModeComboBox->setCurrentIndex(pos->mIndex);
            ui.playModeLabel->setText(pos->mName);
        }

        updatePlayerDisplay();
    }

    mUpdateCount++;
}

void SoccerControlFrame::updatePlayerDisplay()
{
    if (mSelectedPlayer == -1)
        return;

    if (!mInit)
        return;
}

void SoccerControlFrame::editGameTime()
{
    if (!mInit)
        return;

    mUpdater->queueCommand(make_shared<SoccerControlFrameUtil::SetTime>(mGameAspect, ui.gameTimeEdit->text().toFloat()));
}

void SoccerControlFrame::resetGameTime()
{
    if (!mInit)
        return;

    mUpdater->queueCommand(make_shared<SoccerControlFrameUtil::SetTime>(mGameAspect, 0.0));
}

void SoccerControlFrame::editTeam1Goals()
{
    if (!mInit)
        return;

    mUpdater->queueCommand(make_shared<SoccerControlFrameUtil::SetTeamGoals>(mGameAspect, TI_LEFT, ui.team1GoalsEdit->text().toInt()));
}

void SoccerControlFrame::editTeam2Goals()
{
    if (!mInit)
        return;

    mUpdater->queueCommand(make_shared<SoccerControlFrameUtil::SetTeamGoals>(mGameAspect, TI_LEFT, ui.team2GoalsEdit->text().toInt()));
}

void SoccerControlFrame::editTeam1Penalties()
{
    if (!mInit)
        return;
}

void SoccerControlFrame::editTeam2Penalties()
{
    if (!mInit)
        return;
}

void SoccerControlFrame::selectPlayMode(int index)
{
    if (!mInit)
        return;

    ui.playModeButton->setEnabled(false);

    for (auto it = mGameModes.begin(); it != mGameModes.end(); it++)
    {
        if (it->mIndex == index)
        {
            mSelectedPlayMode = it->mMode;
            ui.playModeButton->setEnabled(it->mManual);
        }
    }
}

void SoccerControlFrame::changePlayMode()
{
    if (!mInit)
        return;

    mUpdater->queueCommand(make_shared<SoccerControlFrameUtil::ChangePlayMode>(mGameAspect, mSelectedPlayMode));
}

void SoccerControlFrame::kickOff()
{
    if (!mInit)
        return;

    mUpdater->queueCommand(make_shared<SoccerControlFrameUtil::KickOff>(mGameAspect));
}

void SoccerControlFrame::clearPlayers()
{
    if (!mInit)
        return;
}


void SoccerControlFrame::selectPlayer(int index)
{
    if (!mInit)
        return;

    mSelectedPlayer = -1;
    updatePlayerDisplay();
}

void SoccerControlFrame::editBattery()
{
    if (!mInit)
        return;

    if (mSelectedPlayer == -1)
        return;

}

void SoccerControlFrame::editTemperature()
{
    if (!mInit)
        return;

    if (mSelectedPlayer == -1)
        return;

}

void SoccerControlFrame::resetBattery()
{
    if (!mInit)
        return;

    if (mSelectedPlayer == -1)
        return;

}

void SoccerControlFrame::resetTemperature()
{
    if (!mInit)
        return;

    if (mSelectedPlayer == -1)
        return;


}