#include "logserver.h"
#include "logserverstreambuf.h"
#include <stdarg.h>

using namespace std;
using namespace zeitgeist;

LogServer::LogServer(unsigned int size) :
Node(), ostream(new LogServerStreamBuf(size))
{
}

LogServer::~LogServer()
{
	flush();
	
    // delete the created streambuf
    delete rdbuf();
}

void LogServer::AddStream(std::ostream* stream, unsigned int mask)
{
    GetStreamBuf().AddStream(stream, mask);
}

bool LogServer::RemoveStream(const std::ostream* stream)
{
    return GetStreamBuf().RemoveStream(stream);
}
    
unsigned int LogServer::GetPriorityMask(const std::ostream* stream) const
{
    return GetStreamBuf().GetPriorityMask(stream);
}
    
bool LogServer::SetPriorityMask(const std::ostream* stream, unsigned int mask)
{
    return GetStreamBuf().SetPriorityMask(stream, mask);
}

LogServer& LogServer::Priority(unsigned int prio)
{
    GetStreamBuf().SetCurrentPriority(prio);
    return *this;
}

void LogServer::Printf(const char *inFormat, ...)
{
	char copyBuffer[4096];
	va_list args;
	va_start(args, inFormat);
	vsprintf(copyBuffer, inFormat, args);
	va_end(args);
	(*this) << copyBuffer;
	this->flush();
}

const LogServerStreamBuf& LogServer::GetStreamBuf() const
{
    LogServerStreamBuf* streamBuf = (LogServerStreamBuf*)(rdbuf());
    return *streamBuf;
}

LogServerStreamBuf& LogServer::GetStreamBuf()
{
    LogServerStreamBuf* streamBuf = (LogServerStreamBuf*)(rdbuf());
    return *streamBuf;
}
