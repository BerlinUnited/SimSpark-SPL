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

#include "scenegraphframe_scenegraphwidget.h"
#include "scenegraphframe_propertyframe.h"
#include "scenegraphframe.h"

#include <sparkcontext.h>
#include <sparktreemodel.h>
#include <sparksimulationthread.h>
#include <taskdefinition.h>
#include <sparkcontroller.h>
#include <simspark.h>
#include <numberedactionreceiver.h>
#include <carbon.h>
#include <windowmanager.h>

#include <zeitgeist\leaf.h>

#include <QMenu>
#include <QFileDialog>
#include <QStringList>

namespace SceneGraphFrameUtil
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

int SceneGraphWidget::mMaxCommands = 9;
SceneGraphWidget::ECommand SceneGraphWidget::mCommands[9] = {
        CMD_EXPAND,
        CMD_COLLAPSE,
        CMD_PRINT,
        CMD_DELETE,
        CMD_INSPECT,
        CMD_CUT,
        CMD_PASTE,
        CMD_LOAD,
        CMD_SAVE,
    };
QString SceneGraphWidget::mCommandText[9] =  {
        tr("Expand"),                 //CMD_EXPAND
        tr("Collapse"),               //CMD_COLLAPSE
        tr("Print", "Sends to log."), //CMD_PRINT
        tr("Delete"),                 //CMD_DELETE
        tr("Inspect"),                //CMD_INSPECT
        tr("Cut"),                    //CMD_CUT
        tr("Paste"),                  //CMD_PASTE
        tr("Load"),                   //CMD_LOAD
        tr("Save"),                   //CMD_SAVE
    };

QString SceneGraphWidget::mCommandIconNames[9] =  {
        ":arrow_expand",    //CMD_EXPAND
        ":arrow_collapse",  //CMD_COLLAPSE
        "",                 //CMD_PRINT
        ":delete",          //CMD_DELETE
        "",                 //CMD_INSPECT
        "",                 //CMD_CUT
        "",                 //CMD_PASTE
        "",                 //CMD_LOAD
        "",                 //CMD_SAVE
    };

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

SceneGraphWidget::SceneGraphWidget(boost::shared_ptr<SparkSimulationThread> thread, SceneGraphFrame* creator, QWidget* parentWidget) :
    QWidget(parentWidget),
    mParent(creator),
    mModel(0)
{
    ui.setupUi(this);

    //Initializes the scene graph with the threads data
    updateDisplay(thread);
}

SceneGraphWidget::SceneGraphWidget(const SceneGraphWidget& ref) :
    QWidget(0)
{
    ui.setupUi(this);

    *this = ref;
}

SceneGraphWidget::~SceneGraphWidget()
{
    if (mModel != 0)
        mModel->deleteLater();

    clearActionReceivers();
}

const SceneGraphWidget& SceneGraphWidget::operator=(const SceneGraphWidget& rhs)
{
    initModelView(rhs.mSparkController);

    return *this;
}

void SceneGraphWidget::updateDisplay(boost::shared_ptr<SparkSimulationThread> thread)
{
    mSparkController = boost::shared_ptr<SparkController>(thread->getSparkController());

    //Set header label text
    ui.nameLabel->setText(thread->getTaskDefinition().getName());
    //ui.typeLabel->setText(thread->getTaskDefinition().getTypeString());
    //ui.sparkLabel->setText("");

    if (thread->getTaskDefinition().getType() == TaskDefinition::TT_SERVERTHREAD)
    {
        //Initialize or update model data
        
        //Update scene graph display
        if (mSparkController->getSpark().get())
            updateDisplay(mSparkController);
        else
        {
            //simspark not initialized yet, wait
        }

        //Update spark label text
        if (thread.get())
            ui.sparkLabel->setText(thread->getTaskDefinition().getName());
        else
            ui.sparkLabel->setText(tr("No Server initialized."));
    }
    else
    {
        ui.sparkLabel->setText(tr("Not a spark task."));
    }
}

void SceneGraphWidget::clearActionReceivers()
{
    while (mItemActionReceivers.size() > 0)
    {
        mItemActionReceivers.back()->deleteLater();
        mItemActionReceivers.pop_back();
    }
}

bool SceneGraphWidget::canExecuteCommandInCurrentItem(ECommand command) const
{
    boost::shared_ptr<zeitgeist::Leaf> leaf(getCurrentLeaf());

    if (leaf.get() == 0)
    {
        //Item was already deleted
        LOG_ERROR() << "Leaf was deleted.";
        return false;
    }

    switch (command)
    {
        case CMD_EXPAND:    return canExpandItem(mCurrentIndex, leaf); break;
        case CMD_COLLAPSE:  return canCollapseItem(mCurrentIndex, leaf); break;
        case CMD_PRINT:     return canPrintItem(mCurrentIndex, leaf); break;
        case CMD_DELETE:    return canDeleteItem(mCurrentIndex, leaf); break;
        case CMD_INSPECT:   return canInspectItem(mCurrentIndex, leaf); break;
        case CMD_CUT:       return canCutItem(mCurrentIndex, leaf); break;
        case CMD_PASTE:     return canPasteInItem(mCurrentIndex, leaf); break;
        case CMD_LOAD:      return canLoadInItem(mCurrentIndex, leaf); break;
        case CMD_SAVE:      return canSaveItem(mCurrentIndex, leaf); break;
        default:
            LOG_ERROR() << "Unknown command.";
            break;
    }
    return false;
}

void SceneGraphWidget::executeCommandInCurrentItem(ECommand command)
{
    boost::shared_ptr<zeitgeist::Leaf> leaf(getCurrentLeaf());

    switch (command)
    {
        case CMD_EXPAND:    expandItem(mCurrentIndex, leaf); break;
        case CMD_COLLAPSE:  collapseItem(mCurrentIndex, leaf); break;
        case CMD_PRINT:     printItem(mCurrentIndex, leaf); break;
        case CMD_DELETE:    deleteItem(mCurrentIndex, leaf); break;
        case CMD_INSPECT:   inspectItem(mCurrentIndex, leaf); break;
        case CMD_CUT:       cutItem(mCurrentIndex, leaf); break;
        case CMD_PASTE:     pasteInItem(mCurrentIndex, leaf); break;
        case CMD_LOAD:      loadInItem(mCurrentIndex, leaf); break;
        case CMD_SAVE:      saveItem(mCurrentIndex, leaf); break;
        default:
            LOG_ERROR() << "Unknown command.";
            break;
    }

    mCurrentIndex = QModelIndex();
}

boost::shared_ptr<zeitgeist::Leaf> SceneGraphWidget::getCurrentLeaf() const
{
    SparkTreeItem* item = (SparkTreeItem*)mCurrentIndex.internalPointer();
    return item->leaf().lock();
}

void SceneGraphWidget::pickLeaf(const QString& path)
{
    QStringList searchKeys = path.split(QRegExp("/|\\\\"), QString::SkipEmptyParts);
    searchKeys.push_front("/"); //Root

    QModelIndex found = findLeaf(searchKeys, ui.sceneGraphTreeView->rootIndex(), 0);
   
    if( found.isValid())
    {
        //SelectItem
        ui.sceneGraphTreeView->setCurrentIndex(found);
        ui.sceneGraphTreeView->scrollTo(found);
        onItemClick(found);
    }
    else
        return;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

QModelIndex SceneGraphWidget::findLeaf(const QStringList& keys, const QModelIndex& index, int listIndex)
{
    if (listIndex > keys.size())
        return QModelIndex();

    if (index.isValid()) //otherwise root element
    {
        QString key = keys.at(listIndex);

        QString value = mModel->data(index, Qt::DisplayRole).toString();
        if (value.compare(key) != 0)
        {
            //no match, stop searching this branch
            return QModelIndex();
        }
        else
        {
            //Last comparison reached?
            if (listIndex + 1 == keys.size())
            {
                return index; //found
            }
        }
    }
    
    //Last comparison not reached, search recursively
    QModelIndex found = QModelIndex();
    for (int i = 0; i < mModel->rowCount(index); i++)
    {
        QModelIndex child = mModel->index(i,0, index);
        
        //call recursively
        QModelIndex returned = findLeaf(keys, child, listIndex+1);
        if (returned.isValid())
        {
            found = returned;
            break;
        }
    }
   
    return found;
}

void SceneGraphWidget::updateDisplay(boost::shared_ptr<SparkController> controller)
{
    if (mSparkController.get() == controller.get() && controller.get() != 0 && mModel != 0)
    {
        //Already has correct pointer, update model
        mModel->updateModelData();
    }
    else
    {
        //Different simspark core found, create view
        initModelView(controller);
    }
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void SceneGraphWidget::initModelView(boost::shared_ptr<SparkController> controller)
{
    //Handle new spark object
    mSparkController = boost::shared_ptr<SparkController>(controller);

    if (controller->getSpark().get() == 0)
    {
        //no simspark object given
        return;
    }

    //Setup model
    mModel = new SparkTreeModel(controller->getSpark()->GetCore());

    //Setup view
    ui.sceneGraphTreeView->connectModel(mModel);
    ui.sceneGraphTreeView->show();
    ui.sceneGraphTreeView->setColumnWidth(0, 200);
}

void SceneGraphWidget::addContextMenuItem(QMenu* menu, SceneGraphWidget::ECommand command)
{
    QAction* action;
    NumberedActionReceiver* newReceiver;

    int cmdIndex = (int)command;

    newReceiver = new NumberedActionReceiver(cmdIndex);
    mItemActionReceivers.push_back(newReceiver);
    action = menu->addAction(QIcon(mCommandIconNames[cmdIndex]), mCommandText[cmdIndex], newReceiver, SLOT(receiveAction()));
    action->setEnabled(canExecuteCommandInCurrentItem(command));
    connect(newReceiver, SIGNAL(actionReceived(int)), this, SLOT(onItemContextMenuClick(int)));
}

bool SceneGraphWidget::canExpandItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const
{
    return true;
}

bool SceneGraphWidget::canCollapseItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const
{
    return true;
}

bool SceneGraphWidget::canPrintItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const
{
    return true;
}

bool SceneGraphWidget::canDeleteItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const
{
    return leaf.get();
}

bool SceneGraphWidget::canInspectItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const
{
    return leaf.get();
}

bool SceneGraphWidget::canCutItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const
{
    return leaf.get();
}

bool SceneGraphWidget::canPasteInItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const
{
    return leaf.get();
}

bool SceneGraphWidget::canLoadInItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const
{
    if (leaf.get() == 0)
        return false;

    boost::shared_ptr<oxygen::BaseNode> bn(boost::shared_dynamic_cast<oxygen::BaseNode>(leaf));
    if (bn.get() == 0)
    {
        // Can only load in base nodes
        return false;
    }
    return true;
}

bool SceneGraphWidget::canSaveItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) const
{
    return false;
}

void SceneGraphWidget::expandItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf)
{
    ui.sceneGraphTreeView->expandChildren(index);
}

void SceneGraphWidget::collapseItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf)
{
    ui.sceneGraphTreeView->collapseChildren(index, false);
}

void SceneGraphWidget::printItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf)
{
    QString message = QString("Current tree index:> Row: %1, Col: %2, Ptr: 0x%3\n").
        arg(mCurrentIndex.row()).
        arg(mCurrentIndex.column()).
        arg(QString::number((int)mCurrentIndex.internalPointer(), 16));
    SparkTreeItem* item = (SparkTreeItem*)index.internalPointer();
    message.append(QString("Tree item data:> Name: %1, Class: %2, Ptr: 0x%3\n").
        arg(item->data(0).toString()).
        arg(item->data(1).toString()).
        arg(QString::number((int)item, 16)));

    if (leaf.get() == 0)
    {
        //Item was already deleted
        message.append(QString("Leaf:> (deleted)"));
    }
    else
    {
        boost::shared_ptr<zeitgeist::Node> parent = leaf->GetParent().lock();

        message.append(QString("Leaf:> Path: %1, Siblings: %2, Children: %3, Pointer: 0x%4 (references: %5)").
            arg(QString::fromStdString(leaf->GetFullPath())).
            arg(parent.get() ? QString("%1").arg(parent->GetNumberOfChildren()-1) : QString("(no parent)")).
            arg(leaf->GetNumberOfChildren()).
            arg(QString::number((int)&*leaf, 16)).
            arg(leaf.use_count() - 1)
            );
    }

    LOG_INFO() << message.toStdString().c_str();
}

void SceneGraphWidget::deleteItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf)
{
    if (leaf.get() == 0)
    {
        //Item was already deleted
        LOG_INFO() << "Cant execute delete command. Leaf was already deleted.";
        return;
    }
        
    //Queue the delete command
    SparkCommand::RemoveLeaf* command = new SparkCommand::RemoveLeaf(leaf);
    mSparkController->queueCommand(command, 100, this, SLOT(onRemoveLeaf(int, bool)));
}

void SceneGraphWidget::inspectItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf)
{
    if (leaf.get() == 0)
    {
        //Item was already deleted
        LOG_INFO() << "Cant execute inspect command. Leaf was deleted.";
        return;
    }

    PropertyFrame* propertyFrame = mParent->getPropertyFrame();
    propertyFrame->inspectLeaf(leaf);
}

void SceneGraphWidget::cutItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf) 
{
    if (leaf.get() == 0)
    {
        //Item was already deleted
        LOG_INFO() << "Cant execute cut command. Leaf was deleted.";
        return;
    }

    LOG_INFO() << "Todo";
}

void SceneGraphWidget::pasteInItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf)
{
    if (leaf.get() == 0)
    {
        //Item was already deleted
        LOG_INFO() << "Cant execute paste command. Leaf was deleted.";
        return;
    }

    LOG_INFO() << "Todo";
}

void SceneGraphWidget::loadInItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf)
{
    if (leaf.get() == 0)
    {
        //Item was already deleted
        LOG_INFO() << "Cant execute load command. Leaf was deleted.";
        return;
    }

    boost::shared_ptr<oxygen::BaseNode> bn(boost::shared_dynamic_cast<oxygen::BaseNode>(leaf));
    if (bn.get() == 0)
    {
        // Can only load in base nodes
        LOG_ERROR() << "Leaf is not a base node.";
        return;
    }

    //Open file dialog
    QFileDialog dialog((QWidget*)mParent->getCarbon()->getWindowManager()->getMainWindow(), tr("Load scene from file."), 
        mParent->mLastSceneFileDirectory);
    dialog.setNameFilters(mParent->mSceneFileNameFilters);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setLabelText(QFileDialog::Accept, tr("Load"));
    dialog.setToolTip(tr("Import a scene or model from a file."));
    dialog.setViewMode(QFileDialog::Detail);

    int returning = dialog.exec();
    if (returning != 0)
    {
        //File was chosen
        QStringList files = dialog.selectedFiles();
        QString file = files.at(0);

        //Store file
        mParent->mLastSceneFileDirectory = file;

        //Queue the import command
        boost::shared_ptr<zeitgeist::ParameterList> list(new zeitgeist::ParameterList());
        
        mSparkController->queueCommand(new SparkCommand::ImportScene(file, bn, list), 100, this, SLOT(onImportScene(int, bool)));
    }
}

void SceneGraphWidget::saveItem(const QModelIndex& index, boost::shared_ptr<zeitgeist::Leaf> leaf)   
{
    if (leaf.get() == 0)
    {
        //Item was already deleted
        LOG_INFO() << "Cant execute save command. Leaf was deleted.";
        return;
    }

    LOG_INFO() << "Todo";
}


//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void SceneGraphWidget::updateModelData()
{
    if (mModel == 0)
    {
        LOG_INFO() << "No model initialized. Need a running Server to display scene graph.";
        return;
    }

    mModel->updateModelData();
}

void SceneGraphWidget::collapseAll()
{
    ui.sceneGraphTreeView->collapseAll();
}

void SceneGraphWidget::expandAll()
{
    ui.sceneGraphTreeView->expandAll();
}

void SceneGraphWidget::showItemContextMenu(QPoint pos)
{
    mCurrentIndex = ui.sceneGraphTreeView->indexAt(pos);
    if (!mCurrentIndex.isValid())
    {
        //Did not click on an item -> no context menu
        return;
    }

    QMenu menu(this);
    clearActionReceivers();
    
    //Add command ids to list
    addContextMenuItem(&menu, CMD_EXPAND);
    addContextMenuItem(&menu, CMD_COLLAPSE);
    addContextMenuItem(&menu, CMD_PRINT);
    menu.addSeparator();

    addContextMenuItem(&menu, CMD_LOAD);
    addContextMenuItem(&menu, CMD_SAVE);
    menu.addSeparator();

    //addContextMenuItem(&menu, CMD_CUT);
    //addContextMenuItem(&menu, CMD_PASTE);
    addContextMenuItem(&menu, CMD_DELETE);
    menu.addSeparator();

    //Get popup position 
    QPoint globalpos = ui.sceneGraphTreeView->mapToGlobal(pos);
    
    //Open menu
    menu.exec(globalpos);    
}

void SceneGraphWidget::onItemContextMenuClick(int commandId)
{
    if (commandId < 0 || commandId >= mMaxCommands)
    {
        LOG_ERROR() << "Command index out of range.";
        return;
    }

    ECommand command = (ECommand)commandId;

    executeCommandInCurrentItem(command);
    mCurrentIndex = QModelIndex();
}

void SceneGraphWidget::onItemClick(QModelIndex index)
{
    mCurrentIndex = index;
    inspectItem(index, getCurrentLeaf());
}

void SceneGraphWidget::onImportScene(int id, bool success)
{
    if (success)
    {
        LOG_INFO() << "Import succeeded.";
    }
    else
    {
        LOG_INFO() << "Import failed.";
    }
}

void SceneGraphWidget::onRemoveLeaf(int id, bool success)
{
    if (success)
    {
        LOG_INFO() << "Removing succeeded.";
    }
    else
    {
        LOG_INFO() << "Removing failed.";
    }
}

} //namespace SceneGraphFrameUtil
