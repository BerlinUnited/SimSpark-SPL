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

#ifndef SPARKPROPERTYWIDGET_H
#define SPARKPROPERTYWIDGET_H
/**
 \file sparkpropertywidget.h
 \brief Contains SparkProperty::PropertyWidget class.
*/

#include "ui_sparkpropertywidget.h"

#include "sparkproperty.h"
#include "sparkpropertylist.h"
#include "sparkpropertydata.h"

#include <QImage>
#include <QFrame>
#include <QTableWidget>

#ifndef Q_MOC_RUN
#include <boost/weak_ptr.hpp>
#endif
#include <zeitgeist/leaf.h>

class QTableWidget;
class SparkController;
class SimSpark;

namespace SparkProperty
{

/*!
 \brief A QFrame that contains, displays and controls a SparkProperty::PropertyList.

 It consists of a SparkProperty::PropertyTableWidget which displays the contained PropertyList and a frame which can contain different
 widgets like a Vector3Widget or a MatrixWidget to edit data of these Properties.
*/
class PropertyWidget : public QFrame
{
    Q_OBJECT

public:
    //public functions

    /*!
     \brief Constructor.

     \param parent parent widget
     \param sparkcontroller SparkController to work with
    */
    PropertyWidget(QWidget* parent, boost::shared_ptr<SparkController> sparkcontroller);

    /*!
     \brief Set the managed zeitgeist::Leaf.
    */
    void setLeaf(boost::weak_ptr<zeitgeist::Leaf> leaf);
    /*!
     \brief Release the managed leaf and reset the view.
    */
    void releaseLeaf();
    /*!
     \brief Update the display by refreshing the TableWidget and the edit frame.
    */
    void updateDisplay();
    /*!
     \brief Update the contained property list and update the display.
    */
    void refresh();

    /*!
     \brief Convenience function to connect a receiver to the registerPropertyControlRequest()
      signal of the contained PropertyManager with a blocking queued connection.

     \param receiver Receiver of the signal.
     \param member Member function (without parameters)
    */
    void connectToPropertyManager(QObject* receiver, const char* member);

    /*!
     \brief Initialization of the widget contents.

     Reset leaf path.
     Init property List.
     Update table widget.
     Update list and views.
    */
    void init();

    /*!
     \brief Refresh the displayed properties of the managed leaf.
    */
    void refreshProperties();
    /*!
     \brief Refresh the table widget.

     If the inspected leaf changed, complete reinitializes the table widget.
     Otherwise only updates the contained text.
    */
    void refreshTableWidget();
    /*!
     \brief Refresh the edit frame.

     If the selected table widget entry changed, updates or creates a wiget that can be used to edit the selected Property representation.
    */
    void refreshEditFrame();

private:
    //private functions

    /*!
     \brief Returns SimSpark of the contained SparkController or null pointer if SimSpark was not initialized yet.
    */
    boost::shared_ptr<SimSpark> getSpark();

public slots:
    //public slots

    /*!
     \brief Update the currently selected Property from the current edit widget.
    */
    void updateFromEditWidget();
    /*!
     \brief Called when the table item selection was changed. Refreshes the edit frame.

     \param currentItem new selected item
     \param previousItem old selected item
    */
    void currentTableItemChanged(QTableWidgetItem* currentItem, QTableWidgetItem* previousItem);
    /*!
     \brief Called when the currently selected table item was edited. Calls write update for the Property.

     \param item table item that was changed
    */
    void tableItemChanged(QTableWidgetItem* item);

    /*!
     \brief Called when a Property was changed (after successful read and write update or failed write update).

     Refreshes table widget and edit frame.
     \param index index of the property in the PropertyList
    */
    void onPropertyChanged(int index);

protected:
    //protected members

    Ui::PropertyWidget ui;              /*!< Ui definition object. */
    QFrame* mPropertyEditFrame;         /*!< Frame the property edit widget is contained in. */
    QLayout* mPropertyEditFrameLayout;  /*!< Layout the edit frame is contained in. */
    QWidget* mPropertyEditWidget;       /*!< Property edit widget used to manipulate the value of aselected Property. Can be of different types. */
    SparkProperty::EDataType mEditWidgetDataType; /*!< Data type of the edit widget (not the Property). */
    bool mPreventRefresh;               /*!< Update protection guard. Prevents update-lifelock. */
    PropertyTableWidget* mPropTable;    /*!< Property table containing Property representations. */

    boost::shared_ptr<SparkController> mSparkController; /*!< SparkController of the currently selected leaf. */
    boost::weak_ptr<zeitgeist::Leaf> mLeaf;              /*!< The currently selected leaf. */

    SparkProperty::PropertyList mPropList; /*!< Property list for the currently selected leaf. */
    QTableWidgetItem* mCurrentTableItem;   /*!< Currently selected table item in the mPropTable. */
    int mSelectedIndex;     /*!< Index of the currently selected Property in the mPropList. */
    int mLastSelectedIndex; /*!< Index of the previously selected Property in the mPropList. */
}; 

} //namespace SparkProperty

#endif // SPARKPROPERTYWIDGET_H
