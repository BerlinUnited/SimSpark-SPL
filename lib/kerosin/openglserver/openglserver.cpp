#include "openglserver.h"
#include <SDL/SDL.h>
#include <zeitgeist/scriptserver/scriptserver.h>


using namespace kerosin;
using namespace zeitgeist;

OpenGLServer::OpenGLServer() : Base(), mExtensionReg(new GLExtensionReg())
{
}

OpenGLServer::~OpenGLServer()
{
	SDL_Quit();
}

bool OpenGLServer::Init()
{
	if( SDL_Init(SDL_INIT_VIDEO) < 0 )
	{
		return false;
	}

	const SDL_VideoInfo* info = SDL_GetVideoInfo();

	int			intValue;
	bool		boolValue;
	std::string strVal;

	GetScript()->GetVariable("Viewport.redBits", intValue);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, intValue);
	GetScript()->GetVariable("Viewport.greenBits", intValue);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, intValue);
	GetScript()->GetVariable("Viewport.blueBits", intValue);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, intValue);
	GetScript()->GetVariable("Viewport.alphaBits", intValue);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, intValue);

	GetScript()->GetVariable("Viewport.depthBits", intValue);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, intValue);
	GetScript()->GetVariable("Viewport.stencilBits", intValue);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, intValue);

	GetScript()->GetVariable("Viewport.doubleBuffer", boolValue);
	if (boolValue)
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);
	else
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 0);

	int flags = SDL_OPENGL;
	GetScript()->GetVariable("Viewport.fullScreen", boolValue);
	if (boolValue)
		flags |= SDL_FULLSCREEN;
	
	int xRes, yRes;
	GetScript()->GetVariable("Viewport.xRes", xRes);
	GetScript()->GetVariable("Viewport.yRes", yRes);

	SDL_Surface *screen = SDL_SetVideoMode(xRes, yRes, info->vfmt->BitsPerPixel, flags);
	if ( screen == NULL )
	{
		return false;
    }

	SDL_ShowCursor(SDL_DISABLE);

	GetScript()->GetVariable("Application.title", strVal);
	SDL_WM_SetCaption(strVal.c_str(), NULL);

	mExtensionReg->Init();

	return true;
}

boost::shared_ptr<GLExtensionReg> OpenGLServer::GetExtensionReg() const
{
	return mExtensionReg;
}