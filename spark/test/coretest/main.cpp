#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

#if HAVE_KEROSIN_H
#include <kerosin/kerosin.h>
#endif

using namespace boost;
using namespace std;
using namespace zeitgeist;

#ifdef GetObject
#undef GetObject
#endif

int main()
{
    Zeitgeist zg("." PACKAGE_NAME);

    boost::shared_ptr<CoreContext> context = zg.CreateContext();
#if HAVE_KEROSIN_H
    kerosin::Kerosin kKerosin(zg);
#endif
    oxygen::Oxygen kOxygen(zg);

    boost::shared_ptr<ScriptServer> scriptServer = static_pointer_cast<ScriptServer>(context->Get("/sys/server/script"));

    scriptServer->Run("coretest.rb");

    cout << "CoreTest - A Small Interactive Text-Based Console Sample" << endl << endl;
    cout << "Enter 'exit' command to quit application" << endl << endl;
    bool done = false;
    while (!done)
    {
        std::string command = "";

        boost::shared_ptr<Leaf> selectedObject = scriptServer->GetContext()->GetSelection();

        cout << endl << selectedObject->GetFullPath() << "> ";
        getline(cin, command,'\n');

        if (command.compare("exit")==0)
        {
            done = true;
        }
        else
        {
            scriptServer->Eval(command.c_str());
        }
    }

    return 0;
}
