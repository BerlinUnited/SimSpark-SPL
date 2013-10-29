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

#ifndef SPARKPROPERTYTABLEWIDGET_H
#define SPARKPROPERTYTABLEWIDGET_H
/**
 \file sparkpropertytablewidget.h
 \brief Contains SparkProperty::PropertyTableWidget class.
*/

#include <QTableWidget>

namespace SparkProperty
{

/*!
 \brief QTableWidget that displays the Properties of a SparkProperty::PropertyList.

*/
class PropertyTableWidget : public QTableWidget
{

public:
    //public functions

    /*!
     \brief Constructor.

     \param parent parent widget
    */
    PropertyTableWidget(QWidget* parent);
    /*!
     \brief Destructor.
    */
    ~PropertyTableWidget();

    /*!
     \brief Returns true if currently editing a text field.
    */
    bool isEditing() const;
};

} //namespace SparkProperty

#endif //SPARKPROPERTYTABLEWIDGET_H
