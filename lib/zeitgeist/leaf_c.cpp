#include "leaf.h"
#include "logserver/logserver.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace zeitgeist;

FUNCTION(getName)
{
    if( in.empty() )
    {
        Leaf *leaf = static_cast<Leaf*>(obj);

        leaf->GetLog()->Normal() << leaf->GetName() << endl;
    }
}

/*FUNCTION(status)
{
	leaf->Status();
}*/

FUNCTION(isLeaf)
{
    if( in.empty() )
    {
        Leaf *leaf = static_cast<Leaf*>(obj);

        cout << leaf->IsLeaf() << endl;
    }
}

void CLASS(Leaf)::DefineClass()
{
	DEFINE_FUNCTION(getName);
//	DEFINE_FUNCTION(status);
	DEFINE_FUNCTION(isLeaf);
}
