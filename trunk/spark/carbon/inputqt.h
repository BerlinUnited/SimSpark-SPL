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

#ifndef INPUTQT_H
#define INPUTQT_H

/*!
 \file inputqt.h
 \brief Contains QTInput class.
*/

#include <kerosin/inputserver/input.h>

class QKeyEvent;
class QMouseEvent;

/*!
 \brief A single QT key input for the input server.
*/
class QTInput : public kerosin::Input
{
public:
    /*!
     \brief Complete constructor.

     \param event key event to read key from
     \param keyPress true for press, false for release
    */
    QTInput(const QKeyEvent& event, bool keyPress);
};

#endif //INPUTQT_H
