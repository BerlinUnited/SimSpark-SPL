#include "collisionperceptor.h"

using namespace boost;
using namespace kerosin;

bool CollisionPerceptor::Percept(TDictionary &dictionary)
{
	if (!mCollidees.empty())
	{
		dictionary["collidees"] = mCollidees;
		mCollidees.clear();
	}

	return (!mCollidees.empty());
}

