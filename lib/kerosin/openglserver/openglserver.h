#ifndef OPENGLSERVER_H__
#define OPENGLSERVER_H__

#include <zeitgeist/base.h>
#include "glextensionreg.h"

namespace kerosin
{

class OpenGLServer : public zeitgeist::Base
{
public:
	OpenGLServer();
	~OpenGLServer();

	bool Init();

	boost::shared_ptr<GLExtensionReg>	GetExtensionReg() const;
private:
	boost::shared_ptr<GLExtensionReg>	mExtensionReg;
};

DECLARE_CLASS(OpenGLServer);

} //namespace kerosin

#endif //OPENGLSERVER_H__