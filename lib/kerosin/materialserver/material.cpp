#include "material.h"
#include "materialserver.h"
#include "../textureserver/texture.h"
#include "../textureserver/textureserver.h"
#include "../openglserver/openglserver.h"
#include <zeitgeist/scriptserver/scriptserver.h>

using namespace boost;
using namespace kerosin;
using namespace std;
using namespace zeitgeist;

Material::Material(const boost::shared_ptr<MaterialServer> &materialServer):
mMaterialServer(materialServer)
{
}

Material::~Material()
{
}


bool Material::Load(const std::string& matName)
{
	shared_ptr<Core> theCore = make_shared(mMaterialServer)->GetCore();
	shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(theCore->Get("/sys/server/script"));
	shared_ptr<TextureServer> textureServer = shared_static_cast<TextureServer>(theCore->Get("/sys/server/texture"));

	if ((textureServer.get() == NULL) ||(scriptServer.get() == NULL) || !scriptServer->Run(matName)) return false;

	string diffuse, normal, specular;

	if(scriptServer->GetVariable("Material.Diffuse", diffuse) == false)
	{
		// backup - default diffuse texture
		diffuse = "sys/white.png";
	}
	mTexDiffuse = textureServer->GetTexture(diffuse);

	if(scriptServer->GetVariable("Material.Normal", normal) == false)
	{
		// backup - default normal texture
		normal = "sys/defaultNormal.png";
	}
	mTexNormal = textureServer->GetTexture(normal);

	if(scriptServer->GetVariable("Material.Specular", specular) == false)
	{
		// backup - default specular texture
		specular = diffuse;
	}
	mTexSpecular = textureServer->GetTexture(specular);
	return true;
}

void Material::Bind()
{
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	mTexDiffuse->Bind();
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	mTexNormal->Bind();
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
	mTexSpecular->Bind();
}

void Material::BindAmbient()
{
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	mTexDiffuse->Bind();
}
