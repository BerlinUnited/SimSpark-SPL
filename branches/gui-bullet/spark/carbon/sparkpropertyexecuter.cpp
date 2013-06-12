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

#include "sparkpropertyexecuter.h"
#include "sparkpropertycontrol.h"
#include "cutelogger/logger.h"

namespace SparkProperty
{

//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

PropertyExecuter::PropertyExecuter(PropertyControl* parent)
    : mParent(parent), mGetSemaphore(1), mSetSemaphore(1)
{
    connect(this, SIGNAL(acceptChange(boost::shared_ptr<SparkProperty::Property>)), parent, 
        SLOT(acceptChange(boost::shared_ptr<SparkProperty::Property>)), Qt::QueuedConnection);
    connect(this, SIGNAL(rejectChange(boost::shared_ptr<SparkProperty::Property>)), parent, 
        SLOT(rejectChange(boost::shared_ptr<SparkProperty::Property>)), Qt::QueuedConnection);
    connect(this, SIGNAL(updated(boost::shared_ptr<SparkProperty::Property>, bool)), parent, 
        SLOT(updated(boost::shared_ptr<SparkProperty::Property>, bool)), Qt::QueuedConnection);

    mFailedSetCount = 0;
    mFailedGetCount = 0;
    mSetWait = 5;
    mGetWait = 5;
}

PropertyExecuter::~PropertyExecuter()
{
}

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

void PropertyExecuter::queueGet(boost::shared_ptr<Property> prop)
{
    if (mGetSemaphore.tryAcquire(1, 1000))
    {
        mGetQueue.push_back(prop);
        mGetSemaphore.release();
    }
    else
        LOG_ERROR() << "Could not queue get-call for property " << prop->mCaption << ". Access to PropertyExecuter is locked.";
}

void PropertyExecuter::queueSet(boost::shared_ptr<Property> prop)
{
    if (mSetSemaphore.tryAcquire(1, 1000))
    {
        mSetQueue.push_back(prop);
        mSetSemaphore.release();
    }
    else
        LOG_ERROR() << "Could not queue set-call for property " << prop->mCaption << ". Access to PropertyExecuter is locked.";

}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void PropertyExecuter::onCycle()
{
    bool failedSet = false;
    bool failedGet = false;
    
    //Call queued setters
    if (!mSetQueue.empty())
    {
        if (mSetSemaphore.tryAcquire(1, mSetWait))
        {
            failedSet = true;
            //LOG_DEBUG() << "Acquired after " << mFailedSetCount << " tries.";
            while (!mSetQueue.empty())
            {
                //Lock access to property
                boost::shared_ptr<Property> prop(mSetQueue.front());
                if (prop->lock(mSetWait))
                {
                    failedSet = false;

                    //call setter
                    bool changed = mParent->applyPropertyChange(prop);
                    if (changed)
                    {
                        //notify control about the update (queued connection)
                        emit acceptChange(prop);
                    }
                    else
                    {
                        //notify control about the failed update (queued connection)
                        emit rejectChange(prop);
                    }
                    mSetQueue.pop_front();
                    prop->unlock();
                }
            }
            mSetSemaphore.release();
        }
        else
        {
            failedSet = true;
        }
    }

    if (failedSet)
    {
        mFailedSetCount++;
        mSetWait = 1 + ((mFailedSetCount / 5) * 5);
        if (mFailedSetCount = 20 || mFailedSetCount > 100)
            LOG_ERROR() << "Could not dequeue set-calls after trying " << mFailedSetCount << " times. Access to PropertyExecuter is locked.";
    }
    else
    {
        mFailedSetCount = 0;
        mSetWait = 1;
    }

    //Call queued getters
    if (!mGetQueue.empty())
    {
        if (mGetSemaphore.tryAcquire(1, mGetWait))
        {
            failedGet = true;
            //LOG_DEBUG() << "Acquired after " << mFailedGetCount << " tries.";
            while (!mGetQueue.empty())
            {
                //Lock access to property
                boost::shared_ptr<Property> prop(mGetQueue.front());
                if (prop->lock(mGetWait))
                {
                    failedGet = false;

                    //call getter
                    bool ok = mParent->updateProperty(prop);

                    //notify control about the finished read update (queued connection)
                    emit updated(prop, ok);

                    mGetQueue.pop_front();
                    prop->unlock();
                }
            }
            mGetSemaphore.release();
        }
        else
        {
            failedGet = true;
        }
    }

    if (failedGet)
    {
        mFailedGetCount++;
        mGetWait = mSetWait = 1 + ((mFailedGetCount / 5) * 5);
        if (mFailedGetCount = 20 || mFailedGetCount > 100)
            LOG_ERROR() << "Could not dequeue get-calls after trying " << mFailedGetCount << " times. Access to PropertyExecuter is locked.";
    }
    else
    {
        mFailedGetCount = 0;
        mGetWait = 1;
    }
}

//--------------------------------------------------------------
// Protected virtual
//--------------------------------------------------------------

void PropertyExecuter::endCycle()
{
    onCycle();
}

void PropertyExecuter::waitCycle()
{
    onCycle();
}


} //namespace SparkProperty