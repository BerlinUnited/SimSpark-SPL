#ifndef TEXTURE2D_H__
#define TEXTURE2D_H__

#include "texture.h"

namespace kerosin
{

class Image;

class Texture2D : public Texture
{
	//
	// functions
	//
public:
	Texture2D(const boost::shared_ptr<TextureServer> &textureServer);
	~Texture2D();

	//! bind the texture contained in this object to the corresponding target (user code is responsible for setting correct enables and tex units)
	virtual void Bind() const;
	//! enable the target associated with a texture type (e.g. GL_TEXTURE_2D)
	virtual void Enable() const;
	//! disable the target associated with a texture type (e.g. GL_TEXTURE_2D)
	virtual void Disable() const;
	virtual void Clamp() const;
	virtual void ClampToEdge() const;
	virtual void Repeat() const;

	void Create(boost::shared_ptr<Image> &image);
};

} //kerosin

#endif //TEXTURE2D_H__