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

#include "carbon.h"
#include "windowmanager.h"
#include "simulationmanager.h"
#include "simulation.h"
#include "simulationsetup.h"
#include "plugindialog.h"
#include "ui_plugindialog.h"
#include "abstractplugin.h"
#include "plugininstantiatormodel.h"
#include "plugin.h"
#include "signalplugin.h"
#include "attachableframe.h"

#include <QComboBox>
#include <QMessageBox>
#include <QKeyEvent>
#include <QWhatsThis>

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

PluginDialog::PluginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginDialog)
{
    ui->setupUi(this);

    //Set Plugin table view model
    mModel = boost::shared_ptr<PluginTableModel>(Carbon::get()->getPluginManager()->getTableModel());
    ui->pluginListTableView->setModel(&*mModel);
    ui->pluginListTableView->setColumnWidth(0, 130);  //Extension Checkbox and save location
    ui->pluginListTableView->setColumnWidth(1, 130); //Caption
    ui->pluginListTableView->setColumnWidth(2, 70);  //Class
    ui->pluginListTableView->setColumnWidth(3, 100); //Type
    ui->pluginListTableView->setColumnWidth(4, 50);  //AttachmentPoint

    connect(this, SIGNAL(pluginCaptionChanged(int)), &*mModel, SLOT(updateCaption(int)));
    connect(&*mModel, SIGNAL(pluginCaptionChanged(AbstractPlugin*)), this, SLOT(updatePluginCaption()));

    ui->pluginListTableView->mDialog = this;

    //Set NewPlugin table view model
    mInstantiatorModel = boost::shared_ptr<PluginInstantiatorTableModel>(PluginFactory::getFactory().getTableModel());
    ui->createPluginAvailablePluginTableView->setModel(&*mInstantiatorModel);
    ui->createPluginAvailablePluginTableView->setColumnWidth(0, 200);
    ui->createPluginAvailablePluginTableView->setColumnWidth(1, 100);

    //Set attachment point list
    updateAttachmentPointList();

    //Set button default states
    ui->createPluginButton->setEnabled(false);
    ui->deletePluginButton->setEnabled(false);
    
    mPluginCaption = "";
    mNewPluginCaption = "";
    mChosenPlugin = 0;
    mChosenInstantiator = 0;
}

PluginDialog::~PluginDialog()
{
    LOG_DEBUG() << "Plugindialog destructor.";
    delete ui;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void PluginDialog::createPlugin(bool extension)
{
    if (mChosenInstantiator != 0)
    {
        PluginManager* manager = Carbon::get()->getPluginManager();
        EPluginType type = mChosenInstantiator->getPluginType();

        if (!PluginFactory::getFactory().canCreatePlugin(mChosenInstantiator->getId()))
        {
            return;
        }

        //Create plugin
        QStringList attachments;
        QStringList parameters;
        if (type == PT_FRAME) 
            attachments << ui->createPluginAttachmentComboBox->currentText();
        parameters = deformatText(ui->createPluginParametersTextEdit->toPlainText());

        PluginDefinition definition(mChosenInstantiator->getName(), mNewPluginCaption, type, PluginDefinition::RD_INHERIT,
             parameters, attachments);
        bool addToSimulation = !extension;
        AbstractPlugin* plugin = manager->createPlugin(definition, addToSimulation);

        if (plugin != 0)
        {
            //Update view
            emit pluginAdded(plugin->getPluginId(), plugin->getClassName());
            int size = mModel->rowCount();
            if (size > 0)
            {
                QModelIndex index = mModel->index(size-1, 0);
                ui->pluginListTableView->scrollToBottom();
                ui->pluginListTableView->setCurrentIndex(index);
                ui->pluginListTableView->selectionModel()->select(index, QItemSelectionModel::Select);
                ui->pluginListTableView->setFocus();
                choosePlugin(index);
            }

            if (extension)
            {
                //Declare extension
                manager->declareExtensionPlugin(plugin->getPluginId(), true, true);
            }
        }
    }

    updateAttachmentPointList();
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

QString PluginDialog::formatText(const QStringList& input) const
{
    QString out("");
    for (QStringList::ConstIterator it = input.begin(); it != input.end(); it++)
    {
        out += *it + "\n";
    }

    return out;
}

QStringList PluginDialog::deformatText(const QString& input) const
{
    QStringList out;

    int start = 0;
    int end = input.indexOf('\n');
    if (end == -1)
        end = input.size();
    QString next;
    while (start < input.size())
    {
        next = input.mid(start, end-start);
        if (next.compare("") != 0)
            out += next;
        start = end+1;
        end = input.indexOf('\n', start);
        if (end == -1)
        end = input.size();
    }

    return out;
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void PluginDialog::changePluginCaption(QString value)
{
    mPluginCaption = value;
}

void PluginDialog::changePluginParameters()
{
    if (mChosenPlugin)
    {
        QStringList parameters = deformatText(ui->pluginListParametersTextEdit->toPlainText());
        mChosenPlugin->getPluginDefinition()->setParameters(parameters);
    }
}

void PluginDialog::changeNewPluginParameters()
{
    //Formatting is done when creating new plugin
}

void PluginDialog::changePluginCaption()
{
    if (mChosenPlugin != 0)
    {
        mChosenPlugin->setCaption(mPluginCaption);
        emit pluginCaptionChanged(mChosenPlugin->getPluginId());
    }
}

void PluginDialog::changeAttachmentPoint(QString value)
{
    mAttachmentPoint = value;
}

void PluginDialog::updatePluginCaption()
{
    if (mChosenPlugin != 0)
    {
        ui->pluginListCaptionEdit->setText(mChosenPlugin->getCaption());
    }
}

void PluginDialog::updateAttachmentPointList()
{
    QString currentText = ui->createPluginAttachmentComboBox->currentText();

    ui->createPluginAttachmentComboBox->clear();
    WindowManager::TFrameAttachmentMap& map = Carbon::get()->getWindowManager()->getFrameAttachmentPoints();
    for (WindowManager::TFrameAttachmentMap::iterator it = map.begin(); it != map.end(); it++)
    {
        ui->createPluginAttachmentComboBox->addItem(it->second->getName());
    }

    //try finding old text
    int index = ui->createPluginAttachmentComboBox->findText(currentText);
    if (index != -1)
        ui->createPluginAttachmentComboBox->setCurrentIndex(index);
}

void PluginDialog::changeNewPluginCaption(QString value)
{
    mNewPluginCaption = value;
}

void PluginDialog::unchoosePlugin()
{
    mChosenPlugin = 0;

    ui->pluginListTableView->clearSelection();
    ui->pluginListTableView->clearFocus();

    ui->pluginListClassEdit->setText("");
    ui->pluginListCaptionEdit->setText("");
    ui->pluginListTypeEdit->setText("");

    ui->pluginListParametersTextEdit->clear();
    ui->pluginListParametersTextEdit->setEnabled(false);

    ui->deletePluginButton->setEnabled(false);

    mPluginCaption = "";
}

void PluginDialog::unchooseNewPlugin()
{
    ui->createPluginAvailablePluginTableView->clearSelection();
    ui->createPluginAvailablePluginTableView->clearFocus();

    ui->createPluginCaptionEdit->setText("");

    ui->createPluginAttachmentComboBox->setEnabled(false);
    ui->createPluginAttachmentLabel->setEnabled(false);
    
    ui->createPluginButton->setEnabled(false);

    ui->createPluginParametersTextEdit->clear();
    ui->createPluginParametersTextEdit->setEnabled(false);
  
    mNewPluginCaption = "";
}

void PluginDialog::choosePlugin(QModelIndex value)
{
    unchooseNewPlugin();

    AbstractPlugin* plugin = mModel->getPluginAt(value.row());
    if (plugin == 0)
    {
        unchoosePlugin();
        return;
    }

    mChosenPlugin = plugin;

    ui->pluginListClassEdit->setText(plugin->getClassName());
    ui->pluginListCaptionEdit->setText(plugin->getCaption());
    ui->pluginListTypeEdit->setText(plugin->getPluginTypeString());

    ui->pluginListParametersTextEdit->setEnabled(true);
    ui->pluginListParametersTextEdit->setPlainText(formatText(plugin->getPluginDefinition()->getParameters()));

    mPluginCaption = plugin->getCaption();

    ui->deletePluginButton->setEnabled(true);
}

void PluginDialog::chooseNewPlugin(QModelIndex value)
{
    unchoosePlugin();

    AbstractPluginInstantiator* instantiator = mInstantiatorModel->getInstantiatorAt(value.row());
    if (instantiator == 0)
        unchooseNewPlugin();
    ui->createPluginAvailablePluginTableView->clearSelection();
    
    ui->createPluginCaptionEdit->setText(instantiator->getCaption());

    ui->createPluginParametersTextEdit->setEnabled(true);
    ui->createPluginParametersTextEdit->clear();

    mNewPluginCaption = instantiator->getCaption();
    mChosenInstantiator = instantiator;

    switch (instantiator->getPluginType())
    {
    case PT_FRAME:
        ui->createPluginAttachmentComboBox->setEnabled(true);
        ui->createPluginAttachmentLabel->setEnabled(true);
        break;
    default:
        ui->createPluginAttachmentComboBox->setEnabled(false);
        ui->createPluginAttachmentLabel->setEnabled(false);
        break;
    }

    if (!PluginFactory::getFactory().canCreatePlugin(mChosenInstantiator->getId(), false))
    {
        ui->createPluginButton->setEnabled(false);
    }
    else
        ui->createPluginButton->setEnabled(true);
}

void PluginDialog::createPlugin()
{
    createPlugin(false);
}

void PluginDialog::createExtension()
{
    createPlugin(true);
}

void PluginDialog::deletePlugin()
{
    QModelIndexList list = ui->pluginListTableView->selectedIndexes();

    QString titleExtensionAdd = tr("Romoving an extension (a plugin with an activated checkbox) will stop it from being loaded in all of the SimulationSetups!");

    if (list.size() > 0)
    {
        //Multiple deletion
        std::vector<int> pluginIds;
        int row;
        int id;
        PluginManager* manager = Carbon::get()->getPluginManager();
        QString namelist(tr("Plugins: "));

        //Find out all plugin ids
        bool hasExtensions = false;
        for (int i = 0; i < list.size(); i++)
        {
            row = list.at(i).row();
            id = mModel->headerData(row, Qt::Vertical).toInt();
            if (mModel->data(mModel->index(row, 0), Qt::CheckStateRole) == Qt::Checked) //is extension field checked?
                hasExtensions = true;

            bool alreadyContained = false;
            for (std::vector<int>::iterator it = pluginIds.begin(); it != pluginIds.end(); it++)
            {
                if (*it == id)
                {
                    alreadyContained = true;
                    break;
                }
            }

            if (!alreadyContained)
            {
                pluginIds.push_back(id);
                if (pluginIds.size() != 1)
                    namelist.append(", ");
                namelist.append(mModel->data(mModel->index(row, 1)).toString());
            }
        }

        QMessageBox messageBox;
        messageBox.setText(QString(tr("Do you really want to delete the %1 selected plugins? %2"))
            .arg(list.size())
            .arg(hasExtensions ? titleExtensionAdd : ""));
        messageBox.setInformativeText(namelist);
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::Yes);
        messageBox.setIcon(QMessageBox::Question);
        int ret = messageBox.exec();

        if (ret == QMessageBox::Yes)
        {
            //Delete
            for (std::vector<int>::iterator it = pluginIds.begin(); it != pluginIds.end(); it++)
            {
                bool deleted = manager->deletePlugin(*it, true);
                if (!deleted)
                {
                    LOG_ERROR() << "Failed to delete plugin.";
                }
            }
            unchoosePlugin();
        }
    }
    else if (mChosenPlugin != 0 && ui->deletePluginButton->isEnabled())
    {
        //Single deletion
        PluginManager* manager = Carbon::get()->getPluginManager();
        EPluginType type = mChosenPlugin->getPluginType();

        QMessageBox messageBox;
        messageBox.setText(QString(tr("Do you really want to delete plugin '%1'? %2"))
            .arg(mChosenPlugin->getCaption())
            .arg(mChosenPlugin->isExtension() ? titleExtensionAdd : ""));
        messageBox.setInformativeText(QString(tr("Plugin Class '%1', Plugin Id: %2")).arg(
                                      mChosenPlugin->getClassName()).arg(mChosenPlugin->getPluginId()));
        messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        messageBox.setDefaultButton(QMessageBox::Yes);
        messageBox.setIcon(QMessageBox::Question);
        int ret = messageBox.exec();

        if (ret == QMessageBox::Yes)
        {
            bool deleted = manager->deletePlugin(*mChosenPlugin, true);
            if (!deleted)
            {
                LOG_ERROR() << "Failed to delete plugin.";
            }
            unchoosePlugin();
        }
    }

    updateAttachmentPointList();
}

void PluginDialog::addPluginFile()
{
    bool added = Carbon::get()->getPluginManager()->addPluginIncludeFile();
    if (added)
    {
        ui->createPluginAvailablePluginTableView->reset();
    }
}

void PluginDialog::addPluginDirectory()
{
    bool added = Carbon::get()->getPluginManager()->addPluginIncludeDirectory();
    if (added)
    {
        ui->createPluginAvailablePluginTableView->reset();
    }
}

//--------------------------------------------------------------
// PluginTableView
//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

PluginTableView::PluginTableView(QWidget* parent) :
    QTableView(parent)
{
    mDialog = 0;
}

void PluginTableView::keyPressEvent(QKeyEvent *event)
{
    bool handled = false;
    if (mDialog != 0)
    {
        //event-handling
        if (event->key() == Qt::Key_Delete)
        {
            mDialog->deletePlugin();
            handled = true;
        }
    }

    //hand to base
    if (handled)
        event->accept();
    else
        event->ignore();
}

QModelIndexList PluginTableView::selectedIndexes()
{
    return QTableView::selectedIndexes();
}

