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

#ifndef PLUGINDIALOG_H
#define PLUGINDIALOG_H
/**
 * \file plugindialog.h
 * \brief Contains PluginDialog class, which represents the PluginManager and PluginFactory.
 */

#include "pluginmanager.h"

#include <QDialog>
#include <QModelIndex>
#include <QString>
#include <QTableView>

namespace Ui
{
    class PluginDialog;
}

class PluginInstantiatorTableModel;
class AbstractPluginInstantiator;

/*!
 \brief Dialog to control the PluginManager and PluginFactory.

 Uses Model/View to access the instantiator list and plugin list.
 Using this dialog, plugins of any type can be created, deleted, renamed, or in
 case of AttachableFrames attached and reattached.
*/
class PluginDialog : public QDialog
{
    Q_OBJECT

public:
    //public functions

    /*!
     \brief Basic creator.

     The Dialog is created anytime it is used, and deleted afterwards.
     \param parent parent widget
    */
    PluginDialog(QWidget *parent = 0);
    /*!
     \brief Basic destructor.
    */
    virtual ~PluginDialog();

protected:
    //protected functions

    /*!
     \brief Create a plugin or extension from the currently selected plugin in the plugin class list.

     Normal plugins will be added to the SimulationSetup of the current Simulation. It will be loaded with this SimulationSetup in the future.
     Extensions are added to the PluginManagers extension list. They will be loaded with every SimulationSetup.

     In both cases, the plugin is instantiated in the current Simulation.
    */
    void createPlugin(bool extension);

public: signals:
    /*!
     \brief Emitted when a plugin was added by this dialog.

     \param pluginId id of the new plugin
     \param className class name of the new plugin
    */
    void pluginAdded(int pluginId, QString className);
    /*!
     \brief Emitted when a plugin was deleted by this dialog.

     \param pluginId id of the new plugin
     \param className class name of the new plugin
    */
    void pluginDeleted(int pluginId, QString className);

    /*!
     \brief Emitted, when a plugin caption is changed by this dialog.

     \param pluginId id of the plugin
    */
    void pluginCaptionChanged(int pluginId);

private:
    //private functions

    /*!
        \brief Formats the include path QStringList from the settings object and converts it to a single enter-separated string.

        \param input string list from settings
    */
    QString formatText(const QStringList& input) const;
    /*!
        \brief Creates a string list from a single enter separated string.

        \param input string from ui QPlainTextEdit
    */
    QStringList deformatText(const QString& input) const;

public slots:
    /*!
     \brief Saves a caption value.

     The caption is stored temporarily.
     It is written to the plugin after changePluginCaption(void) was called.

     \param value value to store
    */
    void changePluginCaption(QString value);
    /*!
     \brief Saves the stored caption value to the active plugin.
    */
    void changePluginCaption();
    /*!
     \brief Updates the plugin parameters for an initialized plugin from the ui plain text edit.
    */
    void changePluginParameters();
    /*!
     \brief Updates the plugin parameters for the selected plugin class from the ui plain text edit.
    */
    void changeNewPluginParameters();
    /*!
     \brief Updates the caption view from the model.
    */
    void updatePluginCaption();
    /*!
     \brief Updates the combo box containing attachment points.
    */
    void updateAttachmentPointList();
    /*!
     \brief Saves a caption value for a new plugin instances.

     \param value value to store
    */
    void changeNewPluginCaption(QString value);
    /*!
     \brief Saves an attachment point name for a new plugin instances.

     \param value value to store
    */
    void changeAttachmentPoint(QString value);
    /*!
     \brief Cancels the selection of a plugin object.
    */
    void unchoosePlugin();
    /*!
     \brief Cancels the selection of a plugin class.
    */
    void unchooseNewPlugin();
    /*!
     \brief Selects a plugin of the plugin table view.

     \param value index
    */
    void choosePlugin(QModelIndex value);
    /*!
     \brief Selects a plugin of the plugin instantiator table view.

     \param value index
    */
    void chooseNewPlugin(QModelIndex value);

    /*!
     \brief Creates a plugin using the chosen plugin class.
    */
    void createPlugin();
    /*!
     \brief Creates an extension plugin using the chosen plugin class.

     Extensions are not added to the current SimulationSetup, but only to the list of extension plugins of the PluginManager.
     Extension plugins are loaded with every Simulation and before the other plugins are loaded.
    */
    void createExtension();
    /*!
     \brief Deletes the selected plugin object.
    */
    void deletePlugin();

    /*!
     \brief Opens a file dialog to add a new file to the plugin library list.
    */
    void addPluginFile();
    /*!
     \brief Opens a file dialog to add a new directory to the plugin library directory list.
    */
    void addPluginDirectory();

private:
    //private members

    Ui::PluginDialog *ui; /*!< UI definition. */

    boost::shared_ptr<PluginTableModel> mModel; /*!< Model of the plugin managers plugin list. */
    boost::shared_ptr<PluginInstantiatorTableModel> mInstantiatorModel; /*!< Model of the plugin factories instantiator list. */
    AbstractPlugin* mChosenPlugin; /*!< Plugin object chosen at the moment. */
    AbstractPluginInstantiator* mChosenInstantiator; /*!< Plugin class chosen at the moment. */

    QString mPluginCaption;    /*!< Caption storage for plugin object. */
    QString mNewPluginCaption; /*!< Caption storage for plugin class. */
    QString mAttachmentPoint;  /*!< Attachment point storage for plugin class. */

    QAction* mWhatsThisAction; /*!< Holds shortcut for Whatsthis mode. */
};

/*!
 \brief TableView for the PluginManagers  plugin list.

 Used in the UI class.
*/
class PluginTableView : public QTableView
{
    Q_OBJECT

public:
    //public functions

    /*!
     \brief Basic constructor.

     \param parent parent widget
    */
    PluginTableView(QWidget* parent = 0);

    /*!
     \brief Reacts to key inputs.

     \param event event
    */
    virtual void keyPressEvent(QKeyEvent *event);

    /*!
     \brief Returns a list of all selected model indices.
    */
    virtual QModelIndexList selectedIndexes();

public:
    //public members

     PluginDialog* mDialog; /*!< Parent dialog. */
};

#endif // PLUGINDIALOG_H
