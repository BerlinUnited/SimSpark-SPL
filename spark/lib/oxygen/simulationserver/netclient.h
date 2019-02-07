/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef SPARK_NETCLIENT_H
#define SPARK_NETCLIENT_H

#include <zeitgeist/class.h>
#include <oxygen/oxygen_defines.h>
#include <oxygen/simulationserver/simcontrolnode.h>
#include <oxygen/simulationserver/netcontrol.h>
#include <oxygen/simulationserver/netmessage.h>
#include <oxygen/simulationserver/netbuffer.h>
#include <rcssnet/tcpsocket.hpp>
#include <rcssnet/socketstreambuf.hpp>

namespace oxygen
{

class OXYGEN_API NetClient : public oxygen::SimControlNode
{
public:
    NetClient();
    virtual ~NetClient();

    /** sets the server to connect to */
    void SetServer(const std::string& host);

    /** returns the server this client connects to */
    const std::string& GetServer() const;

    /** sets the port to connect to */
    void SetPort(int port);

    /** returns the port this client connects to */
    int GetPort() const;

    /** sets the client socket type */
    void SetClientType(oxygen::NetControl::ESocketType type);

    /** returns the client socket type */
    oxygen::NetControl::ESocketType GetClientType();

    /** prepares the message with the registered NetMessage instance
        and sends it to the server */
    void SendMessage(const std::string& msg);

protected:
    /** tries to connect to the server, returns true on success */
    bool Connect();

    /** reads all pending message fragments from the server and stores
        them in the mNetBuffer
    */
    void ReadFragments();

    /** closes connection to the server*/
    void CloseConnection();

    /** parses a received message */
    void ParseMessage(const std::string& msg);

protected:
    /** the host name of the server */
    std::string mHost;

    /** the monitor port to connect to */
    int mPort;

    /** the socket type to create */
    oxygen::NetControl::ESocketType mType;

    /** the managed TCP socket */
    boost::shared_ptr<rcss::net::Socket> mSocket;

    /** the receive buffer */
    boost::shared_ptr<oxygen::NetBuffer> mNetBuffer;

    /** the registered message protocol */
    boost::shared_ptr<oxygen::NetMessage> mNetMessage;

    /** the size of the allocated receive buffer */
    int mBufferSize;

    /** the receive buffer */
    boost::shared_array<char> mBuffer;
};

DECLARE_CLASS(NetClient);

} // namespace oxygen

#endif // NETCLIENT_H
