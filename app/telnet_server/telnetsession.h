#ifndef TELNETSESSION_H__
#define TELNETSESSION_H__

/*! \class TelnetSession
	$Id: telnetsession.h,v 1.2 2003/06/22 17:32:16 rollmark Exp $
	
	TelnetSession
	:TODO: Class description for ClientConnection

	HISTORY:
		20.06.2002 MK
			- initial version

*/

#include <winsock.h>
#include <string>

class TelnetDaemon;

class TelnetSession
{
	//
	// functions
	//
public:
	TelnetSession(TelnetDaemon &daemon);
	virtual ~TelnetSession();

	void Init(SOCKET clientSocket, sockaddr_in clientAddr);
	void operator()();
	bool Send(const std::string& data);
	bool WaitForData(std::string &data);

	void Terminate();
   
protected:
private:
	void ProcessCommand(unsigned char command);
	//
	// members
	//
public:
protected:
private:
	SOCKET			mClientSocket;
	sockaddr_in		mClientAddr;

	// options
	bool			mDoEcho;

	// link to daemon which created this session
	TelnetDaemon	&mDaemon;
};

#endif //TELNETSESSION_H__