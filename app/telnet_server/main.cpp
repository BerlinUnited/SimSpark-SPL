#include <iostream>
#include <iomanip>
#include <winsock2.h>
#include "telnetserver.h"

int main()
{
	// Windows socket initialization
	WORD    wVersionRequested; 
	WSADATA wsaData; 
	wVersionRequested = MAKEWORD(2, 0); 

	// Find winsock version
	if ( WSAStartup(wVersionRequested, &wsaData) )
	{
		std::cout << "ERROR: Winsock not found" << std::endl;
		return 1;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		 HIBYTE( wsaData.wVersion ) != 0 )
	{
		std::cout << "ERROR: Incorrect winsock version" << std::endl;
		WSACleanup( );
		return 2;
	}
	else
	{
		std::cout << "Using Winsock v" << std::hex << wsaData.wVersion << std::endl;
	}

	// now we leave the windows specific world
	TelnetServer telnetServer;

	telnetServer.Start();

	bool done = false;
	while (!done)
	{
		std::string command;
		std::cin >> command;
		if (command.compare("quit")==0)
		{
			done = true;
		}
		if (command.compare("status")==0)
		{
			telnetServer.Status();
		}
	}

	telnetServer.Shutdown();
	
	WSACleanup();
	return 0;
}