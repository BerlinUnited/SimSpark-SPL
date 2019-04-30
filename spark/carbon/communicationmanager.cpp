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

#include "communicationmanager.h"
#include "carbon.h"
#include "abstractplugin.h"
#include "topic.h"

#ifndef Q_MOC_RUN
#include <boost/make_shared.hpp>
#endif

using namespace boost;

//--------------------------------------------------------------
// Static
//--------------------------------------------------------------

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

CommunicationManager::CommunicationManager(Carbon* parent)
    : mParent(parent)
{
}

CommunicationManager::~CommunicationManager()
{
}

bool CommunicationManager::hasTopic(const QString& name)
{
    return getTopic(name).get() != 0;
}

shared_ptr<Topic> CommunicationManager::getTopic(const QString& name)
{
    for (auto it = mTopics.begin(); it != mTopics.end(); it++)
    {
        if (it->get()->getName().compare(name) == 0)
        {
            return *it;
        }
    }
    return boost::shared_ptr<Topic>();
}

bool CommunicationManager::registerSender(AbstractPlugin* sender, const QString& topicName, const char* signal, int maxReceivers, 
    ETopicConnectionType connectionType)
{
    if (sender == 0)
        return false;

    if (sender->getQObject() == 0)
    {
        LOG_WARNING() << "Cant create communication topic for receiver without being or having an assigned QObject. ";
        return false;
    }

    //Create topic
    shared_ptr<Topic> topic = getTopic(topicName);
    if (topic.get() == 0)
    {
        mTopics.push_back(make_shared<Topic>(this, topicName));
        emit topicCreated(topicName);

        topic = mTopics.back();
    }

    //Register sender
    topic->registerSender(sender, signal, maxReceivers, connectionType);
    return true;
}

bool CommunicationManager::registerReceiver(AbstractPlugin* receiver, const QString& topicName, const char* slot, int maxSenders, 
    ETopicConnectionType connectionType)
{
    if (receiver == 0)
        return false;

    if (receiver->getQObject() == 0)
    {
        LOG_WARNING() << "Cant create communication topic for receiver without being or having an assigned QObject. ";
        return false;
    }

    //Create topic
    shared_ptr<Topic> topic = getTopic(topicName);
    if (topic.get() == 0)
    {
        mTopics.push_back(make_shared<Topic>(this, topicName));
        emit topicCreated(topicName);

        topic = mTopics.back();
    }

    //Register receiver
    topic->registerReceiver(receiver, slot, maxSenders, connectionType);
    return true;
}
    
//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

QList<boost::shared_ptr<Topic>>::Iterator CommunicationManager::removeTopic(Topic* topic)
{
    for (auto it = mTopics.begin(); it != mTopics.end(); it++)
    {
        if (&**it == topic)
        {
            QString name = it->get()->getName();
            auto pos = mTopics.erase(it);
            emit topicRemoved(name);
            return pos;
        }
    }
    return mTopics.end();
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void CommunicationManager::unregisterSender(const AbstractPlugin* sender, const QString& topicName, bool validPtr)
{
    shared_ptr<Topic> topic = getTopic(topicName);
    if (topic.get())
    {
        topic->unregisterSender(sender, validPtr);
    
        if (topic->isEmpty())
            removeTopic(&*topic);
    }
}

void CommunicationManager::unregisterSender(const AbstractPlugin* sender, bool validPtr)
{
    for (auto it = mTopics.begin(); it != mTopics.end();)
    {
        it->get()->unregisterSender(sender, validPtr);
        if (it->get()->isEmpty())
            it = removeTopic(it->get());
        else
            it++;
    }
}

void CommunicationManager::unregisterReceiver(const AbstractPlugin* receiver, const QString& topicName, bool validPtr)
{
    shared_ptr<Topic> topic = getTopic(topicName);
    if (topic.get())
        topic->unregisterReceiver(receiver, validPtr);
    
    if (topic->isEmpty())
        removeTopic(&*topic);
}

void CommunicationManager::unregisterReceiver(const AbstractPlugin* receiver, bool validPtr)
{
    for (auto it = mTopics.begin(); it != mTopics.end();)
    {
        it->get()->unregisterReceiver(receiver, validPtr);
        if (it->get()->isEmpty())
            it = removeTopic(it->get());
        else
            it++;
    }
}

void CommunicationManager::unregister(const AbstractPlugin* registree, const QString& topicName, bool validPtr)
{
    shared_ptr<Topic> topic = getTopic(topicName);
    if (topic.get())
        topic->unregister(registree, validPtr);
    
    if (topic->isEmpty())
        removeTopic(&*topic);
}

void CommunicationManager::unregister(const AbstractPlugin* registree, bool validPtr)
{
    for (auto it = mTopics.begin(); it != mTopics.end();)
    {
        it->get()->unregister(registree, validPtr);
        if (it->get()->isEmpty())
            it = removeTopic(it->get());
        else
            it++;
    }
}
