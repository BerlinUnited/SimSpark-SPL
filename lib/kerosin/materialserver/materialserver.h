#ifndef MATERIALSERVER_H__
#define MATERIALSERVER_H__

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>

namespace kerosin
{

class Material;

class MaterialServer : public zeitgeist::Leaf
{
	//
	// types
	//
private:
	typedef std::hash_map<std::string, boost::shared_ptr<Material> >	TMaterialCache;

	//
	// functions
	//
public:
	MaterialServer();
	virtual ~MaterialServer();

	//! load (or returned cached) material
	boost::shared_ptr<Material>		GetMaterial(const std::string &name);

	//! make sure that a material is in memory
	bool							Touch(const std::string &name);
protected:
	//! sets up the script variables used for loading
	virtual bool ConstructInternal();
	
	//
	// members
	//
private:
	TMaterialCache	mMaterialCache;		// registry of cached textures
};

DECLARE_CLASS(MaterialServer);

} //kerosin

#endif //MATERIALSERVER_H__
