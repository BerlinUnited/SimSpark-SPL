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

#ifndef SPARKCOMMANDQUEUE_H
#define SPARKCOMMANDQUEUE_H
/**
 * \file sparkcommandqueue.h
 * \brief Contains SparkCommand::CommandQueue class.
 */
#include "isimcontrol.h"
#include "abstractcommandqueue.h"

#include <QString>
#include <QSemaphore>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <list>

class SparkController;

namespace zeitgeist
{
    class Leaf;
}

namespace oxygen
{
    class BaseNode;
}

namespace SparkCommand
{

class Command;

/*!
 \brief A command queue for SparkCommand::Command objects.

 The command queue is an ISimControl object and is registered to the SparkControllers GuiSimControl node.
 The command queue is executed by the simulation server every time a simulation cycle ends or a wait cycle operation is performed.
*/
class CommandQueue : public AbstractCommandQueue, public ISimControl
{
    friend class Command;

public:
    //public functions

    /*!
     \brief Constructor

     \param parent parent controller
    */
    CommandQueue(SparkController* parent = 0);
    /*!
     \brief Destructor.
    */
    ~CommandQueue();

protected:
    //protected virtual functions (ISimControl)

    /*!
     \brief Called when simulation cycle ends. Calls command execution.
    */
    virtual void endCycle();
    /*!
     \brief Called when simulation wait cycle is performed. Calls command execution.
    */
    virtual void waitCycle();

protected:
    //protected members

    SparkController* mController; /*!< Parent spark controller. Used in Commands derived from SparkCommand::Command.*/
};

} //namespace SparkCommand

#endif //SPARKCOMMANDQUEUE_H
