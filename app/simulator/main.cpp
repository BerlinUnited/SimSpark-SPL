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
    timeval tv;
    gettimeofday(&tv, 0);
    srand(tv.tv_usec);

    // print a greeting
    cout << PACKAGE_STRING << "\n"
         << "Copyright (C) 2002, 2003 Koblenz University.\n"
         << "2003, 2004 RoboCup Soccer Server 3D Maintenance Group.\n\n";

    // init zeitgeist
    Zeitgeist zg("." PACKAGE_NAME);

    // setup a browsing context
    shared_ptr<CoreContext> context = zg.CreateContext();

    // init oxygen
    oxygen::Oxygen kOxygen(zg);

    // run the init scripts
    shared_ptr<ScriptServer> scriptServer
        = shared_static_cast<ScriptServer>(context->Get("/sys/server/script"));

    scriptServer->Run("simulator.rb");

#ifdef HAVE_SPADES_HEADERS
    shared_ptr<oxygen::SpadesServer> spadesServer =
        shared_static_cast<oxygen::SpadesServer>(context->Get("/sys/server/spades"));

    spades::SimulationEngineMain(argc, argv, spadesServer.get());
#endif

    return 0;
}
