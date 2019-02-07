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

#include <errno.h>
#include <signal.h>
#include <string>
#include <iostream>
#include <rcssnet/tcpsocket.hpp>
//#include <rcssnet/udpsocket.hpp>
#include <rcssnet/exception.hpp>
#include "behavior.h"
#include <boost/scoped_ptr.hpp>
//#include "soccerbehavior.h"
#include "soccerbotbehavior.h"
//#include "carbehavior.h"
//#include "leggedspherebehavior.h"
//#include "hoap2behavior.h"
#include "naobehavior.h"

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

using namespace rcss::net;
using namespace std;
using namespace boost;

TCPSocket gSocket;
//UDPSocket gSocket;
string gHost = "127.0.0.1";
int gPort = 3100;

// bool to indicate whether to continue the agent mainloop
static bool gLoop = true;

// SIGINT handler prototype
extern "C" void handler(int sig)
{
    if (sig == SIGINT)
        gLoop = false;
}

void PrintGreeting()
{
    cout << "agentspark, a spark demo agent\n"
         << "Copyright (C) 2004 Koblenz University.\n"
         << "2004 RoboCup Soccer Server 3D Maintenance Group.\n\n";
}

void PrintHelp()
{
    cout << "\nusage: agentspark [options]" << endl;
    cout << "\noptions:" << endl;
    cout << " --help      prints this message." << endl;
    cout << " --host=IP   IP of the server." << endl;
    cout << "\n";
}

void ReadOptions(int argc, char* argv[])
{
    for( int i = 0; i < argc; i++)
        {
            if ( strcmp( argv[i], "--help" ) == 0 )
                {
                    PrintHelp();
                    exit(0);
                }
            else if ( strncmp( argv[i], "--host", 6 ) == 0 )
            {
                string tmp=argv[i];
                
                if ( tmp.length() <= 7 ) // minimal sanity check
                {
                    PrintHelp();
                    exit(0);
                }
                gHost = tmp.substr(7);
            }
            
                
        }
}

bool Init()
{
    cout << "connecting to TCP " << gHost << ":" << gPort << "\n";
    //cout << "connecting to UDP " << gHost << ":" << gPort << "\n";

    try
        {
            Addr local(INADDR_ANY,INADDR_ANY);
            gSocket.bind(local);
        }

    catch (BindErr error)
        {
            cerr << "failed to bind socket with '"
                 << error.what() << "'" << endl;

            gSocket.close();
            return false;
        }

    try
        {
            Addr server(gPort,gHost);
            gSocket.connect(server);
        }

    catch (ConnectErr error)
        {
            cerr << "connection failed with: '"
                 << error.what() << "'" << endl;
            gSocket.close();
            return false;
        }

    return true;
}

void Done()
{
    gSocket.close();
    cout << "closed connection to " << gHost << ":" << gPort << "\n";
}

bool SelectInput()
{
    fd_set readfds;
    struct timeval tv = {60,0};
    FD_ZERO(&readfds);
    FD_SET(gSocket.getFD(),&readfds);
    
    while(1) {
        switch(select(gSocket.getFD()+1,&readfds, 0, 0, &tv)) {
        case 1:
            return 1;
        case 0:
            cerr << "(SelectInput) select failed " << strerror(errno) << endl;
            abort();
            return 0;
        default:
            if(errno == EINTR)
                continue;
            cerr << "(SelectInput) select failed " << strerror(errno) << endl;
            abort();
            return 0;
        }
    }
}

void PutMessage(const string& msg)
{
    if (msg.empty())
        {
            return;
        }

    // prefix the message with it's payload length
    unsigned int len = htonl(msg.size());
    string prefix((const char*)&len,sizeof(unsigned int));
    string str = prefix + msg;
    gSocket.send(str.data(),str.size());
}

bool GetMessage(string& msg)
{
    static char buffer[16 * 1024];

    unsigned int bytesRead = 0;
    while(bytesRead < sizeof(unsigned int))
    {
        SelectInput();
        int readResult = gSocket.recv(buffer + bytesRead, sizeof(unsigned int) - bytesRead);
        if(readResult < 0)
            continue;
        bytesRead += readResult;
    }

    //cerr << "buffer = |" << string(buffer+1) << "|\n";
    //cerr << "bytesRead = |" << bytesRead << "|\n";
    //cerr << "Size of buffer = |" << sizeof(buffer) << "|\n";
    //cerr << "buffer = |" << buffer << "|\n";
    //cerr << "buffer[5] = |" << buffer[5] << "|\n";
    //printf ("xxx-%s\n", buffer+5);

    // msg is prefixed with it's total length
    int msgLen = ntohl(*(unsigned int*)buffer);
    // cerr << "GM 6 / " << msgLen << " (bytesRead " << bytesRead << ")\n";
    if(sizeof(unsigned int) + msgLen > sizeof(buffer)) {
        cerr << "message too long; aborting" << endl;
        abort();
    }

    // read remaining message segments
    int msgRead = bytesRead - sizeof(unsigned int);

    //cerr << "msgRead = |" << msgRead << "|\n";

    char *offset = buffer + bytesRead;

    while (msgRead < msgLen)
    {
        if (! SelectInput())
        {
            return false;
        }

        int readLen = sizeof(buffer) - msgRead;
        if(readLen > msgLen - msgRead)
            readLen = msgLen - msgRead;
 
        int readResult = gSocket.recv(offset, readLen);
        if(readResult < 0)
            continue;
        msgRead += readResult;
        offset += readResult;
        //cerr << "msgRead = |" << msgRead << "|\n";
    }

    // zero terminate received data
    (*offset) = 0;

    msg = string(buffer+sizeof(unsigned int));

    // DEBUG
    //cout << msg << endl;

    return true;
}

void Run()
{    
    //scoped_ptr<Behavior> behavior(new SoccerbotBehavior());
    //scoped_ptr<Behavior> behavior(new SoccerBehavior());
    //scoped_ptr<Behavior> behavior(new CarBehavior());
    //scoped_ptr<Behavior> behavior(new LeggedSphereBehavior());
    //scoped_ptr<Behavior> behavior(new Hoap2Behavior());
    scoped_ptr<Behavior> behavior(new NaoBehavior());

    PutMessage(behavior->Init());

    string msg;
    while (gLoop)
        {  
            GetMessage(msg);
            PutMessage(behavior->Think(msg));
        }
}

int
main(int argc, char* argv[])
{
    // registering the handler, catching SIGINT signals 
    signal(SIGINT, handler);

    PrintGreeting();
    ReadOptions(argc,argv);

    if (! Init())
        {
            return 1;
        }

    Run();
    Done();
}
