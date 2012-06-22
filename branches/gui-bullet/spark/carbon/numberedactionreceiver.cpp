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

#include "numberedactionreceiver.h"

NumberedActionReceiver::NumberedActionReceiver(int number1, QObject* parent) :
    QObject(parent),
    mNumber1(number1),
    mNumber2(0),
    mEmitSingle(true),
    mEmitDouble(false)
{
}

NumberedActionReceiver::NumberedActionReceiver(int number1, int number2, QObject* parent) :
    QObject(parent),
    mNumber1(number1),
    mNumber2(number2),
    mEmitSingle(true),
    mEmitDouble(true)
{
}

NumberedActionReceiver::~NumberedActionReceiver()
{
}

void NumberedActionReceiver::receiveAction()
{
    if (mEmitSingle)
        emit actionReceived(mNumber1);
    if (mEmitDouble)
        emit actionReceived(mNumber1, mNumber2);
}
