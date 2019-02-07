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

#ifndef TOPICREGISTRATION_H
#define TOPICREGISTRATION_H
/**
 * \file topicregistration.h
 * \brief Contains TopicRegistration class.
 */

#include "communicationmanager.h"

class AbstractPlugin;

/*!
 \brief Information object on a registration of a plugin in a Topic.

 Used to specify sender or receiver connection partners in a connection to a communication Topic.
*/
class TopicRegistration
{

public:
    // public functions
    
    /*!
     \brief Complete constructor.

     \param registree registered plugin
     \param interfaceName name of the signal or slot of the plugin used in the connection
     \param maxPartners maximum amount of other objects to connect the signal or slot to
     \param connectionType type of the connection
    */
    TopicRegistration(AbstractPlugin* registree, const char* interfaceName, int maxPartners = INT_MAX,
        CommunicationManager::ETopicConnectionType connectionType = CommunicationManager::TCT_AUTO);
    /*!
     \brief Destructor.
    */
    virtual ~TopicRegistration();
    
    /*!
     \brief Returns the registered plugin pointer.
    */
    const AbstractPlugin* getPlugin() const;
    /*!
     \brief Returns the associated QObject pointer of the plugin.
    */
    const QObject* getObject() const;
    /*!
     \brief Returns the name of the signal or slot of the plugin to use.
    */
    const char* getInterfaceName() const;
    /*!
     \brief Returns the type of connection to use.
    */
    CommunicationManager::ETopicConnectionType getConnectionType() const;
    /*!
     \brief Returns the maximum amount of connections for the signal or slot.
    */
    int getMaxConnectionCount() const;
    /*!
     \brief Returns the current amount of connections for the signal or slot.
    */
    int getConnectionCount() const;
    /*!
     \brief Update the current amount of connections for the signal or slot.

     Used by the Topic.
     \param count new value
    */
    void setConnectionCount(int count);

    /*!
     \brief Returns true if the contained pointer is still valid (can be dereferenced).
    */
    bool isInvalid() const;
    /*!
     \brief Invalidates the registration. Afterwards the contained plugins qobject pointer wont be dereferenced by Topic or CommunicationManager anymore.
    */
    void setInvalid();

    /*!
     \brief Returns true if the registrations connection count is smaller than its maximum connection count.
    */
    bool canConnect() const;

    /*!
     \brief Update values of the registration by copying from another registration.

     \param obj object to copy from
    */
    bool read(const TopicRegistration& obj);
    /*!
     \brief Returns true if the registration has been changed since the last updated() call.
    */
    bool hasChanged() const;
    /*!
     \brief Specifies the registration data to be up to date.
    */
    void updated();

private:
    // private members

    AbstractPlugin* mPlugin;    /*!< Plugin pointer. */
    QObject* mObject;           /*!< QObject pointer to connect with. */
    std::string mInterfaceName; /*!< Name of signal or slot to connect with. */
    CommunicationManager::ETopicConnectionType mConnectionType; /*!< Type of connection to use. */
    int mMaxConnectionCount;    /*!< Maximum amount of connections for this registration. */
    int mConnectionCount;       /*!< Current amount of connections for this registration. */
    bool mInvalid;              /*!< True after setInvalid(). Plugin and QObject pointer wont be dereferenced anymore. */

    bool mChanged;              /*!< True after data was changed, until updated() call. */
};

#endif //TOPICREGISTRATION_H
