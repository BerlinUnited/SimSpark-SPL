/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: main.cpp,v 1.3.2.10 2004/01/31 15:05:05 rollmark Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Main of the rcssmonitor3D application

   This is a very simple 3D monitor example which will hopefully be
   used for the first steps of the 3D SoccerServer
*/
#include "glserver.h"
#include "commserver.h"
#include "types.h"
#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>

using namespace std;
using namespace boost;
using namespace zeitgeist;
using namespace oxygen;

#ifdef GetObject
#undef GetObject
#endif

//--------------------------Globals---------------------------------------

// open-gl server
GLserver gGLServer;

// communication Server
boost::shared_ptr<CommServer> gCommServer;

// monitor port
int gPort           = DEFAULT_PORT;

// server machine
char* gSoccerServer = DEFAULT_HOST;

// old mouse position
int gOldX           = DEFAULT_MOUSE_POSX;
int gOldY           = DEFAULT_MOUSE_POSY;

// automatic camera
bool gAutoCam       = DEFAULT_AUTO_CAM;

// window width and height
const int gWidth    = DEFAULT_WIDTH;
const int gHeight   = DEFAULT_HEIGHT;

// field Info:
// soccer field size
float gFieldLength  = DEFAULT_FIELD_LENGTH;
float gFieldWidth   = DEFAULT_FIELD_WIDTH;
float gFieldHeight  = DEFAULT_FIELD_HEIGHT;

float gBorderSize   = DEFAULT_BORDER_SIZE;

// goal box size
float gGoalWidth    = DEFAULT_GOAL_WIDTH;
float gGoalDepth    = DEFAULT_GOAL_DEPTH;
float gGoalHeight   = DEFAULT_GOAL_HEIGHT;

//--------------------------printHelp------------------------------------
//
// simply prints options to the screen
//-----------------------------------------------------------------------
void printHelp()
{
  cout << "\nusage: rcsserver3D [options]" << endl;
  cout << "\noptions:" << endl;
  cout << " --help\t print this message." << endl;
  cout << " --port\t sets the port number" << endl;
  cout << " --server\t sets the server name" << endl;
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
      if( strcmp( argv[i], "--server" ) == 0 )
        {
          if( i+1  < argc)
            {
              gSoccerServer = argv[i+1];
              ++i;
            }
        }
      else if( strcmp( argv[i], "--port" ) == 0 )
        {
          if( i+1 < argc )
            {
              gPort = atoi( argv[i+1] );
              ++i;
            }
        }
      else if( strcmp( argv[i], "--help" ) == 0 )
        {
          printHelp();
          exit(0);
        }
    }
}

//----------------------getObjectParam---------------------------------------
//
// Reads the parameters for an object with given name from given predicate
//---------------------------------------------------------------------------
bool getObjectParam(const Predicate& predicate, const string& name, float& value)
{
  // find the PerfectVision data about the object
  Predicate::Iterator objIter(predicate);

  // advance to the section about object 'name'
  if (! predicate.FindParameter(objIter,name))
    {
      return false;
    }

  // read the position vector
  if (! predicate.GetValue(objIter,value))
    {
      return false;
    }

  return true;
}

//----------------------parseInfoHeader------------------------------------
//
// parses the game parameters sent by the server during the init sequence
//-------------------------------------------------------------------------
bool parseInfoHeader(shared_ptr<Predicate::TList> predicates)
{
    // true if we received an init
    bool recvInit = false;

    // check if we received something
    if (predicates.get() == 0)
        {
            return false;
        }

    // first look for "(init (...))"
    // then read the inner breakets
    for (
         Predicate::TList::const_iterator iter = predicates->begin();
         iter != predicates->end();
         ++iter
         )
        {
            const Predicate& predicate = (*iter);

            // check if it's the init information
            // if so, remember that we received an init
            if (predicate.name != "init")
                {
                    continue;
                }
            else recvInit = true;

            //parse object params
            getObjectParam(predicate, "FieldLength", gFieldLength);
            getObjectParam(predicate, "FieldWidth",  gFieldWidth);
            getObjectParam(predicate, "FieldHeigth", gFieldHeight);
            getObjectParam(predicate, "GoalWidth",   gGoalWidth);
            getObjectParam(predicate, "GoalDepth",   gGoalDepth);
            getObjectParam(predicate, "GoalHeight",  gGoalHeight);
            getObjectParam(predicate, "BorderSize",  gBorderSize);
        }
    // parsing successfull
    return recvInit;
}

//----------------------drawScene----------------------------------------------
//
// Draws all object in given predicate onto the screen
//------------------------------------------------------------------------------
void drawScene(shared_ptr<Predicate::TList> predicates)
{
    static const struct ObjType
    {
        string type;
        float color[4];
        float size;
    } typeMap[] =
        {
            {
                "agent",
                {0.8f, 0.8f, 0.2f, 1.0f},
                0.3f
            },
            {
                "flag",
                {1.0f, 0, 0, 1.0f},
                0.5f
            },
            {
                "ball",
                {1.0f, 1.0f, 1.0f, 1.0f},
                0.111f
            },
            {
                "ballAgent",
                {1.0f, 0.5f, 0.5f, 1.0f},
                0.3f
            }
        };

    static int typeCount = sizeof(typeMap)/sizeof(ObjType);

    if (predicates.get() == 0)
        {
            return;
        }

    // color and size setup
    typedef GLfloat TColor[4];

    // look for "(player x y z)(player x y z)..."
    for (
         Predicate::TList::const_iterator iter = predicates->begin();
         iter != predicates->end();
         ++iter
         )
        {
            const Predicate& predicate = (*iter);

            int idx;
            for (idx=0;idx<typeCount;++idx)
                {
                    const ObjType& type = typeMap[idx];

                    if (type.type == predicate.name)
                        {
                            break;
                        }
                }

            if (idx == typeCount)
                {
                    continue;
                }

            Predicate::Iterator param(predicate);

            salt::Vector3f pos;
            if (! predicate.GetValue(param,pos))
                {
                    continue;
                }

            // if 'automatic-camera' is on
            // we set the camera to look at the ball
            if(predicate.name =="ball" && gAutoCam)
                    gGLServer.SetLookAtPos(pos);

            const ObjType& type = typeMap[idx];
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, type.color);
            gGLServer.DrawSphere(pos, type.size);
            gGLServer.DrawShadowOfSphere(pos, type.size);
        }
}

//---------------------------display--------------------------------------
//
// OpenGL display function:
// can be seen as the mainloop of the monitor
//------------------------------------------------------------------------
void display(void)
{
   static bool readInit = true;
   const Vector3f szGoal1(-gGoalDepth,gGoalHeight,gGoalWidth);
   const Vector3f szGoal2(gGoalDepth,gGoalHeight,gGoalWidth);

   // color constants
   const GLfloat groundColor[4] = {0.1f, 0.5f, 0.1f, 1.0f};
   const GLfloat goalColor[4]   = {1.0f, 1.0f, 1.0f, 1.0f};
   const GLfloat borderColor[4] = {0.2f, 0.8f, 0.2f, 1.0f};

   glClearColor(0.1f,0.1f,0.1f,1.0f);

   glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
   glColor3f (1, 1, 1);

   glLoadIdentity();

   gGLServer.ApplyCamera();

   // ground
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, groundColor);
   gGLServer.DrawGround(Vector3f(-gFieldLength/2,0,-gFieldWidth/2),gFieldLength,gFieldWidth);

   // border
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, borderColor);
   gGLServer.DrawGround(Vector3f(-gFieldLength/2-gBorderSize,0,-gFieldWidth/2-gBorderSize), gBorderSize, gFieldWidth+2*gBorderSize);
   gGLServer.DrawGround(Vector3f(gFieldLength/2,0,-gFieldWidth/2-gBorderSize), gBorderSize, gFieldWidth+2*gBorderSize);
   gGLServer.DrawGround(Vector3f(-gFieldLength/2,0,-gFieldWidth/2-gBorderSize), gFieldLength, gBorderSize);
   gGLServer.DrawGround(Vector3f(-gFieldLength/2,0,gFieldWidth/2), gFieldLength, gBorderSize);

   // fieldBox
   gGLServer.DrawWireBox(
                         Vector3f(-gFieldLength/2.0,0.0,-gFieldWidth/2.0),
                         Vector3f(gFieldLength,gFieldHeight,gFieldWidth)
                         );

   // goal
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goalColor);
   gGLServer.DrawWireBox(Vector3f(-gFieldLength/2,0,-gGoalWidth/2.0),szGoal1);
   gGLServer.DrawGoal(Vector3f(-gFieldLength/2,0,-gGoalWidth/2.0),szGoal1);

   // goal
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goalColor);
   gGLServer.DrawWireBox(Vector3f(gFieldLength/2,0,-gGoalWidth/2.0),szGoal2);
   gGLServer.DrawGoal(Vector3f(gFieldLength/2,0,-gGoalWidth/2.0),szGoal2);

   // check for positions update
   gCommServer->GetMessage();

   // get messages sent from server
   shared_ptr<Predicate::TList> predicates = gCommServer->GetPredicates();

   // if we still didn't parse the init string
   // we do so and set the readInit to 'false'
   // such that we don't have to parse it twice
   if (readInit)
       {
           if(parseInfoHeader(predicates))
               readInit = false;
       }

   // draw cached positions
   drawScene(predicates);

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

//------------------------------keyboad-------------------------------------
//
// processing of keyboard input
// Button Descr. :
//     + if 'w' is pressed the camera will move in
//     + if 's' is pressed the camera will move out
//     + if 'a' is pressed the camera will strafe left
//     + if 'd' is pressed the camera will strafe right
//     + if 'c' is pressed the camera will automatically follow the ball
//--------------------------------------------------------------------------
void keyboard(unsigned char key, int /*x*/, int /*y*/)
{
  const float camDelta = 0.5f;
  salt::Vector3f pos;
  switch (key) {
  case 'w':
      //move cam in
      gGLServer.MoveCamForward(camDelta);
      break;
  case 's':
      //move cam out
      gGLServer.MoveCamForward(-camDelta);
      break;
  case 'a':
      //strafe cam left
      gGLServer.MoveCamStrafe(camDelta);
      break;
  case 'd':
      // strafe cam right
      gGLServer.MoveCamStrafe(-camDelta);
      break;
  case 'c':
      //toggle autocam mode
      gAutoCam = !gAutoCam;
      cout <<"--- AUTOMATIC CAMERA ";
      cout << gAutoCam ? "ON" : "OFF";
      cout << " ----" << endl;
      break;
  default:
      break;
  }
}

int main(int argc, char* argv[])
{
  // init glut
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(gWidth, gHeight);
  glutCreateWindow("rcssmonitor3D");

  //setup the GLserver with camera coordinates
  salt::Vector3f pos(0.0, 7.0,24.0);
  salt::Vector3f lookAt(0.0,0.0,0.0);
  salt::Vector3f up(0.0,1.0,0.0);
  gGLServer = GLserver(gWidth, gHeight, pos, lookAt, up, false);
  gGLServer.InitGL();

  glutDisplayFunc(display);
  glutMotionFunc(mouseMotion);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);

  //init zeitgeist
  Zeitgeist zg("." PACKAGE_NAME);

  //init oxygen
  oxygen::Oxygen kOxygen(zg);

  // register rcsssmonitor3d classes to zeitgeist
  zg.GetCore()->RegisterClassObject(new CLASS(CommServer), "rcssmonitor3d/");

  // run init script
  zg.GetCore()->GetScriptServer()->Run("rcssmonitor3d.rb");

  // print a greeting
  zg.GetCore()->GetLogServer()->Normal()
    << "rcssmonitor3D version 0.2" << endl
    << "Copyright (C) 2003, Heni Ben Amor and Markus Rollmann, "
    << "Universität Koblenz." << endl
    << "Copyright (C) 2004, "
    << "The RoboCup Soccer Server Maintenance Group." << endl;

  // process command line
  processInput(argc, argv);

  zg.GetCore()->GetLogServer()->Normal()
    << "\nType '--help' for further information" << endl;

  // init the commserver
  gCommServer = shared_dynamic_cast<CommServer>
      (zg.GetCore()->New("rcssmonitor3d/CommServer"));

  if (gCommServer.get() == 0)
      {
          zg.GetCore()->GetLogServer()->Normal()
              << "ERROR: cannot create CommServer." << endl;

          return 1;
      }

  gCommServer->Init("SexpParser",gSoccerServer,gPort);

  // enter glut main loop
  glutMainLoop();

  return 0;
}
