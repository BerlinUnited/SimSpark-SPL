#include "logserver.h"
#include <fstream>

using namespace boost;
using namespace zeitgeist;

FUNCTION(addStream)
{
	if (in.size() == 2)
	{
		LogServer *ls = static_cast<LogServer*>(obj);
		std::string name		= boost::any_cast<char*>(in[0]);
		std::string priority	= boost::any_cast<char*>(in[1]);

		std::ostream *stream = NULL;
		unsigned int pLevel	 = LogServer::eNone;
		if (name.compare(":cout") == 0) stream = &std::cout;
		else if (name.compare(":cerr") == 0) stream = &std::cerr;
		else stream = new std::ofstream(name.c_str());
		
		if (priority.compare("eNone") == 0) pLevel = LogServer::eNone;
		if (priority.compare("eDebug") == 0) pLevel = LogServer::eDebug;
		if (priority.compare("eNormal") == 0) pLevel = LogServer::eNormal;
		if (priority.compare("eWarning") == 0) pLevel = LogServer::eWarning;
		if (priority.compare("eError") == 0) pLevel = LogServer::eError;
		if (priority.compare("eAll") == 0) pLevel = LogServer::eAll;
		
		ls->AddStream(stream, pLevel);
	}
}

void CLASS(LogServer)::DefineClass()
{
	DEFINE_BASECLASS(zeitgeist/Node);
	DEFINE_FUNCTION(addStream);
}
