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
  cout << "SpadesTest - A Small Interactive Text-Based Console Sample" << endl << endl;
  cout << "Enter 'exit' command to quit application" << endl << endl;

#ifdef HAVE_SPADES_HEADERS
  shared_ptr<oxygen::SpadesServer> spadesServer =
      shared_static_cast<oxygen::SpadesServer>(context->Get("/sys/server/spades"));
#endif

  // loop until the 'exit' command
  bool done = false;
  while (!done)
    {
      // update the scene by delta seconds
      float delta = 0.1f;

      shared_ptr<oxygen::SceneServer> sceneServer =
        shared_static_cast<oxygen::SceneServer>(context->Get("/sys/server/scene"));
      if (sceneServer.get() != NULL)
        {
          sceneServer->Update(delta);
          cout << "updated the scene by " << delta << " seconds." << endl;
        }

      // print the current location of the sphere collider
      shared_ptr<oxygen::BaseNode> sphereNode =
        shared_static_cast<oxygen::BaseNode>(context->Get("/usr/scene/sphere"));

      if (sphereNode.get() != NULL)
        {
          const salt::Matrix& transform = sphereNode->GetWorldTransform();
          const salt::Vector3f& pos = transform.Pos();
          cout << "found the sphereNode at " << pos[0] << "," << pos[1] << "," << pos[2] << endl;
        }

      // read and execute ruby commands
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
