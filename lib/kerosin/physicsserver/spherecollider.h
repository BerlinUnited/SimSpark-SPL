#ifndef SPHERECOLLIDER_H__
#define SPHERECOLLIDER_H__

#include "collider.h"

namespace kerosin
{

class SphereCollider : public Collider
{
	//
	// Functions
	//
public:
	SphereCollider();

	void SetRadius(float r);

protected:
	virtual bool ConstructInternal();
};

DECLARE_CLASS(SphereCollider);

} //namespace kerosin

#endif //SPHERECOLLIDER_H__
