#ifndef COLLISIONPERCEPTOR_H__
#define COLLISIONPERCEPTOR_H__

/*!
	The CollisionPerceptor class is used to filter collisions as they occur
	within the scenegraph.
*/

#include "perceptor.h"

namespace kerosin
{

class CollisionPerceptor : public Perceptor
{
	//
	// functions
	//
public:
	/*
		Returns:
			"collidees" as a TLeafList
	*/
	virtual bool Percept(TDictionary &dictionary);

	TLeafList& GetCollidees() {	return mCollidees;	}

private:
	TLeafList mCollidees;
};

DECLARE_CLASS(CollisionPerceptor);

}
#endif //COLLISIONPERCEPTOR_H__

