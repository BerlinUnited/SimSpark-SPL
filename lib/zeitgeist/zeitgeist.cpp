#include "zeitgeist.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace zeitgeist;

Zeitgeist::Zeitgeist(string dotName) :
mCore(new Core())
{
        cout << "Starting up Zeitgeist..." << endl;
        mCore->Construct(mCore);

        if (mCore->GetScriptServer() != NULL)
          {
            // setup the dot directory in the script server
            mCore->GetScriptServer()->SetDotName(dotName);

            // run the zeitgeist init script
            mCore->GetScriptServer()->RunInitScript
              (
               "zeitgeist.rb",
               "../../lib/zeitgeist"
               );
          }
}

Zeitgeist::~Zeitgeist()
{
        cout << "Shutting down Zeitgeist..." << endl;
}

boost::shared_ptr<CoreContext>  Zeitgeist::CreateContext()
{
        return mCore->CreateContext();
}

boost::shared_ptr<Core>& Zeitgeist::GetCore()
{
        return mCore;
}
