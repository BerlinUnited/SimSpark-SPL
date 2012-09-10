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

#include "setupdialog.h"
#include "carbon.h"
#include "simulationmanager.h"

#include <QFileDialog>
#include <QFileInfo>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

DefineSetupDialog::DefineSetupDialog(QWidget* parent) :
    QDialog(parent)
{
    //UI Setup
    ui.setupUi(this);
    ui.textLabel->setText("");
    ui.textLabel->setVisible(false);

    //Default values
    mName = "";
    mFilePath = "";
}

DefineSetupDialog::~DefineSetupDialog()
{
}

const QString& DefineSetupDialog::getName() const
{
    return mName;
}

const QString& DefineSetupDialog::getFilePath() const
{
    return mFilePath;
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void DefineSetupDialog::setName(QString name)
{
    mName = name;
    ui.nameEdit->setText(name);
}

void DefineSetupDialog::setFile(QString filepath)
{
    mFilePath = filepath;
    ui.fileEdit->setText(filepath);
}

void DefineSetupDialog::setMessage(QString message)
{
    ui.textLabel->setText(message);

    if (message.compare("") == 0)
        ui.textLabel->setVisible(false);
    else
        ui.textLabel->setVisible(true);
}

void DefineSetupDialog::loadFile()
{
    QString startPath(mFilePath);
    if (startPath.compare("") == 0)
    {
        startPath = Carbon::get()->getSimulationManager()->getDefaultSetupPath();
    }

    QFileInfo info(startPath);

    QFileDialog dialog(this, tr("Define Simulation Setup target."), startPath, Carbon::get()->getSimulationManager()->getDefaultSetupExtensionText());
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setLabelText(QFileDialog::Accept, tr("Ok"));
    dialog.setToolTip(tr("Choose a file to save the setup to."));
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setFilter(Carbon::get()->getSimulationManager()->getDefaultSetupExtensionText());
    dialog.setDefaultSuffix(Carbon::get()->getSimulationManager()->getDefaultSetupExtensionSuffix());
    dialog.selectFile(info.fileName().compare("") == 0 ? "NewSetup" : info.fileName());

    int returning = dialog.exec();
    if (returning != 0)
    {
        setFile(dialog.selectedFiles().at(0));
    }
}
