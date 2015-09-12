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

#ifndef NAMEDACTIONRECEIVER_H
#define NAMEDACTIONRECEIVER_H
/**
 * \file namedactionreceiver.h
 * \brief Contains NamedActionReceiver class.
 */

#include <QObject>

/*!
 \brief Simple public class to catch and name signals and emit them again with a name.

 Used for example in the MenuManager to create a list of MenuActions which send trigger signals to a NamedActionReceiver.
 The receiver emits the action trigger again and annotates it with its own name, which serves as an identification of the action triggered.
*/
class NamedActionReceiver : public QObject
{
    Q_OBJECT

public:
    /*!
     \brief Basic constructor.

     \param name name for the receiver
     \param parent parent object
    */
    NamedActionReceiver(const QString& name, QObject* parent = 0);
    /*!
     \brief Destructor.
    */
    virtual ~NamedActionReceiver();

public: signals:
    /*!
     \brief Emitted when an action is received.

     \param myname the contained name
    */
    void actionReceived(const QString& myname);

public slots:
    /*!
     \brief Emit an actionReceived(int) signal.
    */
    void receiveAction();

public:
    QString mName; /*!< Name that is sent every time an action is received. */
};

#endif //NAMEDACTIONRECEIVER_H
