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

#ifndef NUMBEREDACTIONRECEIVER_H
#define NUMBEREDACTIONRECEIVER_H
/**
 * \file numberedactionreceiver.h
 * \brief Contains NumberedActionReceiver class.
 */

#include <QObject>

/*!
 \brief Simple public class to catch and name signals and emit them again with one or more numbers.

 Used for example in the MenuManager to create a list of MenuActions which send trigger signals to a NumberedActionReceiver.
 The receiver emits the action trigger again and annotates it with its own numbers, which serve as an identification of the action triggered.
*/
class NumberedActionReceiver : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(NumberedActionReceiver)

public:
    /*!
     \brief Basic constructor for one stored number.

     Will emit void actionReceived(int)
     \param number first number for the receiver
     \param number2 second number for the receiver
     \param parent parent object
    */
    NumberedActionReceiver(int number, QObject* parent = 0);

    /*!
     \brief Basic constructor for two stored number2.

     Will emit void actionReceived(int, int)
     \param number first number for the receiver
     \param number2 second number for the receiver
     \param parent parent object
    */
    NumberedActionReceiver(int number, int number2, QObject* parent = 0);
    /*!
     \brief Destructor.
    */
    virtual ~NumberedActionReceiver();

public: signals:
    /*!
     \brief Emitted when an action is received.

     \param mynumber the first contained number
    */
    void actionReceived(int mynumber);

    /*!
     \brief Emitted when an action is received.

     \param mynumber1 the first contained number
     \param mynumber2 the second contained number
    */
    void actionReceived(int mynumber1, int mynumber2);

public slots:
    /*!
     \brief Emit actionReceived signals.
    */
    void receiveAction();

public:

    bool mEmitSingle; /*!< If true, actionReceived(int) is emitted. */
    bool mEmitDouble; /*!< If true, actionReceived(int, int) is emitted. */
    int mNumber1; /*!< Number 1 that is sent every time an action is received. */
    int mNumber2; /*!< Number 2 that is sent every time an action is received. */
};

#endif //NUMBEREDACTIONRECEIVER_H
