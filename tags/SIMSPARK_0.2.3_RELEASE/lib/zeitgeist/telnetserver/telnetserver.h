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

   TelnetServer

        :TODO: Class description for TelnetServer

        HISTORY:
                20.06.2002 MK
                        - initial version

                28.06.2003 MR
                        - modified for inclusion into the zeitgeist library
*/
#ifndef ZEITGEIST_TELNETSERVER_H
#define ZEITGEIST_TELNETSERVER_H

#ifdef _WIN32

// the socket functions require the ws2_32 library
#pragma comment(lib, "ws2_32.lib")

// boost threads are used for thread creation
#pragma comment(lib, "libboost_thread.lib")

#endif

#include <zeitgeist/node.h>
#include <string>

namespace zeitgeist
{

class TelnetDaemon;

/** The TelnetServer provides remote access to the Zeitgeist framework
    using a telnet connection. The user can interact with the system
    using ruby statements.

    The TelnetServer starts a TelnetDaemon wich listens for incoming
    connections and creates a new TelnetSession object for each new
    connection. Each TelnetSession object runs in its own thread
    context.
*/
class TelnetServer : public Node
{
    friend class TelnetDaemon;

    //
    // functions
    //
public:
    /** constructs the TelnetServer and starts the TelnetDaemons,
        listening on the specified port, default 23 */
    TelnetServer(unsigned int port = 23);

    virtual ~TelnetServer();

    /** prints the current status to stdout */
    void Status();

    /** returns the port number the daemon listens on */
    unsigned int GetPort() const                    {       return mPort;   };

    /** returns the hostname of this machine */
    const std::string& GetHostName() const  {       return mHostName;       }

private:
    TelnetServer(const TelnetServer& obj);
    TelnetServer& operator=(const TelnetServer& obj);

    /** called by the daemon to tell its instance */
    void SetDaemon(TelnetDaemon *daemon)    {       mDaemon = daemon;       }

    /** called by the daemon to get access to the scriptserver */
    boost::shared_ptr<ScriptServer> GetScriptServer()
    {       return GetScript();     }

    /** starts the telnetserver, creating the daemon */
    bool Start();

    /** shuts the telnetserver down, returns true on clean exit */
    bool Shutdown();

    //
    // members
    //
protected:
    /** the port number the daemons listens on */
    unsigned int    mPort;

    /** the instance of then daemon */
    TelnetDaemon    *mDaemon;

    /** the hostname of this machine */
    std::string             mHostName;
};

    DECLARE_CLASS(TelnetServer);

}

#endif //ZEITGEIST_TELNETSERVER_H
