/*!
	Zeitgeist Library

	This is the main class, which initializes the Zeitgeist framework, manages
	the core and the main core context. It is the basic interface to the client
	code.
*/

#ifndef ZEITGEIST_H__
#define ZEITGEIST_H__

/*#include "base.h"
#include "class.h"
#include "core.h"
#include "node.h"
*/

#include "core.h"
#include "corecontext.h"
#include "scriptserver/scriptserver.h"
#include "logserver/logserver.h"
#include <boost/shared_ptr.hpp>

namespace zeitgeist
{

class Zeitgeist
{
	//
	// functions
	//
public:
	Zeitgeist();
	~Zeitgeist();

	boost::shared_ptr<CoreContext>	CreateContext();
	boost::shared_ptr<Core>&		GetCore();

	//
	// members
	//
private:
	boost::shared_ptr<Core>	mCore;
};

} //namespace zeitgeist

#endif //ZEITGEIST_H__
