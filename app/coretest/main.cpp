#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

using namespace boost;
using namespace std;
using namespace zeitgeist;

#ifdef GetObject
#undef GetObject
#endif

int main()
{
        Zeitgeist       zg;

        shared_ptr<CoreContext> context = zg.CreateContext();
        oxygen::Oxygen                          kOxygen(zg);

        shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(context->Get("/sys/server/script"));

        scriptServer->Run("sys/script/coretest.rb");

        cout << "CoreTest - A Small Interactive Text-Based Console Sample" << endl << endl;
        cout << "Enter 'exit' command to quit application" << endl << endl;
        bool done = false;
        while (!done)
        {
                std::string command = "";

                boost::shared_ptr<Leaf> selectedObject = scriptServer->GetContext()->GetObject();

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
