/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: sparkmonitorclient.cpp,v 1.2 2004/04/29 12:32:29 rollmark Exp $

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
#include "sparkmonitorclient.h"
#include <zeitgeist/logserver/logserver.h>
#include <netinet/in.h>
#include <rcssnet/exception.hpp>

using namespace oxygen;
using namespace zeitgeist;
using namespace rcss::net;
using namespace salt;
using namespace boost;
using namespace std;

SparkMonitorClient::SparkMonitorClient() :
    SimControlNode()

{
    mHost = "127.0.0.1";
    mPort = 3200;
    mBufferSize = 64 * 1024;
    mBuffer = shared_array<char>(new char[mBufferSize]);
    mType = NetControl::ST_TCP;
    mNetBuffer = shared_ptr<NetBuffer>(new NetBuffer());
}

SparkMonitorClient::~SparkMonitorClient()
{
}

void SparkMonitorClient::OnLink()
{
    // setup SceneServer reference
    mSceneServer = shared_dynamic_cast<SceneServer>
        (GetCore()->Get("/sys/server/scene"));

    if (mSceneServer.get() == 0)
        {
            GetLog()->Error()
                << "(SparkMonitor) ERROR: SceneServer not found\n";
        }
}

void SparkMonitorClient::OnUnlink()
{
    mSceneServer.reset();
    if (mManagedScene.get() != 0)
        {
            mManagedScene->UnlinkChildren();
            mManagedScene.reset();
        }
}

void SparkMonitorClient::InitSimulation()
{
    mSocket = NetControl::CreateSocket(mType);

    if (mSocket.get() == 0)
        {
            return;
        }

    GetLog()->Normal()
        << "(SparkMonitorClient) connecting to "
        << mHost << ":" << mPort << "\n";

    try
        {
            Addr local(INADDR_ANY,INADDR_ANY);
            mSocket->bind(local);
        }

    catch (BindErr error)
        {
            GetLog()->Error()
                << "(SparkMonitorClient) failed to bind socket with '"
                << error.what() << "'" << endl;

            mSocket->close();
            return;
        }

    try
        {
            Addr server(mPort,mHost);
            mSocket->connect(server);
        }

    catch (ConnectErr error)
        {
            GetLog()->Error()
                << "(SparkMonitorClient) connection failed with: '"
                << error.what() << "'" << endl;
            mSocket->close();
            mSocket.reset();
            return;
        }

    mStreamBuf = shared_ptr<SocketStreamBuf>(new SocketStreamBuf(*mSocket));
    mInStream = shared_ptr<istream>(new istream(mStreamBuf.get()));

    if (mSocket->isConnected())
        {
            cout << "(SparkMonitorClient) connected successfully" << endl;
        }

  // assure that a NetMessage object is registered
  mNetMessage = FindChildSupportingClass<NetMessage>();

  if (mNetMessage.get() == 0)
      {
          mNetMessage = shared_ptr<NetMessage>(new NetMessage());
      }

  mSceneImporter = shared_dynamic_cast<SceneImporter>
      (GetCore()->New("RubySceneImporter"));

  if (mSceneImporter.get() == 0)
      {
          GetLog()->Error()
              << "(SpakrMonitorClient) ERROR: cannot create"
              << "a RubySceneImporter instance\n";
      }
}

void SparkMonitorClient::CloseConnection()
{
    if (mSocket.get() == 0)
        {
            return;
        }

    mInStream.reset();
    mStreamBuf.reset();
    mSocket->close();
    mSocket.reset();

    GetLog()->Normal() << "(SparkMonitorClient) closed connection to "
                       << mHost << ":" << mPort << "\n";
}

void SparkMonitorClient::DoneSimulation()
{
    mNetMessage.reset();
    mActiveScene.reset();
    mSceneImporter.reset();
    CloseConnection();
}

void SparkMonitorClient::StartCycle()
{
    if (mSocket.get() == 0)
        {
            return;
        }

    bool gotFragments = false;

    for (;;)
        {

            // test for available data
            int fd = mSocket->getFD();

            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(fd,&readfds);

            timeval time;
            time.tv_sec = 0;
            time.tv_usec = 0;

            int rval = select(fd+1, &readfds, 0, 0, &time );

            if (rval == 0)
                {
                    break;
                }

            if (rval < 0)
                {
                    GetLog()->Error()
                        << "(SparkMonitorClient) ERROR select on client "
                        << "socket failed with '"
                        << strerror(errno) << "'" << endl;
                    CloseConnection();
                    return;
                }

            rval = mSocket->recv(mBuffer.get(),mBufferSize);

            if (rval == 0)
                {
                    CloseConnection();
                    return;
                }

            if (rval < 0)
                {
                    GetLog()->Error()
                        << "(SparkMonitorClient) ERROR: '" << GetName()
                        << "' recv returned error '"
                        << strerror(errno) << "' " << endl;
                    return;
                }

            string fragment(mBuffer.get(),rval);
            mNetBuffer->AddFragment(string(mBuffer.get(),rval));
            gotFragments = true;
        }

    if (gotFragments)
        {
            string msg;
            while (mNetMessage->Extract(mNetBuffer, msg))
                {
                    ParseMessage(msg);
                }
        }
}

void SparkMonitorClient::ParseMessage(const string& msg)
{
    if (
        (mSceneServer.get() == 0) ||
        (mSceneImporter.get() == 0)
        )
        {
            return;
        }

    mActiveScene = mSceneServer->GetActiveScene();

    if (mActiveScene.get() == 0)
        {
            return;
        }

    if (mManagedScene.get() != 0)
        {
            mManagedScene->UnlinkChildren();
        } else
            {
                mManagedScene = shared_dynamic_cast<BaseNode>
                    (GetCore()->New("oxygen/BaseNode"));
                mActiveScene->AddChildReference(mManagedScene);
            }

    mSceneImporter->ParseScene(msg, mManagedScene, shared_ptr<ParameterList>());
}
