/* -*-c++-*- ***************************************************************
                             consolehistory.h
    Stores and provides access to the output of the console.
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
#ifndef UTILITY_CONSOLEHISTORY_H
#define UTILITY_CONSOLEHISTORY_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <vector>

#include <baseinputdevice.h>

class Console;
class ConsoleInputLine;

/*! \class ConsoleHistory
  $Id: consolehistory.h,v 1.1 2002/08/21 14:11:16 fruit Exp $

    ConsoleHistory

    stores and provides access to the output of the console and
    manages the console input line.

    In detail methods are provided to scroll through the console, and
    to print to it and indicate (e.g by coloring) what is printed,
    e.g. a debug message or a simple reaction to user input.  history
    completion and the usual functionality of an input line
    (ConsoleInputLine).
 
    The history completion works as known from the unix shells, the
    methods are CommandBackwards and CommandForwards on first use the
    search-history-backwards-mode is activated (-> mSearchCommandMode)
    and the current input line is stored (lowercase) as a search
    pattern. The command history (-> M_history_CommandIndex) is now
    searched backwards/forwards till a matching command is found which
    replaces the former input line's content. As long as the user only
    invokes the functions CommandBackwards and CommandForwards the
    command history will be searchd backwards rsp. forwards from the
    last hit and matched against the pattern defined on first use.
    the mode is left by invoking any other function.
        
    The history is used by a view of the console (ConsoleBaseView) to
    display the console. Therefore the history entries and their
    attributes.
    - currently only the output type -
    May be used to variate the representation of the history.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConsoleHistory
{
public:
    /*! What type has this output entry? cf. the message system
        (BaseForwarder) output and input are known only to the
        console, e.g. the output of cmdlist and the current user input
        line the other types represent the message types of the
        forwarder system, i.e. a warning message will be stored with
        the type S_OP_Warning.
    */
    enum OutputType
    {
        S_OP_Output = 0,
        S_OP_Debug = 1,
        S_OP_Normal = 2,
        S_OP_Warning = 4,
        S_OP_Error = 8,
        S_OP_Input = 16,
        S_OP_All = 0xffffffff
    };
    /*! One output entry of the console represents one line therefore
        the output is splitted at the line breaks before it is stored.
    */
    struct Entry
    {
        //! the output content
        std::string content;
        //! the output type
        OutputType type;
    };

    //! the container type for the history
    typedef std::vector < Entry > History;

    ConsoleHistory();
    ~ConsoleHistory();
#if 0
    //! pipes the input through the input line
    void processInput(const BaseInputDevice::Input& input);
    //! returns the content of the input line
    std::string getInputLineContent() const;
    //! returns the cursor position in the input line
    int getInputLineCursorPos() const;
#endif
    //! returns the output history
    const History& getHistory() const;
    /*! Returns the current position in the (scrollable) output history,
        returns -1 if the history is empty.
    */
    int getHistoryIndex() const;
    //! stores a command in the command history and prints it with printInput
    void printCommand(const std::string& output);
    //! prints an output of type input and prints it with printLines
    void printInput(const std::string& output);
    //! prints an output of type output and prints it with printLines
    void printOutput(const std::string& output);
    /*! prints an output of type normal (cf Forwarder Message Types) and 
        prints it with printLines.
    */
    void printNormal(const std::string& output);
    /*! prints an output of type error (cf Forwarder Message Types) and 
        prints it with printLines.
    */
    void printError(const std::string& output);
    /*! prints an output of type warning (cf Forwarder Message Types) and 
        prints it with printLines.
    */
    void printWarning(const std::string& output);
    /*! prints an output of type debug (cf Forwarder Message Types) and 
        prints it with printLines.
    */
    void printDebug(const std::string& output);
    //! scrolls the history one line up
    void historyUp();
    //! scrolls the history one line down
    void historyDown();
    //! scrolls the history to the start
    void historyFirst();
    //! scrolls the history to the end
    void historyLast();

    //! the tcsh's search-history-backwards function
    void commandBackwards();
    //! the tcsh's search-history-forwards function
    void commandForwards();

protected:
    /*! the states of the search command mode: 

        Used to decide if the history completion (commandBackwards,
        commandForwards) is restarted or has to be continued from a
        previous search (M_search_command_pattern, M_history_OutputIndex)

        To avoid that every input function must know and set this
        mode, i.e. inactivate it if it is not commandBackwards or
        commandForwards, on every input event (ProcessInput) the state
        is set to a lower level before the action is evaluated.

        If the last action was commandBackwards or commandForwards,
        the state is SCM_Active and set to SCM_Inactivating.  if now
        commandBackwards or commandForwards are executed, they know
        that they were the last action executed, continue their
        action, and set SCM_Active.
    
        Any other action does nothing to this state, and therefore the
        state is SCM_Inactivating on the next input event.  As
        SCM_Inactivating and SCM_Inactive are then set to
        SCM_Inactive, a further call to commandBackwards or
        commandForwards will reinitialize the history completion.
    */
    enum SearchCommandMode
    {
        /*! The search is currently active, i.e. commandBackwards or
            commandForward were the last executed input processing
            methods and have set the search command mode to active.
        */
        SCM_Active = 0,
        /*! We have a new input, the previous state was active, and we
            don't know now what the input action is.
        */
        SCM_Inactivating = 1,
        //! the search is currently inactive
        SCM_Inactive = 2,
        SCM_FORCE32BIT = 0xffffffff
    };

    ConsoleHistory(const ConsoleHistory&) {}
    ConsoleHistory& operator =(const ConsoleHistory &) {}

    //! sets the current position in the (scrollable) output history
    void setHistoryIndex(int index);

    //! returns the current position in the (scrollable) command history
    int getHistoryCommandIndex() const;
    //! sets the current position in the (scrollable) command history
    void setHistoryCommandIndex(int index);
    /*! splits the output at the linebreaks and stores every line 
        in the output history.
    */
    void printLines(OutputType type, const std::string& output);
    /*! Initializes the command search,
        i.e. sets M_search_command_mode, M_search_command_pattern, and
        M_history_command_index.
    */
    void initCommandSearch();
    /*! Compares M_search_command_pattern with the start of the command in
        the history at the index (both are lowercase)
    */
    bool ComparePatternWithCommandHistoryAt(int index);

    //! the history contains a sequential list of all console output till now
    History M_history;

    //! the container type for the command history
    typedef std::vector < std::string > HistoryCommand;
    /*! the command history contains a sequential list of all 
        executed user input till now.
    */
    HistoryCommand M_history_command;
#if 0
    //! the input line of the console
    ConsoleInputLine* M_input_line;
#endif
    //! history scrolling: which is the current position in the output history
    int M_history_output_index;
    /*! history scrolling: which is the current position in the
        command history it may point after the last entry, i.e. be
        equal to the history size.
    */
    int M_history_command_index;

    /*! The string used for comparison with the command history in the 
        search command mode.
    */
    std::string M_search_command_pattern;
    //! the current state of the search command mode
    SearchCommandMode M_search_command_mode;
};

#endif                          // UTILITY_CONSOLEHISTORY_H
