#include "zeitgeist.h"
#include <iostream>

using namespace std;
using namespace zeitgeist;

Zeitgeist::Zeitgeist() :
mCore(new Core())
{
	cout << "Starting up Zeitgeist..." << endl;
	mCore->Construct(mCore);
}

Zeitgeist::~Zeitgeist()
{
	cout << "Shutting down Zeitgeist..." << endl;
}

boost::shared_ptr<CoreContext>	Zeitgeist::CreateContext()
{
	return mCore->CreateContext();
}

boost::shared_ptr<Core>& Zeitgeist::GetCore()
{
	return mCore;
}
