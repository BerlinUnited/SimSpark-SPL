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

#include "scenegraphframe_propertyframe.h"
#include "scenegraphframe.h"

#include <sparkpropertywidget.h>
#include <simulationmanager.h>
#include <serverthread.h>
#include <sparkcontroller.h>
#include <carbon.h>

#include <zeitgeist\leaf.h>
#include <zeitgeist\node.h>

#include <QLayout>

namespace SceneGraphFrameUtil
{

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

PropertyFrame::PropertyFrame(QWidget* parent) :
    QFrame(parent)
{
    ui.setupUi(this);
    mPropertyWidget = 0;
    mCreator = 0;

    mPropertyContainerLayout = new QVBoxLayout();
    mPropertyContainerLayout->setContentsMargins(0,0,0,0);
    ui.propertyWidgetContainer->setLayout(mPropertyContainerLayout);
}

PropertyFrame::~PropertyFrame()
{
}

void PropertyFrame::inspectLeaf(boost::shared_ptr<zeitgeist::Leaf> leaf)
{
    if (mPropertyWidget == 0)
    {
        LOG_ERROR() << "Property Widget not initialized.";
        return;
    }

    mPropertyWidget->setLeaf(leaf);
}

void PropertyFrame::releaseLeaf()
{
    if (mPropertyWidget == 0)
        return;

    mPropertyWidget->releaseLeaf();
}

void PropertyFrame::updateDisplay()
{
    if (mPropertyWidget == 0)
        return;

    mPropertyWidget->updateDisplay();
}

SparkProperty::PropertyWidget* PropertyFrame::getPropertyWidget()
{
    return mPropertyWidget;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void PropertyFrame::init(SceneGraphFrame* creator)
{
    LOG_ASSERT(creator);
    LOG_DEBUG() << "Init property frame.";

    //Store creator. Used to access Carbon.
    mCreator = creator;

    if (mPropertyWidget != 0)
    {
        mPropertyContainerLayout->removeWidget(mPropertyWidget);
        mPropertyWidget->deleteLater();
        mPropertyWidget = 0;
    }

    boost::shared_ptr<ServerThread> thread = boost::shared_ptr<ServerThread>(mCreator->getCarbon()->getSimulationManager()->getSparkServerThread());
    if (thread.get() == 0)
    {
        LOG_DEBUG() << "No spark server thread found.";
        return;
    }

    boost::shared_ptr<SparkController> controller = boost::shared_ptr<SparkController>(thread->getSparkController());
    if (controller.get() == 0)
    {
        LOG_ERROR() << "No spark controller found in server thread.";
        return;
    }    

    //Create property widget
    mPropertyWidget = new SparkProperty::PropertyWidget(ui.propertyWidgetContainer, controller);
    mPropertyContainerLayout->addWidget(mPropertyWidget);

    mPropertyWidget->init();
}

} //namespace SceneGraphFrameUtil