#include "openglserver.h"
#include <SDL/SDL.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include <zeitgeist/fileserver/fileserver.h>
#include <zeitgeist/logserver/logserver.h>

#ifdef HAVE_CONFIG_H
# ifdef PACKAGE_BUGREPORT
#  undef PACKAGE_BUGREPORT
# endif
# ifdef PACKAGE_NAME
#  undef PACKAGE_NAME
# endif
# ifdef PACKAGE_STRING
#  undef PACKAGE_STRING
# endif
# ifdef PACKAGE_TARNAME
#  undef PACKAGE_TARNAME
# endif
# ifdef PACKAGE_VERSION
#  undef PACKAGE_VERSION
# endif
#include "config.h"
#endif

namespace kerosin
{
    class MapHolder
    {
    public:
    //! this structure will be used to map program names to OpenGL IDs
#if HAVE_HASH_MAP
        typedef std::hash_map<std::string, unsigned int> TProgramCache;
#else
        typedef std::map<std::string, unsigned int> TProgramCache;
#endif
        TProgramCache mPrograms;
    };
}

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

OpenGLServer::OpenGLServer() : Leaf(), mExtensionReg(new GLExtensionReg()), mWantsToQuit(false), mHolder( new MapHolder() )
{
}

OpenGLServer::~OpenGLServer()
{
        SDL_Quit();
}

boost::shared_ptr<GLExtensionReg> OpenGLServer::GetExtensionReg() const
{
        return mExtensionReg;
}

void OpenGLServer::Quit()
{
        mWantsToQuit = true;
}

bool OpenGLServer::WantsToQuit() const
{
        return mWantsToQuit;
}

void OpenGLServer::Update()
{
    // Our SDL event placeholder.
    SDL_Event event;

    // Grab all the events off the queue.
    while( SDL_PollEvent( &event ) )
        {
        }
}

void OpenGLServer::SwapBuffers() const
{
        SDL_GL_SwapBuffers();
}

/*!
        Load an ARB program

        returns 0 on fail, non-zero on success
*/
unsigned int OpenGLServer::LoadARBProgram(GLenum target, const char* fileName)
{
#if 0
        // only try to load stuff if the extension is supported
        if (!mExtensionReg->Has_GL_ARB_vertex_program())
        {
                return 0;
        }

        // before actually loading, try the cache
        MapHolder::TProgramCache::iterator entry = mHolder->mPrograms.find(fileName);

        if (entry != mHolder->mPrograms.end())
        {
                // we already have a match
                return (*entry).second;
        }

        unsigned int id = 0;

        // open file
        shared_ptr<FileServer> fileServer = shared_static_cast<FileServer>(GetCore()->Get("/sys/server/file"));
        salt::RFile *file = fileServer->Open(fileName);

        if (!file) return 0;

        unsigned char *buffer = new unsigned char[file->Size()+1];
        file->Read(buffer, file->Size());

        glGenProgramsARB(1, &id);
        glBindProgramARB(target, id);

        // try to load the actual program
        glProgramStringARB(target, GL_PROGRAM_FORMAT_ASCII_ARB, file->Size(), buffer);

        // free memory
        delete file;
        delete []buffer;

        const unsigned char* error = glGetString(GL_PROGRAM_ERROR_STRING_ARB);

        // if an error occured, display error message
        if (error[0] != 0)
        {
                int i;
                glDeleteProgramsARB(1, &id);
                glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &i);
                GetCore()->GetLogServer()->Error() << "ERROR: Loading ARB program (Pos: " << i << ")..." << std::endl;
                GetCore()->GetLogServer()->Error() << "  => " << error << std::endl;
                return 0;
        }

        // enter program into the cache
        mHolder->mPrograms[fileName] = id;

        return id;
#else
	return 0;
#endif	
}

/*!
        Load a vertex program

        returns 0 on fail, non-zero on success
*/
unsigned int OpenGLServer::LoadARBVertexProgram(const char* fileName)
{
        // only try to load stuff if the extension is supported
        if (!mExtensionReg->Has_GL_ARB_vertex_program())
        {
                return 0;
        }

        return LoadARBProgram(GL_VERTEX_PROGRAM_ARB, fileName);
}

/*!
        Load a fragment program

        returns 0 on fail, non-zero on success
*/
unsigned int OpenGLServer::LoadARBFragmentProgram(const char* /*fileName*/)
{
        // only try to load stuff if the extension is supported
//      if (!mExtensionReg->Has_GL_ARB_fragment_program())
//      {
//              return 0;
//      }

//      return LoadARBProgram(GL_FRAGMENT_PROGRAM_ARB, fileName);
    return 0;
}

/*!
        Set up the OpenGL viewport, initialize extension registry
*/
bool OpenGLServer::ConstructInternal()
{
        if( SDL_Init(SDL_INIT_VIDEO) < 0 )
        {
                return false;
        }

        const SDL_VideoInfo* info = SDL_GetVideoInfo();

        int                     intValue;
        bool            boolValue;
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

        // check if fancy lighting is supported
        mSupportsFancyLighting = true;
//      if (!mExtensionReg->Has_GL_ARB_vertex_program() || !mExtensionReg->Has_GL_ARB_fragment_program())
//      {
                GetLog()->Normal() << "WARNING: GL_ARB_vertex_program not supported ... disabling fancy lighting" << std::endl;
                mSupportsFancyLighting = false;
//      }

        glClear(GL_COLOR_BUFFER_BIT);

        GetLog()->Normal() << "Initialized OpenGL Window" << std::endl;
        const unsigned char* val = NULL;
        val = glGetString(GL_RENDERER);
        GetLog()->Normal() << "  GL_RENDERER:   " << val << std::endl;
        val = glGetString(GL_VERSION);
        GetLog()->Normal() << "  GL_VERSION:    " << val << std::endl;
        val = glGetString(GL_EXTENSIONS);
        GetLog()->Normal() << "  GL_EXTENSIONS: " << val << std::endl << std::endl;

        return true;
}

void OpenGLServer::ToggleFancyLighting()
{
        if (mSupportsFancyLighting)
                mSupportsFancyLighting = false;
        else
                mSupportsFancyLighting = true;
}

