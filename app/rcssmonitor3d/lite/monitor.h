/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitor.h,v 1.1.2.1 2004/05/05 15:46:16 fruit Exp $

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
#include "glserver.h"
#include <monitorlib.h>
#include <gamestate.h>
#include <commserver.h>

class Monitor
{
public:
    enum EReturnType { eOK, eOptions, eErrInit, eErrRun };

public:
    ~Monitor();

    static Monitor* Instance();

    /** Initialize the Monitor.
     *  Parse command line options, setup CommServer and Parser,
     *  and run the init script which should register the scenario
     *  and the evaluation function.
     *
     *  \return eOK      on success,
     *          eOptions if command line options should be printed,
     *          eErrInit if initialization failed.
     */
    EReturnType Init(int argc, char* argv[]);

    /** The monitor main loop.
     *  Get input from the communication server, parse the
     *  input, display scene and so on.
     *
     *  \return eOK on success, eErrRun if some error occurred.
     */
    EReturnType Run();

    /** Print some hints on using the command line options. */
    static void Usage();

    /** Print copyright message */
    static void Copyleft();

    void Display();
    void MouseMotion(int x, int y);
    void Keyboard(unsigned char key, int x, int y);
    void Mouse(int button, int state, int x, int y);
    void Reshape(int width, int height);
    void Idle();

private:
    // typedefs, enums, constants
    struct FlagInfo
    {
        salt::Vector3f mOffset;
        float mRadius;
        GLfloat mColor[4];
    };

    enum ECameraMode { eFree, eFollowBall };
    typedef std::map<GameState::EFlagType, FlagInfo> TFlagInfoMap;

    static const GLfloat sGroundColor[4];
    static const GLfloat sGoalColor[4];
    static const GLfloat sBorderColor[4];
    static const GLfloat sLineColor[4];
    static const GLfloat sTeamColorLeft[4];
    static const GLfloat sTeamColorRight[4];
    static const GLfloat sSphereDefaultColor[4];
    static const GLfloat sBallColor[4];

private:
    // Constructor is private since monitor is a singleton
    Monitor(std::string rel_path_prefix = "../../../");
    // methods
    EReturnType InitInternal(int argc, char* argv[]);
    void DrawScene(int pass);
    void DrawStatusLine();
    void DrawFlag(GameState::EFlagType i, salt::Vector3f pos, int pass);
    void DrawPlayer(TTeamIndex side, int unum, const salt::Vector3f& pos, float size, int pass);
    void DrawBall(const salt::Vector3f& pos, float size, int pass);
    //! successor of a camera mode
    ECameraMode NextCameraMode(ECameraMode mode) const;

private:
    // member variables
    GLServer mGLServer;
    GameState mGameState;
    //! the zeitgeist core
    zeitgeist::Zeitgeist mZeitgeist;
    //! the oxygen core
    oxygen::Oxygen mOxygen;
    //! the monitor library
    MonitorLib mMonitorLib;
    //! communication Server
    boost::shared_ptr<CommServer> mCommServer;

    //! window width
    int mWidth;
    //! window height
    int mHeight;
    //! mouse motion coordinates
    salt::Vector2f mMousePosition;
    //! amount by which the camera is moved
    float mCamDelta;
    //! the current camera mode
    ECameraMode mCameraMode;
    //! flag if we want to see uniform numbers
    bool mDrawUnums;
    // connection parameters
    //! the server name
    std::string mServer;
    //! the port number
    int mPort;

    //! flag informations for specific flags
    TFlagInfoMap mFlagInfo;
    //! default flag information (used for all flags not found in mFlagInfo)
    FlagInfo mDefaultFlagInfo;
};

