#ifndef EFFECTOR_H__
#define EFFECTOR_H__

#include <kerosin/sceneserver/basenode.h>

namespace kerosin
{

class Effector : public BaseNode
{
public:
	//! this routine performs the action of the effector. It acts on >base<
	virtual bool Perform(boost::shared_ptr<BaseNode> &base, float deltaTime);
};

DECLARE_CLASS(Effector);

} // namespace kerosin

#endif //EFFECTOR_H__
