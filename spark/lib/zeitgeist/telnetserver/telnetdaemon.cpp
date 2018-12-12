#include "telnetdaemon.h"
#include "telnetserver.h"
#include "telnetsession.h"
#ifndef Q_MOC_RUN
#include <boost/thread/thread.hpp>
#endif
#include <iostream>

using namespace zeitgeist;
using namespace std;
using namespace boost;

TelnetDaemon::TelnetDaemon(TelnetServer &server) : mServer(server)
{
	mDaemonSocket = INVALID_SOCKET;
}

TelnetDaemon::~TelnetDaemon()
{
	Terminate();
}

/*
  The actual thread routine.
*/
void TelnetDaemon::operator()()
{
	//tell the server that the daemon thread is running
	mServer.SetDaemon(this);

	// at this point the bidirectional connection between client/server is
	// established. Now we start the listening socket of the daemon

	if (Init(mServer.GetPort()) == false) return;

	std::cout << mServer.GetHostName() << ": Daemon started, listening for connections..." << std::endl;

	boost::thread_group sessionThreads;

	TelnetSession cc(*this);

	while (AcceptConnection(cc))
	{
		sessionThreads.create_thread(cc);
	}

	// we wait for all session threads to finish, before we finish
	sessionThreads.join_all();
}

bool TelnetDaemon::Init(int portNr)
{
	// create socket
	mDaemonSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	if (mDaemonSocket == INVALID_SOCKET)
	{
		std::cout << "ERROR: Could not create socket" << std::endl;
		return false;
	}

	int iOptval = 1;
	if ( setsockopt( mDaemonSocket, SOL_SOCKET, SO_REUSEADDR, (char *) &iOptval, sizeof(int)) )
	{
		std::cout << "ERROR: Setsockopt failed" << std::endl;
		return false;
	}

	mDaemonAddr.sin_family      = AF_INET;
	mDaemonAddr.sin_port        = htons( USHORT(portNr) );
	mDaemonAddr.sin_addr.s_addr = INADDR_ANY;

	// Bind to the given port
	if ( bind( mDaemonSocket, (struct sockaddr *) &mDaemonAddr, sizeof(mDaemonAddr) ) )
	{
		std::cout << "ERROR: Bind failed" << std::endl;
		return false;
	}

	// change to passive socket
	if ( listen( mDaemonSocket , SOMAXCONN ) )
	{
		std::cout << "ERROR: Listen failed" << std::endl;
		return false;
	}

	return true;
}

void TelnetDaemon::Terminate()
{
	if (mDaemonSocket != INVALID_SOCKET)
	{
		closesocket(mDaemonSocket);
		mDaemonSocket = INVALID_SOCKET;
	}
}

bool TelnetDaemon::AcceptConnection(TelnetSession &cc)
{
	sockaddr_in	clientAddr;
	SOCKET		clientSocket;
	
	// fill with zero terms
	memset( &clientAddr , 0 , sizeof(clientAddr) );

	// wait for connetion
	int len = sizeof(clientAddr);
	clientSocket = accept( mDaemonSocket, (struct sockaddr *)&(clientAddr) , &len );
	if ( clientSocket == INVALID_SOCKET )
	{
		//std::cout << "ERROR: Accept failed" << std::endl;
		return false;
	}

	// get client ip address
	char szClientIp[255];
	strncpy( szClientIp, inet_ntoa(clientAddr.sin_addr), 128);
	std::cout << "Incoming connection from: " << szClientIp << std::endl;

	// create a clientconnection
	cc.Init(clientSocket, clientAddr, mServer.GetScriptServer());
	return true;
}

void TelnetDaemon::Attach(TelnetSession *session)
{
	std::cout << "Attaching TelnetSession " << (void*) session << std::endl;
	mSessions.push_back(session);
}

void TelnetDaemon::Detach(TelnetSession *session)
{
	std::cout << "Detaching TelnetSession " << (void*) session << std::endl;
	mSessions.remove(session);
}

void TelnetDaemon::Status()
{
	std::cout << "  Number of clients: " << mSessions.size() << std::endl;
}