#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

using namespace boost;
using namespace std;
using namespace zeitgeist;

#ifdef GetObject
#undef GetObject
#endif

int
main(int argc, const char *const *argv)
{
  // init zeitgeist
  Zeitgeist zg("." PACKAGE_NAME);

  // setup a browsing context
  shared_ptr<CoreContext> context = zg.CreateContext();

  // init oxygen
  oxygen::Oxygen kOxygen(zg);

  // run the init scripts
  shared_ptr<ScriptServer> scriptServer
    = shared_static_cast<ScriptServer>(context->Get("/sys/server/script"));

  scriptServer->Run("spadestest.rb");

  // print a greeting
  cout << "SpadesTest - A Small Non-Interactive example" << endl << endl;

#ifdef HAVE_SPADES_HEADERS
  shared_ptr<oxygen::SpadesServer> spadesServer =
      shared_static_cast<oxygen::SpadesServer>(context->Get("/sys/server/spades"));

  spades::SimulationEngineMain(argc, argv, spadesServer.get());
#endif

  return 0;
}
