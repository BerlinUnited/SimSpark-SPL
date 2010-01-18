/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
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
#ifndef OXYGEN_NETMESSAGE_H
#define OXYGEN_NETMESSAGE_H

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
class NetBuffer;

    /** \class NetMessage is responsible for the segmentation of raw
        received network data into meaningful messages, stripping any
        meta message information (e.g. msg length prefixes). It
        further is responsible to prepare a message to be sent over
        the network, i.e. to add any necessary meta information.

        The default NetMessage class implements a simple length
        prefixed protocol, i.e. it adds the payload length to a string
        sent over the network.
     */
class OXYGEN_API NetMessage : public zeitgeist::Leaf
{
public:
    NetMessage();
    virtual ~NetMessage();

    /** prepares a message that is sent to a client, i.e. adds any
        neccessary meta information. The default implementation
        prefixes the message with its total length
    */
    virtual void PrepareToSend(std::string& msg);

    /** extracts a message from a network receive buffer into 'msg',
        i.e. it removes any meta information and returns the first
        complete message. The extracted message must be removed from
        the network buffer. ExtractMessage returns true iff it
        successfully extracted a message. The default implementation
        expects length prefixed strings.
     */
    virtual bool Extract(boost::shared_ptr<NetBuffer> buffer, std::string& msg);
};

DECLARE_CLASS(NetMessage);

} // namespace oxygen

#endif // OXYGEN_NETMESSAGE_H
