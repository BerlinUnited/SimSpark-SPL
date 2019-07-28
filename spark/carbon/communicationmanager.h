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

#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H
/**
 * \file communicationmanager.h
 * \brief Contains CommunicationManager class.
 */

#include <QObject>
#include <QStringList>
#include <QList>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

class Topic;
class AbstractPlugin;
class Carbon;

/*!
 \brief Class that controls message Topics and connects and manages individual communication participants.

 Plugins can request and offer communication with other plugins either in a one-way manner (message sender & receiver groups) or in a two way manner
 (message server and client groups).
 Communication is achieved using signals and slots and it is managed in topics identified by a name string.
 For example, a plugin with signal "theSignal()" and slot "theSlot()" can offer "theSignal" using registerSender() or registerServer() and it can offer
 "theSlot()" using "registerReceiver()" or "registerClient()".\n

 Whenever a signal or slot is registered to the CommunicationManager, the manager tries to add it to the Topic.
 The Topic will then thy to connect all the signals in the topic with all the slots.
 You can also add signals as slots to automatically forward a signal in a topic, but be careful not to create an endless signal loop.\n

 Sender-Receiver topics are simple in that manner, that every signal emitted by any sender in the topic will call all the slots in the topic.

 \todo Server-Client communication
*/
class CommunicationManager : public QObject
{
  Q_OBJECT

public:
    // public types

    /*!
     \brief Type of communication connection.
    */
    enum ETopicConnectionType
    {
        TCT_AUTO, /*!< Qt::AutoConnection */
        TCT_BLOCK /*!< Qt::BlockingConnection */
    };

public:
    // public functions

    /*!
     \brief Constructor.

     \param parent parent carbon instance
    */
    CommunicationManager(Carbon* parent);
    /*!
     \brief Destructor.
    */
    virtual ~CommunicationManager();

    /*!
     \brief Returns true if there is a topic with the given name.

     Topics are deleted if there is no receiver or sender with the specified topic anymore.
     \param name name of the topic
     \return true if the topic exists
    */
    bool hasTopic(const QString& name);
    /*!
     \brief Returns the topic with the given name or null pointer if topic does not exist.
    */
    boost::shared_ptr<Topic> getTopic(const QString& name);

    /*!
     \brief Register a sender to a topic.

     Creates the topic if it does not exist yet.
     \param sender sender to connect to topic
     \param topic name of the topic
     \param signal signal of the sender
     \param maxReceivers maximum amount of receivers for connect with the sender
     \param connectionType connection type
    */
    bool registerSender(AbstractPlugin* sender, const QString& topic, const char* signal, int maxReceivers = INT_MAX,
        ETopicConnectionType connectionType = TCT_AUTO);
    /*!
     \brief Register a receiver to a topic.

     Creates the topic if it does not exist yet.
     \param receiver receiver to connect to topic
     \param topic name of the topic
     \param slot slot of the receiver
     \param maxSenders maximum amount of senders to connect with the receiver
     \param connectionType connection type
    */
    bool registerReceiver(AbstractPlugin* receiver, const QString& topic, const char* slot, int maxSenders = INT_MAX,
        ETopicConnectionType connectionType = TCT_AUTO);

protected:
    // protected functions

    /*!
     \brief Removes a topic.

     \param topic topic to remove from list
     \return next position after removed topic (check if it is end of list)
    */
    QList<boost::shared_ptr<Topic>>::Iterator removeTopic(Topic* topic);

public: signals:
    //public signals

    /*!
     \brief Emitted when a topic was created.

     \param name name of the topic
    */
    void topicCreated(QString name);
    /*!
     \brief Emitted when a topic was removed.

     Topics are removed when there are no senders and no receivers anymore.
     \param name name of the topic
    */
    void topicRemoved(QString name);

public slots:	
    //public slots

    /*!
     \brief Unregister a sender from a topic.

     \warning if unregistering in destructor the pointer to the plugin is not valid anymore and the flag validPtr has to be set to false!
     \param sender sender to unregister.
     \param topic topic to unregister from
     \param validPtr use false if unregistering in plugins destructor!
    */
    void unregisterSender(const AbstractPlugin* sender, const QString& topic, bool validPtr = true);
    /*!
     \brief Unregister a sender from all topics.

     \warning if unregistering in destructor the pointer to the plugin is not valid anymore and the flag validPtr has to be set to false!
     \param sender sender to unregister
     \param validPtr use false if unregistering in plugins destructor!
    */
    void unregisterSender(const AbstractPlugin* sender, bool validPtr = true);
    /*!
     \brief Unregister a receiver from a topic.

     \warning if unregistering in destructor the pointer to the plugin is not valid anymore and the flag validPtr has to be set to false!
     \param receiver receiver to unregister
     \param topic topic to unregister from
     \param validPtr use false if unregistering in plugins destructor!
    */
    void unregisterReceiver(const AbstractPlugin* receiver, const QString& topic, bool validPtr = true);
    /*!
     \brief Unregister a receiver from all topics.

     \warning if unregistering in destructor the pointer to the plugin is not valid anymore and the flag validPtr has to be set to false!
     \param receiver receiver to unregister
     \param validPtr use false if unregistering in plugins destructor!
    */
    void unregisterReceiver(const AbstractPlugin* receiver, bool validPtr = true);
    /*!
     \brief Unregister a plugin as sender and as receiver from a topic.

     \warning if unregistering in destructor the pointer to the plugin is not valid anymore and the flag validPtr has to be set to false!
     \param registree receiver and/or sender to unregister
     \param topic topic to unregister from
     \param validPtr use false if unregistering in plugins destructor!
    */
    void unregister(const AbstractPlugin* registree, const QString& topic, bool validPtr = true);
    /*!
     \brief Unregister a plugin as sender and as receiver from all topics.

     \warning if unregistering in destructor the pointer to the plugin is not valid anymore and the flag validPtr has to be set to false!
     \param registree receiver and/or sender to unregister
     \param validPtr use false if unregistering in plugins destructor!
    */
    void unregister(const AbstractPlugin* registree, bool validPtr = true);
    
private:
    // private members

    Carbon* mParent; /*!< Parent carbon instance. */
    QList<boost::shared_ptr<Topic>> mTopics;  /*!< List of topics. */
};

#endif //COMMUNICATIONMANAGER_H
