/* -*-c++-*- ***************************************************************
                             consoleinputline.h
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
#ifndef UTILITY_CONSOLEINPUTLINE_H
#define UTILITY_CONSOLEINPUTLINE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <inputline.h>
#include <inputlinebinding.h>

class Console;

/*! \class ConsoleInputLine
  $Id: consoleinputline.h,v 1.1 2002/08/21 14:11:16 fruit Exp $

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConsoleInputLine : public InputLine
{
public:
    //! add the new actions to the init struct
    struct InitBindingsConsole : public InputLine::InitBindings
    {
        InputLineInitBinding action_execute_1;
        InputLineInitBinding action_execute_2;
        InputLineInitBinding action_deactivate_1;
        InputLineInitBinding action_deactivate_2;
        InputLineInitBinding action_tab_completion_1;
        InputLineInitBinding action_tab_completion_2;
        InputLineInitBinding action_history_command_backwards_1;
        InputLineInitBinding action_history_command_backwards_2;
        InputLineInitBinding action_history_command_forwards_1;
        InputLineInitBinding action_history_command_forwards_2;
        InputLineInitBinding action_history_backwards_1;
        InputLineInitBinding action_history_backwards_2;
        InputLineInitBinding action_history_forwards_1;
        InputLineInitBinding action_history_forwards_2;
        InputLineInitBinding action_history_first_1;
        InputLineInitBinding action_history_first_2;
        InputLineInitBinding action_history_last_1;
        InputLineInitBinding action_history_last_2;
    };

    //! replaces the binding initalization done by InputLine
    ConsoleInputLine(InitInfo* init_info = 0, 
                     InitBindingsConsole* init_bindings = 0);

    //! the default bindings from input events to actions       
    static void DefaultInitBindingsConsole(InitBindingsConsole& init_bindings);

    /*! tells the console to execute the content of the 
        input line (Console::execute) 
    */
    void actionExecute();
    //! deactivates the console
    void actionDeactivate();
    //! tries to complete the word under the cursor context sensitive
    void actionTabCompletion();
    /*! activates the history-command-search, searches backwards through the 
        history for the last executed command which matches the content of 
        the input line at the begin of the history-command-search and replaces 
        the input line.
    */
    void actionHistoryCommandBackwards();
    //! as actionHistoryCommandBackwards but the search is done forwards
    void actionHistoryCommandForwards();
    //! scroll one line back in the console history
    void actionHistoryBackwards();
    //! scroll one line forwards in the console history
    void actionHistoryForwards();
    //! jumps to the first line in the console history
    void actionHistoryFirst();
    //! jumps to the last line in the console history 
    void actionHistoryLast();
    /*! tries to complete the current word before the cursor (case 
        insensitive).

        The completion is context sensitive (at least sometimes ;-) ),
        so at the start of the input commands and aliases are
        complete, after bind variables which are members of the
        command group are chosen, ...
    */
    void tabCompletion();

protected:
    typedef std::list < std::string > StringList;

    /*! removes all currently defined bindings and evaluates and sets
        the bindings defined in the init struct bindings.

        called in the constructor.
        @param bindings
    */
    void setInitBindingsConsole(const InitBindingsConsole& bindings);

    /*! returns the words of the last statement in input_to_cursor and
        sets last_word_start to the position of the first letter of the
        last word.

	@param input_to_cursor should be the return value of
               tabCompletionGetInputToCursor.  The string will be
               splitted at whitespace, ignoring whitespace in strings.
               Only the strings after the last statement separator are
               returned.  If the string was empty or ended with
               whitespace, the last returned string will be a "", this
               implies that the returned string list will never be
               empty.
	
        @param last_word_start will be set to the position of the
               first letter of the last string returned.

	Example : "set ; alias ba"

        Everything to the last ; is dropped, returned is a stringlist
        consisting of alias and ba.  last_word_start is set to the
        position of the 'b'.
    */
    StringList tabCompletionSplit(const std::string& input_to_cursor,
                                  int& last_word_start);
    /*! Get a list of all strings which may be a possible completion
        of the last word in the context of the other paramters.

	A command is asked with complete which parameters it wants.
	If the command returns no preferences, all variables are taken
	by default.
    */
    StringList tabCompletionGetComparables(const StringList& words);
    /*! to_complete is matched against the beginning of every comparable.
	If only one matches, replacement will be set to this.
        @param to_complete
        @param comparables
	@param replacement will be set to the longest string which 
               matches the replacements.
        @param command
        @param prefix may contain a prefix cut away according to the
               commands IgnorePrefixes method.

	Example: to_complete = "s"; comparables = ["set", "search", "quit"]

        replacement will be set to "se", as the matches to "s" are
        "set" and "search", and "se" matches "set" and "search".
    */
    void tabCompletionCompare(const std::string& to_complete,
                              StringList& comparables,
                              std::string& replacement,
                              const std::string& command,
                              std::string& prefix);
    /*! The input line is replaced by replacing the last word.

	If to_complete has no match, nothing is changed.  With one
	match the word to complete is simply replaced.  If the word
	was already complete it is assumed that the next parameter
	should be completed.

	Example : "set"

	As "set" is complete and matches only to the command set, it
	is first replaced with "set ", and then the tabCompletion
	starts again on this imput but tries no to complete the empty
	parameter after set.

	If more than one word matches the word to complete, it is
	replaced with replacement and all possible completions are
	printed (alphabetically sorted) to the console.
    */
    void tabCompletionReplace(const std::string& to_complete,
                              const StringList& comparables,
                              const std::string& replacement,
                              int last_word_start, int last_word_end,
                              bool stringify, const std::string& prefix,
                              bool add_blank);

};

#endif                          // UTILITY_CONSOLEINPUTLINE_H
