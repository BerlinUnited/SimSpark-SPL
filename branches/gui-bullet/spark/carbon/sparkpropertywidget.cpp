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

#include "sparkpropertywidget.h"
#include "sparkpropertymanager.h"
#include "carbon.h"
#include "simspark.h"
#include "sparkcontroller.h"
#include "matrixwidget.h"
#include "vector3widget.h"
#include "vector2x3widget.h"
#include "vector4widget.h"
#include "sparkproperty.h"

#include <oxygen/sceneserver/basenode.h>
#include <QTableWidget>
#include <QLayout>

using namespace boost;
using namespace zeitgeist;

namespace SparkProperty
{

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

PropertyWidget::PropertyWidget(QWidget* parent, boost::shared_ptr<SparkController> sparkcontroller) :
    QFrame(parent),
    mSparkController(sparkcontroller)
{
    ui.setupUi(this);

    mPreventRefresh = false;
    mSelectedIndex = -1;
    mLastSelectedIndex = -2;

    mPropTable = ui.propertyTableWidget;

    mPropertyEditFrame = ui.propertyEditFrame;
    mPropertyEditFrame->setVisible(false);
    mPropertyEditFrameLayout = new QVBoxLayout();
    mPropertyEditFrameLayout->setContentsMargins(0,0,0,0);
    mPropertyEditFrame->setLayout(mPropertyEditFrameLayout);
    mPropertyEditWidget = 0;

    //connections
    connect(&mPropList, SIGNAL(propertyChanged(int)), this, SLOT(onPropertyChanged(int)));
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

void PropertyWidget::init()
{
    shared_ptr<SimSpark> spark = getSpark();
    if (spark.get() == 0)
    {
        return;
    }

    //Reset leaf path
    mLeaf = spark->GetCore()->GetRoot();

    //Init property List
    mPropList.init(mLeaf, mSparkController);

    //Update table widget
    mCurrentTableItem = 0;

    connect(mPropTable, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(tableItemChanged(QTableWidgetItem*)));
    connect(mPropTable, SIGNAL(currentItemChanged(QTableWidgetItem*, QTableWidgetItem*)), this, SLOT(currentTableItemChanged(QTableWidgetItem*, QTableWidgetItem*)));

    //Update list and views
    refresh();
}

void PropertyWidget::setLeaf(weak_ptr<Leaf> leaf)
{
    shared_ptr<SimSpark> spark = getSpark();
    if (spark.get() == 0)
        return;

    mLeaf = leaf;

    refresh();
}

void PropertyWidget::releaseLeaf()
{
    shared_ptr<SimSpark> spark = getSpark();
    if (spark.get() == 0)
        return;

    mLeaf = spark->GetCore()->GetRoot();

    mPropTable->selectionModel()->clear();
    mSelectedIndex = -1;

    refresh();
}

void PropertyWidget::refresh()
{
    shared_ptr<SimSpark> spark = getSpark();
    if (spark.get() == 0)
        return;

    refreshProperties();
    refreshTableWidget();
    refreshEditFrame();
}

void PropertyWidget::updateDisplay()
{
    shared_ptr<SimSpark> spark = getSpark();
    if (spark.get() == 0)
        return;

    mPropList.updateProperties();
    refreshTableWidget();
    refreshEditFrame();
}

void PropertyWidget::connectToPropertyManager(QObject* receiver, const char* member)
{
    const shared_ptr<const PropertyManager> manager(mPropList.getPropertyManager());
    connect(&*manager, SIGNAL(registerPropertyControlRequest()), receiver, member, Qt::BlockingQueuedConnection);
}

void PropertyWidget::refreshProperties()
{
    mPropList.refresh(mLeaf.lock());
}

void PropertyWidget::refreshTableWidget()
{
    SparkProperty::TProperties properties = mPropList.getProperties();

    bool valRefresh =
        (mPropTable->rowCount() == static_cast<int>(properties.size()));

    bool editing = static_cast<PropertyTableWidget*>(mPropTable)->isEditing();
    if (!valRefresh)
    {
        mPropTable->clearContents();
        mPropTable->setRowCount(properties.size());
    }
    else
    {
        //no val refresh while editing
        if (editing)
            return;
    }

    int line = 0;
    for (auto iter = properties.begin(); iter != properties.end(); ++iter)
    {
        const SparkProperty::Property& prop = **iter;
        QTableWidgetItem* nameItem;
        QTableWidgetItem* valueItem;

        //Entry and Value
        if (valRefresh)
        {
            // just refresh values
            nameItem = mPropTable->item(line, 0);
            valueItem = mPropTable->item(line, 1);
            nameItem->setText(prop.getCaption());
            valueItem->setText(prop.getStringValue());
        } 
        else
        {
            // complete refresh, add new items
            nameItem = new QTableWidgetItem(prop.getCaption());
            valueItem = new QTableWidgetItem(prop.getStringValue());
            mPropTable->setItem(line, 0, nameItem);
            mPropTable->setItem(line, 1, valueItem);
        }

        //Flags
        nameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        if (prop.isEditable())
             valueItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
        else
             valueItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        
        //Color
        if (!prop.isEditable())
            valueItem->setBackgroundColor(Qt::lightGray);
        else
            valueItem->setBackgroundColor(Qt::white);

        //Help-Text
        nameItem->setToolTip(prop.getHelp().compare("") == 0 ? prop.getStringValue() : prop.getHelp());
        valueItem->setToolTip(prop.getHelp().compare("") == 0 ? prop.getStringValue() : prop.getHelp());

        ++line;
    }

    if (!valRefresh)
    {
        //Reset selection
        mSelectedIndex = -1;
        mCurrentTableItem = 0;
    }
}

void PropertyWidget::refreshEditFrame()
{
    if (mPreventRefresh)
        return;

    mPreventRefresh = true;

    boost::shared_ptr<SparkProperty::Property> prop = 
        (mSelectedIndex == -1 ? boost::shared_ptr<SparkProperty::Property> () : mPropList.getProperty(mSelectedIndex));
    bool hasEntry = (prop.get() != 0);

    bool err = false;
    if (mLastSelectedIndex != mSelectedIndex && mSelectedIndex >= 0)
    {
        mLastSelectedIndex = mSelectedIndex;
        bool sameType = !hasEntry ? false : (prop->getDataType() == mEditWidgetDataType);
        bool hasWidget = (mPropertyEditWidget != 0);

        //Has widget and has to be deleted?
        if (hasWidget && (!hasEntry || (!prop->isEditable() || !sameType)))
        {
            //Remove old widget
            mPropertyEditFrameLayout->removeWidget(mPropertyEditWidget);
            mPropertyEditWidget->deleteLater();
            mPropertyEditWidget = 0;
            hasWidget = false;
        }

        //Has to create new widget?
        TypedLineEdit* edit;

        if (!hasWidget && hasEntry && prop->isEditable() && !sameType)
        {
            //Add new widget
            EDataType type = prop->getDataType();
            switch (type)
            {
                case DT_INT:    //fall
                case DT_LONG:   //fall
                case DT_FLOAT:  //fall
                case DT_DOUBLE: //fall
                case DT_STRING: 
                    edit = new TypedLineEdit(mPropertyEditFrame); 
                    mPropertyEditWidget = edit;
                    if      (type == DT_INT || type == DT_LONG)     edit->setIntType   (static_cast<const QIntValidator*>   (prop->getValidator()));
                    else if (type == DT_FLOAT || type == DT_DOUBLE) edit->setDoubleType(static_cast<const QDoubleValidator*>(prop->getValidator()));
                    else if (type == DT_STRING)                     edit->setStringType(static_cast<const QRegExpValidator*>(prop->getValidator()));
                    connect(mPropertyEditWidget, SIGNAL(typedChange()), this, SLOT(updateFromEditWidget()));
                    break;
                case SparkProperty::DT_BOOL:   
                    mPropertyEditWidget = 0; //no edit widget for bool values
                    break;
                case SparkProperty::DT_VEC3F:  
                    mPropertyEditWidget = new Vector3Widget(mPropertyEditFrame); 
                    connect(mPropertyEditWidget, SIGNAL(vectorChanged()), this, SLOT(updateFromEditWidget()));
                    break;
                case SparkProperty::DT_RGBA:   
                    mPropertyEditWidget = new Vector4Widget(mPropertyEditFrame); 
                    connect(mPropertyEditWidget, SIGNAL(vectorChanged()), this, SLOT(updateFromEditWidget()));
                    break;
                case SparkProperty::DT_AABB3:  
                    mPropertyEditWidget = new Vector2x3Widget(mPropertyEditFrame);
                    connect(mPropertyEditWidget, SIGNAL(vectorChanged(int)), this, SLOT(updateFromEditWidget()));
                    break;
                case SparkProperty::DT_MAT4X4: 
                    mPropertyEditWidget = new MatrixWidget(mPropertyEditFrame); 
                    connect(mPropertyEditWidget, SIGNAL(matrixChanged()), this, SLOT(updateFromEditWidget()));
                    break;
                default:
                    LOG_ERROR() << "Data type " << prop->getDataType() << " has no data entry widget assigned.";
                    err = true;    
                    break;
            }

            //Add widget to layout
            if (!err && mPropertyEditWidget != 0)
                mPropertyEditFrameLayout->addWidget(mPropertyEditWidget);
        }
        else
        {
            //identical data type, keep
        }
    }
    else
    {
        //No selection
        if (mPropertyEditWidget != 0 && mSelectedIndex == -1)
        {
            //Deselected, remove widget
            mPropertyEditFrameLayout->removeWidget(mPropertyEditWidget);
            mPropertyEditWidget->deleteLater();
            mPropertyEditWidget = 0;
        }
    }

    if (mPropertyEditWidget != 0)
    {   
        ui.propertyEditFrame->setVisible(true);
        mEditWidgetDataType = prop->getDataType();

        //Fill with data
        const salt::Matrix* mat; const salt::Vector3f* vec3; const kerosin::RGBA* rgba; const salt::AABB3* aabb3;
        switch (mEditWidgetDataType)
        {
            case SparkProperty::DT_INT   : //fall
            case SparkProperty::DT_LONG  : //fall
            case SparkProperty::DT_FLOAT : //fall
            case SparkProperty::DT_DOUBLE: //fall
            case SparkProperty::DT_STRING: 
                ((TypedLineEdit*)mPropertyEditWidget)->setText(prop->getCurrentValue()->toString());
                break;
            case SparkProperty::DT_BOOL  : //no edit window for bool
                break;
            case SparkProperty::DT_VEC3F : 
                vec3 = &boost::shared_static_cast<const DVector3f>(prop->getCurrentValue())->mValue;
                ((Vector3Widget*)mPropertyEditWidget)->setVector(vec3->x(), vec3->y(), vec3->z());
                break;
            case SparkProperty::DT_RGBA  : 
                rgba = &boost::shared_static_cast<const DRGBA>(prop->getCurrentValue())->mValue;
                ((Vector4Widget*)mPropertyEditWidget)->setVector(rgba->r(), rgba->g(), rgba->b(), rgba->a());
                break;
            case SparkProperty::DT_AABB3 : 
                aabb3 = &boost::shared_static_cast<const DAABB3>(prop->getCurrentValue())->mValue;
                ((Vector2x3Widget*)mPropertyEditWidget)->getWidget1().setVector(aabb3->minVec.x(), aabb3->minVec.y(), aabb3->minVec.z());
                ((Vector2x3Widget*)mPropertyEditWidget)->getWidget2().setVector(aabb3->maxVec.x(), aabb3->maxVec.y(), aabb3->maxVec.z());
                break;
            case SparkProperty::DT_MAT4X4:
                mat = &boost::shared_static_cast<const DMat4x4>(prop->getCurrentValue())->mValue;
                ((MatrixWidget*)mPropertyEditWidget)->setMatrix(
                    mat->m[0], mat->m[4], mat->m[ 8], mat->m[12],
                    mat->m[1], mat->m[5], mat->m[ 9], mat->m[13],
                    mat->m[2], mat->m[6], mat->m[10], mat->m[14],
                    mat->m[3], mat->m[7], mat->m[11], mat->m[15]);
                break;
            default:
                LOG_ERROR() << "Data type " << prop->getDataType() << " has no data entry widget assigned.";
                break;
        }    
    }
    else
    {
        //no widget created
        mEditWidgetDataType = SparkProperty::DT_UNDEFINED;
        ui.propertyEditFrame->setVisible(false);
    }

    mPreventRefresh = false;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

boost::shared_ptr<SimSpark> PropertyWidget::getSpark()
{
    if (mSparkController.get() == 0)
    {
        LOG_ERROR() << "No spark controller";
        return boost::shared_ptr<SimSpark>();
    }

    return mSparkController->getSpark();
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void PropertyWidget::updateFromEditWidget()
{
    if (mPreventRefresh) //dont update from matrix creation
        return;

    if (mPropertyEditWidget == 0)
    {
        LOG_ERROR() << "Edit widget not defined.";
        return;
    }

    if (mSelectedIndex == 0)
    {
        LOG_ERROR() << "No entry selected.";
        return;
    }

    boost::shared_ptr<Property> selectedProperty(mPropList.getProperty(mSelectedIndex));
    if (selectedProperty.get() == 0)
    {
        LOG_ERROR() << "Could not find selected property in property list by index.";
        return;
    }

    //Read data from widget
    salt::Matrix mat; QMatrix4x4 qmat; DMat4x4* dmat;
    salt::Vector3f vec3; QVector3D qvec3, qvec3a, qvec3b; DVector3f* dvec3;
    kerosin::RGBA rgba; QVector4D qvec4; DRGBA* drgba;
    salt::AABB3 aabb3; DAABB3* daabb3;
    switch (mEditWidgetDataType)
    {
        case SparkProperty::DT_INT:    //fall
        case SparkProperty::DT_LONG:   //fall
        case SparkProperty::DT_FLOAT:  //fall
        case SparkProperty::DT_DOUBLE: //fall
        case SparkProperty::DT_STRING: //fall
            selectedProperty->setValue(dynamic_cast<TypedLineEdit*>(mPropertyEditWidget)->text());
            break;
        case SparkProperty::DT_BOOL: //no edit widget for bool values
            break;
        case SparkProperty::DT_VEC3F:  
            //Convert Vector
            qvec3 = dynamic_cast<Vector3Widget*>(mPropertyEditWidget)->getVector();
            vec3 = salt::Vector3f(qvec3.x(), qvec3.y(), qvec3.z());
            dvec3 = new SparkProperty::DVector3f(vec3);
            //Check if valid and store
            if (selectedProperty->isValid(dvec3))
                selectedProperty->setValue(dvec3);
            else
            {
                delete dvec3;
                LOG_ERROR() << "Value " << dvec3->toString() << " is illegal for entry " << selectedProperty->getCaption();
            }
            break;
        case SparkProperty::DT_RGBA:   
            //Convert Vector
            qvec4 = dynamic_cast<Vector4Widget*>(mPropertyEditWidget)->getVector();
            rgba = kerosin::RGBA(qvec4.x(), qvec4.y(), qvec4.z(), qvec4.w());
            drgba = new SparkProperty::DRGBA(rgba);
            //Check if valid and store
            if (selectedProperty->isValid(drgba))
                selectedProperty->setValue(drgba);
            else
            {
                delete drgba;
                LOG_ERROR() << "Value " << drgba->toString() << " is illegal for entry " << selectedProperty->getCaption();
            }
            break;
        case SparkProperty::DT_AABB3:  
            //Convert Both vectors
            qvec3a = dynamic_cast<Vector2x3Widget*>(mPropertyEditWidget)->getWidget1().getVector();
            qvec3b = dynamic_cast<Vector2x3Widget*>(mPropertyEditWidget)->getWidget2().getVector();
            aabb3 = salt::AABB3(salt::Vector3f(qvec3a.x(), qvec3a.y(), qvec3a.z()), 
                                salt::Vector3f(qvec3b.x(), qvec3b.y(), qvec3b.z()));
            daabb3 = new SparkProperty::DAABB3(aabb3);
            //Check if valid and store
            if (selectedProperty->isValid(daabb3))
                selectedProperty->setValue(daabb3);
            else
            {
                delete daabb3;
                LOG_ERROR() << "Value " << daabb3->toString() << " is illegal for entry " << selectedProperty->getCaption();
            }
            break;
        case SparkProperty::DT_MAT4X4: 
            //Convert Matrix
            qmat = dynamic_cast<MatrixWidget*>(mPropertyEditWidget)->getMatrix();
            mat = salt::Matrix(
                (qmat)(0,0), (qmat)(0,1), (qmat)(0,2), (qmat)(0,3),
                (qmat)(1,0), (qmat)(1,1), (qmat)(1,2), (qmat)(1,3),
                (qmat)(2,0), (qmat)(2,1), (qmat)(2,2), (qmat)(2,3),
                (qmat)(3,0), (qmat)(3,1), (qmat)(3,2), (qmat)(3,3));
            dmat = new SparkProperty::DMat4x4(mat);
            //Check if valid and store
            if (selectedProperty->isValid(dmat))
                selectedProperty->setValue(dmat);
            else
            {
                delete dmat;
                LOG_ERROR() << "Value " << dmat->toString() << " is illegal for entry " << selectedProperty->getCaption();
            }
            break;
        default:
            LOG_ERROR() << "Data type " << mEditWidgetDataType << " has no data entry widget assigned.";
            return;
            break;
    }    

    //Update string value
    selectedProperty->createStringValue();

    //Apply changes
    mPropList.writeProperty(mSelectedIndex);
}

void PropertyWidget::currentTableItemChanged(QTableWidgetItem* currentItem, QTableWidgetItem* previousItem)
{
    mCurrentTableItem = currentItem;
    mSelectedIndex = (currentItem == 0 ? 0 : currentItem->row());
    
    refreshEditFrame();
}

void PropertyWidget::tableItemChanged(QTableWidgetItem* item)
{
    if (item != mCurrentTableItem)
        return;

    if (item->column() != 1)
        return;

    boost::shared_ptr<SparkProperty::Property> prop = mPropList.getProperty(item->row());
    if (prop.get() == 0)
    {
        LOG_ERROR() << "Property with index " << item->row() << " not found";
        return;
    }

    if (item->text().compare(prop->getStringValue()) == 0)
        return;

    //Change value
    if (prop->isEditable())
    {
        if (prop->getValidator() == 0)
        {
            //No validator
            LOG_INFO() << "No validator for entry " << prop->getCaption() << ". Cant apply value " << prop->getStringValue();
            item->setText(prop->getStringValue());
        }
        else if (prop->isValid(prop->getStringValue()))
        {
            //Entry legal, set temp value and queue set call
            prop->setValue(item->text());
            mPropList.writeProperty(item->row());
        }
        else
        {
            //Entry illegal
            LOG_INFO() << "Value " << prop->getStringValue() << " is illegal for entry " << prop->getCaption();
            item->setText(prop->getStringValue());
        }
    }
    else
    {
        LOG_ERROR() << "Entry not meant to be editable.";
        item->setText(prop->getStringValue());
    }
}

void PropertyWidget::onPropertyChanged(int index)
{
    boost::shared_ptr<Property> prop(mPropList.getProperty(index));

    if (prop.get() == 0)
    {
        LOG_ERROR() << "Property with index " << index << " not found.";
        return;
    }

    bool refreshed = false;
    if (mPropTable->rowCount() <= index ||
        mPropTable->item(index, 0)->text().compare(prop->getCaption()) != 0)
    {
        //Item not in list or different item -> update complete table
        refreshTableWidget();
        refreshed = true;
    }

    //Corresponding table widget item has the same index
    QTableWidgetItem* item = mPropTable->item(index, 1);

    if (!refreshed)
    {
        //Update the table item
        item->setText(prop->getStringValue());
    }

    //Update the edit frame
    if (item == mCurrentTableItem)
    {
        if (mPropertyEditWidget != 0)
        {
            refreshEditFrame();
        }
    }
}

} //namespace SparkProperty
