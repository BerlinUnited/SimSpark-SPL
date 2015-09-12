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

#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H
/**
 * \file setupdialog.h
 * \brief Contains DefineSetupDialog class.
 */

#include "ui_setupdialog.h"

#include <QDialog>

/*!
 \brief A Dialog to modify a SimulationSetups name, and/or filePath.
*/
class DefineSetupDialog : public QDialog
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief Constructor.

     \param parent parent widget
    */
    DefineSetupDialog(QWidget* parent = 0);
    /*!
     \brief Destructor.
    */
    virtual ~DefineSetupDialog();

    /*!
     \brief Returns the name.
    */
    const QString& getName() const;
    /*!
     \brief Returns the absolute file path.
    */
    const QString& getFilePath() const;

public slots:
    //public slots

    /*!
     \brief Set the name.

     Will update the line edit content.

     \param name new name
    */
    void setName(QString name);
    /*!
     \brief Set the file path.

     Will update the line edit content.

     \param filepath new file path
    */
    void setFile(QString filepath);
    /*!
     \brief Set the text of th einformation label.

     Empty message will make the label invisible.

     \param message text to display
     */
    void setMessage(QString message = QString(""));


protected slots:
    //protected slots

    /*!
     \brief Opens a file dialog to specify a file.
    */
    void loadFile();

private:
    // private members

    Ui::DefineSetupDialog ui; /*!< Gui-layout object. */

    QString mName; /*!< Current name. */
    QString mFilePath; /*!< Current file path. */
};

#endif // SETUPDIALOG_H
