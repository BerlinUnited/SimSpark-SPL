#include <iostream>
#include <winsock.h>
#include "telnetserver.h"

int main()
{
	// Windows socket initialization
	WORD    wVersionRequested; 
	WSADATA wsaData; 
	wVersionRequested = MAKEWORD(1, 1); 

	// Find winsock version
	if ( WSAStartup(wVersionRequested, &wsaData) )
	{
		std::cout << "Incorrect winsock version\n" << std::endl;
		return 1;
	}

	// now we leave the windows specific world
	TelnetServer myServer;

	myServer.Start();

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
			myServer.Status();
		}
	}

	myServer.Shutdown();
	
	WSACleanup();
	return 0;
}