#ifndef TEXTURESERVER_H__
#define TEXTURESERVER_H__

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

#ifdef HAVE_HASH_MAP
#include <hash_map>
#else
#include <map>
#endif

namespace kerosin
{

class OpenGLServer;
class Texture;

class TextureServer : public zeitgeist::Leaf
{
        //
        // types
        //
private:
#ifdef HAVE_HASH_MAP
        typedef std::hash_map<std::string, boost::shared_ptr<Texture> > TTextureCache;
#else
        typedef std::map<std::string, boost::shared_ptr<Texture> > TTextureCache;
#endif


        //
        // functions
        //
public:
        TextureServer();
        virtual ~TextureServer();

        //! initializes the TextureServer ... checks for the existance of the OpenGLServer
        bool Init(const std::string &openglPath);
        //! retrieve pointer to the OpenGL server ... used by Textures to check extensions
        boost::shared_ptr<OpenGLServer> GetOpenGLServer() const;
        //! load (or returned cached) texture
        boost::shared_ptr<Texture>              GetTexture(const std::string &name);

        //
        // members
        //
private:
        boost::shared_ptr<OpenGLServer> mOpenGLServer;
        TTextureCache                                   mTextureCache;          // registry of cached textures
};

DECLARE_CLASS(TextureServer);

} //kerosin

#endif //TEXTURESERVER_H__
