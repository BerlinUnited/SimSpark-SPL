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
#ifndef OXYGEN_NETCONTROL_H
#define OXYGEN_NETCONTROL_H

#include "simcontrolnode.h"
#include "netbuffer.h"
#include <vector>
#include <rcssnet/socket.hpp>
#ifndef Q_MOC_RUN
#include <boost/shared_array.hpp>
#endif
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
class GameControlServer;
class NetMessage;

/** \class NetControl is a SimControlNode that accepts and manages a
    set of network client connections via UDP or TCP. With each
    simulation cycle it collects all pending client messages in a set
    of network buffers, each corresponding to a client. It furthes
    provides methods to send messages to connected clients.

    The segmentation of the received data into messages and the
    composition of messages is delegated to a NetMessage class that is
    registered to a NetControl node. It is responsible to manage the
    MetaProtocol, i.e. it adds and removes any additional message
    framing and type information.
 */
class OXYGEN_API NetControl : public SimControlNode
{
public:
    enum ESocketType
        {
            ST_TCP,
            ST_UDP
        };

    struct Client
    {
    public:
        int id;
        rcss::net::Addr addr;
        boost::shared_ptr<rcss::net::Socket> socket;

    public:
        Client() : id(-1) {};
        Client(int i, const rcss::net::Addr& a,
               boost::shared_ptr<rcss::net::Socket> s =
               boost::shared_ptr<rcss::net::Socket>()
               ) : id(i), addr(a), socket(s) {};
    };

    // mapping from a client remote address to Client struct
    typedef std::map<rcss::net::Addr, boost::shared_ptr<Client> > TAddrMap;

    // mapping from a client remote address to a corresponding buffer
    typedef std::map<rcss::net::Addr, boost::shared_ptr<NetBuffer> > TBufferMap;

    typedef std::list<rcss::net::Addr> TAddrList;

public:
    NetControl();
    virtual ~NetControl();

    /** creates the managed socket, when the simulation starts */
    virtual void InitSimulation();

    /** called once before the simulation is shut down */
    virtual void DoneSimulation();

    /** called when a new simulation cycle starts, before the
        simulation is stepped */
    virtual void StartCycle();

    /** called when the current simulation cycle ends */
    virtual void EndCycle();

    /** called when a new client connects */
    virtual void ClientConnect(boost::shared_ptr<Client> client);

    /** called when a client disconnects */
    virtual void ClientDisconnect(boost::shared_ptr<Client> client);

    /** sets the local port, on which connections are accepted */
    void SetServerPort(rcss::net::Addr::PortType port);

    /** returns the local port, on which connections are accepted */
    rcss::net::Addr::PortType GetServerPort();

    /** sets the type of the socket on which connections are
        accepted */
    void SetServerType(ESocketType type);

    /** returns the type of the socket on which connections are
        accepted */
    ESocketType GetServerType();

    /** sends a message to the given client */
    void SendClientMessage(boost::shared_ptr<Client> client,
                           const std::string& msg);

    /** sends a message to the client with the given address */
    void SendClientMessage(const rcss::net::Addr& addr,
                           const std::string& msg);

    /** create a socket according to the given ESocketType */
    static boost::shared_ptr<rcss::net::Socket>
    CreateSocket(ESocketType type);

    /** if set true, ReadMessages call will not return immediately if there
     * is no messages to read and it'll wait a little for incomming messages
     */
    void BlockOnReadMessages(bool block);

protected:
    /** returns a human readable description of the socket type and
        port*/
    std::string DescribeSocketType();

    /** checks for and accepts pending TCP connections */
    void AcceptTCPConnections();

    /** reads and stores all available messages */
    void ReadMessages();

    /** reads and stores all available TCP messages */
    void ReadTCPMessages();

    /** reads and stores all available UDP messages. UDP fragments
        from unknown sources generate new client entries
    */
    void ReadUDPMessages();

    /** copies the message fragment with length \param size currently
        in the receive buffer to the input queue of the client with
        the remoted address \param addr
     */
    void StoreFragment(const rcss::net::Addr& addr, int size);

    /** creates a new client entry.
        \param from is the remote adress of the client.
        \param socket gives the loacl socket for a TCP connection the for a TCP connection
    */
    void AddClient(const rcss::net::Addr& from,
                   boost::shared_ptr<rcss::net::Socket> socket =
                   boost::shared_ptr<rcss::net::Socket>());

    /** removes a client entry and closes the associated socket.
        \param from is the remote adress of the client.
    */
    void RemoveClient(const rcss::net::Addr& from);

    /** removes a client entry and closes the associated socket. */
    void RemoveClient(TAddrMap::iterator iter);

    /** removes all clients marked in the mCloseClients list */
    void CloseDeadConnections();

protected:
    /** cached reference to the NetMessage child node */
    boost::shared_ptr<NetMessage> mNetMessage;

    /** the local port, on which connections are accepted */
    rcss::net::Addr mLocalAddr;

    /** the type of created socket */
    ESocketType mSocketType;

    /** the socket used to accept connections */
    boost::shared_ptr<rcss::net::Socket> mSocket;

    /** map of known clients, based on remote address */
    TAddrMap mClients;

    /** list of client connections to be closed */
    TAddrList mCloseClients;

    /** list of queued messages */
    TBufferMap mBuffers;

    /** the size of the allocated receive buffer */
    int mBufferSize;

    /** a buffer to store partial messages to be sent */
    std::vector<std::string> mSendBuffers;

    /** the receive buffer */
    boost::shared_array<char> mBuffer;

    /** the next available unique client id */
    int mClientId;

    /** indicates how much ReadMessages should wait for new messages */
    int mReadTimeout;
};

DECLARE_CLASS(NetControl);

} // namespace oxygen

#endif // OXYGEN_NETCONTROL_H
