#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>
#include <getopt.h>

using namespace boost;
using namespace zeitgeist;

int
//main(int argc, const char *const *argv)
main(int argc, char *const *argv)
{
    // print a greeting
    std::cout << PACKAGE_STRING << "\n"
              << "Copyright (C) 2002, 2003 Koblenz University.\n"
              << "2003, 2004 RoboCup Soccer Server 3D Maintenance Group.\n\n";

    // init zeitgeist
    Zeitgeist zg("." PACKAGE_NAME);

    // init oxygen
    oxygen::Oxygen kOxygen(zg);

    
    std::string initScript="rcssserver3D.rb";

    int option_index;
    int c = 0;

    
    
    // read some command line options
    option long_options[] = {
            // name, has_arg, flag, val
            { "help", no_argument, 0, 'h' },
            { "initscript", required_argument, 0, 'i' },
            { 0, 0, 0, 0 }
    };

    while (true)
    {
        c = getopt_long(argc, argv, "", long_options, &option_index);

        if (c == -1) break;

        switch (c)
        {
        case 0: // a long option with a non-NULL flag has been given.
            break;
        case 'h': // --help
                //std::cout << "You asked for help but you are on your own." << std::endl;
            std::cout << "------------------------------------------------------------------------" << std::endl;
            std::cout << " Usage: rcssserver3D [--help|--initscript <script>] -- <SPADES options>\n" << std::endl;
            std::cout << " --help: Show this help." << std::endl;
            std::cout << " --initscript <script>: Run <script> instead of rcssserver3D.rb" << std::endl;
            std::cout << "   <script> must reside in your ~/.rcssserver3d directory.  If" << std::endl;
            std::cout << "   <script> is not found, the default (rcssserver3D.rb) is loaded." << std::endl;
            std::cout << "\n Options that should be passed to SPADES follow after \'--\'." << std::endl;
            std::cout << " For a list of options refer to the SPADES manual." << std::endl;
            std::cout << "------------------------------------------------------------------------\n" << std::endl;
            exit(1);  //break;
        case 'i': // --initscript
            std::cout << "Using " << std::string(optarg)
                      << " as init script" << std::endl;
            initScript=std::string(optarg);
            break;
        default:
            std::cout << "IGNORING!" << std::endl;
        }
    }


        
    
    // run the init scripts
    shared_ptr<ScriptServer> scriptServer = zg.GetCore()->GetScriptServer();
    if (!scriptServer->RunInitScript(initScript, ""))
    {
            std::cout << "Running "<< initScript << " failed. "
                      << "Running rcssserver3D.rb instead." << std::endl;
            
            scriptServer->RunInitScript("rcssserver3D.rb", "app/simulator");
    }
    
    
#ifdef HAVE_SPADES_HEADERS
    int spadesArgc = (argc - optind + 1);
    char *const *spadesArgv = &argv[optind-1];
    
    shared_ptr<oxygen::SpadesServer> spadesServer =
      shared_static_cast<oxygen::SpadesServer>(zg.GetCore()->Get("/sys/server/spades"));

    spades::SimulationEngineMain(spadesArgc, spadesArgv, spadesServer.get());
#endif

    return 0;
}
