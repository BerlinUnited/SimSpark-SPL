#ifndef TELNETSERVER_H__
#define TELNETSERVER_H__

/*! \class TelnetServer
	$Id: telnetserver.h,v 1.1 2003/04/30 15:59:37 fruit Exp $
	
	TelnetServer
	:TODO: Class description for TelnetServer

	HISTORY:
		20.06.2002 MK
			- initial version

*/

#include <string>

class TelnetDaemon;

class TelnetServer
{
	friend class TelnetDaemon;

	//
	// functions
	//
public:
	TelnetServer(unsigned int port = 23);
	virtual ~TelnetServer() {};

	bool Start();
	bool Shutdown();

	void Status();

	unsigned int GetPort() const			{	return mPort;	};
	const std::string& GetHostName() const	{	return mHostName;	}
private:
	TelnetServer(const TelnetServer& obj);
	TelnetServer& operator=(const TelnetServer& obj);

	void SetDaemon(TelnetDaemon *daemon)	{	mDaemon = daemon;	}

	//
	// members
	//
private:
	unsigned int	mPort;
	TelnetDaemon	*mDaemon;
	std::string		mHostName;
};

#endif //TELNETSERVER_H__