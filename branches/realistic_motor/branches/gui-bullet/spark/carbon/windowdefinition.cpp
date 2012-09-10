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

#include "windowdefinition.h"
#include "carbon.h"

#include <QFrame>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

WindowDefinition::WindowDefinition(QString name, QFrame* frame, FrameAttachmentPoint* attachmentPoint)
{
    mName = name;
    mFrame = frame;
    mFrameAttachmentPoint = attachmentPoint;

    frame->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(frame, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));
}

QString WindowDefinition::getName()
{
    return mName;
}

QFrame* WindowDefinition::getFrame()
{
    return mFrame;
}

FrameAttachmentPoint* WindowDefinition::getFrameAttachmentPoint()
{
    return mFrameAttachmentPoint;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void WindowDefinition::showContextMenu(QPoint pos)
{

}
