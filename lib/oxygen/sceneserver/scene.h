#ifndef SCENE_H__
#define SCENE_H__

/*	\class Scene
	$Id: scene.h,v 1.1 2003/08/29 14:12:56 fruit Exp $

	Scene

	A scene is the root node of a simulatable/displayable hierarchy. It
	is usually created via the scene server. A scene also has the knowledge
	about a number of cameras, which serve as the entry point to the rendering
	of the scene.

	NOTE:

	HISTORY:
		05.11.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include "basenode.h"

namespace kerosin
{

class Scene : public BaseNode
{
public:
	Scene();
	~Scene();
	
	//! return the world transform of this node (always identity, terminates upward recursion)
	virtual const salt::Matrix&	GetWorldTransform()	const;
};

DECLARE_CLASS(Scene);

} //namespace kerosin
#endif //SCENE_H__
