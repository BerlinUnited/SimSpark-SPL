/**
 * Main of the rcssmonitor3D application
 *
 * Desc.  : This is a very simple 3D monitor example
 *          which will hopefully be used for the first steps
 *          of the 3D SoccerServer
 **/
#include "glserver.h"
#include "commserver.h"
#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

using namespace std;
using namespace boost;
using namespace zeitgeist;

#ifdef GetObject
#undef GetObject
#endif

////////////////////////////Globals//////////////////////////////////////
int gPort = 12001; // default spades monitor port
char* gSoccerServer("127.0.0.1");

//old mouse position
int gOldX, gOldY;

//screen width and height
const int gWidth  = 500;
const int gHeight = 500;

// Open-Gl server
GLserver gGLServer;

// Communication Server
CommServer gCommServer(gSoccerServer, gPort);

//--------------------------printHelp------------------------------------
//
// simply prints options to the screen
//-----------------------------------------------------------------------
void printHelp()
{
  cout << "\nusage: rcsserver3D [options]" << endl;
  cout << "\noptions:" << endl;
  cout << " -help\t print this message." << endl;
  cout << " -port\t sets the port number" << endl;
  cout << " -server\t sets the server name" << endl;
  cout << "\n";
}

//--------------------------processInput---------------------------------
//
// process input of options
//-----------------------------------------------------------------------
void processInput(int argc, char* argv[])
{
  for( int i = 0; i < argc; i++)
    {
      if( strcmp( argv[i], "-server" ) == 0 )
        {
          if( i+1  < argc)
            {
              gSoccerServer = argv[i+1];
              ++i;
            }
        }
      else if( strcmp( argv[i], "-port" ) == 0 )
        {
          if( i+1 < argc )
            {
              gPort = atoi( argv[i+1] );
              ++i;
            }
        }
      else if( strcmp( argv[i], "-help" ) == 0 )
        {
          printHelp();
        }
    }
}

//---------------------------display--------------------------------------
//
// OpenGL display function:
// can be seen as the mainloop of the monitor
//------------------------------------------------------------------------
void display(void)
{
   //OPEN GL STUFF
   //initialize colors
   GLfloat groundColor[4] = {0.0f, 0.9f, 0.0f, 1.0f};
   GLfloat teapotColor[4] = {0.8f, 0.8f, 0.2f, 1.0f};

   glClearColor(0.0f,0.0f,0.0f,1.0f);

   glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
   glColor3f (1, 1, 1);

   glLoadIdentity();

   gGLServer.ApplyCamera();

   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, groundColor);

   gGLServer.DrawGround();

   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, teapotColor);

   // check for positions update
   gCommServer.GetMessage();

   // draw cached positions
   const CommServer::TPositions& positions = gCommServer.GetPositions();

   for (
        CommServer::TPositions::const_iterator iter = positions.begin();
        iter != positions.end();
        ++iter
     )
     {
       gGLServer.DrawSphere(*iter, 1.0f);
     }

   glutSwapBuffers();
}

//----------------------------idle----------------------------------------
//
// defines what to do when no action is performed
//------------------------------------------------------------------------
void idle(void)
{
  glutPostRedisplay();
}

//--------------------------mouse-----------------------------------------
//
// when the mouse is first clicked we store the position
// for further use when adjusting the view angle
//------------------------------------------------------------------------
void mouse(int /*button*/, int /*state*/, int x, int y)
{
  gOldX = x;
  gOldY = y;
}

//--------------------------mouseMotion------------------------------------
//
// adjust view-cone of the camera using the current and old mouse pos
//-------------------------------------------------------------------------
void mouseMotion(int x, int y)
{
  Vector2f tmp1((float)x, (float)y);
  Vector2f tmp2((float)gOldX, (float)gOldY);
  gGLServer.SetViewByMouse(tmp1, tmp2);
  gOldX = x;
  gOldY = y;
}


int main(int argc, char* argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(gWidth, gHeight);
  glutCreateWindow("rcssmonitor3D");

  //setup the GLserver
  //with camera coordinates
  salt::Vector3f pos(-8.0, 10.0,4.0);
  salt::Vector3f lookAt(0.0,0.0,0.0);
  salt::Vector3f up(0.0,1.0,0.0);
  gGLServer = GLserver(gWidth, gHeight, pos, lookAt, up, false);
  gGLServer.InitGL();

  glutDisplayFunc(display);
  glutMotionFunc(mouseMotion);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);

  //init zeitgeist
  Zeitgeist zg("." PACKAGE_NAME);
  //init oxygen
  oxygen::Oxygen kOxygen(zg);
  //setting up a brwosing context
  shared_ptr<CoreContext> context = zg.CreateContext();
  shared_ptr<ScriptServer> scriptServer = shared_static_cast<ScriptServer>(context->Get("/sys/server/script"));

  scriptServer->Run("/sys/rcssmonitor3D.rb");
  // print a greeting
  cout << "rcssmonitor3D version 0.2" << endl;
  cout << "Copyright (c) 2003, Heni Ben Amor <amor@uni-koblenz.de> and Markus Rollmann <rollmark@uni-koblenz.de>\n The Robocup SoccerServer Maintainance Group" << endl;
  cout << "Type '--help' for further information" << endl;

  //process typed options
  processInput(argc, argv);

  glutMainLoop();
  return 0;
}
