#ifndef CORE_H__
#define CORE_H__

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace salt
{
class SharedLibrary;
}

namespace zeitgeist
{

class Class;
class Leaf;
class Node;
class FileServer;
class LogServer;
class ScriptServer;
class CoreContext;
class Object;

class Core
{
	//
	// functions
	//
public:
	Core();
	~Core();

	void Construct(const boost::weak_ptr<Core>& self);

	boost::shared_ptr<CoreContext> CreateContext();
	
	boost::shared_ptr<Object> New(const std::string& className);
	bool RegisterClassObject(const boost::shared_ptr<Class> &classObject, const std::string &subDir);
	bool RegisterClassObject(Class *classObject, const std::string &subDir);
	bool ImportBundle(const std::string& bundleName);

	const boost::shared_ptr<FileServer>&	GetFileServer() const;
	const boost::shared_ptr<LogServer>&		GetLogServer() const;
	const boost::shared_ptr<ScriptServer>&	GetScriptServer() const;

	boost::shared_ptr<Leaf> GetRoot() const	{	return mRoot;	}

	boost::shared_ptr<Leaf> Get(const std::string &pathStr);
	boost::shared_ptr<Leaf> Get(const std::string &pathStr, const boost::shared_ptr<Leaf>& base);
	boost::shared_ptr<Leaf> GetChild(const boost::shared_ptr<Leaf> &parent, const std::string &childName);

	void BindClass(const boost::shared_ptr<Class> &newClass) const;
	void GarbageCollectBundles();
	
	//
	// members
	//
private:
	boost::shared_ptr<Leaf>			mRoot;
	boost::shared_ptr<Class>		mNodeClass;
	boost::shared_ptr<Class>		mClassClass;
	boost::shared_ptr<FileServer>	mFileServer;
	boost::shared_ptr<LogServer>	mLogServer;
	boost::shared_ptr<ScriptServer>	mScriptServer;
	boost::weak_ptr<Core>			mSelf;
	std::list<boost::shared_ptr<salt::SharedLibrary> >	mBundles;
};

} //namespace zeitgeist

#endif //CORE_H__
