#include "core.h"
#include "leaf.h"
#include "object.h"
#include "object_c.h"
#include "node.h"
#include "corecontext.h"
#include "fileserver/fileserver.h"
#include "fileserver/filesystem.h"
#include "logserver/logserver.h"
// #include "telnetserver/telnetserver.h"
#include "scriptserver/scriptserver.h"

#include <salt/path.h>
#include <salt/sharedlibrary.h>
#include <iostream>

using namespace boost;
using namespace salt;
using namespace std;
using namespace zeitgeist;

/*!
        Create the fundamental classes needed for the hierarchy to operate
*/
Core::Core() :
mClassClass(new CLASS(Class))
{
}

Core::~Core()
{
        cout << "~Core()" << endl;
        mRoot.reset();
        mFileServer.reset();
        mLogServer.reset();
        mScriptServer.reset();
        mNodeClass.reset();
        cout << "~Core() Freeing bundles" << endl;
        mBundles.clear();
        cout << "~Core() End" << endl;
}

void Core::Construct(const boost::weak_ptr<Core>& self)
{
        mSelf = self;

        // setup the node class
        mNodeClass.reset(new CLASS(Node));
        BindClass(mNodeClass);

        // create the root node
        mRoot = shared_static_cast<Leaf>(mNodeClass->Create());
        mRoot->SetName("");

        shared_ptr<CoreContext> context = CreateContext();

        // register the internal zeitgeist classes
        cout << "  Registering class objects...\n";
        RegisterClassObject(mNodeClass, "zeitgeist/");
        RegisterClassObject(mClassClass, "zeitgeist/");
        RegisterClassObject(new CLASS(Leaf), "zeitgeist/");
        RegisterClassObject(new CLASS(FileServer), "zeitgeist/");
        RegisterClassObject(new CLASS(FileSystem), "zeitgeist/");
        RegisterClassObject(new CLASS(LogServer), "zeitgeist/");
        RegisterClassObject(new CLASS(ScriptServer), "zeitgeist/");
#ifdef  THE_TELNETSERVER_COMPILES_FOR_OLIVER
        RegisterClassObject(new CLASS(TelnetServer), "zeitgeist/");
#endif
        // create the log server
        mLogServer = shared_static_cast<LogServer>(context->New("zeitgeist/LogServer", "/sys/server/log"));
        mLogServer->AddStream(&cout, LogServer::eAll);

        mLogServer->Normal() << "  Creating FileServer..." << endl;
        mFileServer = shared_static_cast<FileServer>(context->New("zeitgeist/FileServer", "/sys/server/file"));

        // create the log server
        mLogServer->Normal() << "  Creating ScriptServer..." << endl;
        mScriptServer = shared_static_cast<ScriptServer>(context->New("zeitgeist/ScriptServer", "/sys/server/script"));
}

boost::shared_ptr<CoreContext> Core::CreateContext()
{
        return shared_ptr<CoreContext>(new CoreContext(make_shared(mSelf), GetRoot()));
}

boost::shared_ptr<Object>
Core::New(const std::string& className)
{
        shared_ptr<CoreContext> context = CreateContext();
        // select the correct class to create our instance
        shared_ptr<Class>       theClass = shared_static_cast<Class>(context->Get("/classes/"+className));

        // here we will store our created instance
        shared_ptr<Object>      instance;
        if (theClass.get() != NULL)
        {
                // create the instance
                instance = theClass->Create();
        }

        return instance;
}

bool Core::RegisterClassObject(const boost::shared_ptr<Class> &classObject, const std::string &subDir)
{
        shared_ptr<CoreContext> context = CreateContext();

        BindClass(classObject);

        return context->Install(classObject, "/classes/" + subDir, true);
}

bool Core::RegisterClassObject(Class *classObject, const std::string &subDir)
{
        return RegisterClassObject(shared_ptr<Class>(classObject), subDir);
}

bool Core::ImportBundle(const std::string& bundleName)
{
        shared_ptr<SharedLibrary> bundle(new SharedLibrary());

        if (!bundle->Open(bundleName))
        {
                mLogServer->Error() << "ERROR: (Core) Could not open '" << bundleName << "'" << endl;
                return false;
        }

        std::list <shared_ptr<Class> > classes;
        void(*Zeitgeist_RegisterBundle)(std::list <shared_ptr<Class> > &) = NULL;

        Zeitgeist_RegisterBundle = (void(*)(std::list <shared_ptr<Class> > &))bundle->GetProcAddress("Zeitgeist_RegisterBundle");

        if (Zeitgeist_RegisterBundle == NULL)
        {
                mLogServer->Error() << "ERROR: (Core) No entry point!" << std::endl;
                return false;
        }

        Zeitgeist_RegisterBundle(classes);

        bool usingClass = false;
        while(!classes.empty())
        {
                //shared_ptr<Class> theClass(classes.back());
                if (RegisterClassObject(classes.back(), ""))
                {
                        classes.back()->SetBundle(bundle);
                        usingClass = true;
                }

                classes.pop_back();
        }

        // we only add the bundle, if it contained a class which was registered
        if (usingClass)
                mBundles.push_back(bundle);

        return true;
}

const boost::shared_ptr<FileServer>& Core::GetFileServer() const
{
        return mFileServer;
}

const boost::shared_ptr<LogServer>& Core::GetLogServer() const
{
        return mLogServer;
}

const boost::shared_ptr<ScriptServer>& Core::GetScriptServer() const
{
        return mScriptServer;
}

boost::shared_ptr<Leaf> Core::Get(const std::string &pathStr)
{
        return Get(pathStr, mRoot);
}

boost::shared_ptr<Leaf> Core::Get(const std::string &pathStr, const boost::shared_ptr<Leaf>& leaf)
{
        boost::shared_ptr<Leaf> current;
        Path path(pathStr);

        // check if we have a relative or absolute path
        if (path.IsAbsolute() || leaf.get() == NULL)
                current = mRoot;
        else
                current = leaf;

        while (!path.IsEmpty() && current.get()!=NULL)
        {
                //printf("%s\n", path.Front().c_str());
                current = current->GetChild(path.Front());
                path.PopFront();
        }

        if (current.get() == NULL)
        {
                mLogServer->Error() << "ERROR: Core::Get() - Could not find object '" << pathStr << "'" << std::endl;
        }

        return current;
}

boost::shared_ptr<Leaf> Core::GetChild(const boost::shared_ptr<Leaf> &parent, const std::string &childName)
{
        // if we have a leaf, then we can't get a child ... ever!!
        if (parent->IsLeaf()) return shared_ptr<Leaf>();

        // ok, no leaf, so let's try to get an existing child
        boost::shared_ptr<Leaf> child = parent->GetChild(childName);

        if (child.get() == NULL)
        {
                // if we hit this branch, then no child with the desired name exists,
                // but we will change this :)
                child = shared_static_cast<Leaf>(mNodeClass->Create());
                child->SetName(childName);

                if (parent->AddChildReference(child) == false)
                {
                        return shared_ptr<Leaf>();
                }
        }

        return child;
}

void Core::BindClass(const boost::shared_ptr<Class> &newClass) const
{
        if (newClass != mClassClass)
        {
                newClass->Construct(newClass, mClassClass);
                newClass->AttachTo(mSelf);
        }
}

template <class T>
struct isUnique: public unary_function<T, bool>
{
        bool operator()(const T& x){ return x.unique(); }
};

void Core::GarbageCollectBundles()
{
        // don't you just love the STL ;-)
        mBundles.erase(remove_if(mBundles.begin(), mBundles.end(), isUnique<shared_ptr<SharedLibrary> >()), mBundles.end());
}
