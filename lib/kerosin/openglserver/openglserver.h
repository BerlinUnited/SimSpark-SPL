#ifndef OPENGLSERVER_H__
#define OPENGLSERVER_H__

#include <zeitgeist/class.h>
#include <zeitgeist/leaf.h>
#include "glextensionreg.h"

namespace kerosin
{

class OpenGLServer : public zeitgeist::Leaf
{
	//
	// types
	//
private:
	//! this structure will be used to map program names to OpenGL IDs
	typedef std::hash_map<std::string, unsigned int>	TProgramCache;

	//
	// functions
	//
public:
	OpenGLServer();
	~OpenGLServer();

	//! setup application window, initialize OpenGL, autoregister extensions
	bool								Init();

	boost::shared_ptr<GLExtensionReg>	GetExtensionReg() const;
	//! if this is called, the application will 'want to quit'
	void								Quit();
	//! true if somebody called 'Quit'
	bool								WantsToQuit() const;
	//! pump SDL event loop
	void								Update();
	//! swap opengl buffer
	void								SwapBuffers() const;

	//! vertex and fragment program loading
	unsigned int LoadARBProgram(GLenum target, const char* fileName);
	//! vertex and fragment program loading
	unsigned int LoadARBVertexProgram(const char* fileName);
	//! vertex and fragment program loading
	unsigned int LoadARBFragmentProgram(const char* fileName);

	bool	SupportsFancyLighting()	const	{	return mSupportsFancyLighting;	}

	void	ToggleFancyLighting();
protected:
	//! set up opengl viewport
	virtual	bool ConstructInternal();

	//
	// members
	//
private:
	//! this contains information on all available extensions
	boost::shared_ptr<GLExtensionReg>	mExtensionReg;
	//! a flag, which can be used to control the shutdown of the display window and the application
	bool								mWantsToQuit;
	//! cache of loaded vertex and fragment programs (assumes that path names to the programs are unique)
	TProgramCache						mPrograms;
	//! flag whether the OpenGL-driver can do fancy lighting or not
	bool								mSupportsFancyLighting;
};

DECLARE_CLASS(OpenGLServer);

} //namespace kerosin

#endif //OPENGLSERVER_H__
