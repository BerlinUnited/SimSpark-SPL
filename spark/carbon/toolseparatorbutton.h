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

#ifndef TOOLSEPARATORBUTTON_H
#define TOOLSEPARATORBUTTON_H
/**
 * \file toolseparatorbutton.h
 * \brief Contains ToolSeparatorButton widget class.
 */

#include <QToolButton>

/*!
 \brief Button with an arrow that changes its direction when toggled.

  Used to separate custom toolbar groups in QtDesigner-created UI definitions.
*/
class ToolSeparatorButton : public QToolButton
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief  Basic constructor.

     \param parent parent widget
    */
    ToolSeparatorButton(QWidget* parent);
    /*!
     \brief Destructor.
    */
    virtual ~ToolSeparatorButton();

public slots:
    //public slots

    /*!
     \brief Updates direction of the arrow on the button.

     \param value if toggled (true), arrow points right (open)
    */
    void updateArrow(bool value);
};

#endif // TOOLSEPARATORBUTTON_H
