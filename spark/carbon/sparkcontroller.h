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

#ifndef SPARKCONTROLLER_H
#define SPARKCONTROLLER_H
/**
 * \file sparkcontroller.h
 * \brief Contains SparkController class, a QObject derived class used to initialize, run and control SimSpark.
 */

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <QObject>
#include <QStringList>
#include <QSemaphore>
#include <vector>

#include "cutelogger/logger.h"
#include "guisimcontrol.h"
#include "sparkcommands.h"

class SimSpark;
class LogStream;
class Settings;
class GuiSimControl;

namespace SparkCommand
{
    class Command;
    class CommandQueue;
    class ExecuteScriptFile;
    class ChangeLogLevel;
}

/*!
 \brief QObject derived class used to initialize, run and control SimSpark.

 The SparkController emits signals informing about the state of SimSpark and is the main interface for Plugins to interact with SimSpark in a managed way.

 After starting SimSpark, the controller is a management system to access SimSpark externally.
 The following elements are all either a part of, or are registered to the SparkController:\n
 - Gui SimulationControlNode to be notified about SimSpark Simulation events (see ISimControl and GuiSimControl)\n
 - Command queues to interact with SimSpark (see AbstractCommandQueue and SparkCommand::CommandQueue\n
 - Render context initialization (see sparkcontroller.cpp)\n
 - Script value initialization (see sparkcontroller.cpp)\n
 - Log initialization (see sparkcontroller.cpp)\n
*/
class SparkController : public QObject
{
    friend class SparkCommand::ExecuteScriptFile;
    friend class SparkCommand::ChangeLogLevel;

    Q_OBJECT

public:
    // public types

    /*!
     \brief Location of resources for SimSpark.

     Used to add additional resource locations for spark initialization easily.
    */
    enum EResourceLocation
    {
        RL_NONE,                /*!< No additional resource paths. */
        RL_DEFAULT,             /*!< Add spark and main script path to the resource paths. */
        RL_INCLUDE,             /*!< Add SimulationManager script include directories to resource paths. */
        RL_DEFAULT_AND_INCLUDE  /*!< As RL_DEFAULT and RL_INCLUDE. */
    };

public:
    // public functions

    /*!
     \brief Constructor.

     Does not create SimSpark object yet.
    */
    SparkController();
    /*!
     \brief Destructor. Stopps and removes SimSpark.
    */
    virtual ~SparkController();

    /*!
     \brief Returns pointer to Spark. Empty before first initialization.
    */
    boost::shared_ptr<SimSpark> getSpark() const;

    /*!
     \brief Returns pointer to the guis SimulationControlNode. Empty before first initialization.
    */
    boost::weak_ptr<GuiSimControl> getSimulationControl();

    /*!
     \brief Initialize spark using a generic resource location definition.

     \param scriptPath simulation initialization script
     \param secondaryScriptPath spark core initialization script; serves as include path for all spark scripts
     \param additionalScripts list of additional scripts to execute after initialization
     \param parameters list of commandline parameters to hand to simspark
     \param location additional resource location definition
     \param restart if true, SimSpark is restarted if neccessary
     \param logSourceName if not empty, spark logging is initialized after creating/reinitializing SimSpark
    */
    int initSpark(const QString& scriptPath, const QString& secondaryScriptPath, const QStringList& additionalScripts, const QStringList& parameters,
        EResourceLocation location, bool restart = false, const QString& logSourceName = QString("Spark"));
    /*!
     \brief Initialize spark with a single additional resource location definition.

     \param scriptPath simulation initialization script
     \param secondaryScriptPath spark core initialization script; serves as include path for all spark scripts
     \param additionalScripts list of additional scripts to execute after initialization
     \param parameters list of commandline parameters to hand to simspark
     \param resourceLocation additional resource location
     \param restart if true, SimSpark is restarted if neccessary
     \param logSourceName if not empty, spark logging is initialized after creating/reinitializing SimSpark
    */
    int initSpark(const QString& scriptPath, const QString& secondaryScriptPath, const QStringList& additionalScripts, const QStringList& parameters,
        const std::string& resourceLocation, bool restart = false, const QString& logSourceName = QString("Spark"));
    /*!
     \brief Initialize spark with a list of additonal resource location definitions.

     \param scriptPath simulation initialization script
     \param secondaryScriptPath spark core initialization script; serves as include path for all spark scripts
     \param additionalScripts list of additional scripts to execute after initialization
     \param parameters list of commandline parameters to hand to simspark
     \param resourceLocations list of additional resource locations
     \param restart if true, SimSpark is restarted if neccessary
     \param logSourceName if not empty, spark logging is initialized after creating/reinitializing SimSpark
    */
    int initSpark(const QString& scriptPath, const QString& secondaryScriptPath, const QStringList& additionalScripts, const QStringList& parameters,
        const std::vector<std::string>& resourceLocations, bool restart = false, const QString& logSourceName = QString("Spark"));

    /*!
     \brief Initializes spark logging.

     Registers LogStreams in the spark LogServer. The LogStreams send log messages with the respective log level and the given sourceId specifying
     SimSpark as source as provided by the caller.

     \param sourceId log source id
    */
    void initLogging(int sourceId);
    /*!
     \brief Changes the log level and/or source id for simspark by creating and/or removing LogStreams.

     Registers LogStreams in the spark LogServer. The LogStreams send log messages with the respective log level and the given sourceId specifying
     SimSpark as source as provided by the caller.

     \param logLevel minimum level to log
     \param sourceId log source id
    */
    void setLogProperties(Logger::LogLevel logLevel = Logger::Debug, int sourceId = -1);

    /*!
     \brief Tries to run spark.
    */
    int runSpark();
    /*!
     \brief Tries to stop spark.
    */
    void stopSpark();
    /*!
     \brief Tries to pause or unpause spark.

     \param pause pause state
    */
    void pauseSpark(bool pause = true);
    /*!
     \brief Tries to reset spark.
    */
    void resetSpark();

    /*!
     \brief Queues a command to be executed by the server thread at the end of the next cycle.

     \param command command to be executed (pointer wll be owned by queue)
     \param timeout timeout to wait for command queue access
     \param callback optional callback target to call with the execution result
     \param member optional member function to call with signature (int, bool) (id, result)
     \param log_error if true, an error is printed immediately if queueing or the command execution fails
     \return true if queueing was successful, false if it timed out
    */
    bool queueCommand(SparkCommand::Command* command, int timeout = 100,
        const QObject* callback = 0, const char* member = "", bool log_error = false);
    /*!
     \brief Queues a command with a specifiy id to be executed by the server thread at the end of the next cycle.

     \param commandId id that will be returned to the optional callback
     \param command command to be executed (pointer wll be owned by queue)
     \param timeout timeout to wait for command queue access
     \param callback optional callback target to call with the execution result
     \param member optional member function to call with signature (int, bool) (id, result)
     \param log_error if true, an error is printed immediately if queueing or the command execution fails
     \return true if queueing was successful, false if it timed out
    */
    bool queueCommand(int commandId, SparkCommand::Command* command, int timeout = 100,
        const QObject* callback = 0, const char* member = "", bool log_error = false);
    /*!
     \brief Queues a command to be executed by the server thread at the end of the next cycle.

     \param command command to be executed
     \param timeout timeout to wait for command queue access
     \param callback optional callback target to call with the execution result
     \param member optional member function to call with signature (int, bool) (id, result)
     \param log_error if true, an error is printed immediately if queueing or the command execution fails
     \return true if queueing was successful, false if it timed out
    */
    bool queueCommand(boost::shared_ptr<SparkCommand::Command> command, int timeout = 100,
        const QObject* callback = 0, const char* member = "", bool log_error = false);
    /*!
     \brief Queues a command with a specifiy id to be executed by the server thread at the end of the next cycle.

     \param commandId id that will be returned to the optional callback
     \param command command to be executed
     \param timeout timeout to wait for command queue access
     \param callback optional callback target to call with the execution result
     \param member optional member function to call with signature (int, bool) (id, result)
     \param log_error if true, an error is printed immediately if queueing or the command execution fails
     \return true if queueing was successful, false if it timed out
    */
    bool queueCommand(int commandId, boost::shared_ptr<SparkCommand::Command> command, int timeout = 100,
        const QObject* callback = 0, const char* member = "", bool log_error = false);

    /*!
     \brief Tries to acquire shared (read) or exclusive (write) runtime access to the SimSpark simulation. Waits a specifyable amount of time.

     There may be any amount of non-exclusive acess at a time, but only one exclusive access.
     Non-exclusive (shared) access should be used when reading from SimSpark data.
     Exclusive access should be used when writing SimSpark data.\n

     Utilizing read access usually is done by a tryAcquireRuntimeAccess() call (lock) followed by releaseRuntimeAccess() call (unlock).
     Utilizing write access usually is done by a tryAcquireRuntimeAccess() call (lock) followed by a releaseRuntimeAccessDelayed() call (unlock).
     The delayed release makes sure that the simulation completes a simulation cycle before other gui elements/plugins are allowed
     to read or write in SimSpark again (for example, they have to wait until Simspark fully loaded a scene file or removed a node).\n

     Requesting exclusive access will immediately block non-exclusive access and wait for all non-exclusive accesses to be released before
     allowing the exclusive access.\n

     If acquiring was a success, true is returned. In this case the access has to be released again!
     \param timeout timeout for getting runtime access
     \param exclusive if true, exclusive access is requested
     \return true if acquiring was successful, false otherwise
    */
    bool tryAcquireRuntimeAccess(int timeout = 1000, bool exclusive = false);
    /*!
     \brief Tries to acquire shared (read) or exclusive (write) runtime access to the SimSpark simulation. Waits infinitely.

     There may be any amount of non-exclusive acess at a time, but only one exclusive access.
     Non-exclusive (shared) access should be used when reading from SimSpark data.
     Exclusive access should be used when writing SimSpark data.\n

     Utilizing read access usually is done by a tryAcquireRuntimeAccess() call (lock) followed by releaseRuntimeAccess() call (unlock).
     Utilizing write access usually is done by a tryAcquireRuntimeAccess() call (lock) followed by a releaseRuntimeAccessDelayed() call (unlock).
     The delayed release makes sure that the simulation completes a simulation cycle before other gui elements/plugins are allowed
     to read or write in SimSpark again (for example, they have to wait until Simspark fully loaded a scene file or removed a node).\n

     Requesting exclusive access will immediately block non-exclusive access and wait for all non-exclusive accesses to be released before
     allowing the exclusive access.
     \param exclusive if true, exclusive access is requested
    */
    void acquireRuntimeAccess(bool exclusive = false);
    /*!
     \brief Releases shared or exclusive runtime access.

     \param exclusive if true, exclusive access is released
    */
    void releaseRuntimeAccess(bool exclusive = false);
    /*!
     \brief Releases shared or exclusive runtime access at the end of the next simulation cycle.

     Uses SparkCommand::ReleaseRuntimeAccessDelayed to release the runtime access.
     Calls checkReleaseRuntimeAccessDelayedExclusive() or checkReleaseRuntimeAccessDelayed() to check if releasing was successful.
     Otherwise, releases externally so the program wont deadlock.
     \param exclusive if true, exclusive access is released
     \sa SparkController::acquireRuntimeAccess()
    */
    void releaseRuntimeAccessDelayed(bool exclusive = false);

protected:
    //protected functions

    /*!
     \brief Executes a script from file in the current script server.

     \return true if executiong was successful
    */
    bool runScript(const QString& path);

    /*!
     \brief Add a log stream to SimSpark.

     \param level log level of the stream to add
     \param sourceId log source id of the stream to add (as received by the Logger)
    */
    void addLogStream(Logger::LogLevel level, int sourceId);

private:
    // private functions

    /*!
     \brief Checks if Spark was initializes and logs an error otherwise.

     \return true if spark was initialized
    */
    bool assertSpark() const;

    /*!
     \brief Initializes a SparkCommand::CommandQueue to have commands executed by the simulation server.
    */
    void initCommandQueue();

    /*!
     \brief Changes the log level and/or source id for simspark by creating and/or removing LogStreams.

     Registers LogStreams in the spark LogServer. The LogStreams send log messages with the respective log level and the given sourceId specifying
     SimSpark as source as provided by the caller.

     Does not acquire the log-semaphore.
     Therefore has to be called only by the SparkCommandQueue, or after locking the mLogAccessSemaphore.

     \param logLevel minimum level to log
     \param sourceId log source id
    */
    void setLogPropertiesProtected(Logger::LogLevel logLevel = Logger::Debug, int sourceId = -1, bool useLogSemaphore = false);

public: signals:
    //public signals

    /*!
     \brief Emittet when spark started running.

     At this point the SparkSimulationThread is already running.
     If there is an error, the thread might start and finish without spark ever running.

     \param controller pointer to this controller
    */
    void sparkRunning(SparkController* controller);
    /*!
     \brief Emittet when stop was called and spark finished.

     At this point the SparkSimulationThread is still running.

     \param controller pointer to this controller
    */
    void sparkFinished(SparkController* controller);
    /*!
     \brief Emittet when spark was paused.

     Spark is actually still running, but waiting to be unpaused.

     \param controller pointer to this controller
    */
    void sparkPaused(SparkController* controller);

    /*!
     \brief Emitted after the log level was changed.

     \param controller the controller that changed the level
     \param level the new log level
    */
    void logLevelChanged(SparkController* controller, Logger::LogLevel level);

protected slots:
    //protected slots

    /*!
     \brief Checks if releasing exclusive access of SimSpark runtime access by a SparkCommand::ReleaseRuntimeAccessDelayed command was successful.

     If releasing was not successful, SimSpark is not running and the access is released anyway.
     \param id id of the command
     \param success true if releasing was successful, false only if SimSpark is not running
    */
    void checkReleaseRuntimeAccessDelayedExclusive(int id, bool success);
    /*!
     \brief Checks if releasing shared access of SimSpark runtime access by a SparkCommand::ReleaseRuntimeAccessDelayed command was successful.

     If releasing was not successful, SimSpark is not running and the access is released anyway.
     \param id id of the command
     \param success true if releasing was successful, false only if SimSpark is not running
    */
    void checkReleaseRuntimeAccessDelayed(int id, bool success);

private:
    // private members

    boost::shared_ptr<SimSpark> mSpark; /*!< SimSpark */
    boost::weak_ptr<GuiSimControl> mSimControl; /*!< The guis SimControlNode */
    std::vector<boost::shared_ptr<LogStream> > mLogStreams; /*!< LogStreams registerd to the spark log server. */

    QStringList mAdditionalScripts; /*!< List of script files to execute before running. */
    QStringList mParameters;        /*!< List of parameters to initialize SimSpark with. */

    Logger::LogLevel mLogLevel; /*!< Current log level of SimSpark. (Controls registered log streams) */
    int mLogSourceId;           /*!< Log source id for this controller. (As given by the Logger) */

    boost::shared_ptr<Settings> mSettings; /*!< Pointer to global settings object. */

    QSemaphore mLogAccessSemaphore; /*!< Semaphore to access log streams directly. Accessing is fine as long as SimSpark is paused. */
    QSemaphore mRuntimeAccessSemaphore; /*!< Semaphore to get shared access to SimSpark data (read).  */
    QSemaphore mExclusiveRuntimeAccessSemaphore; /*!< Semaphore to get exclusive access to SimSpark data (write). */
    boost::shared_ptr<SparkCommand::CommandQueue> mCommandQueue; /*!< Command queue for spark commands. */
};

#endif //SPARKCONTROLLER_H
