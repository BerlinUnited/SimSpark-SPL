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

#ifndef SCENEGRAPHFRAME_PROPERTYFRAME_H
#define SCENEGRAPHFRAME_PROPERTYFRAME_H
/**
 * \file scenegraphframe_propertyframe.h
 * \brief Contains SceneGraphFrameUtil::PropertyFrame class.
 */

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif
#include <QFrame>
#include <vector>

#include "ui_scenegraphframe_propertyframe.h"

class SceneGraphFrame;

namespace zeitgeist
{
    class Core;
    class Leaf;
}

namespace SparkProperty
{
    class PropertyControl;
    class PropertyWidget;
}

namespace SceneGraphFrameUtil
{   

/*!
 \brief A QFrame for the SceneGraphFrame containing a SparkProperty::PropertyWidget.

 The PropertyFrame simply forwards selection of leafs selected in the SceneGraphFrameUtil::SceneGraphWidget.

 \note The PropertyFrame could be extended to contain different property fields for different kinds of scene graph
 information sources should they be implemented.

 \sa SceneGraphFrame
*/
class PropertyFrame : public QFrame
{
    Q_OBJECT

public:
    // public functions

    /*!
     \brief Constructor.

     \param parent parent widget
    */
    PropertyFrame(QWidget* parent = 0);
    /*!
     \brief Destructor.
    */
    virtual ~PropertyFrame();

    /*!
     \brief Inspects a leaf in the contained SparkProperty::PropertyWidget.

     \param leaf leaf to inspect
    */
    void inspectLeaf(boost::shared_ptr<zeitgeist::Leaf> leaf);
    /*!
     \brief Updates the display of the contained SparkProperty::PropertyWidget.
    */
    void updateDisplay();
    /*!
     \brief Releases the selected leaf in the contained SparkProperty::PropertyWidget.
    */
    void releaseLeaf();

    /*!
     \brief Returns the SparkProperty::PropertyWidget.
    */
    SparkProperty::PropertyWidget* getPropertyWidget();

public slots:

    /*!
     \brief Initializes the frame.

     \param creator Creator of the frame
    */
    void init(SceneGraphFrame* creator);

private:
    // private members

    Ui::PropertyFrame ui; /*!< UI definition. */

    SparkProperty::PropertyWidget* mPropertyWidget; /*!< Property Widget that displays spark leaf properties. */
    QLayout* mPropertyContainerLayout; /*!< Layout that contains the property widget. */
    SceneGraphFrame* mCreator; /*!< Creator of the Frame */
};

} //namespace SceneGraphFrameUtil

#endif // SCENEGRAPHFRAME_PROPERTYFRAME_H
