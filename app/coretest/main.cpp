#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>

using namespace boost;
using namespace kerosin;
using namespace zeitgeist;

int main()
{
	Zeitgeist	zg;

	shared_ptr<CoreContext> context = zg.CreateContext();
	Kerosin					kCore(zg);
	
	shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(context->Get("/sys/server/script"));
 
	scriptServer->Run("coretest.rb");

	bool done = false;
	while (!done)
	{
		std::string command = "";
	
		//std::cout << std::endl << scriptServer->GetContext()->GetObject()->GetFullPath() << "> ";
		std::getline(std::cin, command,'\n');

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
