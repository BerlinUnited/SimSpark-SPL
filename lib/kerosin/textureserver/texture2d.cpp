#include "texture2d.h"
#include "../openglserver/openglserver.h"
#include "../imageserver/image.h"

using namespace kerosin;

Texture2D::Texture2D(const boost::shared_ptr<TextureServer> &textureServer) : Texture(textureServer)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::Bind() const
{
	if (mTexID != 0)
	{
		glBindTexture(GL_TEXTURE_2D, mTexID);
	}
}

void Texture2D::Enable() const
{
	glEnable(GL_TEXTURE_2D);
}

void Texture2D::Disable() const
{
	glDisable(GL_TEXTURE_2D);
}

void Texture2D::Create(boost::shared_ptr<Image> &image)
{
	mWidth		= image->Width();
	mHeight		= image->Height();

	Acquire();
	Bind();

	glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE );
	if(image->HasAlpha())
		glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGBA8, mWidth, mHeight, 0, image->Format(), image->Type(), image->Data());
	else
		glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGB8, mWidth, mHeight, 0, image->Format(), image->Type(), image->Data());

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
}

void Texture2D::Clamp() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void Texture2D::ClampToEdge() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture2D::Repeat() const
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}