/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2004 RoboCup Soccer Server 3D Maintenance Group
   $Id: monitor.h,v 1.11 2006/02/08 15:04:52 jamu Exp $

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
#include <commserverbase.h>

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

    /** show keybindings */
    static void KeyBindings();

    void Display();
    void MouseMotion(int x, int y);
    void Keyboard(unsigned char key, int x, int y);
    void SpecialKeys(int glutkey, int x, int y);
    void Mouse(int button, int state, int x, int y);
    void Reshape(int width, int height);
    void Idle();
    void Idle2();

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
    static const GLfloat sGroundColor2D[4];
    static const GLfloat sGoalColor[4];
    static const GLfloat sBorderColor[4];
    static const GLfloat sLineColor[4];
    static const GLfloat sTeamColorLeft[4];
    static const GLfloat sTeamColorRight[4];
    static const GLfloat sSphereDefaultColor[4];
    static const GLfloat sBallColor[4];
    static const GLfloat sDebugColorCyan[4];
    static const GLfloat sDebugColorPink[4];

    static const GLfloat sUnumColor[4];
    static const GLfloat sCenterRadius;
    static const GLfloat sGoalBoxLength;
    static const GLfloat sPenaltyLength;

    static const float s2DLowX;
    static const float s2DLowY;
    static const float s2DScale;

private:
    // Constructor is private since monitor is a singleton
    Monitor(std::string rel_path_prefix = "../../../");
    // methods
    EReturnType InitInternal(int argc, char* argv[]);
    void DrawScene(int pass);
    void DrawStatusLine();
    void DrawStatusText();

    salt::Vector3f Get2DPos(const salt::Vector3f& pos);
    void DrawOverview();

    void DrawFlag(GameState::EFlagType i, salt::Vector3f pos, int pass);
    void DrawPlayer(TTeamIndex side, int unum, const salt::Vector3f& pos, float size, int pass);
    void DrawBall(const salt::Vector3f& pos, float size, int pass);
    void DrawDebug();
    void DrawVelocities();
    long int DiffTime();

    //! Set the default camera positions
    void SetupCameraPositions();

    //! successor of a camera mode
    ECameraMode NextCameraMode(ECameraMode mode) const;
    /** Get the successor of the current camera position from a list.
        If the current camera position is not in the list, the first position from
        the list is returned.
    */
    salt::Vector3f NextCameraPosition(const std::list<salt::Vector3f>& positions) const;
    //! successor of a kick off mode
    CommServerBase::EKickOff NextKickOffMode(CommServerBase::EKickOff mode) const;

private:
    // member variables
    GLServer mGLServer;
    GameState mGameState;
    string mTextureFile;
    
    //JAN
    GameState mOldGameState;

    //! the zeitgeist core
    zeitgeist::Zeitgeist mZeitgeist;
    //! the oxygen core
    oxygen::Oxygen mOxygen;
    //! the monitor library
    MonitorLib mMonitorLib;
    //! communication Server
    boost::shared_ptr<CommServerBase> mCommServer;

    //! a list of standard camera positions for the left team
    std::list<salt::Vector3f> mLeftCamPositions;
    //! a list of standard camera positions for the right team
    std::list<salt::Vector3f> mRightCamPositions;

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
    //! the current kick off mode
    CommServerBase::EKickOff mKickOff;
    //! flag if we want to see uniform numbers
    bool mDrawUnums;
    //! flag if we want to draw the 2D overview
    bool mDrawOverview;
    //! flag if we want to draw debug stuff
    bool mDrawDebug;
    // connection parameters
    //! the server name
    std::string mServer;
    //! the port number
    int mPort;
    //! skip factor
    int mSkip;

    //! flag informations for specific flags
    TFlagInfoMap mFlagInfo;
    //! default flag information (used for all flags not found in mFlagInfo)
    FlagInfo mDefaultFlagInfo;

    //! flag for logserver
    bool mLogserver;
    //! flag for single step
    bool mSingleStep;
    //! flag for drawing velocities
    bool mDrawVelocity;
    //! flag for advancing
    bool mAdvance;
    //! flag for real time or fast playback
    bool mRealTime;
    long int mDiffTime;
};
