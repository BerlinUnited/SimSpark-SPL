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

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
/**
 * \file aboutdialog.h
 * \brief Contains AboutDialog class.
 */

#include "ui_about.h"

#include <QDialog>

/*!
 \brief About dialog window accessible in the main menu.
*/
class AboutDialog : public QDialog
{
public:
    //public functions

    /*!
     \brief Constructor.

     \param parent parent widget
    */
    AboutDialog(QWidget* parent);
    /*!
     \brief Destructor.
    */
    ~AboutDialog();

private:
    //private members

    Ui::AboutDialog ui; /*!< Ui definition. */
};

#endif //ABOUTDIALOG_H
