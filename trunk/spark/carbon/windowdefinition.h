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

#ifndef WINDOWDEFINITION_H
#define WINDOWDEFINITION_H
/**
 * \file windowdefinition.h
 * \brief Contains WindowDefinition class, which defines an instantiated AttachableFrame and its connection to a FrameAttachmentPoint.
 */

#include "frameattachmentpoint.h"

#include <QString>
#include <QObject>

class QFrame;

/*!
 \brief Class defining an instantiated AttachableFrame and its connection to a FrameAttachmentPoint.

 Stored and managed by the WindowManager.
 Every Window is assigned a name that equals the AttachableFrames caption (but may be replaced in the WindowDefinition.xml), and a unique Id.
*/
class WindowDefinition : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(WindowDefinition)

public:
    // public functions

    /*!
     \brief Basic constructor.

     \param name name of the window
     \param frame frame to display
     \param attachmentPoint attachment point the frame is attached to
    */
    WindowDefinition(QString name, QFrame* frame, FrameAttachmentPoint* attachmentPoint);

    /*!
     \brief Returns the name of the Window.
    */
    QString getName();
    /*!
     \brief Returns pointer to the frame.
    */
    QFrame* getFrame();
    /*!
     \brief Returns pointer to the FrameAttachmentPoint.
    */
    FrameAttachmentPoint* getFrameAttachmentPoint();

public slots:
    //public slots

    /*!
     \brief Can display a context menu for the attached frame. (Currently not in use)

     \param pos local position
    */
    void showContextMenu(QPoint pos);

private:
    // private members

    int mId;        /*!< Unique Id of the Window. */
    QString mName;  /*!< Name of the Window. */
    QFrame* mFrame; /*!< Frame to display. */
    FrameAttachmentPoint* mFrameAttachmentPoint; /*!< Attachment point the window is attached to. */
};

#endif // WINDOWDEFINITION_H
