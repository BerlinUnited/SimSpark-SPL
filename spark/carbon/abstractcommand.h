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

#ifndef ABSTRACtCOMMAND_H
#define ABSTRACtCOMMAND_H
/**
 * \file abstractcommand.h
 * \brief Contains AbstractCommand class.
 */

#include <QObject>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

#include "cutelogger/logger.h"

class AbstractCommandQueue;

/*!
 \brief Parent class for Commands queueable in an AbstractCommandQueue.

 Commands derived from AbstractCommand can be queued in an CommandQueue so the command can be executed by a another
 thread than the one creating the command.

 A command contains a type and id number so the derived queue can identify the commands that have been queued.
 Commands are filled with data externally (usually by the AbstractCommandQueue) with parent queue information and id.
 Commands can also be prolonged by calling prolong(). In this case the command will not be removed from the queue and it will be
 executed again.

 \sa AbstractCommandQueue
*/
class AbstractCommand : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(AbstractCommand)

    friend AbstractCommandQueue;

public:
    // public functions

    /*!
     \brief Default destructor.
    */
    virtual ~AbstractCommand();

    /*!
     \brief Abstract execution function.
    */
    virtual bool execute() = 0;
    /*!
     \brief Abstract string conversion.

     Should contain or simply be the name class name. Used to log messages.
    */
    virtual QString toString() = 0;

    /*!
     \brief Returns the id of the command.
    */
    int getId();
    /*!
     \brief Set the id of the command.
    */
    void setId(int id);

    /*!
     \brief Returns true if command execution was skipped and it has to be executed again.
    */
    bool isProlonged() const;

protected:
    //protected functions

    /*!
     \brief Protected constructor.

     \param type optional free type id for the command to use by the derived command queue
    */
    AbstractCommand(int type = 0);

    /*!
     \brief Emit executed signal.

     Called by AbstractCommandQueue.
     \param success true if execution was successful
    */
    void emitExecuted(bool success);
    /*!
     \brief Specifies parent queue for the command externally.

     \param queue pointer to queue
    */
    void setQueue(AbstractCommandQueue* queue);

    /*!
     \brief Prolongs or removes prolong flag from the command.

     If prolonged, the command will not be rmoved from the queue and it will be executed again during the next cycle.
     \param state true to prolong
    */
    void prolong(bool state = true);

public: signals:

    /*!
     \brief Signal that is emitted after the command was executed.

     \param id id of the command
     \param success true for successful execution, false if an error occurred
    */
    void executed(int id, bool success);

protected:
    // protected members

    AbstractCommandQueue* mParentQueue; /*!< Parent command queue the command is queued in. */
    bool mProlonged; /*!< True if the command was skipped and should be executed again. */

    int mType; /*!< User defineable type id for the command. */
    int mId; /*!< User defineable command id for the command. Is emitted with the executed() signal. */
};

#endif //ABSTRACtCOMMAND_H
