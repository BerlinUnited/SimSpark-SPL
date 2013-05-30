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

#include "sparkproperty.h"
#include "carbon.h"
#include "sparkpropertylist.h"
#include "sparkpropertydata.h"

namespace SparkProperty
{

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

Property::Property(boost::shared_ptr<zeitgeist::Leaf> leaf, ClassDescriptor descriptor, const QString& name, const QString& caption, 
    Data* value, bool editable, QValidator* validator, const QString& help) :
        mLeaf(leaf), mClass(descriptor), mName(name), mCaption(caption), mValue(value), mEditable(editable), 
        mValidator(validator), mHelp(help), mParent(0), mUpdateFlag(UF_NONE), mValueSemaphore(1)
{
    createStringValue();
}

Property::Property(boost::shared_ptr<zeitgeist::Leaf> leaf, ClassDescriptor descriptor, const QString& name, const QString& caption, 
    Data* value, const QString& help) :
        mLeaf(leaf), mClass(descriptor), mName(name), mCaption(caption), mValue(value), mEditable(false), 
        mValidator(0), mHelp(help), mParent(0), mUpdateFlag(UF_NONE), mValueSemaphore(1)
{
    createStringValue();
}

Property::~Property()
{
    if (mValidator != 0)
    {
        mValidator->deleteLater(); 
        mValidator = 0;
    }
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

const QString& Property::createStringValue()
{
    mStringValue = mValue->toString();
    return mStringValue;
}

EUpdateFlag Property::getUpdateFlag() const
{
    return mUpdateFlag;
}

const boost::shared_ptr<const Data> Property::getCurrentValue() const
{
    if (mUpdateFlag == UF_WRITING)
        return mTempValue;

    return mValue;
}

const boost::shared_ptr<const Data> Property::getValue() const
{
    return mValue;
}

const boost::shared_ptr<const Data> Property::getTempValue() const
{
    return mTempValue;
}

EDataType Property::getDataType() const
{
    return mValue->mDataType;
}

bool Property::setValue(Data* data)
{
    return setValue(boost::shared_ptr<Data>(data));
}

bool Property::setValue(boost::shared_ptr<Data> data)
{
    bool ok = false;
    if (mValueSemaphore.tryAcquire(1, 100))
    {
        ok = true;
        if (mUpdateFlag == UF_READING) //Do not change while reading
            ok = false;

        if (data->mDataType != mValue->mDataType)
        {
            LOG_ERROR() << "Data type mismatch.";
            ok = false;
        }

        if (ok)
        {
            mTempValue = boost::shared_ptr<Data>(data);
        }
        mValueSemaphore.release();
    }
    else
        LOG_ERROR() << "Could not acquire value semaphore to set value by ptr.";

    return ok;
}

bool Property::setValue(const QString& dataString)
{
    bool ok = false;
    if (mValueSemaphore.tryAcquire(1, 100))
    {
        ok = true;
        if (mUpdateFlag == UF_READING) //Do not change while reading
            ok = false;

        if (!isValid(dataString))
        {
            LOG_ERROR() << "Data String " << dataString << " is not a valid value for Property " << mCaption;
            ok = false;
        }
        
        if (ok)
        {
            boost::shared_ptr<Data> data(Data::createData(getDataType(), dataString));
            if (data.get() == 0)
            {
                LOG_ERROR() << "Failed to create PropertyData from String.";
                ok = false;
            }

            if (ok)
            {
               mTempValue = data;
            }
        }

        mValueSemaphore.release();
    }
    else
        LOG_ERROR() << "Could not acquire value semaphore to set value by string.";

    return ok;
}

bool Property::isValid(Data* data)
{
    if (mValidator == 0)
        return true;

    int n = 0;
    if (data == 0)
        return mValidator->validate(mStringValue, n);
    else
        return mValidator->validate(data->toString(), n);
}

bool Property::isValid(const QString& dataString)
{
    if (mValidator == 0)
        return true;

    int n = 0;
    return mValidator->validate(QString(dataString), n);
}

ClassDescriptor& Property::getClassNc()
{
    return mClass;
}

const ClassDescriptor& Property::getClass() const
{
    return mClass;
}

const QString& Property::getName() const
{
    return mName;
}

const QString& Property::getCaption() const
{
    return mCaption;
}

const QString& Property::getHelp() const
{
    return mHelp;
}

bool Property::isEditable() const
{
    return mEditable;
}

const QString& Property::getStringValue() const
{
    return mStringValue;
}

const QValidator* Property::getValidator() const
{
    return mValidator;
}

boost::weak_ptr<zeitgeist::Leaf> Property::getLeaf()
{
    return mLeaf;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void Property::setValueProtected(Data* data)
{
    setValueProtected(boost::shared_ptr<Data>(data));
}

// Protected write does not use semaphore, because it was already acquired by the PropertyExecuter
void Property::setValueProtected(boost::shared_ptr<Data> data)
{
    bool ok = true;

    if (data->mDataType != mValue->mDataType)
    {
        LOG_ERROR() << "Data type mismatch.";
        ok = false;
    }

    if (ok)
        mTempValue = boost::shared_ptr<Data>(data);
    
    mValueSemaphore.release();
}

bool Property::startUpdate(EUpdateFlag flag)
{
    if (flag == UF_NONE)
    {
        LOG_ERROR() << "Illegal update flag 'NONE' used for updating.";
        return false;
    }

    if (mValueSemaphore.tryAcquire(1, 100))
    {   
        if (mUpdateFlag != UF_NONE)
        {
            //Updating already queued
            mValueSemaphore.release();
            return false;
        }

        mUpdateFlag = flag;
        mValueSemaphore.release();
        return true;
    }
    return false;
}

void Property::updateFinished(bool ok)
{
    if (ok)
    {
        acceptTempValue(); //notifies parent
    }
    else
    {
        if (mValueSemaphore.tryAcquire(1, 100))
        {
            mUpdateFlag = UF_NONE;
            mValueSemaphore.release();
        }
        else
            LOG_ERROR() << "Could not acquire value semaphore.";
    }
}

void Property::acceptTempValue()
{
    if (mTempValue.get() == 0)
    {
        LOG_WARNING() << "Temp value is empty.";
        return;
    }

    if (mValueSemaphore.tryAcquire(1, 100))
    {
        mValue = mTempValue;
        createStringValue();

        mTempValue.reset();
        mUpdateFlag = UF_NONE;
        mValueSemaphore.release();

        notifyParent();
    }
    else
        LOG_ERROR() << "Could not acquire value semaphore.";
}

void Property::rejectTempValue()
{
    if (mTempValue.get() == 0)
    {
        LOG_WARNING() << "Temp value is empty.";
        return;
    }

    if (mValueSemaphore.tryAcquire(1, 100))
    {
        createStringValue();

        mTempValue.reset();
        mUpdateFlag = UF_NONE;
        mValueSemaphore.release();

        notifyParent();
    }
    else
        LOG_ERROR() << "Could not acquire value semaphore.";
}

void Property::setParent(PropertyList* parent)
{
    mParent = parent;
}

bool Property::lock(int time)
{
    return mValueSemaphore.tryAcquire(1, time);
}

void Property::unlock()
{
    mValueSemaphore.release();
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void Property::notifyParent()
{
    if (mParent != 0)
        mParent->updatePropertyChanged(this);
}

} //namespace SparkProperty
