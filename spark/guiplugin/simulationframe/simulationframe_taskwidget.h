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

#ifndef SIMULATIONFRAME_TASKWIDGET_H
#define SIMULATIONFRAME_TASKWIDGET_H
/**
 * \file simulationframe_taskwidget.h
 * \brief Contains SimulationFrameUtil::TaskWidget class.
 */

#include <QFrame>
#include <simulationtask.h>

#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif
#include <QPalette>

#include "ui_simulationframe_taskwidget.h"

class PluginManager;
class SimulationManager;
class SimulationTask;
class SimulationFrame;
class TaskDefinition;
class NumberedActionReceiver;
class NamedActionReceiver;

class QListWidgetItem;

namespace SimulationFrameUtil
{

/*!
 \brief Representative Widget to display and modify a SimulationTask of the currently initialized Simulation.

 The SimulationFrame automatically creates, deletes and handles the display style of the TaskWidget whenever the task list
 of the current Simualtion in the SimulationManager changes.
 The TaskWidget only stores a weak pointer to a SimulationTask in case another plugin removes the task.
 Before accessing it, it has to lock the weak pointer. If the represented task was deleted externally, the TaskWidget
 displays information on the removal and can be deleted manually.
 SimulationTasks can either be started, paused or stopped together (using the SimulationFrame) or one at a time using the controls of the TaskWidget.\n

 The TaskWidget contains three areas: The header-area, the details-area and the info-area.
 The header-area contains the name, type, execution status and the execution controls (pause, stop, run, delete). It is always visible.
 Using a button in the header area or controlled by the SimualtionFrame, the Details area can be displayed or hidden. It contains data fields
 to change the TaskDefinition of the associated task. The details areas labels and usage are dependaant on the type of the SimulationTask.
 The info area is a part of the details area and displays futher information, which can be toggled to be short or long.
 It contains for example information on which plugins math the description currently specified in the TaskWidget.\n

 Any change in the details area updates the current SimulationTask and the SimulationSetup of the Simulation.
 Saving the current Simulation afterwards (either as preset setup or normal save file) will contain these changes.

 Plugin specifications for PluginThreads can be quick-loaded using a load button in the header area.
 Script specifications can be quick-loaded using the context menu.
 A context menu in the widget allows copying the task.\n

 The two lists of a TaskWidget are QListWidgets and are set up to always contain one empty Entry at the end. This way it is really easy to quickly edit
 the lines with mouse and keyboard without having to add new entries via a context menu or add/delete buttons.
*/
class TaskWidget : public QFrame
{
    Q_OBJECT

protected:
    //protected types

    /*!
     \brief Command ids to interact with list entries.
    */
    enum EListCommand
    {
        LC_MOVE_UP = 0, /*!< Move selected list entry up if possible. */
        LC_MOVE_DOWN,   /*!< Move selected list entry down if possible. */
        LC_ADD_ENTRY,   /*!< Add a new entry at selected position. */
        LC_REMOVE_ENTRY /*!< Remove selected list entry. */
    };

public:
    // public functions
    /*!
     \brief Cosntructor.

     \param parent parent frame
     \param task task to represent
    */
    TaskWidget(SimulationFrame* parent, boost::shared_ptr<SimulationTask> task);
    /*!
     \brief Destructor.
    */
    virtual ~TaskWidget();

    /*!
     \brief Catches mouse press event.

     Used to select the widget.

     \param e event
    */
    virtual void mousePressEvent(QMouseEvent * e);

    /*!
     \brief Notification that this widget was selected in the parent SimulationFrame.
    */
    void choose();
    /*!
     \brief Notification that this widget was deselected in the parent SimulationFrame.
    */
    void unchoose();

private:
    //private functions

    /*!
     \brief Changes the button arrow depending on if the details area is shown or hidden.
    */
    void updateDetailsButtonIcon();

    /*!
     \brief Returns task definition of the task as non-const reference.

     \note The definition is only const to make sure that no plugin changes it without knowing the consequences.
     The task and its definition have to be updated in the same way when changing an initializes SimulationTask.

     \param task pointer to this task as shared_ptr
    */
    TaskDefinition& getTaskDefinition(boost::shared_ptr<SimulationTask> task);

    /*!
     \brief Updates the info area content when the task is a pluginthread.

     The info field will contain of all the plugins that match the criteria given in the "class", "caption" and "type" text edit.

     \sa PluginManager::findPlugins()
    */
    void updatePluginMatchList();
    /*!
     \brief Colors the text of a QLineEdit.

     \param edit edit to edit
     \param color text color to use
    */
    void colorText(QLineEdit& edit, const QColor& color);

    /*!
     \brief Adds an entry to the first list widget.

     \param value text to add
    */
    void addFirstListEntry(QString value);
    /*!
     \brief Removes an entry from the first list widget.

     \param index index to remove at
    */
    void removeFirstListEntry(int index);
    /*!
     \brief Adds an entry to the second list widget.

     \param value text to add
    */
    void addSecondListEntry(QString value);
    /*!
     \brief Removes an entry from the second list widget.

     \param index index to remove at
    */
    void removeSecondListEntry(int index);

    /*!
     \brief Checks if the script of the given item is contained in an include directory and changes the items color respectively.

     \param item item to check
    */
    void checkScript(QListWidgetItem* item);

    /*!
     \brief Called after a list entry of one of the list widgets was changed. Updates the lists and asserts an empty line at the list end.

     Depending on which entry was changed and how it was changed the given list is updated. Changing the text of an entry to an empty string removes it,
     changing the empty string at the end will add a new empty string. The given QStringList represents the content of the itemlist and is updated first.
     Afterwards the list content is used to change the QStringList of the respective field in the TaskDefinition of this Widgets SimulationTask.

     \param item item that was changed
     \param listWidget parent of the item
     \param list list to change entries in (contains empty string at the end)
     \param chosenEntry index of the chosen entry, changes when something was deleted or added
     \param first true for first list, false for second
    */
    void changeListEntry(QListWidgetItem* item, QListWidget* listWidget, QStringList& list, int& chosenEntry, bool first);

    /*!
     \brief Returns the index of the given item in the given list widget.

     \param item item to search for
     \param listWidget widget to search in
     \return index of the item or -1 if not found
    */
    int getItemIndex(QListWidgetItem* item, QListWidget* listWidget);

    /*!
     \brief Tries to apply a list command to a list item.

     \param item item to use command on
     \param command command to use
     \return true if successful, false otherwise
    */
    bool useListCommand(QListWidgetItem* item, EListCommand command);

public slots:
    //public slots

    /*!
     \brief Chooses this widget in the parent SimulationFrame.
    */
    void chooseThisWidget();

    /*!
     \brief Updates the widget content.

     \param updateAll if false header button access is only updated when task state changed
    */
    void updateDisplay(bool updateAll = false);
    /*!
     \brief Updates the size of the lists.

     The lists are only 1-line high by default. If the widget is selected and a list contains more than one line of entries, its size is doubled to display
     more of the content.
    */
    void updateListSize();
    /*!
     \brief Updates the display of the tasks execution state and the control elements.

     Sent by the SimulationTask.

     \param state new task state
    */
    void updateState(SimulationTask::ETaskExecutionState state);

    /*!
     \brief Changes the task priority.

     \param index priority index
    */
    void changeTaskPriority(int index);
    
    /*!
     \brief Checks and stores a new task name.

     \param value new name
    */
    void checkName(QString value);
    /*!
     \brief Changes the task name to the value stored by checkName().
    */
    void changeName();
    /*!
     \brief Checks and stores a new task "first" data field value.

     \param value new value
    */
    void checkFirst(QString value);
    /*!
     \brief Changes the tasks "first" data field to the value stored by checkFirst().
    */
    void changeFirst();
    /*!
     \brief Checks and stores a new task "second" data field value.

     \param value new value
    */
    void checkSecond(QString value);
    /*!
     \brief Changes the tasks "second" data field to the value stored by checkSecond().
    */
    void changeSecond();
    /*!
     \brief Checks and stores a new task "third" data field value.

     \param value new value
    */
    void checkThird(QString value);
    /*!
     \brief Changes the tasks "third" data field to the value stored by checkThird().
    */
    void changeThird();
    
    /*!
     \brief Selects a row in the first list.

     \param row tow to select
    */
    void chooseFirstListEntry(int row);
    /*!
     \brief Changes the first list entry of the task definition using the new value of the given item.

     Updates the comple list in case anything was added or removed.

     \param item item to read from
    */
    void changeFirstListEntry(QListWidgetItem* item);
    /*!
     \brief Selects a row in the second list.

     \param row tow to select
    */
    void chooseSecondListEntry(int row);
    /*!
     \brief Changes the second list entry of the task definition using the new value of the given item.

     Updates the comple list in case anything was added or removed.

     \param item item to read from
    */
    void changeSecondListEntry(QListWidgetItem* item);

    /*!
     \brief Pauses or executes the single task.
    */
    void toggleRunning();
    /*!
     \brief Deletes the task.
    */
    void deleteTask();
    /*!
     \brief Copies the task

     \param unique_name if false the new task will have the same name, otherwise a new unique name is created by numbering, starting with "_1"
    */
    void copyTask(bool unique_name = true);
    /*!
     \brief Stops the single task.
    */
    void stopTask();
    /*!
     \brief Loads a definition for the task by a dropdown menu.

     Only used for PluginThreads, contains all of the registered plugins.
    */
    void loadTask();
    /*!
     \brief Reacts to a click in the dropdown menu created by loadTask().

     For plugins, the plugin-id of the plugin to execute in the task is given.

     \param id id of the command
    */
    void onLoadTask(int id);
    /*!
     \brief Clears all action receivers.
    */
    void clearActionReceivers();

    /*!
     \brief Shows or hides the details area.

     \param state true for shown, false for hidden
    */
    void toggleDetail(bool state);
    /*!
     \brief Toggles the length of the info area.
    */
    void toggleLongInfo();
    /*!
     \brief Toggles visibility of the info area.

     \param state true for shown, false for hidden
    */
    void showInfo(bool state);
    /*!
     \brief Toggles visiblity of the task load button.

     \param state true for shown, false for hidden
    */
    void showLoadButton(bool state);

    /*!
     \brief Opens context menu for first text edit.

     Contains the scripts in all executable include directories to be loaded quickly at the text edit.
     \param pos local position
    */
    void showFirstContextMenu(QPoint pos);
    /*!
     \brief Opens context menu for second text edit.

     Contains the scripts in all script include directories to be loaded quickly at the text edit.
     \param pos local position
    */
    void showSecondContextMenu(QPoint pos);
    /*!
     \brief Opens context menu for third text edit.

     Contains the scripts in all script include directories to be loaded quickly at the text edit.
     \param pos local position
    */
    void showThirdContextMenu(QPoint pos);

    /*!
     \brief Reacts to a context menu click in the first text edit.

     \param entry entry that was clicked
    */
    void onFirstContextMenuClick(const QString& entry);
    /*!
     \brief Reacts to a context menu click in the  first text edit.

     \param id id of the command that was clicked
    */
    void onFirstContextMenuClick(int id);
    /*!
     \brief Reacts to a context menu click in the second text edit.

     \param entry entry that was clicked
    */
    void onSecondContextMenuClick(const QString& entry);
    /*!
     \brief Reacts to a context menu click in the second text edit.

     \param id id of the command that was clicked
    */
    void onSecondContextMenuClick(int id);
    /*!
     \brief Reacts to a context menu click in the third text edit.

     \param entry entry that was clicked
    */
    void onThirdContextMenuClick(const QString& entry);
    /*!
     \brief Reacts to a context menu click in the  third text edit.

     \param id id of the command that was clicked
    */
    void onThirdContextMenuClick(int id);

    /*!
     \brief Opens context menu for first list.

     Contains the scripts in all script include directories to be loaded quickly at the currently clicked list item.
     \param pos local position
    */
    void showFirstListContextMenu(QPoint pos);
    /*!
     \brief Reacts to a context menu click in the first list.

     \param entry entry that was clicked
    */
    void onFirstListContextMenuClick(const QString& entry);
    /*!
     \brief Reacts to a context menu click in the first list.

     \param id id of the command that was clicked
    */
    void onFirstListContextMenuClick(int id);
    /*!
     \brief Opens context menu for second list.

     \param pos local position
    */
    void showSecondListContextMenu(QPoint pos);
    /*!
     \brief Reacts to a context menu click in the second list.

     \param entry entry that was clicked
    */
    void onSecondListContextMenuClick(const QString& entry);
    /*!
     \brief Reacts to a context menu click in the second list.

     \param id id of the command that was clicked
    */
    void onSecondListContextMenuClick(int id);

    /*!
     \brief Opens context menu for the widget.

     Can copy or delete the task.

     \param pos local position
    */
    void showWidgetContextMenu(QPoint pos);

private:
    // private members

    static QPalette mDefaultPalette;  /*!< Palette for the widget if not selected. */
    static QPalette mSelectedPalette; /*!< Palette for the widget if selected. (Changes the frame border color) */
    static bool mInitializedStatic;   /*!< True after static variables have been initialized by the first instance. */

    bool mInitialized;            /*!< True after initialization. Prevents too-early-updates. */
    bool mReactToChangeListEntry; /*!< Guard to prevent list updates while changing. */
    bool mLongInfo;               /*!< Specifies if the info area is expanded. */

    Ui::TaskWidget ui;                     /*!< UI object. */
    SimulationManager* mSimulationManager; /*!< Pointer to manager for quick access. */
    PluginManager* mPluginManager;         /*!< Pointer to manager for quick access. */
    boost::weak_ptr<SimulationTask> mTask; /*!< Represented task as weak pointer. */
    SimulationFrame* mParent;              /*!< Creator. */

    int mPluginMatchCount;      /*!< Amount of plugins matching the current specifations. */
    QString mPluginMatchList;   /*!< Info text of matching plugins. */
    QString mName;              /*!< Name value storage. */
    QString mFirstValue;        /*!< First value storage. */
    QString mSecondValue;       /*!< Second value storage. */
    QString mThirdValue;        /*!< Third value storage. */
    int mThirdValueInt;         /*!< Third values as int. */
    QStringList mFirstList;     /*!< First list value storage. */
    QStringList mSecondList;    /*!< Second list value storage. */
    int mFirstListChosenEntry;  /*!< Index of chosen entry in first list. */
    int mSecondListChosenEntry; /*!< Index of chosen entry in second list. */
    int mLastType;              /*!< Type of the task before updating. */

    std::vector<NumberedActionReceiver*> mLoadActionReceivers;         /*!< Action receiver for task load dropdown list. */
    std::vector<NumberedActionReceiver*> mListNumberedActionReceivers; /*!< Action receiver for list context menu commands. */
    std::vector<NamedActionReceiver*> mListNamedActionReceivers;       /*!< Action receiver for list context menu scripts. */
    QPoint mFirstListClickedAt;  /*!< Position where first list was clicked to request context menu. */
    QPoint mSecondListClickedAt; /*!< Position where second list was clicked to request context menu. */

    bool mChosen; /*!< True if widget is selected in the parent SimulationFrame. */
};

}// namespace SimulationFrameUtil

#endif // SIMULATIONFRAME_TASKWIDGET_H
