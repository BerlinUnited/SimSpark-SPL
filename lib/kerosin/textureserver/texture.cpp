#include "texture.h"
#include "../openglserver/openglserver.h"

using namespace boost;
using namespace kerosin;

Texture::Texture(const boost::shared_ptr<TextureServer> &textureServer) :
mTexID(0), mWidth(0), mHeight(0), mTextureServer(textureServer)
{
}

Texture::~Texture()
{
	Reset();
}

void Texture::Reset()
{
	if (mTexID)
	{
		glDeleteTextures(1, &mTexID);
		mTexID = 0;
	}
}

void Texture::Acquire()
{
	Reset();
	glGenTextures(1, &mTexID);
}

unsigned int Texture::GetWidth() const
{
	return mWidth;
}

unsigned int Texture::GetHeight() const
{
	return mHeight;
}

boost::shared_ptr<TextureServer> Texture::GetTextureServer() const
{
	return make_shared(mTextureServer);
}
