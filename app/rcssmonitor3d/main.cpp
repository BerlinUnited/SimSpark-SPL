/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: main.cpp,v 1.3.2.6 2004/01/26 13:35:29 fruit Exp $

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

////////////////////////////Globals//////////////////////////////////////

// monitor port
int gPort = DEFAULT_PORT;

// server machine
char* gSoccerServer = DEFAULT_HOST;

// old mouse position
int gOldX = 0;
int gOldY = 0;

// window width and height
const int gWidth  = DEFAULT_WIDTH;
const int gHeight = DEFAULT_HEIGHT;

// Open-Gl server
GLserver gGLServer;

// Communication Server
boost::shared_ptr<CommServer> gCommServer;

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

            const ObjType& type = typeMap[idx];
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, type.color);
            gGLServer.DrawSphere(pos, type.size);
        }
}

//---------------------------display--------------------------------------
//
// OpenGL display function:
// can be seen as the mainloop of the monitor
//------------------------------------------------------------------------
void display(void)
{
// Laenge       mindestens      100 m
//              hoechstens      110 m
// Breite       mindestens      64 m
//              hoechstens      75 m

   // soccer field size
   const float fieldLength = 105.0;
   const float fieldWidth = 68.0;
   const float fieldHeight = 20.0;

   const float borderSize = 4.0;

   // goal box size
   const float goalWidth = 7.32;
   const float goalDepth = 2.0;
   const float goalHeight = 2.44;

   const Vector3f szGoal1(-goalDepth,goalHeight,goalWidth);
   const Vector3f szGoal2(goalDepth,goalHeight,goalWidth);

   // color constants
   const GLfloat groundColor[4] = {0.0f, 0.9f, 0.0f, 1.0f};
   const GLfloat goalColor[4]   = {1.0f, 1.0f, 1.0f, 1.0f};
   const GLfloat borderColor[4] = {0.0f, 0.0f, 1.0f, 1.0f};

   glClearColor(0.1f,0.1f,0.1f,1.0f);

   glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
   glColor3f (1, 1, 1);

   glLoadIdentity();

   gGLServer.ApplyCamera();

   // ground
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, groundColor);
   gGLServer.DrawGround(Vector3f(-fieldLength/2,0,-fieldWidth/2),fieldLength,fieldWidth);

   // border
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, borderColor);
   gGLServer.DrawGround(Vector3f(-fieldLength/2-borderSize,0,-fieldWidth/2-borderSize), borderSize, fieldWidth+2*borderSize);
   gGLServer.DrawGround(Vector3f(fieldLength/2,0,-fieldWidth/2-borderSize), borderSize, fieldWidth+2*borderSize);
   gGLServer.DrawGround(Vector3f(-fieldLength/2,0,-fieldWidth/2-borderSize), fieldLength, borderSize);
   gGLServer.DrawGround(Vector3f(-fieldLength/2,0,fieldWidth/2), fieldLength, borderSize);

   // fieldBox
   gGLServer.DrawWireBox(
                         Vector3f(-fieldLength/2.0,0.0,-fieldWidth/2.0),
                         Vector3f(fieldLength,fieldHeight,fieldWidth)
                         );

   // goal
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goalColor);
   gGLServer.DrawWireBox(Vector3f(-fieldLength/2,0,-goalWidth/2.0),szGoal1);

   // goal
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goalColor);
   gGLServer.DrawWireBox(Vector3f(fieldLength/2,0,-goalWidth/2.0),szGoal2);

   // check for positions update
   gCommServer->GetMessage();

   // draw cached positions
   shared_ptr<Predicate::TList> predicates = gCommServer->GetPredicates();
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
