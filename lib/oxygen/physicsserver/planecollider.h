#ifndef PLANECOLLIDER_H__
#define PLANECOLLIDER_H__

#include "collider.h"

namespace kerosin
{

class PlaneCollider : public Collider
{
	//
	// Functions
	//
public:
	PlaneCollider();

	void SetParams(float a, float b, float c, float d);

protected:
	virtual bool ConstructInternal();
};

DECLARE_CLASS(PlaneCollider);

} //namespace kerosin

#endif //PLANECOLLIDER_H__
