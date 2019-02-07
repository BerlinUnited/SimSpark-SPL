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

#ifndef GUIGRIDLAYOUT_H
#define GUIGRIDLAYOUT_H
/**
 * \file guigridlayout/guigridlayout.h
 * \brief Contains GuiGridLayout class. One of the possible main layouts.
 */

#include <QMainWindow>

#include "ui_guigridlayout.h"

/*!
 \brief A main layout for the gui containing 4 resizable Frames with 4 TabWidget-FrameAttachmentPoints.

 The UI layout is defined in Ui_GuiGridLayoutClass and was created with QtDesigner.
 The FrameAttachmentPoints are TabWidgets and called left_attachment, center_attachment, right_attachment and bottom_attachment.
*/
class GuiGridLayout : public QMainWindow
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief Basic constructor.

     \param parent parent Widget
     \param flags QtWindowFlags
    */
    GuiGridLayout(QWidget *parent = 0, Qt::WFlags flags = 0);

    /*!
     \brief Basic destructor.
    */
    virtual ~GuiGridLayout();

private:
    // private members

    Ui::GuiGridLayoutClass ui;   /*!< Ui definition class from QtDesigner. */

    QTabWidget *leftTabWidget;   /*!< left_attachment */
    QTabWidget *centerTabWidget; /*!< center_attachment */
    QTabWidget *bottomTabWidget; /*!< bottom_attachment */
    QTabWidget *rightTabWidget;  /*!< right_attachment */
};

#endif // GUIGRIDLAYOUT_H
