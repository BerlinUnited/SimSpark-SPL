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

#ifndef ABSTRACTCOMMANDQUEUE_H
#define ABSTRACTCOMMANDQUEUE_H
/**
 * \file abstractcommandqueue.h
 * \brief Contains AbstractCommandQueue class.
 */

#include <QString>
#include <QSemaphore>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <list>

class AbstractCommand;

/*!
 \brief A Queue that can contain and execute AbstractCommand objects.

 Commands derived from AbstractCommand can be queued in an CommandQueue so the command can be executed by a another
 thread than the one creating the command.

 CommandQueues derived from AbstractCommandQueue (e.g. MyCommandQueue) should correspond to a set of Commands derived from a
 AbstractCommand derived class (e.g. MyCommands). So the type id field in AbstractCommand can be used as class identification.

 Command objects are queued with queueCommand(). For every queue and execution access the command queue has to be locked with a
 Semaphore using lockQueue() and unlockQueue(). Locking/Unlocking is done automatically, but keep in mind, that you might not be able to
 access the queue immediately.
*/
class AbstractCommandQueue
{
    friend class AbstractCommand;

public:
    //public functions

    /*!
     \brief Basic constructor.
    */
    AbstractCommandQueue();
    /*!
     \brief Destructor.
    */
    ~AbstractCommandQueue();

    /*!
     \brief Queue a new command at the end of the current command list.

     The next time executeCommands() is called, the commands execute() function will be called.
     \param command command to queue
     \param timeout time in ms to wait to the command queue semaphore to be acquired. 0 = infinite
     \param log_error if true, an error message is printed immediately additionally to the returning of false
     \return true if queueing the command was successful, false otherwise
    */
    bool queueCommand(boost::shared_ptr<AbstractCommand> command, int timeout = 100, bool log_error = true);

protected:
    //protected functions

    /*!
     \brief Execute all the commands in the list and delete them.

     If a command calls AbstractCommand::prolong(), it will not be removed from the command list.
    */
    void executeCommands();

    /*!
     \brief Tries to lock the command queue.

     \param timeout time in ms to wait for the semaphore to be acquired
     \return true if access was successful
    */
    bool lockQueue(int timeout = 5);
    /*!
     \brief Unlocks the command queue.
    */
    void unlockQueue();

protected:
    //protected members

    QSemaphore mSemaphore; /*!< Command list access semaphore. */
    std::list<boost::shared_ptr<AbstractCommand>> mCommands;  /*!< Command queue.*/
};

#endif //ABSTRACTCOMMANDQUEUE_H
