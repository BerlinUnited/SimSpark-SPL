/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitor.cpp,v 1.2 2004/05/10 14:10:46 fruit Exp $

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
#include <types.h>
#include <getopt.h>
#include <soccer/soccertypes.h>
#include <iomanip>

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
// color for player of the different teams
const GLfloat
Monitor::sTeamColorLeft[4] = {1.0f, 0.2f, 0.2f, 1.0f};
const GLfloat
Monitor::sTeamColorRight[4] = {0.2f, 0.2f, 1.0f, 1.0f};
// ball color
const GLfloat
Monitor::sBallColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};

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
    mServer = DEFAULT_HOST;
    mPort = DEFAULT_PORT;

    FlagInfo fi;
    fi.mOffset = Vector3f(0,0,0);
    fi.mRadius = 0.16;
    memcpy(fi.mColor, sSphereDefaultColor, sizeof(fi.mColor));

    // corner flags left team
    mDefaultFlagInfo = fi;
    fi.mOffset = Vector3f(0,0,1.5);
    memcpy(fi.mColor, sTeamColorLeft, sizeof(fi.mColor));
    mFlagInfo[GameState::eFLAG_1_L] = fi;
    mFlagInfo[GameState::eFLAG_2_L] = fi;
    mFlagInfo[GameState::eGOAL_1_L] = fi;
    mFlagInfo[GameState::eGOAL_2_L] = fi;
    // corner flags right team
    memcpy(fi.mColor, sTeamColorRight, sizeof(fi.mColor));
    mFlagInfo[GameState::eFLAG_1_R] = fi;
    mFlagInfo[GameState::eFLAG_2_R] = fi;
    mFlagInfo[GameState::eGOAL_1_R] = fi;
    mFlagInfo[GameState::eGOAL_2_R] = fi;
}

Monitor::~Monitor()
{
    std::cerr << "(Monitor) shutting down\n";
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

    option long_options[] = {
        // name, has_arg, flag, val
        { "help", no_argument, 0, 'h' },
        { "port", required_argument, 0, 'p' },
        { "server", required_argument, 0, 's' },
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
            break;
        default:
            status = eErrInit;
        }
    }

    mZeitgeist.GetCore()->GetScriptServer()->RunInitScript
        ("rcssmonitor3D-lite.rb", "app/rcssmonitor3d/lite");

    if (status == eOK)
        status = InitInternal(argc,argv);

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
"       --help          print this message and exit\n"
"       --port          specify the port number (default is " << DEFAULT_PORT << ")\n"
"       --server        specify the server host (default is '" << DEFAULT_HOST << "')\n"
"\n";
}

void
Monitor::Copyleft()
{
    std::cerr <<
"rcssmonitor3D-lite written by\n"
"Heni Ben Amor, Oliver Obst, Christoph Ringelstein, and Markus Rollmann;\n"
"Copyright (C) 2003 Universitaet Koblenz, Germany.\n"
"Copyright (C) 2004 The RoboCup Soccer Server Maintenance Group.\n\n";
}

Monitor::EReturnType
Monitor::InitInternal(int argc, char* argv[])
{
    // init the commserver
    mCommServer = shared_dynamic_cast<CommServer>
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
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);

    //setup the GLserver with camera coordinates
    salt::Vector3f pos(0.0,-24.0, 7.0);
    salt::Vector3f lookAt(0.0,0.0,0.0);
    salt::Vector3f up(0.0,0.0,1.0);
    mGLServer = GLServer(mWidth, mHeight, pos, lookAt, up, false);
    mGLServer.InitGL();

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
Monitor::DrawStatusLine()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glColor4fv(sTeamColorLeft);
    mGLServer.DrawSphere(Vector3f(-0.97,0.93,0.0), 0.025, 20);
    glColor4fv(sTeamColorRight);
    mGLServer.DrawSphere(Vector3f( 0.97,0.93,0.0), 0.025, 20);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    stringstream sl, sc, sr;

    sl << mGameState.GetTeamnameLeft() << " " << mGameState.GetScoreLeft();
    sr << mGameState.GetScoreRight() << " " << mGameState.GetTeamnameRight();
    sc << "(" << ((mGameState.GetHalf() == GH_FIRST) ?
                  "first" : "second") << " half) ";

    std::string mode;

    switch (mGameState.GetPlayMode())
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
            glColor3f(1.0, 1.0, 1.0);
            stringstream ss;
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
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - 5.5, -gw/2 - 5.5, 0.05), lw, gw + 11, 0);
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - 5.5, -gw/2 - 5.5, 0.05), 5.5, lw, 0);
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - 5.5, gw/2 + 5.5, 0.05), 5.5, lw, 0);

    // goal box area left side
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 5.5, -gw/2 - 5.5, 0.05), lw, gw + 11, 0);
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 5.5, -gw/2 - 5.5, 0.05), -5.5, lw, 0);
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 5.5, gw/2 + 5.5, 0.05), -5.5, lw, 0);

    // penalty area right side
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - 16.5, -gw/2 - 16.5, 0.05), lw, gw + 33, 0);
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - 16.5, -gw/2 - 16.5, 0.05), 16.5, lw, 0);
    mGLServer.DrawGroundRectangle(Vector3f(fl/2 - 16.5, gw/2 + 16.5, 0.05), 16.5, lw, 0);

    // penalty area left side
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 16.5, -gw/2 - 16.5, 0.05), lw, gw + 33, 0);
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 16.5, -gw/2 - 16.5, 0.05), -16.5, lw, 0);
    mGLServer.DrawGroundRectangle(Vector3f(-fl/2 + 16.5, gw/2 + 16.5, 0.05), -16.5, lw, 0);

    // center circle
    mGLServer.DrawCircle(Vector3f(0.0,0.0,0.05), 9.15);

    // fieldBox
    mGLServer.DrawWireBox(Vector3f(-fl/2.0,-fw/2.0,0.0),
                          Vector3f(fl,fw,fh));

    // draw shadows of cached positions
    DrawScene(0);
    glEnable(GL_DEPTH_TEST);

    // goal
    glColor4fv(sGoalColor);
    mGLServer.DrawWireBox(Vector3f(-fl/2,-gw/2.0,0),szGoal1);
    mGLServer.DrawGoal(Vector3f(-fl/2,-gw/2.0,0),szGoal1);

    // goal
    glColor4fv(sGoalColor);
    mGLServer.DrawWireBox(Vector3f(fl/2,-gw/2.0,0),szGoal2);
    mGLServer.DrawGoal(Vector3f(fl/2,-gw/2.0,0),szGoal2);

    // draw cached positions
    DrawScene(1);
    // draw the info text
    DrawStatusLine();

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
    case ' ':
        //move camera down
        mGLServer.MoveCamUp(-mCamDelta);
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
        mCommServer->SendKickOffCmd();
        break;

    case 'p' :
        // pause simulation
        cout <<"--- Pausing Simulation" << endl;
        mCommServer->SendPauseCmd();
        break;

    case 'r' :
        // run simulation (after pause command)
        cout <<"--- Running Simulation" << endl;
        mCommServer->SendRunCmd();
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
    if (! mCommServer->ReadMessage())
    {
        return;
    }
    boost::shared_ptr<oxygen::PredicateList> predicates =
        mCommServer->GetPredicates();

    mGameState.ProcessInput(predicates);

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
