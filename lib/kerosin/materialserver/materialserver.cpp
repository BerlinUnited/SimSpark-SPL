#include "materialserver.h"
#include <zeitgeist/logserver/logserver.h>
#include <zeitgeist/scriptserver/scriptserver.h>
#include "material.h"

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

MaterialServer::MaterialServer() :
Leaf()
{
}

MaterialServer::~MaterialServer()
{
}


boost::shared_ptr<Material> MaterialServer::GetMaterial(const std::string &name)
{
	TMaterialCache::iterator entry = mMaterialCache.find(name);

	if (entry != mMaterialCache.end())
	{
		// we already have a match
		return (*entry).second;
	}

	shared_ptr<Material> material(new Material(shared_static_cast<MaterialServer>(make_shared(GetSelf()))));

	if (material->Load(name))
	{
		// register the texture, so we will find it later
		mMaterialCache[name] = material;
	}
	else
	{
		material = shared_ptr<Material>();
	}

	return material;
}

bool MaterialServer::Touch(const std::string &name)
{
	return (GetMaterial(name).get() != NULL);
}

// set up the script variables used for loading
bool MaterialServer::ConstructInternal()
{
	shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(GetCore()->Get("/sys/server/script"));

	if (scriptServer.get() == NULL)
	{
		GetLog()->Error() << "ERROR: Could not locate ScriptServer..." << std::endl;
		GetLog()->Error() << "ERROR: Could not construct MaterialServer..." << std::endl;
		return false;
	}

	scriptServer->CreateVariable("Material.Diffuse",	"");
	scriptServer->CreateVariable("Material.Normal",		"");
	scriptServer->CreateVariable("Material.Specular",	"");

	return true;
}