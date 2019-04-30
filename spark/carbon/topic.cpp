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

#include "topic.h"
#include "topicregistration.h"

#ifndef Q_MOC_RUN
#include <boost/make_shared.hpp>
#endif

using namespace boost;

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

Topic::Topic(CommunicationManager* manager, const QString& name)
    : mName(name), mManager(manager)
{
}

Topic::~Topic()
{
    while (mSenders.size() > 0)
    {
        unregisterSender(mSenders.front()->getPlugin());
    }
    while (mReceivers.size() > 0)
    {
        unregisterReceiver(mReceivers.front()->getPlugin());
    }
}

const QString& Topic::getName() const
{
    return mName;
}

bool Topic::hasReceiver(AbstractPlugin* registree) const
{
    return findReceiver(registree).get() != 0;
}

bool Topic::hasSender(AbstractPlugin* registree) const
{
    return findSender(registree).get() != 0;
}

bool Topic::hasRegistree(AbstractPlugin* registree) const
{
    return hasSender(registree) || hasReceiver(registree);
}


int Topic::getReceiverCount() const
{
    return mReceivers.size();
}

int Topic::getSenderCount() const
{
    return mSenders.size();
}

bool Topic::isEmpty() const
{
    return (mSenders.size() == 0 && mReceivers.size() == 0);
}

bool Topic::registerSender(AbstractPlugin* sender, const char* signal, int maxReceivers, 
    CommunicationManager::ETopicConnectionType connectionType)
{
    bool changed = false;
    shared_ptr<TopicRegistration> entry = findSender(sender);
    shared_ptr<TopicRegistration> newEntry = make_shared<TopicRegistration>(sender, signal, maxReceivers, connectionType);
    if (!entry.get())
    {
        mSenders.push_back(newEntry);
        entry = newEntry;
        changed = true;
    }
    else
    {
        changed = entry->read(*newEntry);
    }

    if (changed)
    {
        emit registeredSender(sender);
        updateConnections();

        //connect deregistration on destruction
        connect(entry->getObject(), SIGNAL(destroyed(QObject*)), this, SLOT(updateObjectDeleted(QObject*)));
    }
    return changed;
}

bool Topic::registerReceiver(AbstractPlugin* receiver, const char* slot, int maxSenders, 
    CommunicationManager::ETopicConnectionType connectionType)
{
    bool changed = false;
    shared_ptr<TopicRegistration> entry = findReceiver(receiver);
    shared_ptr<TopicRegistration> newEntry = make_shared<TopicRegistration>(receiver, slot, maxSenders, connectionType);
    if (!entry.get())
    {
        mReceivers.push_back(newEntry);
        entry = newEntry;
        changed = true;
    }
    else
    {
        changed = entry->read(*newEntry);
    }

    if (changed)
    {
        emit registeredReceiver(receiver);
        updateConnections();
    }
    return changed;
}

bool Topic::unregisterSender(const AbstractPlugin* sender, bool validPtr)
{
    int index = -1; 
    shared_ptr<TopicRegistration> entry = findSender(sender, index);
    if (entry.get() == 0)
        return false;
    if (!validPtr)
        entry->setInvalid();

    removeSenderConnections(&*entry);
    mSenders.removeAt(index);
    emit unregisteredSender(entry->getPlugin());
    return true;
}

bool Topic::unregisterReceiver(const AbstractPlugin* receiver, bool validPtr)
{
    int index = -1; 
    shared_ptr<TopicRegistration> entry = findReceiver(receiver, index);
    if (entry.get() == 0)
        return false;
    if (!validPtr)
        entry->setInvalid();

    removeReceiverConnections(&*entry);
    mReceivers.removeAt(index);
    emit unregisteredReceiver(entry->getPlugin());
    return true;
}

bool Topic::unregister(const AbstractPlugin* registree, bool validPtr)
{
    bool unregistered = unregisterSender(registree, validPtr);
    unregistered = unregisterReceiver(registree, validPtr) ? true : unregistered;
    return unregistered;
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

bool Topic::unregister(QObject* object, bool validPtr)
{
    bool unregistered = false;
    for (auto it = mSenders.begin(); it != mSenders.end(); it++)
    {
        if (it->get()->getObject() == object)
        {
            unregistered = unregisterSender(it->get()->getPlugin(), validPtr);
            break;
        }
    }
    for (auto it = mSenders.begin(); it != mSenders.end(); it++)
    {
        if (it->get()->getObject() == object)
        {
            unregistered = unregisterSender(it->get()->getPlugin(), validPtr) ? true : unregistered;
            break;
        }
    }
    return unregistered;
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void Topic::updateConnections()
{
    //Remove changed connections
    for (auto it = mSenders.begin(); it != mSenders.end(); it++)
    {
        if (it->get()->hasChanged())
        {
            removeSenderConnections(&**it);
            it->get()->updated();
        }
    }
    for (auto it = mReceivers.begin(); it != mReceivers.end(); it++)
    {
        if (it->get()->hasChanged())
        {
            removeReceiverConnections(&**it);
            it->get()->updated();
        }
    }

    // Connect all senders with receivers
    for (auto it = mSenders.begin(); it != mSenders.end(); it++)
    {
        //Max connections for sender reached?
        if (!it->get()->canConnect())
            continue;

        //Sender invalid?
        if (it->get()->isInvalid())
            continue;

        for (auto it_rec = mReceivers.begin(); it_rec != mReceivers.end(); it_rec++)
        {
            //Max connections for receiver reached?
            if (!it_rec->get()->canConnect())
                continue;

            //Receiver invalid?
            if (it_rec->get()->isInvalid())
                continue;

            //Connect
            TopicRegistration* sender = &**it;
            TopicRegistration* receiver = &**it_rec;

            mConnections.push_back(QPair<TopicRegistration*, TopicRegistration*>(sender, receiver));
            sender->setConnectionCount(sender->getConnectionCount() + 1);
            receiver->setConnectionCount(receiver->getConnectionCount() + 1);

            Qt::ConnectionType qtype;
            CommunicationManager::ETopicConnectionType biggerType = 
                (sender->getConnectionType() > receiver->getConnectionType() ? 
                sender->getConnectionType() : receiver->getConnectionType());
            switch(biggerType)
            {
            case CommunicationManager::TCT_AUTO: //Always automatic connection
                qtype = Qt::AutoConnection;
                break;
            case CommunicationManager::TCT_BLOCK: //Always blocking connection
                qtype = Qt::BlockingQueuedConnection;
            }
            bool ok = connect(sender->getObject(), sender->getInterfaceName(), receiver->getObject(), receiver->getInterfaceName());
            if (ok)
            {
                emit connected(sender->getPlugin(), receiver->getPlugin(), getSelf());
            }
            else
            {
                //revert
                mConnections.pop_back();
                sender->setConnectionCount(sender->getConnectionCount() - 1);
                receiver->setConnectionCount(receiver->getConnectionCount() - 1);
            }
        }
    }
}

void Topic::removeReceiverConnections(TopicRegistration* registration)
{
    removeConnections(registration, false, true);
}

void Topic::removeSenderConnections(TopicRegistration* registration)
{
    removeConnections(registration, true, false);
}


void Topic::removeConnections(TopicRegistration* registration, bool send, bool receive)
{
    for (auto it = mConnections.begin(); it != mConnections.end();)
    {
        if ((send && it->first == registration) || (receive && it->second == registration))
        {
            //Disconnect
            TopicRegistration* sender = it->first;
            TopicRegistration* receiver = it->second;
            it = mConnections.erase(it);
            sender->setConnectionCount(sender->getConnectionCount() - 1);
            receiver->setConnectionCount(receiver->getConnectionCount() - 1);
            
            //disconnect manually if both objects still exist (otherwise this already happened)
            if (!sender->isInvalid() && !receiver->isInvalid())
                disconnect(sender->getObject(), sender->getInterfaceName(), receiver->getObject(), receiver->getInterfaceName()); 

            emit disconnected(receiver->getPlugin(), receiver->getPlugin(), getSelf());
        }
    }
}

shared_ptr<TopicRegistration> Topic::findSender(const AbstractPlugin* sender) const
{
    int i = 0;
    return findSender(sender, i);
}

shared_ptr<TopicRegistration> Topic::findSender(const AbstractPlugin* sender, int& index) const
{
    int i = 0;
    for (auto it = mSenders.begin(); it != mSenders.end(); it++)
    {
        if (it->get()->getPlugin() == sender)
        {
            index = i;
            return *it;
        }
        i++;
    }
    return shared_ptr<TopicRegistration>();
}

shared_ptr<TopicRegistration> Topic::findReceiver(const AbstractPlugin* receiver) const
{
    int i = 0;
    return findReceiver(receiver, i);
}

shared_ptr<TopicRegistration> Topic::findReceiver(const AbstractPlugin* receiver, int& index) const
{
    int i = 0;
    for (auto it = mReceivers.begin(); it != mReceivers.end(); it++)
    {
        if (it->get()->getPlugin() == receiver)
        {
            index = i;
            return *it;
        }
        i++;
    }
    return shared_ptr<TopicRegistration>();
}

shared_ptr<Topic> Topic::getSelf() const
{
    return mManager->getTopic(mName);
}

//--------------------------------------------------------------
// Slots
//--------------------------------------------------------------

void Topic::updateObjectDeleted(QObject* registree)
{
    unregister(registree, false);
}
