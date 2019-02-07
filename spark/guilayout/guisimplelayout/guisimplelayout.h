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

#ifndef GUISIMPLELAYOUT_H
#define GUISIMPLELAYOUT_H
/**
 * \file guisimplelayout/guisimplelayout.h
 * \brief Contains GuiSimpleLayout class. One of the possible main layouts.
 */

#include <QMainWindow>

#include "ui_guisimplelayout.h"

/*!
 \brief A main layout for the gui containing 2 resizable Frames with 2 TabWidget-FrameAttachmentPoints.

 The UI layout is defined in Ui_GuiSimpleLayoutClass and was created with QtDesigner.
 The FrameAttachmentPoints are TabWidgets and called left_attachment and right_attachment.
*/
class GuiSimpleLayout : public QMainWindow
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief Basic constructor.

     \param parent parent Widget
     \param flags QtWindowFlags
    */
    GuiSimpleLayout(QWidget *parent = 0, Qt::WFlags flags = 0);

    /*!
     \brief Basic destructor.
    */
    virtual ~GuiSimpleLayout();

private:
    // private members

    Ui::GuiSimpleLayoutClass ui;   /*!< Ui definition class from QtDesigner. */

    QTabWidget *mLeftTabWidget;   /*!< left_attachment */
    QTabWidget *mRightTabWidget;  /*!< right_attachment */
};

#endif // GUISIMPLELAYOUT_H
