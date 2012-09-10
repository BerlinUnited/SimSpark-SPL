#include "telnetsession.h"
#include "telnetdaemon.h"
#include "telnetserver.h"
#include "../scriptserver/scriptserver.h"
#include <iostream>

using namespace zeitgeist;
using namespace boost;

TelnetSession::TelnetSession(TelnetDaemon &daemon) 
	: mClientSocket(INVALID_SOCKET), mDaemon(daemon)
{
	memset(&mClientAddr, 0, sizeof(mClientAddr));

	// options
	mDoEcho = false;
}

TelnetSession::~TelnetSession()
{
}

void TelnetSession::Init(SOCKET clientSocket, sockaddr_in clientAddr, boost::shared_ptr<ScriptServer> scriptServer)
{
	mClientSocket	= clientSocket;
	mClientAddr		= clientAddr;
	mScriptServer	= scriptServer;
}

void TelnetSession::operator()()
{
	// attach ourselves to the daemon which created us
	mDaemon.Attach(this);

	bool done = false;
	while(!done)
	{
		Send(mDaemon.GetServer().GetHostName()+": ");
		std::string input;

		if (WaitForData(input) == false)
		{
			done = true;
		}
		else
		{
			if (input.compare("exit")==0)
			{
				Terminate();
			}
			else
			{
				mScriptServer->Eval(input);
				//Send("Executing: '"+input+"'\r\n");
			}
		}
	}

	// remove ourselves from the daemon
	mDaemon.Detach(this);

	closesocket(mClientSocket);
}

bool TelnetSession::Send(const std::string& data)
{
	if ( send( mClientSocket , data.c_str() , data.size() , 0 ) != (int)data.size() )
	{
		std::cout << "ERROR: Sending data to client failed" << std::endl;
		return false;
	}

	return true;
}

bool TelnetSession::WaitForData(std::string &data)
{
	const unsigned char IAC = 255;	// interpret as command
	// global would be faster....
	std::string validChars = "abcdefghijklmnopqrstuvwxyz ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~`!@#$%^&*()-_=+{[}]\\|;:'\"<,>.?/";

	int  sizeEchoBuf = 0;
	char echoBuf[4];

	// empty the buffer
	memset( echoBuf , 0 , 4 );

	unsigned char buf = 0;

	while ( buf != 13 )
	{	
		buf = 0;
		memset( echoBuf , 0 , 4 );
		sizeEchoBuf = 0;

		if(recv( mClientSocket, (char*)&buf , 1 , 0) < 1)
		{ 
			//std::cout << "ERROR: Receiving data from client failed" << std::endl;
			return false;
		}

		switch((unsigned char)buf)
		{
		case 13:	// return was pressed
			echoBuf[0] = 13;
			echoBuf[1] = 10;
			sizeEchoBuf = 2;
			break;
		case 8:	// backspace was pressed
		case 127:	// backspace was pressed
			if ( data.size() > 0 )
			{
				// cut off one char
				data.resize(data.size()-1);
				echoBuf[0] = buf;
				echoBuf[1] = 32;
				echoBuf[2] = buf;
				sizeEchoBuf = 3;
			}
			else
			{
				continue;
			}
			break;
		case IAC:
			{
				recv( mClientSocket, (char*)&buf, 1 , 0);
				ProcessCommand(buf);
			}
			break;
		default:
			if (validChars.find(buf) != -1)
			{
				// add to result buffer
				data += buf;

				// echo the pressed char
				echoBuf[0] = buf;
				sizeEchoBuf = 1;
			}
			break;
		}

		// echo
		if ( mDoEcho == true && sizeEchoBuf>0)
		{
			if (send( mClientSocket , echoBuf , sizeEchoBuf , 0 ) != sizeEchoBuf)
			{
				std::cout << "ERROR: Sending data to client failed" << std::endl;
				return false;
			}
		}
	}
	
	return true;	
}

void TelnetSession::Terminate()
{
	shutdown(mClientSocket, 0x01);
}

void TelnetSession::ProcessCommand(unsigned char command)
{
	unsigned char option = 0;

	std::cout << "Command: " << (unsigned int)command;
	if (command >= 251 && command <= 254)
	{
		// these commands need another byte from the client
		unsigned char buf;
		recv( mClientSocket, (char*)&buf, 1 , 0);
		option = buf;
		std::cout <<  " - " << (unsigned int)option;
	}
	std::cout << std::endl;

	switch (command)
	{
	case 253:	// DO
		switch(option)
		{
		case 1: // do echo
			mDoEcho = true;
			break;
		}
		break;
	case 254:	// DON'T
		switch(option)
		{
		case 1: // do echo
			mDoEcho = false;
			break;
		}
		break;
	};
}