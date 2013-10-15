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

#include "typedlineedit.h"
#include "carbon.h"

#include <QMouseEvent>

TypedLineEdit::TypedLineEdit(QWidget* parent)
{

}

TypedLineEdit::~TypedLineEdit()
{

}

void TypedLineEdit::setIntType(const QIntValidator* validator, int scrollWidth)
{
    mType = T_INT;
    
    if (validator != 0)
        setValidator(validator);

   mScroll = scrollWidth*1.0;
}

void TypedLineEdit::setDoubleType(const QDoubleValidator* validator, double scrollWidth)
{
    mType = T_DOUBLE;
    
    if (validator != 0)
        setValidator(validator);

    mScroll = scrollWidth;
}

void TypedLineEdit::setStringType(const QRegExpValidator* validator)
{
    mType = T_STRING;

    if (validator != 0)
        setValidator(validator);
}

void TypedLineEdit::setText(const QString& text, bool skipfocus)
{
    if (!skipfocus || !hasFocus()) QLineEdit::setText(text);
}

void TypedLineEdit::wheelEvent(QWheelEvent* e)
{
    if (mType == T_DOUBLE)
    {
        double val = text().toDouble();
        val += mScroll * e->delta() * 0.01;
        setText(QString::number(val), false);

        emit typedChange();
    }
    if (mType == T_INT)
    {
        int val = text().toInt();
        val += (int)(mScroll * e->delta() *0.01);
        setText(QString::number(val), false);

        emit typedChange();
    }
    if (mType == T_STRING)
    {
    }
}