/*	\class Texture
	$Id: texture.h,v 1.2 2003/08/22 22:43:16 rollmark Exp $

	This is the base class of all OpenGL based textures. In OpenGL a texture
	is represented by a so-called texture ID. This is a simple handle. The
	basic operations for creating/deleting this handle are contained in this
	class.

	Usually, textures are created via the texture server.

	NOTE:

	HISTORY:
		14.10.02 - MK
			- Initial version

	TODO:
		- support mipmap building (currently done only when SGIS_generate_mipmap is supported)

	TOFIX:
*/
#ifndef TEXTURE_H__
#define TEXTURE_H__

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace kerosin
{

class TextureServer;

class Texture
{
	//
	// functions
	//
public:
	Texture(const boost::shared_ptr<TextureServer> &textureServer);
	virtual ~Texture();
	
	//! release the associated OpenGL texture
	void Reset();

	//! acquire an OpenGL texture handle (texture not loaded)
	void Acquire();

	//! bind the texture contained in this object to the corresponding target (user code is responsible for setting correct enables and tex units)
	virtual void Bind() const = 0;
	//! enable the target associated with a texture type (e.g. GL_TEXTURE_2D)
	virtual void Enable() const = 0;
	//! disable the target associated with a texture type (e.g. GL_TEXTURE_2D)
	virtual void Disable() const = 0;
	virtual void Clamp() const = 0;
	virtual void ClampToEdge() const = 0;
	virtual void Repeat() const = 0;

	// accessors
	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

	boost::shared_ptr<TextureServer>	GetTextureServer() const;
	//
	// members
	//
protected:
	unsigned int					mTexID;				// OpenGL texture handle (initialized to 0)
	unsigned int					mWidth;				// width of texture
	unsigned int					mHeight;			// height of texture
private:
	boost::weak_ptr<TextureServer>	mTextureServer;		// texture server, which created this object
};

}

#endif //TEXTURE_H__
