/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: spark.h,v 1.2 2004/04/11 17:12:22 rollmark Exp $

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
#ifndef KEROSIN_SPARK_H
#define KEROSIN_SPARK_H

#include <zeitgeist/zeitgeist.h>
#include <kerosin/kerosin.h>
#include <oxygen/oxygen.h>

namespace spark
{
class Spark
{
public:
    // define common input constants
    enum ECmds
        {
            CmdTimer    = 1,
            CmdMouseX   = 2,
            CmdMouseY   = 3,
            CmdUp       = 4,
            CmdDown     = 5,
            CmdLeft     = 6,
            CmdRight    = 7,
            CmdForward  = 8,
            CmdBackward = 9,
            CmdQuit     = 10,

            // first user defined input constant
            CmdUser = 11
        };

public:
    Spark(const std::string& relPathPrefix);
    virtual ~Spark();

    /** inits the Spark lib, returns true on success, has to be called once
        before any other spark methods */
    bool Init(int argc, char** argv);

    /** starts the Spark runloop, returns after cmdQuit is
        processed */
    void Run();

    //
    // user callbacks

    /** called once after Spark finished it's init */
    virtual bool InitApp(int argc, char** argv);

    /** process user defined input constants */
    virtual void ProcessInput(kerosin::InputServer::Input& input);

    /** take user defined action after processing all input events and
        prior rendering the current frame */
    virtual void UpdatePreRenderFrame();

    //
    // set/get methods

    /** sets the location of the FPS Controller to be used */
    bool SetFPSController(const std::string& path);

    /** sets the FPSController to be used */
    void SetFPSController(boost::shared_ptr<oxygen::FPSController> controller);

    /** sets the horizontal mouse sensitivity */
    void SetHorizontalSensitivity(float s);

    /** returns the horizontal mouse sensitivity */
    float GetHorizontalSensitivity();

    /** sets the horizontal mouse sensitivity */
    void SetVerticalSensitivity(float s);

    /** returns the vertical mouse sensitivity */
    float GetVerticalSensitivity();

    /** returns the last measured timestep */
    float GetDeltaTime();

    /** returns the total time passed */
    float GetTime();

    /** returns the total number of rendered frames */
    int GetFramesRendered();

    /** sets the size of a simstep*/
    void SetSimStep(float time);

    /** returns the zeitgeist instance */
    zeitgeist::Zeitgeist& GetZeitgeist();

    /** returns the zeitgeist core instance */
    boost::shared_ptr<zeitgeist::Core> GetCore();

    /** returns the LogServer */
    boost::shared_ptr<zeitgeist::LogServer> GetLog();

    /** returns the ScriptServer */
    boost::shared_ptr<zeitgeist::ScriptServer> GetScriptServer();

    /** returns the SceneServer */
    boost::shared_ptr<oxygen::SceneServer> GetSceneServer();

    /** returns the current active Scene */
    boost::shared_ptr<oxygen::Scene> Spark::GetActiveScene();

protected:
    /** reads input from the InputServer processes common commands and
        calls ProcessInput to handle user defined commands
     */
    void ReadInput();

protected:
    boost::shared_ptr<zeitgeist::LogServer> mLogServer;
    boost::shared_ptr<zeitgeist::ScriptServer> mScriptServer;
    boost::shared_ptr<oxygen::SceneServer> mSceneServer;
    boost::shared_ptr<oxygen::FPSController> mFPSController;
    boost::shared_ptr<kerosin::OpenGLServer> mOpenGLServer;
    boost::shared_ptr<kerosin::InputServer> mInputServer;
    boost::shared_ptr<kerosin::RenderServer> mRenderServer;

    zeitgeist::Zeitgeist mZeitgeist;
    oxygen::Oxygen mOxygen;
    kerosin::Kerosin mKerosin;

    //! the last measured timestep
    float mDeltaTime;

    //! total time passed
    float mTime;

    //! total frames rendered
    int mFramesRendered;

    //! horizontal mouse sensitivity
    float mHorSens;

    //! vertical mouse sensitivity
    float mVertSens;

    //! the deltaTime of a single simStep, set to nonzero to force a
    //fixed simstep
    float mSimStep;

    //! the summed up deltatimes until a simStep is accumulated
    float mSumDeltaTime;
};

} // namespace kerosin

#endif // SPARK_SPARK_H
