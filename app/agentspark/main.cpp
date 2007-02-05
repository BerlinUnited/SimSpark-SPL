/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: main.cpp,v 1.8 2007/02/05 06:51:01 jboedeck Exp $

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

#include <string>
#include <iostream>
#include <rcssnet/tcpsocket.hpp>
#include <rcssnet/exception.hpp>
#include <netinet/in.h>
#include <behavior.h>
#include <boost/scoped_ptr.hpp>
//#include "soccerbehavior.h"
#include "soccerbotbehavior.h"
//#include "carbehavior.h"
//#include "leggedspherebehavior.h"
//#include "hoap2behavior.h"


using namespace rcss::net;
using namespace std;
using namespace boost;

TCPSocket gSocket;
string gHost = "127.0.0.1";
int gPort = 3100;

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
    FD_ZERO(&readfds);
    FD_SET(gSocket.getFD(),&readfds);

    return select(gSocket.getFD()+1,&readfds, 0, 0, 0) > 0;
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
    write(gSocket.getFD(), str.data(), str.size());
}

bool GetMessage(string& msg)
{
    // try to read the first message segment
    if (! SelectInput())
    {
        return false;
    }

    static char buffer[16 * 1024];

    unsigned int bytesRead = read(gSocket.getFD(), buffer, sizeof(buffer));
    cerr << "buffer = |" << string(buffer+1) << "|\n";
    cerr << "bytesRead = |" << bytesRead << "|\n";
    cerr << "Size of buffer = |" << sizeof(buffer) << "|\n";
    cerr << "buffer = |" << buffer << "|\n";
    cerr << "buffer[5] = |" << buffer[5] << "|\n";
    printf ("xxx-%s\n", buffer+5);
    
    if (bytesRead < sizeof(unsigned int))
    {
        return false;
    }

    // msg is prefixed with it's total length
    unsigned int msgLen = ntohl(*(unsigned int*)buffer);
    cerr << "GM 6 / " << msgLen << "\n";

    // read remaining message segments
    unsigned int msgRead = bytesRead - sizeof(unsigned int);

    cerr << "msgRead = |" << msgRead << "|\n";

    char *offset = buffer + bytesRead;

    while (msgRead < msgLen)
    {
        if (! SelectInput())
        {
            return false;
        }

        msgRead += read(gSocket.getFD(), offset, sizeof(buffer) - msgRead);
        cerr << "msgRead = |" << msgRead << "|\n";
        offset += msgRead;
    }

    // zero terminate received data
    (*offset) = 0;

    msg = string(buffer+sizeof(unsigned int));

    //cerr << msg << endl;

    return true;
}

void Run()
{    
    scoped_ptr<Behavior> behavior(new SoccerbotBehavior());
    //scoped_ptr<Behavior> behavior(new SoccerBehavior());
    //scoped_ptr<Behavior> behavior(new CarBehavior());
    //scoped_ptr<Behavior> behavior(new LeggedSphereBehavior());
    //scoped_ptr<Behavior> behavior(new Hoap2Behavior());

    PutMessage(behavior->Init());

    string msg;
    while (GetMessage(msg))
        {
            PutMessage(behavior->Think(msg));
        }
}

int
main(int argc, char* argv[])
{
    PrintGreeting();
    ReadOptions(argc,argv);

    if (! Init())
        {
            return 1;
        }

    Run();
    Done();
}
