/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: main.cpp,v 1.2.2.4 2003/12/30 14:10:21 fruit Exp $

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
   useful for the first steps of 3D SoccerServer
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
    if (predicates.get() == 0)
        {
            return;
        }

    // look for "(player x y z)(player x y z)..."
    for (
         Predicate::TList::const_iterator iter = predicates->begin();
         iter != predicates->end();
         ++iter
         )
        {
            const Predicate& predicate = (*iter);

            if (predicate.name != "player")
                {
                    continue;
                }

            Predicate::TParameterList::const_iterator param
                = predicate.parameter.begin();

            salt::Vector3f pos;
            if (! predicate.GetValue(param,pos))
                {
                    continue;
                }

            gGLServer.DrawSphere(pos, 1.0f);
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

  // register rcsssmonitor3d classes to zeitgeist
  zg.GetCore()->RegisterClassObject(new CLASS(CommServer), "rcssmonitor3d/");

  // run init script
  zg.GetCore()->GetScriptServer()->RunInitScript("rcssmonitor3D.rb", "../rcssmonitor3d");

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
