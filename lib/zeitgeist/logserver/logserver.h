#ifndef LOGSERVER_H__
#define LOGSERVER_H__

/*! \class LogServer
	$Id: logserver.h,v 1.2 2003/04/30 14:21:53 fruit Exp $
	
	LogServer

	The log server is responsible for holding a bunch of ostreams, which
	data can be streamed to. It is THE logging facility used within zeitgeist.

	HISTORY:
		22.08.2002 MK
			- initial version - reclaimed from rcssserver3D :)

*/

#include <iostream>
#include "../node.h"

namespace zeitgeist
{

class LogServerStreamBuf;

class LogServer : public Node , public std::ostream
{
	// types
	//
public:
    enum EPriorityLevel 
    {
        eNone    = 0,
        eDebug   = 1,
        eNormal  = 2,
        eWarning = 4,
        eError   = 8,
        eAll     = 0xffffffff
    };

	// functions
	//
public:
    LogServer(unsigned int size = 1024);
    virtual ~LogServer();

    /*! Add a stream to the list of streams.

        First, it is checked if the stream is already in. If the stream
        is found, we only install a new priority mask, so no stream can
        be added twice.

        @param stream   the stream to add
        @param mask     the (new) priority mask for the stream
    */
    void AddStream(std::ostream *stream, unsigned int mask = eAll);
    /*! Remove a stream from the list of streams.
        @param stream   the stream to remove
        @return         true if the stream was found (and thus removed)
    */
    bool RemoveStream(const std::ostream *stream);
    /*! Set priority mask of a stream in the list.
        @param stream   the stream for which we want to set the priority mask
        @param mask     the new priority mask
        @return         true if the stream was found
    */
    bool SetPriorityMask(const std::ostream *stream, unsigned int mask);
    /*! Get priority mask of a stream in the list.
        @param stream   the stream for which we want to set the priority mask
        @return         the priority mask; 0 if stream was not found
    */
    unsigned int GetPriorityMask(const std::ostream *stream) const;

    //! Priority selection for the messages to be written
    LogServer& Priority(unsigned int prio);

    LogServer& Debug()    { return Priority(eDebug); }
    LogServer& Normal()   { return Priority(eNormal); }
    LogServer& Warning()  { return Priority(eWarning); }
    LogServer& Error()    { return Priority(eError); }

	void Printf(const char *inFormat, ...);
	
private:
    LogServer(const LogServer& obj);
    LogServer& operator=(const LogServer& obj);

    const LogServerStreamBuf& GetStreamBuf() const;
    LogServerStreamBuf& GetStreamBuf();
};

DECLARE_CLASS(LogServer);

} //namespace zeitgeist

#endif //LOGSERVER_H__
