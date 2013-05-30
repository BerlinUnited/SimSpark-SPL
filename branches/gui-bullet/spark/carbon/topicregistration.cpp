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

#include "topicregistration.h"
#include "abstractplugin.h"
#include "cutelogger\logger.h"

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

TopicRegistration::TopicRegistration(AbstractPlugin* registree, const char* interfaceName, int maxPartners, 
    CommunicationManager::ETopicConnectionType connectionType) 
    :   mPlugin(registree), 
        mInterfaceName(interfaceName), 
        mMaxConnectionCount(maxPartners),
        mConnectionType(connectionType), 
        mConnectionCount(0),
        mInvalid(false)
{
    if (registree == 0)
        LOG_FATAL() << "Registered NULL to a topic.";

    mObject = registree->getQObject();
    if (mObject == 0)
    {
        LOG_ERROR() << "TopicRegistration has no QObject to send or receive data. Connections wont work";
    }

    mChanged = false;
}

TopicRegistration::~TopicRegistration()
{

}

const AbstractPlugin* TopicRegistration::getPlugin() const
{
    return mPlugin;
}

const QObject* TopicRegistration::getObject() const
{
    return mObject;
}

const char* TopicRegistration::getInterfaceName() const
{
    return mInterfaceName.c_str();
}

CommunicationManager::ETopicConnectionType TopicRegistration::getConnectionType() const
{
    return mConnectionType;
}

int TopicRegistration::getMaxConnectionCount() const
{
    return mMaxConnectionCount;
}

int TopicRegistration::getConnectionCount() const
{
    return mConnectionCount;
}

void TopicRegistration::setConnectionCount(int count)
{
    if (count > mMaxConnectionCount)
    {
        LOG_ERROR() << "Connection count " << count << " bigger than defined maximum " << mMaxConnectionCount << ".";
        return;
    }

    mConnectionCount = count;
}

void TopicRegistration::setInvalid()
{
    mInvalid = true;
}

bool TopicRegistration::isInvalid() const
{
    return mInvalid;
}

bool TopicRegistration::canConnect() const
{
    return mConnectionCount + 1 <= mMaxConnectionCount;
}

bool TopicRegistration::read(const TopicRegistration& obj)
{
    if (mPlugin != obj.mPlugin)
    {
        mChanged = true;
        mPlugin = obj.mPlugin;
    }
    if (mObject != obj.mObject)
    {
        mChanged = true;
        mObject = obj.mObject;
    }
    if (mInterfaceName.compare(obj.mInterfaceName) != 0)
    {
        mChanged = true;
        mInterfaceName = obj.mInterfaceName;
    }
    if (mMaxConnectionCount != obj.mMaxConnectionCount)
    {
        mChanged = true;
        mMaxConnectionCount = obj.mMaxConnectionCount;
    }
    if (mConnectionType != obj.mConnectionType)
    {
        mChanged = true;
        mConnectionType = obj.mConnectionType;
    }
    return mChanged;
}

bool TopicRegistration::hasChanged() const
{
    return mChanged;
}

void TopicRegistration::updated()
{
    mChanged = false;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------
