/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitor.cpp,v 1.16 2004/06/17 11:14:13 cringel Exp $

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
Monitor::sGoalColor[4]   = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat
Monitor::sBorderColor[4] = {0.2f, 0.8f, 0.2f, 1.0f};
const GLfloat
Monitor::sLineColor[4]   = {0.6f, 1.0f, 0.6f, 1.0f};
const GLfloat
Monitor::sSphereDefaultColor[4] = { 0.8, 0.8, 0.2, 1.0 };

const GLfloat
Monitor::sUnumColor[4]      = {1.0, 1.0, 1.0, 1.0};
// color for player of the different teams
const GLfloat
Monitor::sTeamColorLeft[4] = {1.0f, 0.2f, 0.2f, 1.0f};
const GLfloat
Monitor::sTeamColorRight[4] = {0.2f, 0.2f, 1.0f, 1.0f};
// ball color
const GLfloat
Monitor::sBallColor[4] = {1.0f, 0.67f, 0.0f, 1.0f};
// debug color1
const GLfloat
Monitor::sDebugColorCyan[4] = {0.0f, 0.8f, 0.8f, 0.3f};
// debug color2
const GLfloat
Monitor::sDebugColorPink[4] = {1.0f, 0.0f, 1.0f, 0.5f};

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
Monitor::s2DLowX = 0.01; // left pos

const float
Monitor::s2DLowY = 0.12; // top pos

const float
Monitor::s2DScale = 0.01; // Skalierung

Monitor::Monitor(std::string rel_path_prefix)
    : mZeitgeist("." PACKAGE_NAME, rel_path_prefix),
      mOxygen(mZeitgeist),
      mMonitorLib(mZeitgeist)
{
    mWidth = DEFAULT_WIDTH;
    mHeight = DEFAULT_HEIGHT;
    mCamDelta = 0.5;
    mCameraMode = eFree;
    mDrawUnums = true;
    mDrawOverview = true;
    mDrawDebug = false;
    mServer = DEFAULT_HOST;
    mPort = DEFAULT_PORT;
    mSkip = 1;

    mLogserver = false;
    mSingleStep = false;
    mAdvance = false;

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
    fi.mOffset = Vector3f(0,0,0.5);
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
"   --help      print this message and exit\n"
"   --port      specify the port number (default is " << DEFAULT_PORT << ")\n"
"   --server    specify the server host (default is '" << DEFAULT_HOST << "')\n"
"   --logfile   specify the logfile to read (not with --server)\n"
"   --msgskip   every but the nth message should be discarded (default is 1)\n"
"\n"
"Press '?' for a list of keybindings.\n"
"\n";
}

void
Monitor::Copyleft()
{
    std::cerr <<
"rcssmonitor3D-lite written by\n"
"Heni Ben Amor, Oliver Obst, Christoph Ringelstein, Markus Rollmann & Jan Murray;\n"
"Copyright (C) 2003 Universitaet Koblenz, Germany.\n"
"Copyright (C) 2004 The RoboCup Soccer Server Maintenance Group.\n\n";
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
        "r          | unpause the simulation/logplayer\n"
        "n          | toggle display of uniform numbers\n"
        "1          | toggle display of debug information\n"
        "2          | toggle display of two dimensional overview\n"
        "?          | display keybindings\n"
        "----------------------------------------------------\n"
        "CAMERA MOVEMENT\n"
        "----------------------------------------------------\n"
        "c          | toggle automatic (ball centered) camera\n"
        "w/s        | move camera forward/back (zoomlike)\n"
        "a/d        | move camera left/right\n"
        "+/-        | move camera up/down\n"
        "3          | move camera behind left goal\n"
        "4          | move camera behind right goal\n"
        "Arrow keys | move camera\n"
        "----------------------------------------------------\n"
        "SIMULATION\n"
        "----------------------------------------------------\n"
        "k          | kick off (start the game)\n"
        "b          | drop the ball at its current position\n"
        "[SPACE]    | toggle kick off side (left-right-random)\n"
        "----------------------------------------------------\n"
        "LOGPLAYER\n"
        "----------------------------------------------------\n"
        "m          | toggle single step mode for logplayer\n"
        ">          | move one step forward\n"
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

    //setup the GLserver with camera coordinates
    salt::Vector3f pos(0.0,-24.0, 7.0);
    salt::Vector3f lookAt(0.0,0.0,0.0);
    salt::Vector3f up(0.0,0.0,1.0);
    mGLServer = GLServer(mWidth, mHeight, pos, lookAt, up, false);
    mGLServer.InitGL();

    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    return eOK;
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
            mGLServer.SetLookAtPos(pos);
        }
    }

}

void
Monitor::DrawStatusText()
{
    std::stringstream sl, sc, sr;

    sl << mGameState.GetTeamnameLeft() << " " << mGameState.GetScoreLeft();
    sr << mGameState.GetScoreRight() << " " << mGameState.GetTeamnameRight();
    sc << "(" << ((mGameState.GetHalf() == GH_FIRST) ?
                  "first" : "second") << " half) ";

    int play_mode = mGameState.GetPlayMode();

    switch (play_mode)
    {
    case PM_KickOff_Left:
        mKickOff = CommServerBase::eRight;
        break;
    case PM_KickOff_Right:
        mKickOff = CommServerBase::eLeft;
        break;
    };

    sc << mGameState.PlayMode2Str(play_mode) << " ";
    sc << "t=" << setiosflags(ios::fixed) << setprecision(2) << mGameState.GetTime();

    glColor3f(1.0, 1.0, 1.0);
    mGLServer.DrawTextPix(sl.str().c_str(),
                          Vector2f( 0, mGLServer.GetTextHeight()));
    mGLServer.DrawTextPix(sc.str().c_str(),
                          Vector2f( 0, mGLServer.GetTextHeight()),
                          GLServer::eCENTER);
    mGLServer.DrawTextPix(sr.str().c_str(),
                          Vector2f( 0, mGLServer.GetTextHeight()),
                          GLServer::eRIGHT);
}

void
Monitor::DrawStatusLine()
{
    glColor4fv(sTeamColorLeft);
    mGLServer.DrawSphere(Vector3f(-0.97,0.93,0.0), 0.025, 20);
    glColor4fv(sTeamColorRight);
    mGLServer.DrawSphere(Vector3f( 0.97,0.93,0.0), 0.025, 20);

    int play_mode = mGameState.GetPlayMode();

    if (play_mode == PM_BeforeKickOff)
    {
        glColor4fv(sSphereDefaultColor);
        switch (mKickOff)
        {
        case CommServerBase::eRandom:
            mGLServer.DrawSphere(Vector3f( 0.0,0.93,0.0), 0.025, 20);
            break;
        case CommServerBase::eLeft:
            mGLServer.DrawSphere(Vector3f( -0.92,0.93,0.0), 0.025, 20);
            break;
        case CommServerBase::eRight:
            mGLServer.DrawSphere(Vector3f( 0.92,0.93,0.0), 0.025, 20);
            break;
        }
    }
}

Vector3f Monitor::Get2DPos(const Vector3f& pos)
{
    return Vector3f
        (
         -1.0 + s2DLowX + s2DScale * (pos[0] + mGameState.GetFieldLength()/2),

         1.0 - s2DLowY - (s2DScale * mGameState.GetFieldWidth()) +
         s2DScale * (pos[1] + mGameState.GetFieldWidth()/2),

         0.0
         );
}

void
Monitor::DrawOverview()
{
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

    Vector3f pos;
    float size;
    if (mGameState.GetBall(pos, size))
    {
        glColor4fv(sBallColor);
        mGLServer.DrawSphere(Get2DPos(pos), b_radius, res);
    }

    int i = 0;
    int unum;
    TTeamIndex side;
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

        if (mDrawUnums)
            {
                glColor4fv(sUnumColor);
                stringstream ss;
                ss << unum;
                mGLServer.DrawText3D(ss.str(), Get2DPos(pos));
            }

        ++i;
    }
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
        glColor4fv(fi.mColor);
        mGLServer.DrawSphere(pos + fi.mOffset, fi.mRadius);
    }
}

void
Monitor::DrawPlayer(TTeamIndex side,
                    int unum, const salt::Vector3f& pos, float size, int pass)
{
    if (pass == 0)
    {
        mGLServer.DrawShadowOfSphere(pos, size);
    }
    else
    // pass=1, second pass with depth test
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

        mGLServer.DrawSphere(pos, size);

        if (mDrawUnums && unum > 0)
        {
            glColor4fv(sUnumColor);
            std::stringstream ss;
            ss << unum;
            mGLServer.DrawText3D(ss.str(), pos);
        }
    }
}

void
Monitor::DrawBall(const salt::Vector3f& pos, float size, int pass)
{
    if (pass == 0)
    {
        mGLServer.DrawShadowOfSphere(pos, size);
    }
    else
    // pass=1, second pass with depth test
    {
        glColor4fv(sBallColor);
        mGLServer.DrawSphere(pos, size);
    }
}

// Example for DrawDebug
void
Monitor::DrawDebug()
{
    // this example method just draws the positive axes on the field 
    glColor4fv(sDebugColorPink);
    
    mGLServer.DrawArbitraryLine(Vector3f(0,0,0), Vector3f(10,0,0));
    mGLServer.DrawArbitraryLine(Vector3f(0,0,0), Vector3f(0,10,0));
    mGLServer.DrawArbitraryLine(Vector3f(0,0,0), Vector3f(0,0,10));
    return;
}

                                                       
void
Monitor::Display()
{


    const Vector3f szGoal1 = mGameState.GetGoalSize(false);
    const Vector3f szGoal2 = mGameState.GetGoalSize(true);

    glClearColor(0.15f,0.15f,0.3f,1.0f);
    glClear (GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
    glColor3f (1, 1, 1);

    glLoadIdentity();
    mGLServer.ApplyCamera();

    const float& fl = mGameState.GetFieldLength();
    const float& fw = mGameState.GetFieldWidth();
    const float& fh = mGameState.GetFieldHeight();
    const float& lw = mGameState.GetLineWidth();
    const float& bs = mGameState.GetBorderSize();
    const float& gw = mGameState.GetGoalWidth();

    // ground
    glColor4fv(sGroundColor);

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
    glEnable(GL_DEPTH_TEST);

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

    if (mDrawDebug)
        DrawDebug();
    
    DrawStatusText();

    // draw 2D Elements
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // draw the info text
    DrawStatusLine();

    // draw two dimensional overview
    if (mDrawOverview)
        {
            DrawOverview();
        }

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    // (end 2D Elements)


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
        break;
    case '+':
        //move camera up
        mGLServer.MoveCamUp(mCamDelta);
        break;
    case '-':
        //move camera down
        mGLServer.MoveCamUp(-mCamDelta);
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
        mGLServer.SetCameraPos(Vector3f(-hfl-2,0,0.2));
        mGLServer.SetLookAtPos(Vector3f(0,0,1));
        mGLServer.SetUpVector(Vector3f(0,0,1));
        break;
    case '4':
        //Move cam behind right goal
        mGLServer.SetCameraPos(Vector3f(hfl+2,0,0.2));
        mGLServer.SetLookAtPos(Vector3f(0,0,1));
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
        mCommServer->SendKickOffCmd(mKickOff);
        break;
    case 'b':
        // drop ball
        mCommServer->SendDropBallCmd();
        break;
    case 'p' :
        // pause simulation
        cout <<"--- Pausing Simulation" << endl;
        mCommServer->SendPauseCmd();
        break;
    case ' ':
        mKickOff = NextKickOffMode(mKickOff);
        break;
    case 'r' :
        // run simulation (after pause command)
        cout <<"--- Running Simulation" << endl;
        mCommServer->SendRunCmd();
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
    case '?' :
        // show keybindings
        KeyBindings();
        break;
#if 0
    case 'v':
        mCommServer->SendToWorldModel("(ball (pos 49 20 1) (vel 6.0 0.0 0.1))");
        break;
#endif
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

void
Monitor::Idle()
{

    usleep (10);

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
    static int n = 10; // the first 10 messages should not be skipped
    --n;
    if (!first_time && n > 0) return;
    if (n <= 0)
    {
        n = mSkip;
        first_time = false;
    }


        boost::shared_ptr<oxygen::PredicateList> predicates =
        mCommServer->GetPredicates();

        mGameState.ProcessInput(predicates);

        mAdvance = false;

        glutPostRedisplay();

}

Monitor::ECameraMode
Monitor::NextCameraMode(ECameraMode mode) const
{
    switch (mode)
    {
    case eFree: return eFollowBall;
    default:    return eFree;
    }
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
