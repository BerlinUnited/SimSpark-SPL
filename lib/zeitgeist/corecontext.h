#ifndef CORECONTEXT_H__
#define CORECONTEXT_H__

/*! \class CoreContext
	$Id: corecontext.h,v 1.2 2003/04/30 14:21:50 fruit Exp $
	
	CoreContext

	This class is responsible for representing a browsing context within
	the object hierarchy. It is usually created by the Core object.

	Why was the browsing context separated from the Core?

	Well, the usage scenario of the framework envisions multiple clients
	browsing the same object hierarchy. Therefore it was necessary to
	place the current browsing context into a distinct object.

	HISTORY:
		05.07.2002 MK
			- initial version

*/

#include <string>
#include <list>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace zeitgeist
{

class Core;
class Leaf;

class CoreContext
{
	//
	// types
	//
private:
	typedef std::list< boost::shared_ptr<Leaf> > TObjectStack;

	//
	// functions
	//
public:
	CoreContext(const boost::shared_ptr<Core> &core, const boost::shared_ptr<Leaf> &root);
	virtual ~CoreContext();

	boost::shared_ptr<Leaf>	New(const std::string& className, const std::string& pathStr);
	bool					Delete(const std::string& name);
	boost::shared_ptr<Leaf>	Select(const std::string& pathStr);
	bool					Install(const boost::shared_ptr<Leaf>& leaf, const std::string& pathStr, bool isNamed = false);
	boost::shared_ptr<Leaf>	Get(const std::string& pathStr);
	
	boost::shared_ptr<Leaf>	GetObject() const	{	return mObject;	}
	boost::shared_ptr<Core> GetCore() const		{	return mCore;	}

	void					ListObjects() const;

	//! push current active object on object stack
	void					Push();
	//! makes the top of the object stack the current object, no change if stack is empty
	void					Pop();
	//! lists the objects on the stack
	void					Dir() const;
	
private:
	CoreContext(const CoreContext& obj);
	CoreContext& operator=(const CoreContext& obj);

	//
	// members
	//
private:
	boost::shared_ptr<Core>		mCore;			// pointer to the core object, we are browsing
	boost::shared_ptr<Leaf>		mObject;		// the currently active object
	std::string					mPath;			// the current working path
	TObjectStack				mObjectStack;	// object stack
};

} // namespace zeitgeist

#endif //CORECONTEXT_H__
