#ifndef MATERIAL_H__
#define MATERIAL_H__

#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

namespace kerosin
{

class Texture;
class MaterialServer;

class Material
{
	//
	// Functions
	//
public:
	Material(const boost::shared_ptr<MaterialServer> &materialServer);
	virtual ~Material();


	bool Load(const std::string& matName);

	void Bind();

	void BindAmbient();

	//
	// Members
	//
private:
	boost::weak_ptr<MaterialServer>	mMaterialServer;		// material server, which created this object
	boost::shared_ptr<Texture> mTexDiffuse;
	boost::shared_ptr<Texture> mTexNormal;
	boost::shared_ptr<Texture> mTexSpecular;
};

} //namespace kerosin

#endif //MATERIAL_H__
