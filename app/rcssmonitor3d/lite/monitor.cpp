 /* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitor.cpp,v 1.27.4.1 2007/05/22 05:14:09 fruit Exp $

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
*/
#include "monitor.h"
#include "gamestate.h"
#include <getopt.h>
#include <iomanip>
#include <sstream>
#include <soccer/soccertypes.h>
#include <types.h>
#include <unistd.h>
#include <kerosin/openglserver/openglwrapper.h>
#include <queue>
void
display()
{
    Monitor::Instance()->Display();
}

void
mouseMotion(int x, int y)
{
    Monitor::Instance()->MouseMotion(x, y);
}

void
keyboard(unsigned char key, int x, int y)
{
    Monitor::Instance()->Keyboard(key, x, y);
}

void
specialkeys(int glutkey, int x, int y)
{
    Monitor::Instance()->SpecialKeys(glutkey, x, y);
}

void
mouse(int button, int state, int x, int y)
{
    Monitor::Instance()->Mouse(button, state, x, y);
}

void
reshape(int width, int height)
{
    Monitor::Instance()->Reshape(width, height);
}

void
idle()
{
    Monitor::Instance()->Idle();
}
// color constants
const GLfloat
Monitor::sGroundColor[4] = {0.1f, 0.5f, 0.1f, 1.0f};
const GLfloat
Monitor::sSecondGroundColor[4] = {0.05f, 0.43f, 0.07f, 1.0f};
const GLfloat
Monitor::sGoalColor[4]   = {0.8f, 0.8f, 0.8f, 1.0f};
const GLfloat
Monitor::sBorderColor[4] = {0.25f, 0.35f, 0.25f, 1.0f};
const GLfloat
Monitor::sLineColor[4]   = {0.8f, 1.0f, 0.8f, 0.8f};
const GLfloat
Monitor::sSphereDefaultColor[4] = { 0.8, 0.8, 0.2, 1.0 };

const GLfloat
Monitor::sUnumColor[4]      = {1.0, 1.0, 1.0, 1.0};
// color for player of the different teams
const GLfloat
Monitor::sTeamColorLeft[4] = {0.8f, 0.2f, 0.2f, 1.0f};
const GLfloat
Monitor::sTeamColorRight[4] = {0.2f, 0.2f, 0.8f, 1.0f};
// ball color
const GLfloat
Monitor::sBallColor[4] = {1.0f, 0.67f, 0.0f, 1.0f};
// debug color1
const GLfloat
Monitor::sDebugColorCyan[4] = {0.0f, 0.8f, 0.8f, 0.3f};
// debug color2
const GLfloat
Monitor::sDebugColorPink[4] = {1.0f, 0.0f, 1.0f, 0.5f};
//color for ground of overview field
const GLfloat
Monitor::sGroundColor2D[4] = {0.0f, 0.5f, 0.0f, 0.6f};

// radius of center circle
const float
Monitor::sCenterRadius = 9.15;

// length of the goal box
const float
Monitor::sGoalBoxLength = 5.5;

// length of the penalty area
const float
Monitor::sPenaltyLength = 16.5;

// 2D Overview constants
const float
Monitor::s2DLowX = 0.02; // left pos

const float
Monitor::s2DLowY = 0.15; // 0.12; // top pos

const float
Monitor::s2DScale = 0.0075; // Skalierung

Monitor::Monitor(std::string rel_path_prefix)
    : mZeitgeist("." PACKAGE_NAME, rel_path_prefix),
      mOxygen(mZeitgeist),
      mMonitorLib(mZeitgeist)
{
    mWidth = DEFAULT_WIDTH;
    mHeight = DEFAULT_HEIGHT;
    mCamDelta = 0.5;

    mCameraMode = eCenterBall;
    mShowCamIconTime = 0;
    mCamModeMap[eFree] = "[F]"; // F_ree mode
    mCamModeMap[eFollowBall] = "[A]"; // A_utomatic cam
    mCamModeMap[eCenterBall] = "[X]" ; // keep cam at X_coord of ball

    mDrawUnums = true;
    mDrawOverview = false;
    mDrawDebug = false;
    mDrawVelocity = false;
    mPauseGame = false;
    mServer = DEFAULT_HOST;
    mPort = DEFAULT_PORT;
    mSkip = 1;

    mCenterBallCameraY = -32.0;
    mCenterBallCameraZ = 20.0;
    mShowSecondView = false;
    mUseTexture = false;

    mLogserver = false;
    mSingleStep = false;
    mAdvance = false;
    mRealTime = true;
    mDiffTime = 0;
    mSecondHalfKickOff = CommServerBase::eRandom;
    
    FlagInfo fi;
    fi.mOffset = Vector3f(0,0,0);
    fi.mRadius = 0.1;
    memcpy(fi.mColor, sSphereDefaultColor, sizeof(fi.mColor));

    // corner flags left team
    mDefaultFlagInfo = fi;
    fi.mOffset = Vector3f(0,0,1.5);
    memcpy(fi.mColor, sTeamColorLeft, sizeof(fi.mColor));
    mFlagInfo[GameState::eFLAG_1_L] = fi;
    mFlagInfo[GameState::eFLAG_2_L] = fi;
    // goal flags left team
    fi.mOffset = Vector3f(0,0,1.6);
    mFlagInfo[GameState::eGOAL_1_L] = fi;
    mFlagInfo[GameState::eGOAL_2_L] = fi;
    // goal flags right team
    memcpy(fi.mColor, sTeamColorRight, sizeof(fi.mColor));
    mFlagInfo[GameState::eGOAL_1_R] = fi;
    mFlagInfo[GameState::eGOAL_2_R] = fi;
    // corner flags right team
    fi.mOffset = Vector3f(0,0,1.5);
    mFlagInfo[GameState::eFLAG_1_R] = fi;
    mFlagInfo[GameState::eFLAG_2_R] = fi;

    mTextureFile="grass.ppm";
}

Monitor::~Monitor()
{
}

Monitor*
Monitor::Instance()
{
    static std::auto_ptr<Monitor> the_monitor(new Monitor());
    return the_monitor.get();
}

Monitor::EReturnType
Monitor::Init(int argc, char* argv[])
{
    int c = 0;
    EReturnType status = eOK;
    int option_index;
    //bool logplayer = false;
    bool server = false;

    option long_options[] = {
        // name, has_arg, flag, val
        { "help", no_argument, 0, 'h' },
        { "port", required_argument, 0, 'p' },
        { "server", required_argument, 0, 's' },
        { "logfile", required_argument, 0, 'l' },
        { "msgskip", required_argument, 0, 'm' },
        { "texture", required_argument, 0, 't' },
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
            status = eOptions;
            break;
        case 'p': // --port
            mPort = atoi(optarg);
            break;
        case 's': // --server
            mServer = std::string(optarg);
            server = true;
            break;
        case 'l': // -- logplayer
            mServer = std::string(optarg);
            mLogserver = true;
            break;
        case 'm': // --msgskip
            mSkip = atoi(optarg);
            break;
        case 't': // --texture
            mTextureFile = std::string(optarg);
            break;
        default:
            status = eErrInit;
        }
    }

//    if (logplayer && server)
    if (mLogserver && server)
    {
        status = eErrInit;
        mZeitgeist.GetCore()->GetLogServer()->Error()
            << "ERROR: cannot play logfiles and connect to simulator at the same time\n";
    }

    mZeitgeist.GetCore()->GetScriptServer()->RunInitScript
        ("rcssmonitor3D-lite.rb", "app/rcssmonitor3d/lite");

    if (status == eOK)
    {
        shared_ptr<CoreContext> context = mZeitgeist.GetCore()->CreateContext();
        if (mLogserver)
        {
            context->New("rcssmonitor3d/LogfileServer", "/sys/server/comm");
        } else {
            context->New("rcssmonitor3d/CommServer", "/sys/server/comm");
        }

        status = InitInternal(argc,argv);
    }

    return status;
}

Monitor::EReturnType
Monitor::Run()
{
    glutMainLoop();
    return eOK;
}

void
Monitor::Usage()
{
    std::cerr <<
"Usage: rcssserver3D-lite [options]\n"
"Simple visualization of rcssserver3D soccer matches\n\n"
"   --help      Print this message and exit.\n"
"   --port      Specify the port number (default is " << DEFAULT_PORT << ").\n"
"   --server    Specify the server host (default is '" << DEFAULT_HOST << "').\n"
"   --logfile   Specify the logfile to read (not with --server).\n"
"   --msgskip   Every but the nth message should be discarded (default is 1).\n"
"   --texture   Set the name of the texturefile to use.  If no absolute path is given\n"
"               the file is assumed to reside in your ~/.rcssserver3d/ directory.\n"
"\n"
"Press '?' for a list of keybindings.\n"
"\n";
}

void
Monitor::Copyleft()
{
    std::cerr << "rcssmonitor3D-lite written by\n"
              << "Heni Ben Amor, Oliver Obst, Christoph Ringelstein, Markus Rollmann & Jan Murray;\n"
              << "Copyright (C) 2003 Universitaet Koblenz, Germany.\n"
              << "Copyright (C) 2004-2007 The RoboCup Soccer Server Maintenance Group.\n\n"
              << "Special additions by Tobias Warden from TZI Bremen/virtual werder.\n\n";
    
}

void
Monitor::KeyBindings()
{
    std::cerr <<
        "\n\nKeybindings for rcssmonitor3D-lite:\n"
        "\n"
        "----------------------------------------------------\n"
        "GENERAL\n"
        "----------------------------------------------------\n"
        "q          | quit the monitor\n"
        "p          | pause the simulation/logplayer\n"
        //"r          | unpause the simulation/logplayer\n"
        "n          | toggle display of uniform numbers\n"
        "1          | toggle display of debug information\n"
        "2          | toggle display of 2D overview\n"
        "v          | toggle display of velocities\n"
        "t          | toggle display of texture\n"
        "Z          | switch to fullscreen\n"
        "X          | toggle display of 2nd view (pip)\n"
        "?          | display keybindings\n"
        "----------------------------------------------------\n"
        "CAMERA MOVEMENT\n"
        "----------------------------------------------------\n"
        "c          | switch through camera modes (F,A,X)\n"
        "Arrow keys | move camera\n"
        "w/s        | move camera forward/back (zoomlike)\n"
        "a/d        | move camera left/right\n"
        "(/)        | rotate camera left/right\n"
        "[/]        | adjust y-coord of camera in autocam mode (X)\n"
        "{/}        | adjust height of camera in autocam mode (X)\n"
        "+/-        | move camera up/down\n"
        "3          | move camera behind left goal\n"
        "4          | move camera behind right goal\n"
        "5          | cycle through stationary cam positions on\n"
        "           | left half\n"
        "6          | cycle through stationary cam positions on\n"
        "           | right half\n"
        "----------------------------------------------------\n"
        "SIMULATION\n"
        "----------------------------------------------------\n"
        "k          | kick off (start the game)\n"
        "b          | drop the ball at its current position\n"
        "g          | kick the goalies towards the field,\n"
        "           | if they sit on a goal\n"
        "[SPACE]    | toggle kick off side (left-right-random)\n"
        "----------------------------------------------------\n"
        "LOGPLAYER\n"
        "----------------------------------------------------\n"
        "f          | toggle fast/realtime replay\n"
        "F          | Realtime replay\n"
        "m          | toggle single step mode for logplayer\n"
        ">          | move one step forward\n"
        "<          | move one step back\n"
        "----------------------------------------------------\n"
        "\n";

}

Monitor::EReturnType
Monitor::InitInternal(int argc, char* argv[])
{
    // init the commserver
    mCommServer = shared_dynamic_cast<CommServerBase>
        (mZeitgeist.GetCore()->Get("/sys/server/comm"));

    if (mCommServer.get() == 0)
    {
        mZeitgeist.GetCore()->GetLogServer()->Error()
            << "ERROR: CommServer not found\n";
        return eErrInit;
    }

    mCommServer->Init("SexpParser",mServer,mPort);

    // init glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(mWidth, mHeight);
    glutCreateWindow("rcssmonitor3D");
    glutDisplayFunc(display);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialkeys);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutSetCursor(GLUT_CURSOR_NONE);
    

    // setup the GLserver with camera coordinates
    // and texture
    salt::Vector3f pos(0.0,-30.0, 12.0);
    salt::Vector3f lookAt(0.0,0.0,0.0);
    salt::Vector3f up(0.0,0.0,1.0);
    mGLServer = GLServer(mWidth, mHeight, pos, lookAt, up, false);

    mGLServer.InitTexture(mTextureFile);
    
    mGLServer.InitGL();

    glBlendFunc(GL_SRC_ALPHA,GL_ONE);



// Somewhere in the initialization part of your program
glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);

// Create light components
GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[] = { 0.0f, 0.0f, 10.0f, 2.0f };

// Assign created components to GL_LIGHT0
glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
glLightfv(GL_LIGHT0, GL_POSITION, position);




    return eOK;
}

void
Monitor::SetupCameraPositions()
{
    mLeftCamPositions.clear();
    mRightCamPositions.clear();
    float hfl = mGameState.GetFieldLength()/2;
    float hfw = mGameState.GetFieldWidth()/2;

    salt::Vector3f lp, rp;

    // first position somewhere in the middle of the respective half
    // on the side of the field
    lp[0] = -1*hfl/2;        rp[0] = -lp[0];
    lp[1] = -hfw-4;          rp[1] = lp[1];
    lp[2] = 5.5;             rp[2] = lp[2];
    mLeftCamPositions.push_back(lp);
    mRightCamPositions.push_back(rp);

    // second position somewhere in the middle of the respective half
    // but zoomed into the field
    lp[0] = -1*hfl/2;        rp[0] = -lp[0];
    lp[1] = -hfw+4;          rp[1] = lp[1];
    lp[2] = 10.0;             rp[2] = lp[2];
    mLeftCamPositions.push_back(lp);
    mRightCamPositions.push_back(rp);

    // third position for watching the penalty area
    lp[0] = -hfl+5;          rp[0] = -lp[0];
    lp[1] = -hfw+4;          rp[1] = lp[1];
    lp[2] = 5.5;             rp[2] = lp[2];
    mLeftCamPositions.push_back(lp);
    mRightCamPositions.push_back(rp);



}

void
Monitor::DrawScene(int pass)
{
    Vector3f pos;
    for (int i = static_cast<int>(GameState::eFLAG_1_L);
         i != static_cast<int>(GameState::eILLEGAL);
         ++i)
    {
        if (mGameState.GetFlag(static_cast<GameState::EFlagType>(i), pos))
        {
            DrawFlag(static_cast<GameState::EFlagType>(i), pos, pass);
        }
    }

    int i = 0;
    int unum;
    TTeamIndex side;
    float size;
    while (mGameState.GetPlayer(i, pos, side, unum, size))
    {
        DrawPlayer(side, unum, pos, size, pass);
        ++i;
    }

    if (mGameState.GetBall(pos, size))
    {
        DrawBall(pos, size, pass);

        if (mCameraMode == eFollowBall && pass == 0)
        {
            Vector3f& ballGroundPos = pos;
            ballGroundPos[2] = 0.0f;
            mGLServer.SetLookAtPos(ballGroundPos);
        }

        if (mCameraMode == eCenterBall && pass == 0)
        {
            Vector3f ballGroundPos = pos;
            ballGroundPos[2] = 0.0f;
            mGLServer.SetLookAtPos(ballGroundPos);
            ballGroundPos[1] = mCenterBallCameraY;
            ballGroundPos[2] = mCenterBallCameraZ;
            mGLServer.SetCameraPos(ballGroundPos);
        }
    }

}

void
Monitor::DrawStatusText()
{
    std::stringstream sl, sc, sr;
    static std::stringstream debug;

    sl << mGameState.GetTeamnameLeft() << " " << mGameState.GetScoreLeft();
    sr << mGameState.GetScoreRight() << " " << mGameState.GetTeamnameRight();

    if (mShowCamIconTime>=0)
    {
        mShowCamIconTime--;
        sc << mCamModeMap[mCameraMode] << "   ";
    }

    sc << "(" << ((mGameState.GetHalf() == GH_FIRST) ?
                  "first" : "second") << " half) ";

    int play_mode = mGameState.GetPlayMode();

    switch (play_mode)
    {
    case PM_PlayOn:
        if (mSecondHalfKickOff==CommServerBase::eRandom)
        {
            mSecondHalfKickOff=mKickOff;    //store who will kickoff at 2nd half
        }
        
        
        break;
    case PM_BeforeKickOff:
        if (mGameState.GetHalf() == GH_SECOND)
        {
            mKickOff=mSecondHalfKickOff;
        }
        break;
    case PM_KickOff_Left:
        mKickOff = CommServerBase::eRight;
        break;
    case PM_KickOff_Right:
        mKickOff = CommServerBase::eLeft;
        break;
//     case PM_Goal_Left:
//         cout << "GOOOAAAAAAAAAAAAAAAAAAAAAAAAL! (left) \n";
//         break;
//     case PM_Goal_Right:
//         cout << "GOOOAAAAAAAAAAAAAAAAAAAAAAAAL! (right) \n";
//         break;
    };

    sc << mGameState.PlayMode2Str(play_mode) << " ";
    sc << "t=" << setiosflags(ios::fixed) << setprecision(2) << mGameState.GetTime();

    glColor3f(1.0, 1.0, 1.0);
    mGLServer.DrawTextPix(sl.str().c_str(),
                          Vector2f( 35, mGLServer.GetTextHeight()));
    mGLServer.DrawTextPix(sc.str().c_str(),
                          Vector2f( 0, mGLServer.GetTextHeight()),
                          GLServer::eCENTER);
    mGLServer.DrawTextPix(sr.str().c_str(),
                          Vector2f( -35, mGLServer.GetTextHeight()),
                          GLServer::eRIGHT);

    if (mDrawDebug)
    {
#if 0
        ++frame;
        time=glutGet(GLUT_ELAPSED_TIME);
        if (time - timebase > 1000)
        {
            debug.str("");
            debug << setiosflags(ios::fixed) << setprecision(2)
                  << "FPS: " << frame*1000.0/(time-timebase);
        }
        timebase = time;
        frame = 0;
#elif 1
        debug.str("");
        Vector3f pos;
        mGameState.GetBall(pos);
        debug << setiosflags(ios::fixed) << setprecision(2)
              << mGameState.GetTime() << "   Ball position: " << pos;
        pos[2] = 0.0;
        debug << "   Ground distance from origin: " << pos.Length();
#endif

        mGLServer.DrawTextPix(debug.str().c_str(),
                              Vector2f( 0,  glutGet(GLUT_WINDOW_HEIGHT) - mGLServer.GetTextHeight()),
                              GLServer::eCENTER);
    }
}

void
Monitor::DrawStatusLine()
{
    float aspect = float(mGLServer.GetWidth())/mGLServer.GetHeight();
    
    glColor4fv(sTeamColorLeft);
    mGLServer.DrawSphere(Vector3f(-0.97*aspect,0.93,0.0), 0.025, 25);
    glColor4fv(sTeamColorRight);
    mGLServer.DrawSphere(Vector3f( 0.97*aspect,0.93,0.0), 0.025, 20);


    // Test: draw a big blue ball
    //mGLServer.DrawSphere(Vector3f( 0.5,.9,0.0), 0.1, 20);

    int play_mode = mGameState.GetPlayMode();

    if (play_mode == PM_BeforeKickOff)
    {
        glColor4fv(sSphereDefaultColor);
        switch (mKickOff)
        {
        case CommServerBase::eRandom:
            mGLServer.DrawSphere(Vector3f( 0.0,0.93,0.0), 0.025, 25);
            break;
        case CommServerBase::eLeft:
            mGLServer.DrawSphere(Vector3f( -0.92*aspect,0.93,0.0), 0.025, 25);
            break;
        case CommServerBase::eRight:
            mGLServer.DrawSphere(Vector3f( 0.92*aspect,0.93,0.0), 0.025, 25);
            break;
        }
    }
}

Vector3f Monitor::Get2DPos(const Vector3f& pos)
{
    float aspect = float(mGLServer.GetWidth())/mGLServer.GetHeight();
    //cout << "(get2dpos) aspect = " << aspect << endl;
    
    return Vector3f
        (
            aspect*(-1.0) + s2DLowX + s2DScale * (pos[0] + mGameState.GetFieldLength()/2),

            1.0 - s2DLowY - (s2DScale * mGameState.GetFieldWidth()) +
            s2DScale * (pos[1] + mGameState.GetFieldWidth()/2),

            0.0
         );
}

void
Monitor::DrawOverview()
{
    glDisable(GL_LIGHTING);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1);

    // radius of player

    float pl_radius = 0.01;
//         = sSzY / mGameState.GetFieldWidth() *
//         mGameState.GetAgentRadius();

    // radius of ball
    float b_radius = 0.005;

//         sSzY / mGameState.GetFieldWidth() *
//         mGameState.GetBallRadius();

    // resolution of a sphere
    int res  = 8;

    const float& fl = mGameState.GetFieldLength();
    const float& fw = mGameState.GetFieldWidth();
//     const float& fh = mGameState.GetFieldHeight();
//     const float& lw = mGameState.GetLineWidth();
//     const float& bs = mGameState.GetBorderSize();
    const float& gw = mGameState.GetGoalWidth();

    // field rect

    glEnable(GL_BLEND);                // Turn Blending On
    glColor4fv(sGroundColor2D);
    glBegin(GL_TRIANGLE_STRIP);
    glVertex3fv(Get2DPos(Vector3f(-fl/2,-fw/2,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(fl/2,-fw/2,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(-fl/2,fw/2,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(fl/2,fw/2,0)).GetData());
    glEnd();
    glDisable(GL_BLEND);                // Turn Blending Off
        
    glColor4fv(sLineColor);
    glBegin(GL_LINE_LOOP);
    glVertex3fv(Get2DPos(Vector3f(-fl/2,-fw/2,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(fl/2,-fw/2,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(fl/2,fw/2,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(-fl/2,fw/2,0)).GetData());
    glEnd();

    // middle line
    glBegin(GL_LINES);
    glVertex3fv(Get2DPos(Vector3f(0,-fw/2,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(0,fw/2,0)).GetData());
    glEnd();

    // middle circle
    mGLServer.DrawCircle(Get2DPos(Vector3f(0,0,0)),sCenterRadius*s2DScale);

    // left goal box
    glBegin(GL_LINE_STRIP);
    glVertex3fv(Get2DPos(Vector3f(-fl/2,gw/2+sGoalBoxLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(-fl/2+sGoalBoxLength,gw/2+sGoalBoxLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(-fl/2+sGoalBoxLength,-gw/2-sGoalBoxLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(-fl/2,-gw/2-sGoalBoxLength,0)).GetData());
    glEnd();

    // right goal box
    glBegin(GL_LINE_STRIP);
    glVertex3fv(Get2DPos(Vector3f(+fl/2,gw/2+sGoalBoxLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(+fl/2-sGoalBoxLength,gw/2+sGoalBoxLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(+fl/2-sGoalBoxLength,-gw/2-sGoalBoxLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(+fl/2,-gw/2-sGoalBoxLength,0)).GetData());
    glEnd();

    // penalty area left
    glBegin(GL_LINE_STRIP);
    glVertex3fv(Get2DPos(Vector3f(-fl/2,gw/2+sPenaltyLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(-fl/2+sPenaltyLength,gw/2+sPenaltyLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(-fl/2+sPenaltyLength,-gw/2-sPenaltyLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(-fl/2,-gw/2-sPenaltyLength,0)).GetData());
    glEnd();

    // penalty area right
    glBegin(GL_LINE_STRIP);
    glVertex3fv(Get2DPos(Vector3f(+fl/2,gw/2+sPenaltyLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(+fl/2-sPenaltyLength,gw/2+sPenaltyLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(+fl/2-sPenaltyLength,-gw/2-sPenaltyLength,0)).GetData());
    glVertex3fv(Get2DPos(Vector3f(+fl/2,-gw/2-sPenaltyLength,0)).GetData());
    glEnd();

    Vector3f pos,oldpos;
    float size;

    int i = 0;
    int unum;
    TTeamIndex side;

    //JAN
    double timeDiff = mGameState.GetTime() - mOldGameState.GetTime();

    while (mGameState.GetPlayer(i, pos, side, unum, size))
    {
        switch (side)
            {
            case TI_LEFT:
                glColor4fv(sTeamColorLeft);
                break;
            case TI_RIGHT:
                glColor4fv(sTeamColorRight);
                break;
            default:
                glColor4fv(sSphereDefaultColor);
            }

        mGLServer.DrawSphere(Get2DPos(pos), pl_radius, res);

        if (mDrawVelocity && timeDiff != 0.0)
        {

            glColor3f(1.0f,1.0f,1.0f);
            mOldGameState.GetPlayer(i, oldpos, side, unum, size);
            mGLServer.DrawArbitraryLine(Get2DPos(pos), Get2DPos(pos + (pos-oldpos)/timeDiff));
        }

        if (mDrawUnums)
            {
                glColor4fv(sUnumColor);
                stringstream ss;
                ss << unum;
                mGLServer.DrawText3D(ss.str(), Get2DPos(pos));
            }

        ++i;
    }

    static double pass = mGameState.GetTime();
    if (pass == mGameState.GetTime()) return;
    pass = mGameState.GetTime();
#if 0
    static double vel0 = 0.0;
    static double vel1 = 0.0;
    static std::queue<Vector3f> prev;
    while (prev.size() < 10) prev.push(Vector3f(0,0,0));

    mOldGameState.GetPlayer(0, oldpos, side, unum, size);
    mGameState.GetPlayer(0,pos,side,unum,size);
    vel1 = ((pos-oldpos)/timeDiff).x();


    std::cerr << mGameState.GetTime() << " " << pos << "  "
              << vel0 << " " << vel1 << " "
              << pos.x() << " " << (pos-prev.front()).x() << "\n";

    prev.push(pos);
    prev.pop();

    vel0 = vel1;
#elif 0
    mGameState.GetPlayer(0,pos,side,unum,size);
    std::cerr << mGameState.GetTime() << " "
              << (pos - Vector3f(-1,-1,pos[2])).Length() << "\n";
#elif 0
    static double vel0 = 0.0;
    static double vel1 = 0.0;
    static bool once = true;

    if (once)
    {
        once = false;
        std::cerr << "## Time | pos.z | oldvel.z | vel.z | velZDiff | TimeDiff | vel.z*TimeDiff \n\n";
    }

    mOldGameState.GetBall(oldpos, size);
    mGameState.GetBall(pos, size);

    vel1 = (pos-oldpos).z()/timeDiff;
    if ((vel0 < 0) && !(vel1 <= 0))
    {
        std::cerr << mGameState.GetTime() << " " << pos.z() << " " << vel0 << " " << vel1
                  << " " << vel1 - vel0  << " " << timeDiff << " " << (pos-oldpos).z() << "\n";
    }
    vel0 = vel1;
#elif 0
    static Vector3f vel0;
    static Vector3f vel1;
    static bool once = true;

    if (once)
    {
        once = false;
        std::cerr << "## Time | pos | oldvel | vel | velDiff \n\n";
    }

    mOldGameState.GetBall(oldpos, size);
    mGameState.GetBall(pos, size);


    vel1 = (pos-oldpos)/timeDiff;
    std::cerr << mGameState.GetTime() << " " << pos << " "
              << vel0 << " " << vel1 << " " << vel1 - vel0 << "\n";
    vel0 = vel1;

#endif
    if (mGameState.GetBall(pos, size))
    {
        glDisable(GL_DEPTH_TEST);
        glColor4fv(sBallColor);
        mGLServer.DrawSphere(Get2DPos(pos), b_radius, res);
        if (mDrawVelocity && timeDiff != 0.0)
        {
            glColor3f(1.0f,1.0f,1.0f);
            mOldGameState.GetBall(oldpos);
            mGLServer.DrawArbitraryLine(Get2DPos(pos), Get2DPos(pos + (pos-oldpos)/timeDiff));
        }
        glEnable(GL_DEPTH_TEST);
    }

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);

    glEnable(GL_LIGHTING);
    
}

void
Monitor::DrawFlag(GameState::EFlagType i, salt::Vector3f pos, int pass)
{
    TFlagInfoMap::const_iterator j = mFlagInfo.find(i);
    FlagInfo fi;
    if (j == mFlagInfo.end())
    {
        memcpy(&fi, &mDefaultFlagInfo, sizeof(FlagInfo));
    } else {
        memcpy(&fi, &(j->second), sizeof(FlagInfo));
    }

    // pass=0, first pass with depth test disabled
    if (pass == 0)
    {
        mGLServer.DrawShadowOfSphere(pos + fi.mOffset, fi.mRadius);
    }
    else
    // pass=1, second pass with depth test
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, fi.mColor);
        //glColor4fv(fi.mColor);
        mGLServer.DrawSphere(pos + fi.mOffset, fi.mRadius);
    }
}

void
Monitor::DrawPlayer(TTeamIndex side,
                    int unum, const salt::Vector3f& pos, float size, int pass)
{
    if (pass == 0)
    {
        const GLfloat outerlineColor[] = {0.2f,0.2f,0.2f,1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, outerlineColor);
        mGLServer.DrawShadowOfSphere(pos, size);
    }
    else
    // pass=1, second pass with depth test
    {
        switch (side)
        {
        case TI_LEFT:
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sTeamColorLeft);
            //glColor4fv(sTeamColorLeft);
            break;
        case TI_RIGHT:
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sTeamColorRight);
            //glColor4fv(sTeamColorRight);
            break;
        default:
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sSphereDefaultColor);
            //glColor4fv(sSphereDefaultColor);
        }

        mGLServer.DrawSphere(pos, size, 8, false);

        glDisable(GL_LIGHTING);

        if (mDrawUnums && unum > 0)
        {
            glColor4fv(sUnumColor);
            std::stringstream ss;
            ss << unum;
            mGLServer.DrawText3D(ss.str(), pos + salt::Vector3f(0.0,0.0,0.3f));
        }

        glEnable(GL_LIGHTING);
    }
}

void
Monitor::DrawBall(const salt::Vector3f& pos, float size, int pass)
{
    if (pass == 0)
    {
        const GLfloat outerlineColor[] = {0.2f,0.2f,0.2f,1.0f};
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, outerlineColor);
        mGLServer.DrawShadowOfSphere(pos, size);
    }
    else
    // pass=1, second pass with depth test
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sBallColor);
        //glColor4fv(sBallColor);
        mGLServer.DrawSphere(pos, size);
    }
}

// Example for DrawDebug
void
Monitor::DrawDebug()
{
    // this example method just draws the positive axes on the field
    glColor4fv(sDebugColorPink);

    // mGLServer.DrawArbitraryLine(Vector3f(0,0,0), Vector3f(10,0,0));
    // mGLServer.DrawArbitraryLine(Vector3f(0,0,0), Vector3f(0,10,0));
    // mGLServer.DrawArbitraryLine(Vector3f(0,0,0), Vector3f(0,0,10));

    Vector3f pos;
    float size;

    if (mGameState.GetBall(pos, size))
    {
        mGLServer.DrawCircle(pos, 1.0, 0 , 360);
    }

    return;
}


void
Monitor::DrawVelocities()
{
    Vector3f pos, oldpos;

    float timeDiff = mGameState.GetTime() - mOldGameState.GetTime();

    // if no time has passed, no velocity can be calculated
    // TODO: this should set a default for stop clock cycles
    if (timeDiff == 0.0)
        return;

    //glDisable(GL_DEPTH_TEST);

    glColor3f(0.2f, 0.2f, 0.2f);

    if (mGameState.GetBall(pos) && mOldGameState.GetBall(oldpos))
    {
        //DrawBall(pos, size, pass);
        //For scaling must divide by (timeDiff)

        mGLServer.DrawArbitraryLine(pos, pos + (pos-oldpos)/timeDiff);
        //mGLServer.DrawArbitraryLine(pos, pos + (pos-oldpos)/timeDiff);
    }

    int i = 0;
    int unum;
    TTeamIndex side;
    float size;
    while (mGameState.GetPlayer(i, pos, side, unum, size))
    {
        mOldGameState.GetPlayer(i, oldpos, side, unum, size);
        mGLServer.DrawArbitraryLine(pos, pos + (pos-oldpos)/timeDiff);
        ++i;
    }

    //glEnable(GL_DEPTH_TEST);

}


void
Monitor::Display()
{

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);

    glEnable(GL_LIGHTING);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const Vector3f szGoal1 = mGameState.GetGoalSize(false);
    const Vector3f szGoal2 = mGameState.GetGoalSize(true);

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    //glClearColor(0.15f,0.15f,0.3f,1.0f);
    glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glColor3f (1.0, 1.0, 1.0);

    glLoadIdentity();
    mGLServer.ApplyCamera();

    const float& fl = mGameState.GetFieldLength();
    const float& fw = mGameState.GetFieldWidth();
    const float& fh = mGameState.GetFieldHeight();
    const float& lw = mGameState.GetLineWidth();
    const float& bs = mGameState.GetBorderSize();
    const float& gw = mGameState.GetGoalWidth();

	// skybox background
    if (mUseTexture)
    {
        glEnable(GL_TEXTURE_2D);
        mGLServer.DrawSkyBackground(0,0,100,200,200,200);
//    glDisable(GL_TEXTURE_2D);
    }

        
    // ground
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sGroundColor);
    //glColor4fv(sGroundColor);
    //glColor4fv(sDebugColorPink);

    // left half
    //JANxxx

//     if (mUseTexture)
//         glEnable(GL_TEXTURE_2D);

    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + lw, -fw/2 + lw, 0),
                                  fl/2 - lw - lw/2, fw - 2*lw, 0);
    

    // right half
    mGLServer.DrawGroundRectangle(Vector3f(lw/2, -fw/2 + lw, 0),
                                  fl/2 - lw - lw/2, fw - 2*lw, 0);
    if (mUseTexture)
        glDisable(GL_TEXTURE_2D);

// ------------------------------

//     glPushMatrix();
//     glRotatef(0,1,0,0);
//     //glTranslatef(pos[0],pos[1], pos[2]);
//     glNormal3f(0,0,1);

//     glBegin(GL_QUADS);
//     glVertex3f(-4.5,-4.5,3.0);
//     glVertex3f( 4.5,-4.5,3.0);
//     glVertex3f( 4.5, 4.5,3.0);
//     glVertex3f(-4.5, 4.5,3.0);
//     glEnd();
//     glPopMatrix();

    //--------------------------------------------------------------
    
    // border
    glColor4fv(sBorderColor);

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sBorderColor);    
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2-bs,-fw/2-bs,-0.01), fl + 2*bs, fw+2*bs,-0.01);

    glDisable (GL_DEPTH_TEST);

    const GLfloat outerlineColor[] = {0.2f,0.2f,0.2f,1.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, outerlineColor);
    //glColor3f(0.2f,0.2f,0.2f);
    glLineWidth(4);
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2-bs,-fw/2-bs,0.01),Vector3f(-fl/2-bs,fw/2+bs,0.02));
    mGLServer.DrawArbitraryLine(Vector3f( fl/2+bs,-fw/2-bs,0.01),Vector3f( fl/2+bs,fw/2+bs,0.02));
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2-bs,-fw/2-bs,0.01),Vector3f(fl/2+bs,-fw/2-bs,0.02));
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2-bs, fw/2+bs,0.01),Vector3f(fl/2+bs,fw/2+bs,0.02));

    // fieldBox
//     glLineWidth(1);
//     mGLServer.DrawWireBox(Vector3f(-fl/2.0,-fw/2.0,0.0),
//                           Vector3f(fl,fw,fh));

    // lines
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sLineColor);
    //glColor4fv(sLineColor);
    glLineWidth(3);
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2,-fw/2,0.01),Vector3f(-fl/2,fw/2,0.02));
    mGLServer.DrawArbitraryLine(Vector3f( fl/2,-fw/2,0.01),Vector3f( fl/2,fw/2,0.02));
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2,-fw/2,0.01),Vector3f(fl/2,-fw/2,0.02));
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2, fw/2,0.01),Vector3f(fl/2,fw/2,0.02));

    mGLServer.DrawArbitraryLine(Vector3f(0,-fw/2,0.01),Vector3f(0,fw/2,0.02));
    
    // goal box area right side
    mGLServer.DrawArbitraryLine(Vector3f(fl/2 - sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.02),
                                Vector3f(fl/2, -gw/2 - sGoalBoxLength, 0.02));
    mGLServer.DrawArbitraryLine(Vector3f(fl/2 - sGoalBoxLength, gw/2 + sGoalBoxLength, 0.02),
                                Vector3f(fl/2, gw/2 + sGoalBoxLength, 0.02));
    mGLServer.DrawArbitraryLine(Vector3f(fl/2 - sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.02),
                                Vector3f(fl/2 - sGoalBoxLength, gw/2 + sGoalBoxLength, 0.02) );
    
    //mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.05), lw, gw + 11, 0);
    //mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.05), sGoalBoxLength, lw, 0);
    //mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sGoalBoxLength, gw/2 + sGoalBoxLength, 0.05), sGoalBoxLength, lw, 0);

    // goal box area left side
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2 + sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.02),
                                Vector3f(-fl/2, -gw/2 - sGoalBoxLength, 0.02));
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2 + sGoalBoxLength, gw/2 + sGoalBoxLength, 0.02),
                                Vector3f(-fl/2, gw/2 + sGoalBoxLength, 0.02));
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2 + sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.02),
                                Vector3f(-fl/2 + sGoalBoxLength, gw/2 + sGoalBoxLength, 0.02) );

    //mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.05), lw, gw + 11, 0);
    //mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.05), -sGoalBoxLength, lw, 0);
    //mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sGoalBoxLength, gw/2 + sGoalBoxLength, 0.05), -sGoalBoxLength, lw, 0);

    // penalty area right side
    mGLServer.DrawArbitraryLine(Vector3f(fl/2 - sPenaltyLength, -gw/2 - sPenaltyLength, 0.02),
                                Vector3f(fl/2, -gw/2 - sPenaltyLength, 0.02));
    mGLServer.DrawArbitraryLine(Vector3f(fl/2 - sPenaltyLength, gw/2 + sPenaltyLength, 0.02),
                                Vector3f(fl/2, gw/2 + sPenaltyLength, 0.02));
    mGLServer.DrawArbitraryLine(Vector3f(fl/2 - sPenaltyLength, -gw/2 - sPenaltyLength, 0.02),
                                Vector3f(fl/2 - sPenaltyLength, gw/2 + sPenaltyLength, 0.02) );

    //mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sPenaltyLength, -gw/2 - sPenaltyLength, 0.05), lw, gw + 33, 0);
    //mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sPenaltyLength, -gw/2 - sPenaltyLength, 0.05), sPenaltyLength, lw, 0);
    //mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sPenaltyLength, gw/2 + sPenaltyLength, 0.05), sPenaltyLength, lw, 0);

    // penalty area left side
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2 + sPenaltyLength, -gw/2 - sPenaltyLength, 0.02),
                                Vector3f(-fl/2, -gw/2 - sPenaltyLength, 0.02));
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2 + sPenaltyLength, gw/2 + sPenaltyLength, 0.02),
                                Vector3f(-fl/2, gw/2 + sPenaltyLength, 0.02));
    mGLServer.DrawArbitraryLine(Vector3f(-fl/2 + sPenaltyLength, -gw/2 - sPenaltyLength, 0.02),
                                Vector3f(-fl/2 + sPenaltyLength, gw/2 + sPenaltyLength, 0.02) );

    //mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sPenaltyLength, -gw/2 - sPenaltyLength, 0.05), lw, gw + 33, 0);
    //mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sPenaltyLength, -gw/2 - sPenaltyLength, 0.05), -sPenaltyLength, lw, 0);
    //mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sPenaltyLength, gw/2 + sPenaltyLength, 0.05), -sPenaltyLength, lw, 0);

    glLineWidth(2);

    mGLServer.DrawCircle(Vector3f(-fl/2, -fw/2, 0.02), 1.0);
    mGLServer.DrawCircle(Vector3f(-fl/2,  fw/2, 0.02), 1.0);
    mGLServer.DrawCircle(Vector3f( fl/2, -fw/2, 0.02), 1.0);
    mGLServer.DrawCircle(Vector3f( fl/2,  fw/2, 0.02), 1.0);

    // center circle
    mGLServer.DrawCircle(Vector3f(0.0,0.0,0.015), sCenterRadius);

    // draw shadows of cached positions
    DrawScene(0);
    glEnable(GL_DEPTH_TEST);

    // goal
    glLineWidth(1);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sGoalColor);
    //glColor3f(0.6f,0.6f,0.4f);
    mGLServer.DrawWireBox(Vector3f(-fl/2 - 0.0,-gw/2.0,0.0),szGoal1);
    //glColor4fv(sGoalColor);
    mGLServer.DrawGoal(Vector3f(-fl/2,-gw/2.0,0),szGoal1,lw/2.0);

    // goal
    glLineWidth(1);
    //glColor3f(0.6f,0.6f,0.4f);
    mGLServer.DrawWireBox(Vector3f(fl/2 + 0.0,-gw/2.0,0.0),szGoal2);
    //glColor4fv(sGoalColor);
    mGLServer.DrawGoal(Vector3f(fl/2,-gw/2.0,0),szGoal2,lw/2.0);

    glLineWidth(1);

    // draw cached positions
    DrawScene(1);

    glDisable(GL_LIGHTING);

    if (mDrawDebug)
        DrawDebug();

    // JAN
    if (mDrawVelocity)
        DrawVelocities();

    DrawStatusText();

    // draw 2D Elements
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    float aspect = float(mGLServer.GetWidth())/mGLServer.GetHeight();
    
    gluOrtho2D(-1.0*aspect, 1.0*aspect, -1.0,  1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw the info text
    DrawStatusLine();


    // mit fenstergroesse skalieren
//     glBegin(GL_QUADS);
//     glVertex2f(1.0,1.0);
//     glVertex2f(1.0,0.0);
//     glVertex2f(0.0,0.0);
//     glVertex2f(0.0,1.0);
//     glEnd();

    // draw two dimensional overview
    if (mDrawOverview)
    {
        DrawOverview();
    }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    // (end 2D Elements)
		
		checkAndShow();

    glutSwapBuffers();
}

void
Monitor::MouseMotion(int x, int y)
{
    Vector2f tmp(static_cast<float>(x), static_cast<float>(y));
    mGLServer.SetViewByMouse(tmp, mMousePosition);
    mMousePosition = tmp;
    glutPostRedisplay();
}

void
Monitor::Keyboard(unsigned char key, int /*x*/, int /*y*/)
{
    float hfl = mGameState.GetFieldLength()/2;
    Vector3f uv = mGLServer.GetUpVector();

    salt::Vector3f pos;
    switch (key) {
    case 'w':
        //move cam in
        mGLServer.MoveCamForward(mCamDelta);
        break;
    case 's':
        //move cam out
        mGLServer.MoveCamForward(-mCamDelta);
        break;
    case 'a':
        //strafe cam left
        mGLServer.MoveCamStrafe(mCamDelta);
        break;
    case 'd':
        // strafe cam right
        mGLServer.MoveCamStrafe(-mCamDelta);
        break;
    case 'c':
        // select camera mode
        mCameraMode = NextCameraMode(mCameraMode);
        // reset time to show icon (10 cycles)
        mShowCamIconTime = 10;
        break;
    case '+':
        //move camera up
        mGLServer.MoveCamUp(mCamDelta);
        break;
    case '-':
        //move camera down
        mGLServer.MoveCamUp(-mCamDelta);
        break;
    case '{':
        // select CenterBall camera mode
        mCenterBallCameraZ += 1.0;
        break;
    case '}':
        // select CenterBall camera mode
        mCenterBallCameraZ -= 1.0;
        break;
    case ']':
        mCenterBallCameraY += 2.0;
        break;
    case '[':
        mCenterBallCameraY -= 2.0;
        break;
    case ')':
        {
            //Rotate cam to right
            Vector3f t(mGLServer.GetLookAtPos()-mGLServer.GetCameraPos() );
            Vector3f d(cos(0.02) * t.x() + sin(0.02) * t.y()
                       ,-sin(0.02) * t.x() + cos(0.02) * t.y(), t.z());
            mGLServer.SetLookAtPos(d + mGLServer.GetCameraPos());
            mGLServer.SetUpVector(Vector3f(0,0,1));
        }
        break;
    case '(':
        {
            //Rotate cam to left
            Vector3f t(mGLServer.GetLookAtPos()-mGLServer.GetCameraPos() );
            Vector3f d(cos(0.02) * t.x() - sin(0.02) * t.y()
                       ,sin(0.02) * t.x() + cos(0.02) * t.y(), t.z());
            mGLServer.SetLookAtPos(d + mGLServer.GetCameraPos());
            mGLServer.SetUpVector(Vector3f(0,0,1));
        }
        break;
    case '1':
        // toggle drawing of debug stuff
        mDrawDebug  = !mDrawDebug;
        break;
    case '2':
        // toggle drawing of 2D overview
        mDrawOverview  = !mDrawOverview;
        break;
    case '3':
        //Move cam behind left goal
        mGLServer.SetCameraPos(Vector3f(-hfl-5,0,4.2));
        mGLServer.SetLookAtPos(Vector3f(0,0,1));
        mGLServer.SetUpVector(Vector3f(0,0,1));
        break;
    case '4':
        //Move cam behind right goal
        mGLServer.SetCameraPos(Vector3f(hfl+5,0,4.2));
        mGLServer.SetLookAtPos(Vector3f(0,0,1));
        mGLServer.SetUpVector(Vector3f(0,0,1));
        break;
    case '5':
        //Move cam to left half
        mGLServer.SetCameraPos(NextCameraPosition(mLeftCamPositions));
        if (mCameraMode == eFree) mGLServer.SetLookAtPos(Vector3f(0,0,1));
        mGLServer.SetUpVector(Vector3f(0,0,1));
        break;
    case '6':
        //Move cam to right half
        mGLServer.SetCameraPos(NextCameraPosition(mRightCamPositions));
        if (mCameraMode == eFree) mGLServer.SetLookAtPos(Vector3f(0,0,1));
        mGLServer.SetUpVector(Vector3f(0,0,1));
        break;
    case 'n':
        // toggle drawing of unums
        mDrawUnums = !mDrawUnums;
        break;
    case 'q':
        // quit
        mCommServer->SendDisconnectCmd();
        exit(0);
    case 'k' :
        // kick off
        cerr << "(rcssmonitor3d) " << mGameState.GetTime() << ": Kickoff "
             << ((mKickOff == CommServerBase::eLeft) ? "left" :
                 ((mKickOff == CommServerBase::eRight) ? "right" : "random")) << endl;
        mCommServer->SendKickOffCmd(mKickOff);
        break;
    case 'b':
        // drop ball
        mCommServer->SendDropBallCmd();
        break;
    case 'g':
        // Kick goalies if they are on the goals
        KickGoalies();
        break;
    case 'p' :
        // pause simulation
        mPauseGame = !mPauseGame;
        if (mPauseGame)
        {
            cout <<"(rcssmonitor3d) -- Pausing Simulation --" << endl;
            mCommServer->SendPauseCmd();
        } else {
            cout <<"(rcssmonitor3d) -- Running Simulation --" << endl;
            mCommServer->SendRunCmd();
        }
        break;
    case ' ':
        mKickOff = NextKickOffMode(mKickOff);
        break;
    case 'l' :
        // assign free kick to right team
        cerr << "(rcssmonitor3d) " << mGameState.GetTime()
             << ": Freekick left not implemented\n";
        //mCommServer->SendToWorldModel("(playMode free_kick_left)");
        break;
    case 'r' :
        // assign free kick to right team
        cerr << "(rcssmonitor3d) " << mGameState.GetTime()
             << ": Freekick right not implemented\n";
        //mCommServer->SendToWorldModel("(playMode free_kick_right)");
        break;
    case 't':
        // toggle texture
        mUseTexture = ! mUseTexture;
        break;
    case 'X':
        // select CenterBall camera mode
        mShowSecondView = ! mShowSecondView;
        break;

    case 'm' :
        // toggle single step mode (aka _m_anual advance)
        mSingleStep = !mSingleStep;
        mAdvance = false;
        break;
    case '>' :
        // advance a step
        mAdvance = true;
        mSingleStep = true;
        break;
    case '<' :
        if (mLogserver)
        {
            mCommServer->GoBack(2);
            mCommServer->ReadMessage();
            boost::shared_ptr<oxygen::PredicateList> predicates = mCommServer->GetPredicates();
            mOldGameState = mGameState;
            mGameState.ProcessInput(predicates);
            Display();
        }
        break;


    case '?' :
        // show keybindings
        KeyBindings();
        break;

    case 'f' :
        // fast
    mRealTime = !mRealTime;
        if (mRealTime)
            mDiffTime=DiffTime();
        break;

    case 'F' :
        // slow
    mRealTime = true;
    mDiffTime = DiffTime() ;
        break;

    case 'v':
        mDrawVelocity = !mDrawVelocity;
        //mCommServer->SendToWorldModel("(ball (pos 49 20 1) (vel 6.0 0.0 0.1))");
        break;

    case 'Z':
        glutFullScreen();
        glutPositionWindow(0,0);
        
        //    mDrawVelocity = !mDrawVelocity;
        //mCommServer->SendToWorldModel("(ball (pos 49 20 1) (vel 6.0 0.0 0.1))");
        break;

    default:
        return;
    }

    glutPostRedisplay();

}

void
Monitor::SpecialKeys(int glutkey, int /*x*/, int /*y*/)
{
    salt::Vector3f pos;
    switch (glutkey) {
    case GLUT_KEY_UP:
        //strafe cam forward
        mGLServer.MoveCamStrafeForward(mCamDelta);
        break;
    case GLUT_KEY_DOWN:
        //strafe cam back
        mGLServer.MoveCamStrafeForward(-mCamDelta);
        break;
    case GLUT_KEY_LEFT:
        //strafe cam left
        mGLServer.MoveCamStrafe(mCamDelta);
        break;
    case GLUT_KEY_RIGHT:
        // strafe cam right
        mGLServer.MoveCamStrafe(-mCamDelta);
        break;
    default:
        return;
    }

    glutPostRedisplay();

}

void
Monitor::Mouse(int /*button*/, int /*state*/, int x, int y)
{
    mMousePosition = Vector2f(static_cast<float>(x),
                              static_cast<float>(y));
}

void
Monitor::Reshape(int width, int height)
{
    mGLServer.Reshape(width,height);
}

long int
Monitor::DiffTime()
{

    int time;
    unsigned int utime, ugametime;
    float gametime;


    time = glutGet(GLUT_ELAPSED_TIME) * 1000;// millisec (0,001) --> micro
    utime = (unsigned int) time;

    gametime = mGameState.GetTime() * 1000000; // sec (in float) -->microsec (0,000001)
    ugametime = (unsigned int) gametime;

    return ugametime - utime;
}

void
Monitor::Idle()
{


    int time;
    unsigned int utime, ugametime;
    float gametime;

    time = glutGet(GLUT_ELAPSED_TIME) * 1000;// millisec (0,001) --> micro
    utime = (unsigned int) time;

    gametime = mGameState.GetTime() * 1000000; // sec (in float) -->microsec (0,000001)
    ugametime = (unsigned int) gametime;


    if ((DiffTime() - mDiffTime  > 0) && mRealTime && mLogserver)
    {
        usleep(DiffTime() - mDiffTime);
    }
    else usleep(10);

    // If we are in singlestep mode and not advancing then return
    if (mLogserver && mSingleStep && !mAdvance)
        return;

    if (mGameState.IsFinished())
    {
        cerr << "simulation finished... monitor exiting\n";
        exit(0);
    }

    if (! mCommServer->ReadMessage())
    {
        return;
    }

    static bool first_time = true;
    if (first_time) SetupCameraPositions();

    static int n = 10; // the first 10 messages should not be skipped
    --n;
    if (!first_time && n > 0) return;
    if (n <= 0)
    {
        n = mSkip;

        // set the goal markers to the right height
        if (first_time)
        {
            mFlagInfo[GameState::eGOAL_1_L].mOffset[2] = mGameState.GetGoalHeight();
            mFlagInfo[GameState::eGOAL_2_L].mOffset[2] = mGameState.GetGoalHeight();
            mFlagInfo[GameState::eGOAL_1_R].mOffset[2] = mGameState.GetGoalHeight();
            mFlagInfo[GameState::eGOAL_2_R].mOffset[2] = mGameState.GetGoalHeight();
        }
        
        first_time = false;
    }


    boost::shared_ptr<oxygen::PredicateList> predicates =
        mCommServer->GetPredicates();

    //        if (mDrawVelocity)
    mOldGameState = mGameState;

    mGameState.ProcessInput(predicates);

    mAdvance = false;
    glutPostRedisplay();
}


Monitor::ECameraMode
Monitor::NextCameraMode(ECameraMode mode) const
{
    switch (mode)
    {
    case eFree:
        return eFollowBall;
    case eFollowBall:
        return eCenterBall;
    default:
        return eFree;
    }
}

salt::Vector3f
Monitor::NextCameraPosition(const list<salt::Vector3f>& positions) const
{
    if (positions.empty()) return salt::Vector3f(0,0,0);

    list<salt::Vector3f>::const_iterator i;

    i = find(positions.begin(), positions.end(), mGLServer.GetCameraPos());
    if (i != positions.end()) i++;
    if (i == positions.end()) i = positions.begin();

    return *i;
}

CommServerBase::EKickOff
Monitor::NextKickOffMode(CommServerBase::EKickOff mode) const
{
    switch (mode)
    {
    case CommServerBase::eRandom: return CommServerBase::eLeft;
    case CommServerBase::eLeft: return CommServerBase::eRight;
    default: return CommServerBase::eRandom;
    }
}

void
Monitor::KickGoalies()
{
    Vector3f pos;
    int i = 0;
    int unum;
    TTeamIndex side;
    float size;
    std::stringstream cmd;

    cerr << "(rcssmonitor3d) " << mGameState.GetTime()
         << ": Kicking Goalies." << endl;

    while (mGameState.GetPlayer(i, pos, side, unum, size))
    {
        if (unum == 1 && pos[2] >= mGameState.GetGoalHeight())
        {
            // we found a goalie
            cmd << "(agent (team " << (side==TI_RIGHT ? "Right" : "Left")
                << ")(unum " << unum << ")(pos " << pos[0]<< " " << pos[1] << " " << pos[2]
                << ")(vel " << pos[0]/-pos[0] << " 0.0 1.0))";

            mCommServer->SendToWorldModel(cmd.str());
        }
        ++i;
    }
}

// void
// Monitor::DrawReport()
// {
//     char buf[1024];

//     glEnable(GL_BLEND);         // Turn Blending On
//     glColor4fv(sReportColor);
//     mGLServer.DrawGroundRectangle( Vector3f( -0.5, -0.7, 0 ), 1.0, 1.4, 0); 
//     glColor3f(1.0, 1.0, 1.0);

//     mGLServer.DrawTextPix("--   rcssmonitor3D Game Report   --",
//                           Vector2f( 0, 4*mGLServer.GetTextHeight() + 100),
//                           GLServer::eCENTER);

//     sprintf(buf, "Ball Existence In Left Field : %'.2f %%", 100.0*mBallInLeft/(mBallInLeft+mBallInCenter+mBallInRight) );
//     mGLServer.DrawTextPix(buf, Vector2f( 0, 7*mGLServer.GetTextHeight() + 100), GLServer::eCENTER);

//     sprintf(buf, "Ball Existence In Field's Center : %'.2f %%", 100.0*mBallInCenter/(mBallInLeft+mBallInCenter+mBallInRight) );
//     mGLServer.DrawTextPix(buf, Vector2f( 0, 8*mGLServer.GetTextHeight() + 100), GLServer::eCENTER);

//     sprintf(buf, "Ball Existence In Right Field : %'.2f %%", 100.0*mBallInRight/(mBallInLeft+mBallInCenter+mBallInRight) );
//     mGLServer.DrawTextPix(buf, Vector2f( 0, 9*mGLServer.GetTextHeight() + 100), GLServer::eCENTER);

//     glDisable(GL_BLEND);         // Turn Blending Off
// }

void
Monitor::checkAndShow()
{
    if (! mShowSecondView) return;
    
    Vector3f pos;
    float size;
    mGameState.GetBall(pos, size);
    
    if (pos[0] > 35)
        drawSecondView(20, 30, Vector3f(48.0f,-10.0f,5.0f), Vector3f(50.0f,18.0f,-15.0f) );
    else if (pos[0] < -35)
        drawSecondView(20, 30, Vector3f(-48.0f,-10.0f,5.0f), Vector3f(-50.0f,18.0f,-15.0f) );
    else
        drawSecondView(20, 30, Vector3f(0.0f,-6.0f,4.0f) + pos , Vector3f(0.0f,6.0f,-6.0f) + pos );
}

void
Monitor::drawSecondView( int sx, int sy, Vector3f campos, Vector3f lookat )
{
    const Vector3f szGoal1 = mGameState.GetGoalSize(false);
    const Vector3f szGoal2 = mGameState.GetGoalSize(true);

    glColor3f (1, 1, 1);
    glLoadIdentity();

		Vector3f look = mGLServer.GetLookAtPos();
		Vector3f pos = mGLServer.GetCameraPos();
		
    mGLServer.SetCameraPos( campos );
    mGLServer.SetLookAtPos( lookat );
		mGLServer.ApplyCamera();
		glViewport(sx, sy, mWidth/3 - 60, 160);

    const float& fl = mGameState.GetFieldLength();
    const float& fw = mGameState.GetFieldWidth();
    const float& fh = mGameState.GetFieldHeight();
    const float& lw = mGameState.GetLineWidth();
    const float& bs = mGameState.GetBorderSize();
    const float& gw = mGameState.GetGoalWidth();

    // ground
    glColor4fv(sSecondGroundColor);

    // left half
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + lw, -fw/2 + lw, 0),
                                  fl/2 - lw - lw/2, fw - 2*lw, 0);

    // right half
    mGLServer.DrawGroundRectangle(Vector3f(lw/2, -fw/2 + lw, 0),
                                  fl/2 - lw - lw/2, fw - 2*lw, 0);

    // border
    glColor4fv(sBorderColor);
    glDisable (GL_DEPTH_TEST);

    // border at left goal
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2-bs,-fw/2-bs,0),
                                  bs, fw+2*bs, 0);

    // border at right goal
    mGLServer.DrawGroundRectangle(Vector3f(fl/2,-fw/2-bs,0),
                                  bs, fw+2*bs, 0);

    // long top border
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2,-fw/2-bs,0),
                                  fl, bs, 0);

    // long bottom border
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2,fw/2,0),
                                  fl, bs, 0);

    // lines
    glColor4fv(sLineColor);

    // left goal
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2,-fw/2,0), lw, fw, 0);

    // right goal
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - lw,-fw/2,0), lw, fw, 0);

    // long top border
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + lw,-fw/2,0),
                                  fl - 2*lw, lw, 0);

    // long bottom border
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + lw,fw/2 - lw,0),
                                  fl - 2*lw, lw, 0);

    // middle line
    mGLServer.DrawGroundRectangle(Vector3f(-lw/2,-fw/2 + lw,0),
                                  lw, fw - 2*lw, 0);

    // goal box area right side
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.05), lw, gw + 11, 0);
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.05), sGoalBoxLength, lw, 0);
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sGoalBoxLength, gw/2 + sGoalBoxLength, 0.05), sGoalBoxLength, lw, 0);

    // goal box area left side
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.05), lw, gw + 11, 0);
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sGoalBoxLength, -gw/2 - sGoalBoxLength, 0.05), -sGoalBoxLength, lw, 0);
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sGoalBoxLength, gw/2 + sGoalBoxLength, 0.05), -sGoalBoxLength, lw, 0);

    // penalty area right side
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sPenaltyLength, -gw/2 - sPenaltyLength, 0.05), lw, gw + 33, 0);
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sPenaltyLength, -gw/2 - sPenaltyLength, 0.05), sPenaltyLength, lw, 0);
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - sPenaltyLength, gw/2 + sPenaltyLength, 0.05), sPenaltyLength, lw, 0);

    // penalty area left side
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sPenaltyLength, -gw/2 - sPenaltyLength, 0.05), lw, gw + 33, 0);
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sPenaltyLength, -gw/2 - sPenaltyLength, 0.05), -sPenaltyLength, lw, 0);
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + sPenaltyLength, gw/2 + sPenaltyLength, 0.05), -sPenaltyLength, lw, 0);

    // center circle
    mGLServer.DrawCircle(Vector3f(0.0,0.0,0.05), sCenterRadius);

    // fieldBox
    mGLServer.DrawWireBox(Vector3f(-fl/2.0,-fw/2.0,0.0),
                          Vector3f(fl,fw,fh));

    // draw shadows of cached positions
    DrawScene(0);
    glDisable(GL_DEPTH_TEST);

    // goal
    glColor4fv(sGoalColor);
    mGLServer.DrawWireBox(Vector3f(-fl/2,-gw/2.0,0),szGoal1);
    mGLServer.DrawGoal(Vector3f(-fl/2,-gw/2.0,0),szGoal1,lw/2.0);

    // goal
    glColor4fv(sGoalColor);
    mGLServer.DrawWireBox(Vector3f(fl/2,-gw/2.0,0),szGoal2);
    mGLServer.DrawGoal(Vector3f(fl/2,-gw/2.0,0),szGoal2,lw/2.0);

    // draw cached positions
    DrawScene(1);

    // JAN
    if (mDrawVelocity)
        DrawVelocities();

    glLoadIdentity();

    mGLServer.SetCameraPos( pos );
    mGLServer.SetLookAtPos( look );
		mGLServer.ApplyCamera();
}

