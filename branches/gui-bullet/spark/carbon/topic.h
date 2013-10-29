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

#ifndef TOPIC_H
#define TOPIC_H
/**
 * \file topic.h
 * \brief Contains Topic class.
 */

#include "communicationmanager.h"

#include <QObject>
#include <QList>
#include <QPair>

class TopicRegistration;

/*!
 \brief Communication Topic between plugins. Created and managed by the CommunicationManager.

 A Topic contains a list of senders and a list of receivers.
 Whenever a sender or receiver is added to the Topic, the Topic tries to connect all the senders with all the receivers in the topic,
 depending on their specifications of maximum communication partners and using their specified signals and slots.

 If the signals cant be connected to the slots because of a function signature that does not match, Qt will output warning log messages.
 Connections can always be downgrades (connecting signals with more parameters to slots with less parameters) as long as the parameters that are
 present can explicitly converted from the signal signature to the slot signature.
*/
class Topic : public QObject
{
  Q_OBJECT

public:
    // public functions

    /*!
     \brief Constructor.

     \param parent parent communication manager
     \param name name of the topic
    */
    Topic(CommunicationManager* parent, const QString& name);
    /*!
     \brief Destructor.
    */
    virtual ~Topic();

    /*!
     \brief Returns true if the specified plugin is registered as receiver.

     \param registree plugin to search for
    */
    bool hasReceiver(AbstractPlugin* registree) const;
    /*!
     \brief Returns true if the specified plugin is registered as sender.

     \param registree plugin to search for
    */
    bool hasSender(AbstractPlugin* registree) const;
    /*!
     \brief Returns true if the specified plugin is registered as sender or receiver.

     \param registree plugin to search for
    */
    bool hasRegistree(AbstractPlugin* registree) const;

    /*!
     \brief Returns the name of the topic.
    */
    const QString& getName() const;
    /*!
     \brief Returns the amount of registered receivers.
    */
    int getReceiverCount() const;
    /*!
     \brief Returns the amount of registered senders.
    */
    int getSenderCount() const;
    /*!
     \brief Returns true if there are neither receivers nor senders registered.

     Results in deletion by CommunicationManager.
    */
    bool isEmpty() const;

    /*!
     \brief Register a sender to the topic.

     Will re-initialize connections between all senders and receivers.
     \param sender new sender to register
     \param signal signal to connect with receivers
     \param maxReceivers maximum amount of receivers to connect the sender with
     \param connectionType type of connection
     \return true if registration succeeded
    */
    bool registerSender(AbstractPlugin* sender, const char* signal, int maxReceivers = INT_MAX,
        CommunicationManager::ETopicConnectionType connectionType = CommunicationManager::TCT_AUTO);
    /*!
     \brief Register a receiver to the topic.

     Will re-initialize connections between all senders and receivers.
     \param receiver new receiver to register
     \param slot slot to connect with senders
     \param maxSenders maximum amount of senders to connect receiver with
     \param connectionType type of connection
     \return true if registration succeeded
    */
    bool registerReceiver(AbstractPlugin* receiver, const char* slot, int maxSenders = INT_MAX,
        CommunicationManager::ETopicConnectionType connectionType = CommunicationManager::TCT_AUTO);

    /*!
     \brief Unregister a sender from the topic.

     \warning if unregistering in destructor the pointer to the plugin is not valid anymore and the flag validPtr has to be set to false!
     \param sender sender plugin to remove
     \param validPtr if false, will invalidate the topic registree, preventing the pointer to be used again (use false if unregistering in plugins destructor!nNeeded in case of deregistration in plugin destructor)
     \return true if deregistration succeeded
    */
    bool unregisterSender(const AbstractPlugin* sender, bool validPtr = true);
    /*!
     \brief Unregister a receiver from the topic.

     \warning if unregistering in destructor the pointer to the plugin is not valid anymore and the flag validPtr has to be set to false!
     \param receiver receiver plugin to remove
     \param validPtr if false, will invalidate the topic registree, preventing the pointer to be used again (use false if unregistering in plugins destructor!nNeeded in case of deregistration in plugin destructor)
     \return true if deregistration succeeded
    */
    bool unregisterReceiver(const AbstractPlugin* receiver, bool validPtr = true);
    /*!
     \brief Unregister a plugin as receiver and as sender from the topic.

     \warning if unregistering in destructor the pointer to the plugin is not valid anymore and the flag validPtr has to be set to false!
     \param registree sender/receiver plugin to remove
     \param validPtr if false, will invalidate the topic registree, preventing the pointer to be used again (use false if unregistering in plugins destructor!nNeeded in case of deregistration in plugin destructor)
     \return true if deregistration succeeded
    */
    bool unregister(const AbstractPlugin* registree, bool validPtr = true);

protected:
    // protected functions

    /*!
     \brief Unregister a plugin as receiver and as sender from the topic.

     Called after plugin deletion (as QObject).
     \param registree sender/receiver plugin to remove
     \param validPtr if false, will invalidate the topic registree, preventing the pointer to be used again (use false if unregistering in plugins destructor!nNeeded in case of deregistration in plugin destructor)
     \return true if deregistration succeeded
    */
    bool unregister(QObject* registree, bool validPtr = true);

private:
    // private functions

    /*!
     \brief Returns the topic shared pointer, stored in the parent CommunicationManager.
    */
    boost::shared_ptr<Topic> getSelf() const;

    /*!
     \brief Try to connect all senders with all receivers in the topic.

     Senders and receivers are only connected if their maximum number of connections wont be succeeded.
     Existing connections will be kept if possible.
    */
    void updateConnections();
    /*!
     \brief Remove all connections for a receiver topic registration.

     \param registration registration to remove connections of
    */
    void removeReceiverConnections(TopicRegistration* registration);
    /*!
     \brief Remove all connections of a sender topic registration.

     \param registration registration to remove connections of
    */
    void removeSenderConnections(TopicRegistration* registration);
    /*!
     \brief Remove all connections of a sender and/or receiver topic registration.

     \param registration registree to remove connections of
     \param send search registree as sender
     \param receive search registree as receiver
    */
    void removeConnections(TopicRegistration* registration, bool send, bool receive);

    /*!
     \brief Return a topic registry entry for a sender plugin.

     \param sender plugin to search for
     \param index storage for index of the registree
     \return pointer to registree or null pointer if nothing was found
    */
    boost::shared_ptr<TopicRegistration> findSender(const AbstractPlugin* sender, int& index) const;
    /*!
     \brief Return a topic registry entry for a sender plugin.

     \param sender plugin to search for
     \return pointer to registree or null pointer if nothing was found
    */
    boost::shared_ptr<TopicRegistration> findSender(const AbstractPlugin* sender) const;
    /*!
     \brief Return a topic registry entry for a receiver plugin.

     \param receiver plugin to search for
     \param index storage for index of the registree
     \return pointer to registree or null pointer if nothing was found
    */
    boost::shared_ptr<TopicRegistration> findReceiver(const AbstractPlugin* receiver, int& index) const;
    /*!
     \brief Return a topic registry entry for a receiver plugin.

     \param receiver plugin to search for
     \return pointer to registree or null pointer if nothing was found
    */
    boost::shared_ptr<TopicRegistration> findReceiver(const AbstractPlugin* receiver) const;

public: signals:
    //public signals

    /*!
     \brief Emitted when a sender was registered in the topic.

     \param sender registered sender
    */
    void registeredSender(AbstractPlugin* sender);
    /*!
     \brief Emitted when a receiver was registered in the topic.

     \param receiver registered receiver
    */
    void registeredReceiver(AbstractPlugin* receiver);

    /*!
     \brief Emitted when a sender was unregistered.

     \param sender unregistered sender
     \param validPtr true if the pointer still can be dereferenced
    */
    void unregisteredSender(const AbstractPlugin* sender, bool validPtr = true);
    /*!
     \brief Emitted when a receiver was unregistered.

     \param receiver unregistered receiver
     \param validPtr true if the pointer still can be dereferenced
    */
    void unregisteredReceiver(const AbstractPlugin* receiver, bool validPtr = true);

    /*!
     \brief Emitted when a connection was established by the Topic.

     \param sender sender of the connection
     \param receiver receiver of the connection
     \param topic pointer to this Topic
    */
    void connected(const AbstractPlugin* sender, const AbstractPlugin* receiver, boost::shared_ptr<Topic> topic);
    /*!
     \brief Emitted when a connection that was established by this topic was removed.

     \param sender sender of the connection
     \param receiver receiver of the connection
     \param topic pointer to this Topic
    */
    void disconnected(const AbstractPlugin* sender, const AbstractPlugin* receiver, boost::shared_ptr<Topic> topic);

public slots:	
    //public slots

    /*!
     \brief Object removal notification. Removes all connections of this object as sender or receiver from the Topic.

     At this point the derived class is not existant anymore.
     All receivers of the signals emitted by this function and following functions must not dereference the registree pointer anymore.
     \param registree registree that was deleted without deregistration
    */
    void updateObjectDeleted(QObject* registree);
    
private:
    // private members
    CommunicationManager* mManager; /*!< Parent CommunicationManager. */

    QString mName; /*!< Name of the Topic. */
    QList<boost::shared_ptr<TopicRegistration>> mSenders; /*!< List of senders. */
    QList<boost::shared_ptr<TopicRegistration>> mReceivers; /*!< List of receivers. */
    QList<QPair<TopicRegistration*, TopicRegistration*>> mConnections; /*!< List of connections between senders and receivers. */
};

#endif //TOPIC_H
