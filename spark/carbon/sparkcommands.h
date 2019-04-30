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

#ifndef SPARKCOMMANDS_H
#define SPARKCOMMANDS_H
/**
 * \file sparkcommands.h
 * \brief Contains SparkCommand::Command and derived command classes.
 */

#include "isimcontrol.h"
#include "abstractcommand.h"

#include <QObject>
#include <QString>
#include <QSemaphore>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

#include "cutelogger/logger.h"

class SparkController;

namespace zeitgeist
{
    class Leaf;
    class ParameterList;
}

namespace oxygen
{
    class BaseNode;
}

namespace SparkCommand
{

class CommandQueue;

/*!
 \brief Abstract base class for general commands queuable in a SparkCommand::CommandQueue.

 The commands are executed by the simulation server thread.
*/
class Command : public AbstractCommand
{
    Q_OBJECT
    Q_DISABLE_COPY(Command)

    friend class CommandQueue;

public:
    //public types

    /*!
     \brief Command-type id.
    */
    enum ECommandType
    {
        CT_EVAL_SCRIPT = 0,               /*!< Evaluate a script command. */
        CT_EXECUTE_SCRIPT_FILE,           /*!< Execute a script file. */
        CT_IMPORT_SCENE,                  /*!< Import a scene file. */
        CT_REMOVE_LEAF,                   /*!< Remove a leaf from the node tree. */
        CT_CHANGE_LOG_LEVEL,              /*!< Change the log SparkController log level. */
        CT_RELEASE_RUNTIME_ACCESS_DELAYED /*!< Release runtime access semaphore after a wait cycle. */
    };

public:
    // public functions

    /*!
     \brief Destructor.
    */
    virtual ~Command();

protected:
    //protected functions

    /*!
     \brief Constructor.

     \param type command type id
    */
    Command(int type);

    /*!
     \brief Return the parent controller.
    */
    SparkController* getController();
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Evaluate a script command in the servers script context.
*/
class EvalScript : public Command
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param commandString string to evaluate
    */
    EvalScript(const QString& commandString) :
      Command(CT_EVAL_SCRIPT), mCommandString(commandString) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "EvalScript";}

private:
    //private members

    QString mCommandString; /*!< String to evaluate. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Executes a script file in the servers script context.
*/
class ExecuteScriptFile : public Command
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param file absolute path to the file
    */
    ExecuteScriptFile(const QString& file) :
      Command(CT_EXECUTE_SCRIPT_FILE), mFile(file) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "ExecuteScriptFile";}

private:
    //private members

    QString mFile; /*!< Absolute path to the file. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Import a scene file in a BaseNode.
*/
class ImportScene : public Command
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param file absolute filepath of the scene file to load
     \param node node to load scene file in
     \param parameters parameter list for script file
    */
    ImportScene(const QString& file, boost::shared_ptr<oxygen::BaseNode> node, boost::shared_ptr<zeitgeist::ParameterList> parameters) :
      Command(CT_IMPORT_SCENE), mFile(file), mNode(node), mParameters(parameters) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "ImportScene";}

private:
    //private members

    QString mFile; /*!< Absolute filepath of the scene file to load. */
    boost::shared_ptr<oxygen::BaseNode> mNode; /*!< Node to load scene file in. */
    boost::shared_ptr<zeitgeist::ParameterList> mParameters; /*!< Parameter list for script file. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Unlinks and removes a Leaf from the zeitgeist tree.
*/
class RemoveLeaf : public Command
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param leaf leaf to unlink and remove
    */
    RemoveLeaf(boost::shared_ptr<zeitgeist::Leaf> leaf) :
      Command(CT_REMOVE_LEAF), mLeaf(leaf) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "RemoveLeaf";}

private:
    //private members

    boost::shared_ptr<zeitgeist::Leaf> mLeaf; /*!< Leaf to unlink and remove. */
};

/*!
 \brief Command queuable in SparkCommand::CommandQueue. Releases access to the runtime access semaphore after a number of cycles.

 Used to release a coordinated multiple or unique external read access to SimSpark.
 \sa SparkController
*/
class ReleaseRuntimeAccessDelayed : public Command
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param waitCycles number of cycles to wait
     \param exclusive if true, exclusive access is released
    */
    ReleaseRuntimeAccessDelayed(int waitCycles, bool exclusive) :
      Command(CT_RELEASE_RUNTIME_ACCESS_DELAYED), mWaitCycles(waitCycles), mExclusive(exclusive), mWaited(0) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "ReleaseRuntimeAccessDelayed";}

private:
    //private members

    int mWaitCycles; /*!< Number of cycles to wait. */
    int mWaited; /*!< Amount of cycles waited */
    bool mExclusive; /*!< True for exclusive exxess, false for shared read access. */
};


/*!
 \brief Command queuable in SparkCommand::CommandQueue. Change the SparkController log level and source id.
*/
class ChangeLogLevel : public Command
{
public:
    //public functions

    /*!
     \brief Complete constructor.

     \param level new log level
     \param logsource new log source id as returned by the Logger
    */
    ChangeLogLevel(Logger::LogLevel level, int logsource) :
      Command(CT_CHANGE_LOG_LEVEL), mLevel(level), mSource(logsource) {}

    /*!
     \brief Executes the command. Called by simulation server thread.
    */
    virtual bool execute();
    /*!
     \brief String conversion.
    */
    virtual QString toString() { return "ChangeLogLevel";}

private:
    //private functions

    Logger::LogLevel mLevel; /*!< New log level. */
    int mSource; /*!< New log source id as returned by the Logger. */
};

} //namespace SparkCommands

#endif //SPARKCOMMANDS_H
