#ifndef SPHERE_H__
#define SPHERE_H__

#include "basenode.h"

namespace kerosin
{

class Sphere : public BaseNode
{
	//
	// Function
	//
public:
	Sphere();

	// set/get properties
	float GetRadius() const;
	void SetRadius(float radius);

private:
	virtual void RenderInternal();

	//
	// Members
	//
private:
	float	mRadius;
};

DECLARE_CLASS(Sphere);

} //namespace kerosin

#endif //SPHERE_H__
