#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

using namespace boost;
using namespace std;
using namespace zeitgeist;

int
main(int argc, const char *const *argv)
{
    // print a greeting
    cout << PACKAGE_STRING << "\n"
         << "Copyright (C) 2002, 2003 Koblenz University.\n"
         << "2003, 2004 RoboCup Soccer Server 3D Maintenance Group.\n\n";

    // init zeitgeist
    Zeitgeist zg("." PACKAGE_NAME);

    // init oxygen
    oxygen::Oxygen kOxygen(zg);

    // run the init scripts
    shared_ptr<ScriptServer> scriptServer = zg.GetCore()->GetScriptServer();
    scriptServer->Run("simulator.rb");

#ifdef HAVE_SPADES_HEADERS
    shared_ptr<oxygen::SpadesServer> spadesServer =
      shared_static_cast<oxygen::SpadesServer>(zg.GetCore()->Get("/sys/server/spades"));

    spades::SimulationEngineMain(argc, argv, spadesServer.get());
#endif

    return 0;
}
