/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
   $Id: main.cpp,v 1.13 2004/04/20 12:46:17 fruit Exp $

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
#include <sstream>
#include <zeitgeist/zeitgeist.h>
#include <oxygen/oxygen.h>
#include <soccer/soccertypes.h>
#include <commserver.h>
#include <monitorlib.h>
#include <monitorparser.h>
#include <types.h>
#include "glserver.h"

using namespace std;
using namespace boost;
using namespace zeitgeist;
using namespace oxygen;

#ifdef GetObject
#undef GetObject
#endif

//--------------------------Globals---------------------------------------
//
// simply prints options to the screen
//-----------------------------------------------------------------------

// open-gl server
GLServer gGLServer;

// communication Server
boost::shared_ptr<CommServer> gCommServer;

// last parsed expressions
MonitorParser::TExprList gExprList;

// monitor port
int gPort           = DEFAULT_PORT;

// server machine
char* gSoccerServer = DEFAULT_HOST;

// old mouse position
int gOldX           = DEFAULT_MOUSE_POSX;
int gOldY           = DEFAULT_MOUSE_POSY;

// automatic camera
bool gAutoCam       = DEFAULT_AUTO_CAM;

// draw player unums
bool gDrawUnums     = DEFAULT_DRAW_UNUMS;

// window width and height
const int gWidth    = DEFAULT_WIDTH;
const int gHeight   = DEFAULT_HEIGHT;

// camera step size
const float gCamDelta = 0.5f;

// game state data
GameState gGameState;

// game parameter data
GameParam gGameParam;

// the parser instance
shared_ptr<MonitorParser> gParser;

// sphere types description
struct SphereType
{
    float color[4];
    float size;

    SphereType(const float* c, float s)
    {
        size = s;
        for (int i=0;i<4;++i) color[i] = c[i];
    }

    SphereType()
    {
        for (int i=0;i<4;++i) color[i] = 1.0f;
        size = 1.0f;
    }

};

// mapping from a MonitorParser expression type to a sphere description
typedef std::map<MonitorParser::EExprType, SphereType> TSphereMap;
TSphereMap gSphereMap;

// color for player of the different teams
const float gTeamLColor[4] = {1.0f, 0.2f, 0.2f, 1.0f};
const float gTeamRColor[4] = {0.2f, 0.2f, 1.0f, 1.0f};

//--------------------------printHelp------------------------------------
//
// simply prints options to the screen
//-----------------------------------------------------------------------
void
printHelp()
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
void
processInput(int argc, char* argv[])
{
    for( int i = 0; i < argc; i++)
    {
        if( strcmp( argv[i], "--server" ) == 0 )
        {
            if( i+1  < argc)
            {
                gSoccerServer = argv[i+1];
                ++i;
                cout << "server set to " << gSoccerServer << "\n";
            }
        }
        else if( strcmp( argv[i], "--port" ) == 0 )
        {
            if( i+1 < argc )
            {
                gPort = atoi( argv[i+1] );
                ++i;
                cout << "port set to " << gPort << "\n";
            }
        }
        else if( strcmp( argv[i], "--help" ) == 0 )
        {
            printHelp();
            exit(0);
        }
    }
}

//---------------------------drawObject------------------------------------------
//
// draws a sphere object
//-------------------------------------------------------------------------------
void
drawObject(const SphereType& type, const MonitorParser::Expression& expr)
{
    const float* color = type.color;

    Vector3f pos = expr.pos;

    switch (expr.etype)
    {
    case MonitorParser::ET_BALL:
        // if 'automatic-camera' is on
        // we set the camera to look at the ball
        if (gAutoCam)
        {
            gGLServer.SetLookAtPos(pos);
        }
        break;

    case MonitorParser::ET_AGENT:
        // team
        switch (expr.team)
        {
        case TI_LEFT:
            color = gTeamLColor;
            break;

        case TI_RIGHT:
            color = gTeamRColor;
            break;

        default:
            // keep default color
            break;
        }
        break;

    case MonitorParser::ET_FLAG:
        if (salt::gAbs(pos[1]) == gGameParam.GetGoalWidth() / 2.0) return;
        pos[2] = 1.5;
        break;

    default:
        return;
    }

    glColor4fv(color);
    gGLServer.DrawSphere(pos, type.size);
    gGLServer.DrawShadowOfSphere(pos, type.size);

    if (
        (gDrawUnums) &&
        (expr.unum > 0)
        )
    {
        glColor3f   ( 1.0, 1.0, 1.0  );
        stringstream ss;
        ss << expr.unum;
        gGLServer.DrawText3D(ss.str().c_str(),expr.pos);
    }
}

//---------------------------DrawStatusText--------------------------------------
//
// draws a text, describing the current game state
//-------------------------------------------------------------------------------
void
drawStatusText()
{
    stringstream sl, sc, sr;

    sl << gGameState.GetTeamL() << " " << gGameState.GetScoreL();
    sr << gGameState.GetScoreR() << " " << gGameState.GetTeamR();
    sc << "(" << ((gGameState.GetHalf() == GH_FIRST) ?
                  "first" : "second") << " half) ";

    std::string mode;

    switch (gGameState.GetPlayMode())
    {
    case PM_BeforeKickOff:
        mode = STR_PM_BeforeKickOff;
        break;
    case PM_KickOff_Left:
        mode = STR_PM_KickOff_Left;
        break;
    case PM_KickOff_Right:
        mode = STR_PM_KickOff_Right;
        break;
    case PM_PlayOn:
        mode = STR_PM_PlayOn;
        break;
    case PM_KickIn_Left:
        mode = STR_PM_KickIn_Left;
        break;
    case PM_KickIn_Right:
        mode = STR_PM_KickIn_Right;
        break;
    case PM_CORNER_KICK_LEFT:
        mode = STR_PM_CORNER_KICK_LEFT;
        break;
    case PM_CORNER_KICK_RIGHT:
        mode = STR_PM_CORNER_KICK_RIGHT;
        break;
    case PM_GOAL_KICK_LEFT:
        mode = STR_PM_GOAL_KICK_LEFT;
        break;
    case PM_GOAL_KICK_RIGHT:
        mode = STR_PM_GOAL_KICK_RIGHT;
        break;
    case PM_OFFSIDE_LEFT:
        mode = STR_PM_OFFSIDE_LEFT;
        break;
    case PM_OFFSIDE_RIGHT:
        mode = STR_PM_OFFSIDE_RIGHT;
        break;
    case PM_GameOver:
        mode = STR_PM_GameOver;
        break;
    case PM_Goal_Left:
        mode = STR_PM_Goal_Left;
        break;
    case PM_Goal_Right:
        mode = STR_PM_Goal_Right;
        break;
    case PM_FREE_KICK_LEFT:
        mode = STR_PM_FREE_KICK_LEFT;
        break;
    case PM_FREE_KICK_RIGHT:
        mode = STR_PM_FREE_KICK_RIGHT;
        break;
    default:
        mode = STR_PM_Unknown;
        break;
    };

    sc << mode << " ";
    sc << "t=" << gGameState.GetTime();

    glColor3f(1.0, 1.0, 1.0);
    gGLServer.DrawTextPix(sl.str().c_str(),Vector2f( 0, gGLServer.GetTextHeight()));
    gGLServer.DrawTextPix(sc.str().c_str(),Vector2f( 0, gGLServer.GetTextHeight()), GLServer::eCENTER);
    gGLServer.DrawTextPix(sr.str().c_str(),Vector2f( 0, gGLServer.GetTextHeight()), GLServer::eRIGHT);
}

//----------------------drawScene----------------------------------------------
//
//-------------------------------------------------------------------------------

void
drawScene()
{
    for (
        MonitorParser::TExprList::iterator iter = gExprList.begin();
        iter != gExprList.end();
        ++iter)
    {
        MonitorParser::Expression& expr = (*iter);

        TSphereMap::iterator iter = gSphereMap.find(expr.etype);

        if (iter == gSphereMap.end())
        {
            // no corresponding sphere description found,
            // unknown entry or a gamestate expression
            continue;
        }

        drawObject((*iter).second,expr);
    }
}

//---------------------------display--------------------------------------
//
// OpenGL display function:
// can be seen as the mainloop of the monitor
//------------------------------------------------------------------------
void
display(void)
{
    const Vector3f szGoal1(
        -gGameParam.GetGoalDepth(),
        gGameParam.GetGoalWidth(),
        gGameParam.GetGoalHeight()
        );
    const Vector3f szGoal2(
        gGameParam.GetGoalDepth(),
        gGameParam.GetGoalWidth(),
        gGameParam.GetGoalHeight()
        );

    // color constants
    const GLfloat groundColor[4] = {0.1f, 0.5f, 0.1f, 1.0f};
    const GLfloat goalColor[4]   = {1.0f, 1.0f, 1.0f, 1.0f};
    const GLfloat borderColor[4] = {0.2f, 0.8f, 0.2f, 1.0f};
    const GLfloat lineColor[4]   = {0.6f, 1.0f, 0.6f, 1.0f};

    glClearColor(0.15f,0.15f,0.3f,1.0f);
    glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glColor3f (1, 1, 1);

    glLoadIdentity();
    gGLServer.ApplyCamera();

    const float& fl = gGameParam.GetFieldLength();
    const float& fw = gGameParam.GetFieldWidth();
    const float& fh = gGameParam.GetFieldHeight();
    const float& lw = gGameParam.GetLineWidth();
    const float& bs = gGameParam.GetBorderSize();
    const float& gw = gGameParam.GetGoalWidth();

    // ground
    glColor4fv(groundColor);

    // left half
    gGLServer.DrawGroundRectangle(Vector3f(
                                      -fl/2 + lw,
                                      -fw/2 + lw,
                                      0
                                      ),
                                  fl/2 - lw - lw/2,
                                  fw - 2*lw
                                  ,0
        );

    // right half
    gGLServer.DrawGroundRectangle(Vector3f(
                                      lw/2,
                                      -fw/2 + lw,
                                      0
                                      ),
                                  fl/2 - lw - lw/2,
                                  fw - 2*lw
                                  ,0
        );

    //
    // border
    //
    glColor4fv(borderColor);
    glDisable (GL_DEPTH_TEST);

    // border at left goal
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2-bs,-fw/2-bs,0),
                                  bs, fw+2*bs, 0);

    // border at right goal
    gGLServer.DrawGroundRectangle(Vector3f(fl/2,-fw/2-bs,0),
                                  bs, fw+2*bs, 0);

    // long top border
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2,-fw/2-bs,0),
                                  fl, bs, 0);

    // long bottom border
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2,fw/2,0),
                                  fl, bs, 0);

    //
    // lines
    //
    glColor4fv(lineColor);

    // left goal
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2,-fw/2,0),
                                  lw, fw, 0);

    // right goal
    gGLServer.DrawGroundRectangle(Vector3f(fl/2 - lw,-fw/2,0),
                                  lw, fw, 0);

    // long top border
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2 + lw,-fw/2,0),
                                  fl - 2*lw, lw, 0);

    // long bottom border
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2 + lw,fw/2 - lw,0),
                                  fl - 2*lw, lw, 0);

    // middle line
    gGLServer.DrawGroundRectangle(Vector3f(-lw/2,-fw/2 + lw,0),
                                  lw, fw - 2*lw, 0);

    // goal box area right side
    gGLServer.DrawGroundRectangle(Vector3f(fl/2 - 5.5, -gw/2 - 5.5, 0.05), lw, gw + 11, 0);
    gGLServer.DrawGroundRectangle(Vector3f(fl/2 - 5.5, -gw/2 - 5.5, 0.05), 5.5, lw, 0);
    gGLServer.DrawGroundRectangle(Vector3f(fl/2 - 5.5, gw/2 + 5.5, 0.05), 5.5, lw, 0);

    // goal box area left side
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 5.5, -gw/2 - 5.5, 0.05), lw, gw + 11, 0);
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 5.5, -gw/2 - 5.5, 0.05), -5.5, lw, 0);
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 5.5, gw/2 + 5.5, 0.05), -5.5, lw, 0);

    // penalty area right side
    gGLServer.DrawGroundRectangle(Vector3f(fl/2 - 16.5, -gw/2 - 16.5, 0.05), lw, gw + 33, 0);
    gGLServer.DrawGroundRectangle(Vector3f(fl/2 - 16.5, -gw/2 - 16.5, 0.05), 16.5, lw, 0);
    gGLServer.DrawGroundRectangle(Vector3f(fl/2 - 16.5, gw/2 + 16.5, 0.05), 16.5, lw, 0);

    // penalty area left side
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 16.5, -gw/2 - 16.5, 0.05), lw, gw + 33, 0);
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 16.5, -gw/2 - 16.5, 0.05), -16.5, lw, 0);
    gGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 16.5, gw/2 + 16.5, 0.05), -16.5, lw, 0);

    // center circle
    gGLServer.DrawCircle(Vector3f(0.0,0.0,0.05), 9.15);

    // fieldBox
    gGLServer.DrawWireBox(Vector3f(-fl/2.0,-fw/2.0,0.0),
                          Vector3f(fl,fw,fh));

    glEnable(GL_DEPTH_TEST);

    // goal
    glColor4fv(goalColor);
    gGLServer.DrawWireBox(Vector3f(-fl/2,-gw/2.0,0),szGoal1);
    gGLServer.DrawGoal(Vector3f(-fl/2,-gw/2.0,0),szGoal1);

    // goal
    glColor4fv(goalColor);
    gGLServer.DrawWireBox(Vector3f(fl/2,-gw/2.0,0),szGoal2);
    gGLServer.DrawGoal(Vector3f(fl/2,-gw/2.0,0),szGoal2);

    // draw cached positions
    drawScene();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor4fv(gTeamLColor);
    gGLServer.DrawSphere(Vector3f(-0.97,0.93,0.0), 0.025, 20);
    glColor4fv(gTeamRColor);
    gGLServer.DrawSphere(Vector3f( 0.97,0.93,0.0), 0.025, 20);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    drawStatusText();


    glutSwapBuffers();
}

//----------------------------idle----------------------------------------
//
// checks for new messages and parses them
//------------------------------------------------------------------------
void
idle(void)
{
    if (! gCommServer->GetMessage())
    {
        return;
    }


    boost::shared_ptr<oxygen::PredicateList> predicates =
        gCommServer->GetPredicates();

    if (
        (predicates.get() != 0) &&
        (predicates->GetSize() > 0)
        )
    {
        // parse the received expressions
        gParser->ParsePredicates(*predicates,gGameState,
                                 gGameParam,gExprList);
    }

    glutPostRedisplay();
}

//--------------------------mouse-----------------------------------------
//
// when the mouse is first clicked we store the position
// for further use when adjusting the view angle
//------------------------------------------------------------------------
void
mouse(int /*button*/, int /*state*/, int x, int y)
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
    glutPostRedisplay();
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
void
keyboard(unsigned char key, int /*x*/, int /*y*/)
{
    salt::Vector3f pos;
    switch (key) {
    case 'w':
        //move cam in
        gGLServer.MoveCamForward(gCamDelta);
        break;
    case 's':
        //move cam out
        gGLServer.MoveCamForward(-gCamDelta);
        break;
    case 'a':
        //strafe cam left
        gGLServer.MoveCamStrafe(gCamDelta);
        break;
    case 'd':
        // strafe cam right
        gGLServer.MoveCamStrafe(-gCamDelta);
        break;
    case 'c':
        //toggle autocam mode
        gAutoCam = !gAutoCam;
        cout <<"--- Automatic Camera ";
        cout << gAutoCam ? "ON" : "OFF";
        cout << " ----" << endl;
        break;

    case '+':
        //move camera up
        gGLServer.MoveCamUp(gCamDelta);
        break;

    case 'n':
        // toggle drawing of unums
        gDrawUnums = !gDrawUnums;
        break;

    case '-':
    case ' ' :
        //move camera down
        gGLServer.MoveCamUp(-gCamDelta);
        break;

    case 'q':
        // quit
        cout <<"--- Disconnect" << endl;
        gCommServer->SendDisconnectCmd();
        exit(0);

    case 'k' :
        // kick off
        cout <<"--- Kick Off" << endl;
        gCommServer->SendKickOffCmd();
        break;

    case 'p' :
        // pause simulation
        cout <<"--- Pausing Simulation" << endl;
        gCommServer->SendPauseCmd();
        break;

    case 'r' :
        // run simulation (after pause command)
        cout <<"--- Running Simulation" << endl;
        gCommServer->SendRunCmd();
        break;
#if 0
    case 'v':
        gCommServer->SendToWorldModel("(ball (pos 49 20 1) (vel 6.0 0.0 0.1))");
        break;
#endif
    default:
        return;
    }

    glutPostRedisplay();
}

//------------------------------reshape-------------------------------------
//
// processing of reshape events (the OpenGl window is resized)
// width, height are the new dimensions of the windw
//--------------------------------------------------------------------------
void
reshape(int width, int height)
{
    gGLServer.Reshape(width,height);
}

//------------------------------setupSpherMap-------------------------------
//
// sets up the map holding the different sphere descriptions
//--------------------------------------------------------------------------
void
setupSphereMap()
{
    gSphereMap.clear();

    const float agentColor[] = {0.8f, 0.8f, 0.2f, 1.0f};
    gSphereMap[MonitorParser::ET_AGENT] =
        SphereType(agentColor, 0.22f);

    const float flagColor[] = {1.0f, 0, 0, 1.0f};
    gSphereMap[MonitorParser::ET_FLAG] =
        SphereType(flagColor, 0.15f);

    const float ballColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    gSphereMap[MonitorParser::ET_BALL] =
        SphereType(ballColor, 0.111f);

    const float ballAgentColor[] = {0.9f, 0.9f, 0.9f, 1.0f};
    gSphereMap[MonitorParser::ET_BALLAGENT] =
        SphereType(ballAgentColor, 0.22f);
}

//--------------------------------main--------------------------------------
//
//--------------------------------------------------------------------------
int
main(int argc, char* argv[])
{
    // init glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(gWidth, gHeight);
    glutCreateWindow("rcssmonitor3D");
    glutDisplayFunc(display);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    //setup the GLserver with camera coordinates
    salt::Vector3f pos(0.0,-24.0, 7.0);
    salt::Vector3f lookAt(0.0,0.0,0.0);
    salt::Vector3f up(0.0,0.0,1.0);
    gGLServer = GLServer(gWidth, gHeight, pos, lookAt, up, false);
    gGLServer.InitGL();

    //init zeitgeist
    Zeitgeist zg("." PACKAGE_NAME, "../../../");

    //init oxygen
    oxygen::Oxygen kOxygen(zg);

    // init monitor lib
    MonitorLib ml(zg);

    // run init script
    zg.GetCore()->GetScriptServer()->RunInitScript
        ("rcssmonitor3D-lite.rb", "app/rcssmonitor3d/lite");

    // print a greeting
    zg.GetCore()->GetLogServer()->Normal()
        << "rcssmonitor3D version 0.2" << endl
        << "Copyright (C) 2003, Heni Ben Amor, Oliver Obst, Christoph Ringelstein, and Markus Rollmann; "
        << "Universität Koblenz." << endl
        << "Copyright (C) 2004, "
        << "The RoboCup Soccer Server Maintenance Group." << endl;
    zg.GetCore()->GetLogServer()->Normal()
        << "\nType '--help' for further information" << endl;

    // process command line
    processInput(argc, argv);

    // setup the sphere types
    setupSphereMap();

    // init the commserver
    gCommServer = shared_dynamic_cast<CommServer>
        (zg.GetCore()->Get("/sys/server/comm"));

    if (gCommServer.get() == 0)
    {
        zg.GetCore()->GetLogServer()->Error()
            << "ERROR: CommServer not found\n";
        return 1;
    }

    gCommServer->Init("SexpParser",gSoccerServer,gPort);

    // init the parser
    gParser = shared_dynamic_cast<MonitorParser>
        (zg.GetCore()->Get("/sys/server/parser"));

    if (gParser.get() == 0)
    {
        zg.GetCore()->GetLogServer()->Error()
            << "ERROR: MonitorParser not found\n";
        return 1;
    }

    // enter glut main loop
    glutMainLoop();

    return 0;
}
