#ifndef FPSCONTROLLER_H__
#define FPSCONTROLLER_H__

#include "basenode.h"

namespace kerosin
{

class FPSController : public BaseNode
{
	//
	// Functions
	//
public:
	FPSController();
	~FPSController();

	void	AdjustHAngle(const float delta)	{	mHAngle += delta;	}
	void	AdjustVAngle(const float delta)	{	mVAngle += delta;	}
	void	Forward(const bool state)		{	mForward = state;	}
	void	Backward(const bool state)		{	mBackward = state;	}
	void	StrafeLeft(const bool state)	{	mLeft = state;	}
	void	StrafeRight(const bool state)	{	mRight= state;	}
	void	Up(const bool state)			{	mUp = state;	}
	void	Down(const bool state)			{	mDown = state;	}

private:
	virtual void PrePhysicsUpdateInternal(float deltaTime);
	//
	// Members
	//
protected:
	float	mHAngle;	// horizontal angle
	float	mVAngle;	// vertical angle
	
	// event states
	bool	mForward;	// forward is 'pressed'
	bool	mBackward;	// backward is 'pressed'
	bool	mLeft;		// left is 'pressed'
	bool	mRight;		// right is 'pressed'
	bool	mUp;		// up is 'pressed'
	bool	mDown;		// down is 'pressed'
};

DECLARE_CLASS(FPSController);

} //namespace kerosin

#endif //FPSCONTROLLER_H__
