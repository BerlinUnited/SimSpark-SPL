#ifndef TELNETDAEMON_H__
#define TELNETDAEMON_H__

/*! \class TelnetDaemon
	$Id: telnetdaemon.h,v 1.1 2003/04/30 15:59:37 fruit Exp $
	
	TelnetServer
	:TODO: Class description for TelnetDaemon

	HISTORY:
		19.06.2002 MK
			- initial version

*/

#include <winsock2.h>
#include <list>

class TelnetSession;
class TelnetServer;

class TelnetDaemon
{
	//
	// types
	//
public:
protected:
private:
	typedef std::list<TelnetSession*>	TSessionList;

	//
	// functions
	//
public:
	TelnetDaemon(TelnetServer &server);
	virtual ~TelnetDaemon();

	void operator()();

	void Terminate();
	void Status();

	void Attach(TelnetSession *session);
	void Detach(TelnetSession *session);
	
	const TelnetServer&	GetServer() const	{	return mServer;	}
	
protected:
private:
	bool Init(int portNr);
	bool AcceptConnection(TelnetSession& cc);

	//
	// members
	//
public:
protected:
private:
	SOCKET			mDaemonSocket;
	sockaddr_in		mDaemonAddr;
	TSessionList	mSessions;
	TelnetServer	&mServer;
};

#endif //TELNETDAEMON_H__
