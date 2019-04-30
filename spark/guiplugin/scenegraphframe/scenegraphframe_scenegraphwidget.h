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

#ifndef SCENEGRAPHFRAME_SCENEGRAPHWIDGET_H
#define SCENEGRAPHFRAME_SCENEGRAPHWIDGET_H
/**
 * \file scenegraphframe_scenegraphwidget.h
 * \brief Contains SceneGraphFrameUtil::SceneGraphWidget class.
 */

#include "ui_scenegraphframe_scenegraphwidget.h"

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <QWidget>
#include <vector>

class SparkTreeModel;
class SparkController;
class SimSpark;
class SparkSimulationThread;
class NumberedActionReceiver;
class SceneGraphFrame;

namespace zeitgeist
{
    class Core;
    class Leaf;
}

namespace SceneGraphFrameUtil
{   

/*!
 \brief Widget displaying a spark scene graph.

 The Widget uses SparkTreeModel and SparkTreeView to access a SimSpark object.

 It contains a SparkTreeView and controls to update, expand or collapse the view.
 SceneGraphWidget are created by and contained in the SceneGraphFrame.
 An automatic update timer of the SceneGraphFrame may regularly call the updateDisplay() function to force an update.
*/
class SceneGraphWidget : public QWidget
{
    Q_OBJECT

public:

    /*!
     \brief Identification for commands that can be executed on an item in the scene graph.
    */
    enum ECommand
    {
        CMD_EXPAND = 0, /*!< Expand recursively. */
        CMD_COLLAPSE,   /*!< Collapse recursively. */
        CMD_PRINT,      /*!< Print properties of the item and its undelying data. */
        CMD_DELETE,     /*!< Delete the item and its underlying data. */
        CMD_INSPECT,    /*!< Inspect the item in the properties section. */
        CMD_CUT,        /*!< Starts moving the item to a new location. */
        CMD_PASTE,      /*!< Inserts an item that has been cut at a new location. */
        CMD_LOAD,       /*!< Load data from a file and inserts it into the scene graph. */
        CMD_SAVE        /*!< Save the scene graph item to a file. */
    };

    static int mMaxCommands;             /*!< Number of available commands. */
    static ECommand mCommands[9];        /*!< List of available commands. */
    static QString mCommandText[9];      /*!< Names of available commands. Used in context menu. */
    static QString mCommandIconNames[9]; /*!< Names of icons for available commands. Used in context menu. */

public:
    // public functions

    /*!
     \brief Constructor.

     Stores the SparkController reference and waits for the first update call. If the controller already posesses a SimSpark object, an initial
     update is executed.

     \param thread thread which contains the SparkController
     \param creator parent SceneGraphFrame
     \param parentWidget parent widget
    */
    SceneGraphWidget(boost::shared_ptr<SparkSimulationThread> thread, SceneGraphFrame* creator, QWidget* parentWidget = 0);
    /*!
     \brief Copy constructor.
    */
    SceneGraphWidget(const SceneGraphWidget& ref);
    /*!
     \brief Destructor.
    */
    virtual ~SceneGraphWidget();
    /*!
     \brief Copy operator.
    */
    const SceneGraphWidget& operator=(const SceneGraphWidget& rhs);

    /*!
     \brief Updates the display by assigning a new thread to read from.

     \param thread
    */
    void updateDisplay(boost::shared_ptr<SparkSimulationThread> thread);

    /*!
     \brief Removes all action receivers.
    */
    void clearActionReceivers();

    /*!
     \brief Returns true if the command is available for the currently selected leaf.

     \param command command to check availability for
    */
    bool canExecuteCommandInCurrentItem(ECommand command) const;
    /*!
     \brief Executes the given command on the currently selected leaf.

     \param command command to execute
    */
    void executeCommandInCurrentItem(ECommand command);

    /*!
     \brief Returns the currently selected leaf.
    */
    boost::shared_ptr<zeitgeist::Leaf> getCurrentLeaf() const;

    /*!
/    \brief Select the leaf at an absolute path.

     \param path absolute path in the scene graph
    */
    void pickLeaf(const QString& path);

protected:
    //protected functions
 
    /*!
     \brief Find a leaf by hierarchically searching with a list of keys, starting from a model index.

     Recursion calls findLeaf again with listIndex+1
     \param keys list of keys to search with
     \param root model index to start from
     \param listIndex index in the list to use
    */
    QModelIndex findLeaf(const QStringList& keys, const QModelIndex& root, int listIndex);

    /*!
     \brief Updates the display using the given spark controller object.
    */
    void updateDisplay(boost::shared_ptr<SparkController> controller);

private:
    //private functions

    /*!
     \brief Initializes model and view with the given spark controller object.
    */
    void initModelView(boost::shared_ptr<SparkController> controller);

    /*!
     \brief Creates a context menu command entry for the given command.

     \param menu menu to add command entry to
     \param command command to create entry for
    */
    void addContextMenuItem(QMenu* menu, ECommand command);

    /*!
     \brief Returns true if the leaf at the given index can be expanded.
    */
    bool canExpandItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const;
    /*!
     \brief Returns true if the leaf at the given index can be collapsed.
    */
    bool canCollapseItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const;
    /*!
     \brief Returns true if the leaf at the given index can be printed to the console.
    */
    bool canPrintItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const;
    /*!
     \brief Returns true if the leaf at the given index can be deleted from the scene graph.
    */
    bool canDeleteItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const;
    /*!
     \brief Returns true if the leaf at the given index can be inspected in a property widget.
    */
    bool canInspectItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const;
    /*!
     \brief Returns true if the leaf at the given index can cut.
    */
    bool canCutItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const;
    /*!
     \brief Returns true if a currently cut can be pasted at the given leaf at the given index.
    */
    bool canPasteInItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const;
    /*!
     \brief Returns true if a scene file can be loaded in the the leaf at the given index.
    */
    bool canLoadInItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const;
    /*!
     \brief Returns true if the leaf at the given index can be saved to file.
    */
    bool canSaveItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const;

    /*!
     \brief Recursively expands the children of the specified item.
    */
    void expandItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf);
    /*!
     \brief Recursively collapses the children of the specified item.
    */
    void collapseItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf);
    /*!
     \brief Prints detailed information on the specified item.

     Prints the item index, the contained pointer, the proxy model data (class and name), and if the actual model pointer is still valid,
     information absolute path of the item, siblings, and children.
    */
    void printItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf);
    /*!
     \brief Deletes the specified item from the scene graph.

     Queues a delete command in the SparkControllers command queue. The item will be deleted by the Simulation-Server at the end of the next
     simulation cycle or wait cycle.
    */
    void deleteItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf);
    /*!
     \brief Inspects the specified item in the property widget.
    */
    void inspectItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf);
    /*!
     \brief Cuts the specified item from the scene graph location. Marks it for moving to a pase position.

     \todo not implemented yet
    */
    void cutItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf); 
    /*!
     \brief Pastes a cut item into the specified leaf.

     \todo not implemented yet
    */
    void pasteInItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf);
    /*!
     \brief Open a scene graph file load dialog to load a file into the specified leaf.

     Queues a load command in the SparkControllers command queue. The file will be loaded by the Simulation-Server at the end of the next
     simulation cycle or wait cycle.
    */
    void loadInItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf);
    /*!
     \brief Save the specified item by exporting the leaf into a file.

     \todo not implemented yet. needs serialization or special handling of source-file parents in spark
    */
    void saveItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf);

public slots:
    //public slots

    /*!
     \brief Updates the model data with the currently stored threads SparkController.
    */
    void updateModelData();
    /*!
     \brief Collapses all items of the SparkTreeView.
    */
    void collapseAll();
    /*!
     \brief Expands all items of the SparkTreeView.
    */
    void expandAll();

    /*!
     \brief Displays context menu for an item in the scene graph.

     \param pos local position
    */
    void showItemContextMenu(QPoint pos);
    /*!
     \brief Invoke a command on the item in the scene graph.

     \param command command id
    */
    void onItemContextMenuClick(int command);
    /*!
     \brief Call inspect command on the clicked item.

     \param index model index of the item
    */
    void onItemClick(QModelIndex index);

    /*!
     \brief Logs success or failure message after trying to load a scene graph file.
    */
    void onImportScene(int id, bool success);
    /*!
     \brief Logs success or failure message after trying to delete a scene graph file.
    */
    void onRemoveLeaf(int id, bool success);

private:
    // private members

    Ui::SceneGraphWidget ui; /*!< UI definition. */
    SceneGraphFrame* mParent; /*!< Parent SceneGraphFrame */

    SparkTreeModel* mModel; /*!< Spark tree model. */
    boost::shared_ptr<SparkController> mSparkController; /*!< Pointer to spark controller. */

    std::vector<NumberedActionReceiver*> mItemActionReceivers; /*!< Action receiver for scene graph item context menu. */
    QModelIndex mCurrentIndex; /*!< Currently selected index in the scene graph. */
};

} //namespace SceneGraphFrameUtil

#endif // SCENEGRAPHFRAME_SCENEGRAPHWIDGET_H
