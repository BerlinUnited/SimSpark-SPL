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

#include "vector2x3widget.h"
#include "carbon.h"

#include <QLayout>
#include <QLine>

Vector2x3Widget::Vector2x3Widget(QWidget* parent)
{
    QLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);

    mWidget1 = new Vector3Widget(this);
    mWidget2 = new Vector3Widget(this);

    QFrame *line = new QFrame(this);
    line ->setFrameShape(QFrame::HLine);

    layout->addWidget(mWidget1);
    layout->addWidget(line);
    layout->addWidget(mWidget2);

    connect(mWidget1, SIGNAL(vectorChanged()), this, SLOT(widget1Changed()));
    connect(mWidget2, SIGNAL(vectorChanged()), this, SLOT(widget2Changed()));
}

Vector2x3Widget::~Vector2x3Widget()
{

}

Vector3Widget& Vector2x3Widget::getWidget1()
{
    return *mWidget1;
}

Vector3Widget& Vector2x3Widget::getWidget2()
{
    return *mWidget2;
}

void Vector2x3Widget::widget1Changed()
{
    emit vectorChanged(1);
}

void Vector2x3Widget::widget2Changed()
{
    emit vectorChanged(2);
}
