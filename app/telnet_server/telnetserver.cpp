#include "telnetserver.h"
#include <boost/thread/thread.hpp>
#include <iostream>
#include "telnetdaemon.h"

TelnetServer::TelnetServer(unsigned int port) : mPort(port)
{
	mDaemon = NULL;
}

bool TelnetServer::Start()
{
	if (mDaemon != NULL)
	{
		Shutdown();
	}

	char buffer[512];
	gethostname(buffer, 511);
	mHostName = buffer;
	
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