#include "textureserver.h"
#include <zeitgeist/logserver/logserver.h>
#include "../openglserver/openglserver.h"
#include "../imageserver/imageserver.h"
#include "texture2d.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

TextureServer::TextureServer() : Leaf()
{
}

TextureServer::~TextureServer()
{
}

bool TextureServer::Init(const std::string &openglPath)
{
        mOpenGLServer = shared_static_cast<OpenGLServer>(GetCore()->Get(openglPath));

        if (mOpenGLServer.get() != NULL)
        {
                GetLog()->Error()       << "ERROR - TextureServer::Init(): Requires OpenGLServer under '"
                                                        << openglPath << "'" << std::endl;
                return false;
        }

        return true;
}

boost::shared_ptr<OpenGLServer> TextureServer::GetOpenGLServer() const
{
        return mOpenGLServer;
}

boost::shared_ptr<Texture> TextureServer::GetTexture(const std::string &name)
{
        TTextureCache::iterator entry = mTextureCache.find(name);

        if (entry != mTextureCache.end())
        {
                // we already have a match
                return (*entry).second;
        }

        shared_ptr<Texture> texture;

        // no match for that name, so we have to load it
        shared_ptr<ImageServer> imageServer = shared_static_cast<ImageServer>(GetCore()->Get("/sys/server/image"));
        shared_ptr<Image> image = imageServer->Load(name.c_str());

        if (image.get())
        {
                Texture2D *tex2D = new Texture2D(shared_static_cast<TextureServer>(make_shared(GetSelf())));
                tex2D->Create(image);
                texture.reset(tex2D);
        }

        // register the texture, so we will find it later
        mTextureCache[name] = texture;
        return texture;
}
