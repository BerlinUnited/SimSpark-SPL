#ifndef TELNETSESSION_H__
#define TELNETSESSION_H__

/*! \class TelnetSession
	$Id: telnetsession.h,v 1.1 2003/08/07 08:34:03 rollmark Exp $
	
	TelnetSession
	:TODO: Class description for ClientConnection

	HISTORY:
		20.06.2002 MK
			- initial version

*/

#include <winsock.h>
#include <string>
#include <boost/shared_ptr.hpp>

namespace zeitgeist
{

class ScriptServer;
class TelnetDaemon;

class TelnetSession
{
	//
	// functions
	//
public:
	TelnetSession(TelnetDaemon &daemon);
	virtual ~TelnetSession();

	void Init(SOCKET clientSocket, sockaddr_in clientAddr, boost::shared_ptr<ScriptServer> scriptServer);
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

	//! options
	bool			mDoEcho;

	//! link to daemon which created this session
	TelnetDaemon	&mDaemon;

	//! the context associated with this session
	boost::shared_ptr<ScriptServer> mScriptServer;
};

}

#endif //TELNETSESSION_H__