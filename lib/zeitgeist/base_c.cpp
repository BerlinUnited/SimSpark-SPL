#include "base.h"
#include "logserver/logserver.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace zeitgeist;

FUNCTION(getName)
{
	Base *base = static_cast<Base*>(obj);

	base->GetLog()->Normal() << base->GetName() << endl;
}

/*FUNCTION(status)
{
	base->Status();
}*/

FUNCTION(isLeaf)
{
	Base *base = static_cast<Base*>(obj);

	cout << base->IsLeaf() << endl;
}

void CLASS(Base)::DefineClass()
{
	DEFINE_FUNCTION(getName);
//	DEFINE_FUNCTION(status);
	DEFINE_FUNCTION(isLeaf);
}
