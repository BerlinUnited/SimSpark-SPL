#ifndef SCRIPTSERVER_H__
#define SCRIPTSERVER_H__

/*! \class ScriptServer
	$Id: scriptserver.h,v 1.3 2003/06/03 14:15:06 fruit Exp $
	
	ScriptServer
	:TODO: Class description for ScriptServer

	HISTORY:
		09.07.2002 MK
			- initial version

*/

#include <zeitgeist/leaf.h>
#include <zeitgeist/class.h>

#ifndef __GNUC__
#define EXTERN extern __declspec(dllimport)
#endif
#include <ruby/ruby.h>
#undef EXTERN

namespace zeitgeist
{

class CoreContext;

class ScriptServer : public Leaf
{
	//
	// types
	//
public:
protected:
private:

	//
	// functions
	//
public:
	ScriptServer();
	virtual ~ScriptServer();

	bool Run(const std::string &fileName);
	bool Eval(const std::string &command);
	
	// script variable functions
	void CreateVariable(const std::string &varName, int value);
	void CreateVariable(const std::string &varName, float value);
	void CreateVariable(const std::string &varName, const std::string &value);
	bool GetVariable(const std::string &varName, int &value);
	bool GetVariable(const std::string &varName, bool &value);
	bool GetVariable(const std::string &varName, std::string &value);

	boost::shared_ptr<CoreContext> GetContext() const;

protected:
	//! initialization
	virtual bool ConstructInternal();
		
private:
	ScriptServer(const ScriptServer& obj);
	ScriptServer& operator=(const ScriptServer& obj);

	VALUE GetVariable(const std::string &varName);

	//
	// members
	//
public:
protected:
private:
};

DECLARE_CLASS(ScriptServer);

} //namespace zeitgeist

#endif //SCRIPTSERVER_H__
