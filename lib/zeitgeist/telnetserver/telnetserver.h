#ifndef TELNETSERVER_H__
#define TELNETSERVER_H__

/*! \class TelnetServer
	$Id: telnetserver.h,v 1.1 2003/08/07 08:34:03 rollmark Exp $
	
	TelnetServer
	:TODO: Class description for TelnetServer

	HISTORY:
		20.06.2002 MK
			- initial version

		28.06.2003 MR
			- modified for inclusion into the zeitgeist library

*/

#ifdef _WIN32

// the socket functions require the ws2_32 library
#pragma comment(lib, "ws2_32.lib")

// boost threads are used for thread creation
#pragma comment(lib, "libboost_thread.lib")

#endif

#include <zeitgeist/node.h>
#include <string>

namespace zeitgeist
{

class TelnetDaemon;

class TelnetServer : public Node
{
	friend class TelnetDaemon;

	//
	// functions
	//
public:
	TelnetServer(unsigned int port = 23);
	virtual ~TelnetServer();

	void Status();

	unsigned int GetPort() const			{	return mPort;	};
	const std::string& GetHostName() const	{	return mHostName;	}
private:
	TelnetServer(const TelnetServer& obj);
	TelnetServer& operator=(const TelnetServer& obj);

	void SetDaemon(TelnetDaemon *daemon)	{	mDaemon = daemon;	}

	boost::shared_ptr<ScriptServer> GetScriptServer()	{	return GetScript();	}

	bool Start();
	bool Shutdown();

	//
	// members
	//
private:
	unsigned int	mPort;
	TelnetDaemon	*mDaemon;
	std::string		mHostName;
};

DECLARE_CLASS(TelnetServer);

}

#endif //TELNETSERVER_H__