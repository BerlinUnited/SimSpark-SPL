#include "telnetserver.h"
#ifndef Q_MOC_RUN
#include <boost/thread/thread.hpp>
#endif
#include <iostream>
#include "telnetdaemon.h"
#include "../logserver/logserver.h"

using namespace zeitgeist;
using namespace std;

TelnetServer::TelnetServer(unsigned int port) : Node(), mPort(port) 
{
	mDaemon = NULL;

#ifdef _WIN32
	// Windows socket initialization
	WORD    wVersionRequested; 
	WSADATA wsaData; 
	wVersionRequested = MAKEWORD(1, 1); 

	// Find winsock version
	if ( WSAStartup(wVersionRequested, &wsaData) )
	{
		GetLog()->Normal() << "TelnetServer: Incorrect winsock version\n" << endl;
	}

#endif

	char buffer[512];
	gethostname(buffer, 511);
	mHostName = buffer;

	Start();
}

TelnetServer::~TelnetServer()
{
	Shutdown();

#ifdef _WIN32
	WSACleanup();
#endif
}

bool TelnetServer::Start()
{
	if (mDaemon != NULL)
	{
		Shutdown();
	}

	// here we start the actual worker thread
	TelnetDaemon daemon(*this);
	boost::thread daemonThread(daemon);

	return true;
}

bool TelnetServer::Shutdown()
{
	if (mDaemon != NULL)
	{
		mDaemon->Terminate();
		mDaemon = NULL;
	}
	return true;
}

void TelnetServer::Status()
{
	std::cout << "TelnetServer::Status()\n";

	if (mDaemon == NULL)
	{
		std::cout << "  No daemon running...\n";
		return;
	}

	std::cout << "  Daemon running on port " << GetPort() << std::endl;
	
	mDaemon->Status();

	std::cout << std::endl;
}