#ifndef KEROSIN_H__
#define KEROSIN_H__

/*! \class Kerosin
	$Id: kerosin.h,v 1.2 2003/04/30 14:21:11 fruit Exp $
	
	Kerosin
	:TODO: Class description for Kerosin

	HISTORY:
		11.07.2002 MK
			- initial version

*/

#include "soundserver/soundserver.h"

#include "inputserver/inputserver.h"

#include "imageserver/imageserver.h"

#include "fontserver/fontserver.h"
#include "fontserver/font.h"

#include "openglserver/openglserver.h"

#include "textureserver/textureserver.h"
#include "textureserver/texture.h"

#include "materialserver/materialserver.h"
#include "materialserver/material.h"

// scene graph
#include "sceneserver/sceneserver.h"
#include "sceneserver/axis.h"
#include "sceneserver/basenode.h"
#include "sceneserver/camera.h"
#include "sceneserver/fpscontroller.h"
#include "sceneserver/light.h"
#include "sceneserver/scene.h"
#include "sceneserver/sphere.h"
#include "sceneserver/staticmesh.h"
#include "sceneserver/transform.h"

// physics
#include "physicsserver/physicsserver.h"
#include "physicsserver/body.h"
#include "physicsserver/odeobject.h"
#include "physicsserver/space.h"
#include "physicsserver/world.h"
// collider
#include "physicsserver/collider.h"
#include "physicsserver/planecollider.h"
#include "physicsserver/spherecollider.h"

// agent aspect
#include "agentaspect/agentaspect.h"
#include "agentaspect/effector.h"
#include "agentaspect/forceeffector.h"
#include "agentaspect/perceptor.h"
#include "agentaspect/visionperceptor.h"

// control aspect
#include "controlaspect/controlaspect.h"

#include <zeitgeist/zeitgeist.h>

namespace kerosin
{

class Kerosin
{
	//
	// types
	//
public:
protected:
private:

	//
	// functions
	//
public:
	Kerosin(zeitgeist::Zeitgeist &zg);
	virtual ~Kerosin() {};
protected:
private:
	Kerosin(const Kerosin& obj);
	Kerosin& operator=(const Kerosin& obj);

	//
	// members
	//
public:
protected:
private:
};

} //namespace kerosin

#endif //KEROSIN_H__
