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

#ifndef VECTOR2X3WIDGET_H
#define VECTOR2X3WIDGET_H
/**
 * \file vector2x3widget.h
 * \brief Contains Vector2x3Widget class.
 */

#include <QWidget>
#include <QVector3D>

#include "vector3widget.h"

/*!
 \brief Widget containing two Vector3Widget.
*/
class Vector2x3Widget : public QWidget
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief  Basic constructor.

     \param parent parent widget
    */
    Vector2x3Widget(QWidget* parent);
    /*!
     \brief Destructor.
    */
    virtual ~Vector2x3Widget();

    /*!
     \brief Returns first widget.
    */
    Vector3Widget& getWidget1();
    /*!
     \brief Returns second widget.
    */
    Vector3Widget& getWidget2();

public: signals:
    //public signals

    /*!
     \brief Emitted when one of the vectors changed.

     \param id 1 or 2
    */
    void vectorChanged(int id);

protected slots:
    //public slots

    /*!
     \brief Emit vectorChanged(1)
    */
    void widget1Changed();
    /*!
     \brief Emit vectorChanged(2)
    */
    void widget2Changed();

private:
    // private members

    Vector3Widget* mWidget1; /*!< First widget. */
    Vector3Widget* mWidget2; /*!< Second widget. */
};

#endif // VECTOR2X3WIDGET_H
