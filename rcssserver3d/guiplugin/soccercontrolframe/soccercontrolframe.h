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

#ifndef SOCCERCONTROLFRAME_H
#define SOCCERCONTROLFRAME_H
/**
 * \file soccercontrolframe.h
 * \brief Contains SoccerControlFrame and SoccerFrameUpdateControl classes.
 */

#include "ui_soccercontrolframe.h"
#include "attachableframe.h"
#include "isimcontrol.h"
#include "sparkcommandqueue.h"

#include <boost/shared_ptr.hpp>
#include <QObject>
#include <QTimer>

#include <soccer/soccertypes.h>

class ServerThread;
class SimSpark;
class Settings;

class SoccerFrameUpdateControl;
class SoccerControlFrame;

class GameStateAspect;
class SoccerRuleAspect;
class BallStateAspect;

namespace oxygen
{
    class GameControlServer;
}

/*!
 \brief Update command queue read and set soccer variables and settings in the SimSpark simulation.
*/
class SoccerFrameUpdateControl : public QObject, public SparkCommand::CommandQueue // is ISimControl
{
    Q_OBJECT

public:
    //public functions

    /*!
     \brief Constructor.
    */
    SoccerFrameUpdateControl(SoccerControlFrame* parent);
    /*!
     \brief Destructor.
    */
    ~SoccerFrameUpdateControl();

    /*!
     \brief Called when the simulation cycle starts.

     Emits stateStartCycle()
    */
    virtual void startCycle();
    /*!
     \brief Called when the simulation cycle ends.

     Executes update commands.
     Emits stateEndCycle()
    */
    virtual void endCycle();
    /*!
     \brief Called when the simulation executes a wait-cycle.

     Executes update commands.
     Emits stateWaitCycle()
    */
    virtual void waitCycle();

public: signals:
    //public signals

    /*!
     \brief Emittet when simualtion cycle starts.

     Calls update function.
    */
    void stateStartCycle();
    /*!
     \brief Emitted when simulation cycle ends.

     Calls update function.
    */
    void stateEndCycle();
    /*!
     \brief Emitted when simulation executes a wait-cycle.

     Calls update function.
    */
    void stateWaitCycle();

public: 
    //public members

    SoccerControlFrame* mParent; /*!< Parent frame. */
};

/*!
 \brief AttachableFrame plugin that displays the state of a Ssoccer simulation and can control it.

 The SoccerControlFrame will wait and try to identify a soccer simulation in every Spark ServerThread that is started.
 When it has found a ServerThread that runs a soccer simulation, the frame will register an updater to the SparkController and repeatedly update the 
 view on soccer variables like the team names, goals and the game time.

 The soccer values can be changed. 
 A List of SoccerFrameUtil::SoccerCommand derived classes can queue update calls for soccer variables and safely change the current game state.

 \todo Player control not implemented yet. Anyway, for player control to be effective there need to be additional functions to control them.
 At the moment the only available functions would be battery state and temperature.

 \todo Currently the frame only controls basic soccer game control with the GameStateAspect and GameControlServer.
 Additional control with the BallStateAspect, SoccerRuleAspect and the trainer command parser could be added.
*/
class SoccerControlFrame : public AttachableFrame
{
    Q_OBJECT
    GUI_FRAME

public: 
    //public types

    /*!
     \brief Description of the current game mode.

     Contains all the soccer game modes (BeforeKickoff, GoalLeft, FreekickRight and so on) and is robust to extension of the game modes.
    */
    struct GameMode
    {
    public:
        //public functions

        /*!
         \brief full constructor.

         \param index index of the game mode in the combobox
         \param mode spark game mode id
         \param manual can it be selected by the user
         \param name name string of the game mode
        */
        GameMode(int index, TPlayMode mode, bool manual, QString name)
            : mIndex(index), mMode(mode), mName(name), mManual(manual) {}

        TPlayMode mMode; /*!< spark game mode id */
        QString mName; /*!< name string of the game mode */
        int mIndex; /*!< index of the game mode in the combobox */
        bool mManual; /*!< can it be selected by the user */
    };

public:
    // public functions

    /*!
     \brief Constructor.
    */
    SoccerControlFrame();
    /*!
     \brief Destructor.
    */
    ~SoccerControlFrame();

public:
    //public virtual functions

    /*!
     \brief Initialization function.

     \param parameters parameter list (not used)
    */
    virtual void init(const QStringList& parameters);

    /*!
     \brief Loads settings from global settings object.
    */
    virtual void loadSettings();
    /*!
     \brief Saves settings to global settings object.
    */
    virtual void saveSettings();
    /*!
     \brief Initializes settings with default values.
    */
    virtual void loadDefaultSettings();

    /*!
     \brief Returns true after initialization of SimSpark simulation access.
    */
    bool initialized();

protected:
    //protected functions

    /*!
     \brief Tries to initialize spark simulation access.

     Will fail if there is no Spark-Server, if the server has no SparkController, if the controller has not initialized SimSpark, or
     if the Simulation does not have the required soccer control nodes.

     Tries to initialize again every time a server thread was created or started.
    */
    bool tryInitSpark();
    /*!
     \brief Registers the updater control to the SparkController.

     Called when spark access was initialized successfully.
    */
    void registerUpdater();

protected slots:
    //protected slots

    /*!
     \brief Tries to initialize SimSpark access after a server was created.

     \param server serverthread that was created
    */
    void updateServerCreated(boost::shared_ptr<ServerThread> server);
    /*!
     \brief Tries to initialize SimSpark access after spark was started.

     \param controller that startet SimSpark
    */
    void updateSparkRunning(SparkController* controller);

    /*!
     \brief Update function called after a run or wait simulation cycle finished.

     Disables updates until the update timer ticks for the next time and calls readyUpdate().
    */
    void onCycleFinished();

public slots:
    //public slots

    /*!
     \brief Causes an update of the display the next time a simulation cycle ends.
    */
    void readyUpdate();
    /*!
     \brief Updates the data fields with the current soccer variables.
    */
    void updateDisplay();
    /*!
     \brief updates the data fields of the currently selected agent.

     \todo not implemented yet
    */
    void updatePlayerDisplay();

    /*!
     \brief Updates the game time from the ui text edit.
    */
    void editGameTime();
    /*!
     \brief Resets the game time to the first value recorded.
    */
    void resetGameTime();

    /*!
     \brief Updates team 1 goals from the ui text edit.
    */
    void editTeam1Goals();
    /*!
     \brief Updates team 2 goals from the ui text edit.
    */
    void editTeam2Goals();
    /*!
     \brief Updates team 1 penalties from the ui text edit.

     \todo Penalties are just a possibility. They are not recorded in SimSpark yet.
    */
    void editTeam1Penalties();
    /*!
     \brief Updates team 2 penalties from the ui text edit.

     \todo Penalties are just a possibility. They are not recorded in SimSpark yet.
    */
    void editTeam2Penalties();

    /*!
     \brief Select a play mode from the ui combo box. It will be changed after confirmation by a button click.

     \param index index of the mode entry
    */
    void selectPlayMode(int index);
    /*!
     \brief Change the current game mode as chosen in the ui combo box.
    */
    void changePlayMode();
    
    /*!
     \brief Change game mode to KickOff.
    */
    void kickOff();
    /*!
     \brief Moves players away from the ball and resets their position.

     \todo not implemented yet, because there is no direct implementation in SoccerRuleAspect. Integrate the commands of trainer command parser
     for this.
    */
    void clearPlayers();

    /*!
     \brief Chooses an agent by index.

     \todo player control not implemented yet
     \param index index of the player
    */
    void selectPlayer(int index);
    /*!
     \brief Changes the battery value of an agent from the ui text edit.

     \todo player control not implemented yet
    */
    void editBattery();
    /*!
     \brief Changes the temperature value of an agent from the ui text edit.

     \todo player control not implemented yet
    */
    void editTemperature();
    /*!
     \brief Resets the battery of an agent to the first recorded value.

     \todo player control not implemented yet
    */
    void resetBattery();
    /*!
     \brief Resets the temperature of an agent to the first recorded value.

     \todo player control not implemented yet
    */
    void resetTemperature();

private:
    // private members

    Ui::SoccerControlFrame ui; /*!< Ui definition object. */
    boost::shared_ptr<Settings> mSettings; /*!< Global Settings object. */
    boost::shared_ptr<SoccerFrameUpdateControl> mUpdater; /*!< Update control for the soccer frame. Registered to the current SparkControllers GuiSimControl node. */
    boost::shared_ptr<SparkController> mSparkController; /*!< SparkController of the observed simualtion. */
    QTimer mUpdateTimer; /*!< Update timer. When it ticks, the next simualtion cycle will update the display. */

    bool mInit;          /*!< True after spark access was initialized. */
    bool mLastInitState; /*!< Initialization state during last init try. */
    int mSelectedPlayer; /*!< Currently selected agents index. */
    int mUpdateCount;    /*!< Amount of updates calls since last reset. */
    bool mReadyUpdate;   /*!< If true (activated by update timer) update will be called in next simulation cycle. */
    TPlayMode mSelectedPlayMode; /*!< Currently selected play mode in ui combo box. */

    boost::shared_ptr<ServerThread> mServerThread;                   /*!< Observes server thread. */
    boost::shared_ptr<oxygen::GameControlServer> mGameControlServer; /*!< Observes game control server. */
    boost::shared_ptr<SoccerRuleAspect> mRuleAspect;                 /*!< Observes servers soccer rule aspect. */
    boost::shared_ptr<BallStateAspect> mBallAspect;                  /*!< Observed servers ball state asoect. */
    boost::shared_ptr<GameStateAspect> mGameAspect;                  /*!< Observed servers game state aspect. */
    //std::vector<boost::shared_ptr<AgentAspect>> mAgentAspects;       /*!< Observed servers agent state aspects. */

    QString mGameControlServerPath; /*!< Expected scene graph path of game control server. (Loaded from settings) */
    QString mGameStateAspectPath;   /*!< Expected scene graph path of game state aspect. (Loaded from settings) */
    QString mSoccerRuleAspectPath;  /*!< Expected scene graph path of soccer rule aspect. (Loaded from settings) */
    QString mBallStateAspectPath;   /*!< Expected scene graph path of ball state aspect. (Loaded from settings) */

    QMap<int, GameMode> mGameModes; /*!< Map containing all of the game modes with their TPlayMode indices. */
};

#endif // SOCCERCONTROLFRAME_H
