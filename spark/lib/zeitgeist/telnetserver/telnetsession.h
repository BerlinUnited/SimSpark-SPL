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

   TelnetSession

        HISTORY:
                20.06.2002 MK
                        - initial version

*/
#ifndef ZEITGEIST_TELNETSESSION_H
#define ZEITGEIST_TELNETSESSION_H

#include <winsock.h>
#include <string>
#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

namespace zeitgeist
{

class ScriptServer;
class TelnetDaemon;

/** TelnetSession is responsible for managing one connection to a
    remote client. It is created by a TelnetDaemon object and runs in
    its own thread context.

    It features basic telnet session setup and option
    negotiation with the client, like the used echo settings.

    TelnetSession display a simple prompt ':', waits for the user to
    enter a command, and executes it using the zeitgeist ScriptServer.

    !TODO!: the implemented ruby commands simply output to
    stdout. Therefore their output is not remotely visible. Steps to do:
    - extend the CoreContext to provide stream or printf semantics
    - alter all the commands to use these output functions
    - implement a CoreContext that forwards the output over the network
 */

class TelnetSession
{
    //
    // functions
    //
public:
    /** constructs a TelnetSession object */
    TelnetSession(TelnetDaemon &daemon);
    virtual ~TelnetSession();

    /** receives and remebers the socket, the local adress and a reference to the ScriptServer */
    void Init(SOCKET clientSocket, sockaddr_in clientAddr, boost::shared_ptr<ScriptServer> scriptServer);

    /** contains the code run inside the thread for this Session,
        called from the boost thread library */
    void operator()();

    /** sends a string over the connected socket, returns true on success */
    bool Send(const std::string& data);

    /** waits for data on the connected socket and interprets
        special characters like backspace and return. It further
        provides an echo mechanism, if the client enables it. The
        function returns true on success with data containing the
        received string */
    */
    bool WaitForData(std::string &data);

    /** closes the associated socket */
    void Terminate();

protected:
private:
    /** processes special telnet control characters like del and enter */
    void ProcessCommand(unsigned char command);

    //
    // members
    //
public:
protected:
private:
    /** the connected client socket */
    SOCKET                      mClientSocket;

    /** the local adress of the socket */
    sockaddr_in         mClientAddr;

    /** true, if the client enabled the echo mechanism */
    bool                        mDoEcho;

    /** a reference to the daemon which created this session */
    TelnetDaemon        &mDaemon;

    /** the core context associated with this session */
    boost::shared_ptr<ScriptServer> mScriptServer;
};

}

#endif // ZEITGEIST_TELNETSESSION_H
