/* -*-c++-*- ***************************************************************
                             consolesettings.h
    contains all variables which customize the console behaviour.
                           ------------------------
    begin                : Aug 16 2002  Oliver Obst
    copyright            : (C) 2002 by The RoboCup Soccer Simulator
                           Maintenance Group.
    email                : sserver-admin@lists.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU GPL as published by the Free Software   *
 *   Foundation; either version 2 of the License, or (at your option) any  *
 *   later version.                                                        *
 *                                                                         *
 ***************************************************************************/
#ifndef UTILITY_CONSOLESETTINGS_H
#define UTILITY_CONSOLESETTINGS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <list>
#include <map>
#include <string>
#include <vector>

#include <vector3.h>
#include "variables/convarcallbackconsole.h"

#if 0
namespace Utility
{
}
#endif

class Console;
class ConsoleForwarder;

/*! constants: should be static const class members, 
    but VC++ does not support this :-(
*/

//! a vector describing the color red in RGB
static const Vector3 COLOR_RED(1, 0, 0);
//! a vector describing the color green in RGB
static const Vector3 COLOR_GREEN(0, 1, 0);
//! a vector describing the color blue in RGB
static const Vector3 COLOR_BLUE(0, 0, 1);
//! a vector describing the color yello in RGB
static const Vector3 COLOR_YELLOW(1, 1, 0);
//! a vector describing the color magenta in RGB
static const Vector3 COLOR_MAGENTA(1, 0, 1);
//! a vector describing the color cyan in RGB
static const Vector3 COLOR_CYAN(0, 1, 1);
//! a vector describing the color white in RGB
static const Vector3 COLOR_WHITE(1, 1, 1);

//! the constants for the callback functionalities
static const int CS_CALL_BACK_WARNING = 0;
static const int CS_CALL_BACK_ERROR = 1;
static const int CS_CALL_BACK_DEBUG = 2;

/*! \class ConsoleSettings

    Contains all variables which customize the console behaviour.
    These variables are stored as ConVars and can be changed in the
    console.
  
    The callback functionality of ConVars is sometimes used and
    therefore a ConVarCallBackConsole object is contained in
    ConsoleSettings.

    To add a new callback functionality 
    - an appropriate CS_CALL_BACK_xxx constant has to be created
    - the callback property of the variable has to be set (see
      mShowWarning for an example)
    - updateOnCallBack has to be extended

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.

    $Id: consolesettings.h,v 1.2 2002/10/07 15:57:56 fruit Exp $
*/
class ConsoleSettings
{
public:
    //! the default console settings initialization
    struct InitInfo
    {
        bool show_warning;
        bool show_error;
        bool show_debug;
        int history_lines;
        int text_lines;
        bool show_var_signature;
        std::string prompt;
        Vector3 color_warning;
        Vector3 color_error;
        Vector3 color_debug;
        Vector3 color_normal;
        Vector3 color_output;
        Vector3 color_input;
        Vector3 color_interactive;
        Vector3 color_prompt;
        Vector3 color_cursor;
        std::string basePath;
    };

    /*! The forwarder is given so that a console can create its
        ConsoleSettings member in its constructor without the need to
        get in turn in the ConsoleSettings constructor the forwarder
        from the console in short: avoid a cycle which can not be
        handled by g++ does the initialization with initInfo or if not
        given with a default init struct provided by DefaultInitInfo
    */
    ConsoleSettings(const std::ostream* stream,
                    InitInfo* init_info = 0);
    ~ConsoleSettings();

    //! sets the init struct to the default initialization values
    static void defaultInitInfo(InitInfo& init_info);

    //! is the console interested in warning messages?
    bool getShowWarning() const;
    //! is the console interested in warning messages?
    void setShowWarning(bool value);
    //! is the console interested in error messages?
    bool getShowError() const;
    //! is the console interested in error messages?
    void setShowError(bool value);
    //! is the console interested in debug messages?
    bool getShowDebug() const;
    //! is the console interested in debug messages?
    void setShowDebug(bool value);

    /*! should a variable display itself compact or with 
        full signature information ?
    */
    bool getShowVarSignature() const;
    /*! should a variable display itself compact or with 
        full signature information ?
    */
    void setShowVarSignature(bool value);
    //! how many lines of the console's output history should be displayed?
    int getHistoryLines() const;
    //! how many lines of the console's output history should be displayed?
    void setHistoryLines(int value);
    //! into how many lines should the screen be divided on conole display?
    int getTextLines() const;
    //! into how many lines should the screen be divided on conole display?
    void setTextLines(int value);
    //! which prompt should be displayed?
    std::string getPrompt() const;
    //! which prompt should be displayed?
    void setPrompt(const std::string& value);

    //! in what color should warning messages be displayed?
    Vector3 getColorWarning() const;
    //! in what color should warning messages be displayed?
    void setColorWarning(const Vector3& value);
    //! in what color should error messages be displayed?
    Vector3 getColorError() const;
    //! in what color should error messages be displayed?
    void setColorError(const Vector3& value);
    //! in what color should debug messages be displayed?
    Vector3 getColorDebug() const;
    //! in what color should debug messages be displayed?
    void setColorDebug(const Vector3& value);
    //! in what color should standard messages be displayed?
    Vector3 getColorNormal() const;
    //! in what color should standard messages be displayed?
    void setColorNormal(const Vector3& value);
    //! in what color should the console output be displayed?
    Vector3 getColorOutput() const;
    //! in what color should the console output be displayed?
    void setColorOutput(const Vector3& value);
    //! in what color should user input be displayed in the history?
    Vector3 getColorInput() const;
    //! in what color should user input be displayed in the history?
    void setColorInput(const Vector3& value);
    //! in what color should user input be displayed?
    Vector3 getColorInteractive() const;
    //! in what color should user input be displayed?
    void setColorInteractive(const Vector3& value);
    //! in what color should the prompt be displayed?
    Vector3 getColorPrompt() const;
    //! in what color should the prompt be displayed?
    void setColorPrompt(const Vector3& value);
    //! in what color should the cursor be displayed?
    Vector3 getColorCursor() const;
    //! in what color should the cursor be displayed?
    void setColorCursor(const Vector3& value);
    //! the base path for path related operations
     std::string getBasePath() const;
    //! the base path for path related operations
    void setBasePath(const std::string& value);

    /*! used by the callback function of ConVarCallBackConsole.
        Reads depending on the index the value of a variable and calls
        some methods (updateVariableXXX) to update the state of some
        objects, e.g. the message interests of the console: debug,
        warning, error
    */
    void updateOnCallBack(int index, 
                          const std::ostream* stream = 0);

    /*! updates the forwarders to the console settings (updateWarning,
        updateError, updateDebug)
     */
    void updateForwarder();

protected:
     ConsoleSettings(const ConsoleSettings&);
     ConsoleSettings& operator =(const ConsoleSettings&);

    //! initialize the member variables with the init info from the init struct
    void initSettings(InitInfo* init_info);

    /*! initialize the callback mappings from constants 
        to functions (mCallBack)
    */
    void initCallBack();

    /*! Create and initialize the Console Variables which will provide
        access to the console settings. These will be constants
        or reference to member variables mostly.
    */
    void initVars(const std::ostream* stream = 0);
    /*! the message interests: which message types should be shown in
        the console? */
    void initVarsMessages(const std::ostream* stream = 0);
    //! the color constants
    void initConstantsColor();
    //! the color of console elements
    void initVarsColor();
    //! representation configuration
    void initVarsRepresentation();
    //! misc configuration
    void initVarsMisc();

    /*! update the console stream message interest 
        in warning, error and debug messages dependend on the values 
        provided by msettings.
    */
    void update(int activate, int priority_mask, 
                const std::ostream* stream = 0) const;

    void updateWarning(const std::ostream* stream = 0) const;
    void updateError(const std::ostream* stream = 0) const;
    void updateDebug(const std::ostream* stream = 0) const;

    /*! the callback object contains the call back functions 
        of some ConVars created in InitVars 
     */
    ConVarCallBackConsole* M_callback;

    /*! A mapping from a number constant (CS_CALL_BACK_WARNING) to the 
        functionality to execute. Used by updateVariable to evaluate which 
        method has to be executed on the callback.
    */
    typedef void (ConsoleSettings::*CallBackFunction)
        (const std::ostream* stream) const;

    typedef std::map < int, CallBackFunction > CallBackMap;
    CallBackMap M_callback_map;

    //! is the console interested in warning messages? 
    bool M_show_warning;
    //! is the console interested in error messages? 
    bool M_show_error;
    //! is the console interested in debug messages? 
    bool M_show_debug;
    /*! should a variable display itself compact or with 
        full signature information ? 
    */
    bool M_show_var_signature;
    //! how many lines of the history should be shown in the console ? 
    int M_history_lines;
    //! into how many lines should the screen be divided on conole display? 
    int M_text_lines;
    //! which prompt should be displayed? 
    std::string M_prompt;
    //! in what color should warning messages be displayed? 
    Vector3 M_color_warning;
    //! in what color should error messages be displayed? 
    Vector3 M_color_error;
    //! in what color should debug messages be displayed? 
    Vector3 M_color_debug;
    //! in what color should standard messages be displayed? 
    Vector3 M_color_normal;
    //! in what color should the console output be displayed? 
    Vector3 M_color_output;
    //! in what color should user input be displayed in the history? 
    Vector3 M_color_input;
    //! in what color should user input be displayed? 
    Vector3 M_color_interactive;
    //! in what color should the prompt be displayed? 
    Vector3 M_color_prompt;
    //! in what color should the cursor be displayed? 
    Vector3 M_color_cursor;

    //! the base path for path related operations 
    std::string M_base_path;
};

#endif                          // UTILITY_CONSOLSETTINGS_H
