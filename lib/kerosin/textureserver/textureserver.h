#ifndef TEXTURESERVER_H__
#define TEXTURESERVER_H__

#include <zeitgeist/leaf.h>

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
	typedef std::hash_map<std::string, boost::shared_ptr<Texture> >	TTextureCache;

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
	boost::shared_ptr<Texture>		GetTexture(const std::string &name);
	
	//
	// members
	//
private:
	boost::shared_ptr<OpenGLServer>	mOpenGLServer;
	TTextureCache					mTextureCache;		// registry of cached textures
};

DECLARE_CLASS(TextureServer);

} //kerosin

#endif //TEXTURESERVER_H__