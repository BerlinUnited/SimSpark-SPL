#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

using namespace boost;
using namespace std;
using namespace zeitgeist;

int
main(int argc, const char *const *argv)
{
  // init zeitgeist
  Zeitgeist zg("." PACKAGE_NAME);

  // init oxygen
  oxygen::Oxygen kOxygen(zg);

  // run the init scripts
  shared_ptr<ScriptServer> scriptServer = zg.GetCore()->GetScriptServer();
  scriptServer->Run("spadestest.rb");

  // print a greeting
  cout << "SpadesTest - A Small Non-Interactive example" << endl << endl;

#ifdef HAVE_SPADES_HEADERS
  shared_ptr<oxygen::SpadesServer> spadesServer =
      shared_static_cast<oxygen::SpadesServer>(zg.GetCore()->Get("/sys/server/spades"));

  spades::SimulationEngineMain(argc, argv, spadesServer.get());
#endif

  return 0;
}
