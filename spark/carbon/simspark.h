/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
   $Id$

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

#ifndef GUISIMSPARK_H
#define GUISIMSPARK_H
/**
 * \file simspark.h
 * \brief Contains SimSpark class.
 */

#include <spark/spark.h>
#include <zeitgeist/logserver/logserver.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#ifndef Q_MOC_RUN
#include <boost/shared_ptr.hpp>
#endif

class SimulationTimer;
class LogStream;

/*!
 \brief Spark derived class. Contains the Spark Core and handles initialization and basic controls.

 <b>General:</b>\n
 This class is the heart of the Simulation, the only class derived from Spark and - together with SparkController - representing the interface to the Spark
 library and its simulation environment.\n

 <b>Definition and creation:</b>\n
 SimSpark is started by a ServerThread, which is a SimulationTask defined by a TaskDefinition in a SimulationSetup.
 The definition for properties of the spark execution can be done using any of the plugins and modification options the Gui offers.
 See SimulationFrame and SetupFrame for two plugins that allow creating new SimulationSetups or changing the setup in the current Simulation.\n

The initialization and execution properties of SimSpark consist of:\n
Main Script: The script initializing the Simulation (e.g. "rcssserver3d.rb"). Will be executed the first time the simulation is executed.\n
Secondary Script: The script initializing the Spark core (e.g. "spark.rb"). The other initialization scripts (oxygen.rb, salt.rb, etc.) are expected 
to be in the same directory. Will be executed once at time of initialization. Usually contains the base set of functions that can be used in other scripts.\n
Additional Scripts: A list of additional scripts to execute every time the Simulation is started. Executed after the main script and before calling run().\n


 <b>Script files:</b>\n
 SimSpark uses ruby scripts to initialize the core, setup the simulation and access it during runtime.
 The secondary script (e.g. "spark.rb") usually initializes the core and specifies macro functions for other scripts to use.
 The main script initializes the simulation and sets up the content of the zeitgeist tree.
 The additional scripts are meant to provide a fast and easy way to change the Simulation properties without the need of bein able to understand
 Ruby or the spark script interface.
 With a rich variety of elementary scripts the setup of a Simulation can be altered to essentially form new Simulation in the scope of the implemented
 and accessible functionality.

 For example, the "rcssserver3d.rb" script file starts a server, defines a camera, starts the soccer simulation and possibly adds an internal monitor.
 By writing different kinds of elementary scripts that each accomplish only one of these things, a list of additional scripts can alter the initial
 setup of the Simulation.
 To make sure that there are no dangerously illegal calls, the elementary scripts should check and asset a correct order.
 This means for example, that any script that modifies the initialization of the soccer simulation should check if the required dynamic libraries
 for the soccer simulation have already been loaded.

 Usually the script-solution as opposed to a solution who uses compiled code is meant to be as flexible as possible.
 However, this also leaves the user with the full responsibility of understanding the underlying functionality.
 A middle-level in form of easily combineable script-blocks each modifying the simulation in an understandable way makes the program powerful and flexible
 as well as more easily controllable, understandable and more robust at the same time.

 <b>Using an own SimSpark Monitor:</b>\n
 Most of the rendering functions, texture initialization and other OpenGL commands used by Spark and its plugins wont call glMakeCurrent() to make
 the created rendering context current in the simulation server thread.
 Therefore its easy to render the scene graph managed in the simulation server thread in another thread, e.g. with a GlWidget in the gui main thread.
 For this to succeed the renderer has to offer the OpenGl-Context to the SparkController using the OpenGlManager, so the server can initialize
 textures and so on in the correct context.
 In the case of there being an external renderer, the SparkController will set the script variable $gui_enableExternalRendering and disable rendering
 of custom render nodes.\n

 \todo <b>Using an own SimSpark Monitor:</b>\n
 Rendering the scene graph externallyonly works if there are no plugins in SimSpark that depend on owning the rendering context, like for
 example vision perceptors for robots that have to render the scene themselves.
 To change this, either implement a plugin in Spark that can manage switching the OpenGl-context (which might be costly due to additional thread barriers),
 or write a rendering plugin in Spark which renders the whole scene into a texture which than can be copied or offered to a different rendering context
 in the Gui. This way, a renderer in the gui would just have to display this texture instead of rendering everything itself and the rendering control
 remains within SimSpark.\n

 \todo <b>Problems with SimSpark:</b>\n
 SimSpark was initially not meant to be started more than once. It is not possible to create more than one instance of a zeitgeist::Core, because there are
 several static references in the code.
 It is also no possible to gracefully shutdown the single zeitgeist::Core completely so it could be cleanly reinitialized.
 Therefore it is currently neither possible to instantiate more than one SparkSimulationThread in one Simulation, nor can two Simulations in one sesssion
 use a different core initialization Script.\n
 Currently the SimulationManager stores the first ServerThread that is created and prevents it from being deleted.
 If the Simulation is stopped, SimSpark sadly cant be stopped completely using SimulationServer::Quit(), but it is paused and tried to be resetted.
 Even if this might work for Simulations who only differ in simple initialization skripts, it is a major drawback for the flexibility of a general
 simulation system and should be reworked as soon as possible.\n
*/
class SimSpark : public spark::Spark
{

    friend class SparkController;

public:
    //public types

    /*!
     \brief Describes the state of SimSpark.
    */
    enum ESimSparkState
    {
        SSS_PREINIT,      /*!< State after creation but before execution of the init script. */
        SSS_INITIALIZED,  /*!< State after execution of the init script. */
        SSS_RUNNING,      /*!< SimulationServer is running. */
        SSS_PAUSED        /*!< SimulationServer is paused. */
    };

    /*!
     \brief Types of ruby variables that can be initialized at time of SimSpark setup.
    */
    enum ERubyVariableType
    {
        RVT_BOOL,   /*!< Boolean value. */
        RVT_INT,    /*!< Integer value. */
        RVT_FLOAT,  /*!< Float value. */
        RVT_STRING, /*!< String value. */
        RVT_NIL     /*!< Nil value. */
    };

    /*!
     \brief Struct defining a single ruby value to be set during SimSpark initialization.
    */
    struct RubyValue
    {
        /*!
         \brief Constructor for nil value.
        */
        RubyValue();
        /*!
         \brief Constructor for string value.
        */
        RubyValue(std::string val);
        /*!
         \brief Constructor for int value.
        */
        RubyValue(int val);
        /*!
         \brief Constructor for float value.
        */
        RubyValue(float val);
        /*!
         \brief Constructor for boolean value.
        */
        RubyValue(bool val);

        /*!
         \brief Returns the value as a string for a ruby script text initializing this value.
        */
        std::string toString() const;

        ERubyVariableType mType; /*!< Type of the variable. */
        std::string mString;     /*!< String value. */
        int mInt;                /*!< Int value. */
        float mFloat;            /*!< Float value. */
        bool mBool;              /*!< Bool value. */
    };

    /*! 
      \brief Definition for a log stream to add while initializing.
    */
    struct LogStreamDefinition
    {
        /*! 
          \brief Constructor.
        */
        LogStreamDefinition(boost::shared_ptr<LogStream> stream, zeitgeist::LogServer::EPriorityLevel level, bool sync) :
            mStream(stream),
            mLevel(level),
            mSync(sync)
            {}

        boost::shared_ptr<LogStream> mStream;        /*!< The log stream. */
        zeitgeist::LogServer::EPriorityLevel mLevel; /*!< The zeitgeist log level. */
        bool mSync;                                  /*!< Synchronization flag. */
    };

public:
    //public functions

    /*!
     \brief Constructor.

     Stores script paths but does not initialize spark yet.

     \param sparkScriptsPath path for spark scripts (kerosin.rb, oxygen.rb, etc.)
     \param sparkScript spark initialization script (e.g. spark.rb), has to be in sparkScriptsPath
     \param scriptPath excution script path to run at Simulation startup
     \param additionalScripts additional scripts to execute after executing main script and before running the simulationserver
    */
    SimSpark(const std::string& sparkScriptsPath, const std::string& sparkScript, const std::string& scriptPath, const std::vector<std::string>& additionalScripts);
    /*!
     \brief Destructor.
    */
    virtual ~SimSpark();

    /*!
     \brief Returns true if there is a script value with the given name in the initialization list.
    */
    bool HasScriptValue(const std::string& name) const;
    /*!
     \brief Returns the script value from the initialization list with the specified name.

     \param name script value to look for
     \return the registered script value or nil, if the value is not specified
    */
    RubyValue GetScriptValue(const std::string& name) const;
    /*!
     \brief Add a script value to the initialization list.

     If a ScriptValue with this name is already specified, its value is replaced.

     The ScriptValues are set after SimSpark initialization before calling the first run script.

     \param name name of the variable
     \param value value of the variable
    */
    void AddScriptValue(const std::string& name, const RubyValue& value);
    /*!
     \brief Removes a script variable from the initialization list.

     \param name variable to remove
    */
    void RemoveScriptValue(const std::string& name);
    /*!
     \brief Clears the script variable initialization list.
    */
    void ClearScriptValues();

    /*!
     \brief Initializes Spark with the usual command line options.
    */
    bool Init(int argc, char** argv);
    /*!
     \brief Reinitializes spark by defining new scripts and script paths, resetting the core and running the simulation initialization script again.

     \todo Since spark cant be complete resetted, the sparkScript has to be identical with the previous one. The Initialization script cant be called twice.
     If spark could be resetted completely, the handling would be much better and it would be possible to run multiple instances of Spark. See SimSpark
     documentation.

     \param sparkScriptsPath path for spark scripts (kerosin.rb, oxygen.rb, etc.)
     \param sparkScript spark initialization script (e.g. spark.rb), has to be in sparkScriptsPath
     \param scriptPath excution script path to run
     \param additionalScripts additional scripts to execute after executing main script and before running the simulationserver
    */
    bool Reinit(const std::string& sparkScriptsPath, const std::string& sparkScript, const std::string& scriptPath, const std::vector<std::string>& additionalScripts);
    /*!
     \brief Resets SimSpark by pausing the Simulation and removing all nodes that can safely be removed.

     Currently only removes the scene. The parameters of the sys-Servers should be reinitialized when Reinit() calls InitApp() again.
    */
    bool Reset();

    /*!
     \brief Adds a new resource location path.

     If spark is not initialized yet, the location is stored and added to spark immediately after creation.
     Resetting spark resets the list of additional resources.

     \param location new resource location path
    */
    void AddResourceLocation(const std::string& location);

    /*!
     \brief Adds a logstream.

     If spark is not initialized yet, the stream is stored and added to spark immediately after creation.
     After initialization the list of log streams is resetted.

     \param stream stream to add
     \param level target log level of the stream
     \param syncStream if true, stream will be synced whenever a message arrives
    */
    void AddLogStream(boost::shared_ptr<LogStream> stream, zeitgeist::LogServer::EPriorityLevel level, bool syncStream);

    /*!
     \brief Executes zeitgeist SimulationServer.

     \return 0 in case of no error, otherwise error code
    */
    int Run();

    /*!
     \brief Start the simulation run script at mScriptPath and additional scripts defined in constructor.

     \return 0 in case of no error, otherwise error code
    */
    int ExecuteRunScripts();
    /*!
     \brief Pauses the zeitgeist SimulationServer if it is running.

     \return true if pausing succeeded
    */
    bool Pause();
    /*!
     \brief Continues the zeitgeist SimulationServer if it is paused.

     \return true if continuing succeeded
    */
    bool Continue();
    /*!
     \brief Signals the zeigeist SimulationServer to quit. This ends SimSpark.

     After quitting, SimSpark can never be restarted. See SimSpark documentation.

     \sa SimSpark
    */
    bool Quit();

    /*!
     \brief Returns true if SimSpark is initialized, running or paused.
    */
    bool IsInitialized();
    /*!
     \brief Returns true if SimSpark is running or running and currently paused.
    */
    bool IsRunning();
    /*!
     \brief Returns true if SimSpark is running and currently paused. (The SimulationServer is half-actively waiting)
    */
    bool IsPaused();

protected:
    //protected functions

    /*!
     \brief Post-Initialization function.

     Called once after Spark finished it's init.
     Defines the simulation step and adds resource locations.

     \param argc commandline argument count
     \param argv commandline arguments
    */
    virtual bool InitApp(int argc, char** argv);

    /*!
     \brief Execute a script file in the owned ruby context.

     \todo Error logging using ruby (getting the ruby error string) was not possible at this point.
     Including ruby.h or zeitgeist/scriptserver/rubywrapper.h caused some unresolvable compile-errors in combination with Qt.
     The project probably has to be upgraded to ruby 1.9 to make it possible.
     Therefore, the log info about error scripts cant be read out.
     At the moment, the user has to check on the console instead.
     It could be helpful to write another access function to the ruby error string that does not include ruby.h.

     \param path absolute filepath of the file
    */
    bool RunScript(const std::string& path);

private:
    std::string mScriptPath;                     /*!< Excution script path to run at Simulation startup. */
    std::string mSparkScript;                    /*!< Spark initialization script (e.g. spark.rb), has to be in mSparkScriptsPrefix. */
    std::string mSparkScriptsPrefix;             /*!< Path for spark scripts (kerosin.rb, oxygen.rb, etc.). */
    std::vector<std::string> mAdditionalScripts; /*!< Additional scripts to execute after executing main script and before running the simulationserver. */
    std::vector<std::string> mResourceLocations; /*!< Additional resource locations to add after initialization. */
    std::vector<LogStreamDefinition> mLogStreams; /*!< Additional log streams to add while initializing. */

    bool mReset;       /*!< True if currently resetted but not restarted. Causes run to call simulation initialization script at mScriptPath again. */
    bool mRunExecuted; /*!< Resetting is not yet possible. This member prevents a second execution of the main script. */
    bool mReady;       /*!< In initialized state after run scripts have been called. Ready to call Run() method. */

    ESimSparkState mState; /*!< Current state of SimSpark. */

    std::vector<std::pair<std::string, RubyValue>> mScriptValues; /*!< List of parameter names and values to initialize in the script servers ruby context */
};

#endif //GUISIMSPARK_H
