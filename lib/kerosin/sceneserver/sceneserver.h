#ifndef SCENESERVER_H__
#define SCENESERVER_H__

/*	\class SceneServer
	$Id: sceneserver.h,v 1.1 2003/04/30 14:21:41 fruit Exp $

	SceneServer

	The scene server manages displayable subtrees within the object
	hierarchy. Each subtree begins with a Scene node. The scene server
	knows which scene node is currently active and updates/displays
	that node (and its corresponding subtree). 

	NOTE:

	HISTORY:
		05.11.02 - MK
			- Initial version

	TODO:

	TOFIX:
*/

#include <zeitgeist/leaf.h>

namespace kerosin
{

class Scene;

class SceneServer : public zeitgeist::Leaf
{
	//
	// Functions
	//
public:
	SceneServer();
	~SceneServer();

	//! create a new scene hierarchy at a specific location, new hierarchy is also made current
	boost::shared_ptr<Scene>	CreateScene(const std::string &location);
	//! set the active scene
	bool						SetActiveScene(const std::string &location);

	//! update the state of the current active scene (deltaTime is in seconds)
	void						Update(float deltaTime);
	//! display the current active scene
	void						Render();

protected:
	//! overload this to perform stuff after the object has been created and attached to a core
	virtual bool ConstructInternal();

	//
	// Members
	//
private:
	boost::shared_ptr<Scene>	mActiveScene;

	unsigned int mAmbientVP;
};

DECLARE_CLASS(SceneServer);

} //namespace kerosin
#endif //SCENESERVER_H__