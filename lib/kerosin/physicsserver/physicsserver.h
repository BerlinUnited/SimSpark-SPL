#ifndef PHYSICSSERVER_H__
#define PHYSICSSERVER_H__

#include <zeitgeist/leaf.h>

namespace kerosin
{

class PhysicsServer : public zeitgeist::Leaf
{
	//
	// Functions
	//
public:
	PhysicsServer();
	~PhysicsServer();
};

DECLARE_CLASS(PhysicsServer);

} //namespace kerosin

#endif //PHYSICSSERVER_H__
