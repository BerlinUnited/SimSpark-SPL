/* -*-c++-*- ***************************************************************
                             consoleinputline.cc
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
#include "consoleinputline.h"
#include "consoleinputlinebinding.h"
#include "console.h"
#include "consolehistory.h"
#include "commands/commandserver.h"
#include "variables/convarserver.h"

#if 0
#include <inputserver.h>
#endif

#include <algorithm>

using namespace std;

ConsoleInputLine::ConsoleInputLine(InitInfo* init_info, 
                                   InitBindingsConsole* init_bindings) :
    InputLine(init_info, init_bindings)
{
    // initialize the keybindings: 
    // deletes and rewrites the initialization of InputLine
    InitBindingsConsole bindings;
    if (init_bindings != 0)
        bindings = *init_bindings;
    else
        DefaultInitBindingsConsole(bindings);
    setInitBindingsConsole(bindings);
}

void
ConsoleInputLine::DefaultInitBindingsConsole(InitBindingsConsole& init_bindings)
{
    // init the bindings/actions of the input line
    InputLine::DefaultInitBindings(init_bindings);

    init_bindings.action_execute_1.set(false, false, false, ISDC_NUMPADENTER);
    init_bindings.action_execute_2.set(false, false, false, ISDC_RETURN);

    init_bindings.action_deactivate_1.set(false, false, false, ISDC_GRAVE);
    init_bindings.action_deactivate_2.set(false, false, false, ISDC_ESCAPE);

    init_bindings.action_tab_completion_1.set(false, false, false, ISDC_TAB);
    init_bindings.action_tab_completion_2.setInvalid();

    init_bindings.action_history_command_backwards_1.set(false, false, false,
                                                     ISDC_UP);
    init_bindings.action_history_command_backwards_2.setInvalid();

    init_bindings.action_history_command_forwards_1.set(false, false, false,
                                                    ISDC_DOWN);
    init_bindings.action_history_command_forwards_2.setInvalid();

    init_bindings.action_history_backwards_1.set(false, false, false, ISDC_PGUP);
    init_bindings.action_history_backwards_2.setInvalid();

    init_bindings.action_history_forwards_1.set(false, false, false, ISDC_PGDN);
    init_bindings.action_history_forwards_2.setInvalid();

    init_bindings.action_history_first_1.set(true, false, false, ISDC_HOME);
    init_bindings.action_history_first_2.setInvalid();

    init_bindings.action_history_last_1.set(true, false, false, ISDC_END);
    init_bindings.action_history_last_2.setInvalid();
}

void
ConsoleInputLine::setInitBindingsConsole(const InitBindingsConsole& bindings)
{
    // remove the current bindings
    removeAllBindings();

    // set the bindings of inputline
    InputLine::setInitBindings(bindings);

    // set the console bindings
    if (bindings.action_execute_1.isValid())
        setBinding(
            new ConsoleInputLineBinding(bindings.action_execute_1,
                                        &ConsoleInputLine::actionExecute));
    if (bindings.action_execute_2.isValid())
        setBinding(
            new ConsoleInputLineBinding(bindings.action_execute_2,
                                        &ConsoleInputLine::actionExecute));
    if (bindings.action_deactivate_1.isValid())
        setBinding(
            new ConsoleInputLineBinding(bindings.action_deactivate_1,
                                        &ConsoleInputLine::actionDeactivate));
    if (bindings.action_deactivate_2.isValid())
        setBinding(
            new ConsoleInputLineBinding(bindings.action_deactivate_2,
                                        &ConsoleInputLine::actionDeactivate));
    if (bindings.action_tab_completion_1.isValid())
        setBinding(
            new ConsoleInputLineBinding(bindings.action_tab_completion_1,
                                        &ConsoleInputLine::actionTabCompletion)
            );
    if (bindings.action_tab_completion_2.isValid())
        setBinding(
            new ConsoleInputLineBinding(bindings.action_tab_completion_2,
                                        &ConsoleInputLine::actionTabCompletion)
            );
    if (bindings.action_history_command_backwards_1.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_command_backwards_1,
                &ConsoleInputLine::actionHistoryCommandBackwards)
            );
    if (bindings.action_history_command_backwards_2.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_command_backwards_2,
                &ConsoleInputLine::actionHistoryCommandBackwards)
            );
    if (bindings.action_history_command_forwards_1.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_command_forwards_1,
                &ConsoleInputLine::actionHistoryCommandForwards)
            );
    if (bindings.action_history_command_forwards_2.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_command_forwards_2,
                &ConsoleInputLine::actionHistoryCommandForwards)
            );
    if (bindings.action_history_backwards_1.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_backwards_1,
                &ConsoleInputLine::actionHistoryBackwards)
            );
    if (bindings.action_history_backwards_2.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_backwards_2,
                &ConsoleInputLine::actionHistoryBackwards)
            );
    if (bindings.action_history_forwards_1.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_forwards_1,
                &ConsoleInputLine::actionHistoryForwards)
            );
    if (bindings.action_history_forwards_2.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_forwards_2,
                &ConsoleInputLine::actionHistoryForwards)
            );
    if (bindings.action_history_first_1.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_first_1,
                &ConsoleInputLine::actionHistoryFirst)
            );
    if (bindings.action_history_first_2.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_first_2,
                &ConsoleInputLine::actionHistoryFirst)
            );
    if (bindings.action_history_last_1.isValid())
        setBinding(
            new ConsoleInputLineBinding(
                bindings.action_history_last_1,
                &ConsoleInputLine::actionHistoryLast)
            );
    if (bindings.action_history_last_2.isValid())
        setBinding(
            new ConsoleInputLineBinding(bindings.action_history_last_2,
                                        &ConsoleInputLine::actionHistoryLast));
}

void
ConsoleInputLine::actionExecute()
{
    // print at least a newline
    string input = getInput();
    if (input == "")
    {
        Console::instance().getHistory().printCommand("\n");
    }
    else
    {
        Console::instance().getHistory().printCommand(input);
    }

    // let the console execute the line
    Console::instance().execute(getInput());

    // clear the input buffer
    memset(M_inputBuffer, 0, M_info.M_bufferSize);
    M_inputLength = 0;
    M_cursorPos = 0;
}

void
ConsoleInputLine::actionDeactivate()
{
    Console::instance().setActive(false);

    // prevent repetition of stall keys
    M_lastInput.type = BaseInputDevice::IDT_NONE;
    M_inKeyRepeat = false;
    M_keyHoldTime = -1;
}

void
ConsoleInputLine::actionTabCompletion()
{
    tabCompletion();
}

void
ConsoleInputLine::actionHistoryCommandBackwards()
{
    Console::instance().getHistory().commandBackwards();
}

void
ConsoleInputLine::actionHistoryCommandForwards()
{
    Console::instance().getHistory().commandForwards();
}

void
ConsoleInputLine::actionHistoryBackwards()
{
    Console::instance().getHistory().historyUp();
}

void
ConsoleInputLine::actionHistoryForwards()
{
    Console::instance().getHistory().historyDown();
}

void
ConsoleInputLine::actionHistoryFirst()
{
    Console::instance().getHistory().historyFirst();
}

void
ConsoleInputLine::actionHistoryLast()
{
    Console::instance().getHistory().historyLast();
}

void
ConsoleInputLine::tabCompletion()
{
    // get the input to the cursor
    string input_to_cursor = getInput(0, getCursorPosition() - 1);

/* new stuff
    // try to scan the input
	pair <bool, ConsoleParser::tStatementList> scanResult;
	scanResult = Console::instance().getParser ().Scan (input_to_cursor, false);
	if (!scanResult.first)
	{
		gDispatchDebug ("tabCompletion: scan failed.\n");
		return;
	}

	// we need only the last statement
	ConVar::tConVars statement = scanResult.second.at (scanResult.second.size () - 1);

	StringList comparables;
	if (statement.size () == 0)
	{
		// complete a command or an alias
		
		// get the commands
		CommandServer::tNames commands = CommandServer::instance ().List ();
		copy (commands.begin (), commands.end (), back_inserter (comparables));
		
		// get the aliases
		ConsoleAliases::StringList aliases = Console::instance().getAliases ().getAliases ();
		copy (aliases.begin (), aliases.end (), back_inserter (comparables));
	}

	string replacement = tabCompletionCompare ();
	
	// statement.size () == 1 -> cmdlist
	// sonst : suche command, gib ihm die parameter zur vervollstängigung
	StringList checked_words;
	
*/
    // the position of the word to complete
    int last_word_start;
    int last_word_end = input_to_cursor.size();

    // split the input into single words
    StringList words = tabCompletionSplit(input_to_cursor, last_word_start);

    // replace an alias
    string command = *words.begin();
    string alias;
    StringList checked_words;
    if (Console::instance().getAliases().exists(command))
    {
        alias = Console::instance().getAliases().get(command);
        int dummy;
        checked_words = tabCompletionSplit(alias, dummy);
        copy(++words.begin(), words.end(), back_inserter(checked_words));
    }
    else
    {
        copy(words.begin(), words.end(), back_inserter(checked_words));
    }
    // get a list of all strings which should be tried to match
    StringList comparables = tabCompletionGetComparables(checked_words);

    // should the word to complete be stringified?
    checked_words.pop_front();
    bool stringify = false;
    bool add_blank = true;
    const ConCommand* con_command = CommandServer::instance().get(command);
    if (con_command != 0)
    {
        stringify = con_command->shouldStringify(checked_words);
        add_blank = con_command->addBlank();
    }

    // remove all comparables which do not match and return 
    // the longest common match in replacement
    string to_complete = *words.rbegin();
    string replacement;
    string prefix;
    tabCompletionCompare(to_complete, comparables, replacement, 
                         command, prefix);

    tabCompletionReplace(to_complete, comparables, replacement, last_word_start,
                         last_word_end, stringify, prefix, add_blank);
}

void
ConsoleInputLine::tabCompletionCompare(const std::string& to_complete,
                                       StringList& comparables,
                                       std::string& replacement,
                                       const std::string& command,
                                       std::string& prefix)
{
    if (comparables.size() == 0)
    {
        return;
    }

    // compare case insensitive
    string to_complete_lower = to_complete;
    transform(to_complete.begin(), to_complete.end(), to_complete_lower.begin(),
              tolower);

    // cut an ignorable prefix from to_complete, e.g. + in "invoke +cmd_quit"
    const ConCommand *con_command = CommandServer::instance().get(command);
    if (con_command != 0)
    {
        ConCommand::StringList prefixes = con_command->ignorePrefixes();
        for (ConCommand::StringList::iterator iter = prefixes.begin();
             iter != prefixes.end(); ++iter)
        {
            string compare = *iter;;
            transform(compare.begin(), compare.end(), compare.begin(), tolower);
            if (compare == to_complete_lower.substr(0, iter->size()))
            {
                prefix = compare;
                to_complete_lower = to_complete_lower.substr(iter->size());
                break;
            }
        }
    }

    int completion_length = to_complete_lower.size();
    bool first_match = true;
    for (StringList::iterator iter = comparables.begin();
         iter != comparables.end();)
    {
        string comparable_part = iter->substr(0, completion_length);
        transform(comparable_part.begin(), comparable_part.end(),
                  comparable_part.begin(), tolower);
        if (to_complete_lower == comparable_part)
        {
            // find the shortest general match
            if (first_match)
            {
                first_match = false;

                replacement = *iter;
                transform((*iter).begin(), (*iter).end(), replacement.begin(),
                          tolower);
            }
            else
            {
                string current_match = *iter;
                string new_replacement;

                transform((*iter).begin(), (*iter).end(), current_match.begin(),
                          tolower);

                string::iterator replacement_iter = replacement.begin();
                string::iterator current_match_iter = current_match.begin();
                while ((replacement_iter != replacement.end())
                       && (current_match_iter != current_match.end())
                       && (*replacement_iter == *current_match_iter))
                {
                    new_replacement += *replacement_iter;
                    ++replacement_iter;
                    ++current_match_iter;
                }

                replacement = new_replacement;
            }

            ++iter;
        }
        else
        {
            iter = comparables.erase(iter);
        }

    }
}

ConsoleInputLine::StringList 
ConsoleInputLine::tabCompletionGetComparables(const StringList& words)
{
    StringList comparables;

    string command = *words.begin();
    transform(command.begin(), command.end(), command.begin(), tolower);

    if (words.size() == 1)
    {
        // complete a command or an alias

        // get the commands
        CommandServer::tNames commands = CommandServer::instance().List();
        copy(commands.begin(), commands.end(), back_inserter(comparables));

        // get the aliases
        ConsoleAliases::StringList aliases =
            Console::instance().getAliases().getAliases();
        copy(aliases.begin(), aliases.end(), back_inserter(comparables));
    }
    else
    {
        // the first word must be a command
        const ConCommand* con_command = CommandServer::instance().get(command);
        if (con_command == 0)
        {
            return comparables;
        }

        // ask the command, what parameters it expects
        ConCommand::StringList parameters;
        ConCommand::StringList commandHint;
        copy(++words.begin(), words.end(), back_inserter(parameters));
        if (con_command->Complete(parameters, commandHint))
        {
            // take the commands hint
            copy(commandHint.begin(), commandHint.end(),
                 back_inserter(comparables));

        }
        else
        {
            // no hint given -> check if the command can take this many parameters,
            // and then use the console variables as completion candidates
//                      ConCommand::tConComSignature& signatures = con_command->getSignature ();

//                      if (signatures.size () == 0) {
            // the command takes any argument -> suggest a variable (ConEcho)
            ConVarServer::tVarNames variables = gConVarServer.List();
            copy(variables.begin(), variables.end(),
                 back_inserter(comparables));

/*			} else {
			// check if there exists a signature of this command, which has at least this many parameters
				bool success = false;

				for (ConCommand::tConComSignature::const_iterator iter = signatures.begin (); 
					!success && (iter != signatures.end ()); ++iter)
				{
					ConVar::teConVarTypes* signature = *iter;
					if (words.size () - 1 <= signature->size ())
					{
						success = true;
					}
				}

				if (success)
				{
				// default : variables
					ConVarServer::tVarNames variables = gConVarServer.List ();
					copy (variables.begin (), variables.end (), back_inserter (comparables));
				}
			}
*/ }
    }

    return comparables;
}

void
ConsoleInputLine::tabCompletionReplace(const string& to_complete,
                                       const StringList& comparables,
                                       const string& replacement,
                                       int last_word_start, int last_word_end,
                                       bool stringify, const string& prefix,
                                       bool add_blank)
{
    if (comparables.size() == 0)
    {
        return;
    }

    // display the current inputline
    string inputLine = getInput();
    if (comparables.size() == 1)
    {
        string niceReplacement = prefix + comparables.front();
        // avoid the conversion of the user input to lower case
//              niceReplacement = to_complete + niceReplacement.substr (to_complete.size () - prefix.size ());

        // put the replacement in ' if the command needs a string
        if (stringify)
        {
            niceReplacement = "'" + niceReplacement + "'";
        }

        // one match found -> replace the input
        string start = inputLine.substr(0, last_word_start) + niceReplacement;

        if (add_blank)
        {
            start += " ";
        }

        setInput((start + inputLine.substr(last_word_end)).c_str());
        setCursorPosition(start.size());

        // check if the word to match was already complete
        // -> try to match the next word (requested by MR)
        string match = comparables.front();
        transform(match.begin(), match.end(), match.begin(), tolower);
        if (to_complete == match)
        {
            tabCompletion();
        }
    }
    else if (comparables.size() > 1)
    {
        // display the current inputline
        Console::instance().getHistory().PrintInput(inputLine);

        // avoid the conversion of the user input to lower case
        string niceReplacement = prefix + replacement;
        //              string niceReplacement = to_complete + replacement.substr (to_complete.size () - prefix.size ());

        // several comparables found -> replace as much as possible, display the matches
        string start = inputLine.substr(0, last_word_start) + niceReplacement;
        setInput((start + inputLine.substr(last_word_end)).c_str());
        setCursorPosition(start.size());

        // sort the comparables
        StringList sorted;
        copy(comparables.begin(), comparables.end(), back_inserter(sorted));
        sorted.sort();

        // print the comparables to the console and suggest to format them in columns
        for (StringList::iterator iter = sorted.begin(); iter != sorted.end();
             ++iter)
        {
            Console::instance().getHistory().PrintInput(*iter);
        }
    }
}

ConsoleInputLine::StringList ConsoleInputLine::tabCompletionSplit(const std::
                                                                   string &
                                                                   input_to_cursor,
                                                                   int
                                                                   &last_word_start)
{
    // split the input_to_cursor at whitespace into single words
    StringList
        words;

    // start with leftposition as the first letter which is not whitespace
    // leftposition points to the first letter of a word, rightposition to the last letter of a whitespace sequence
    int
        leftPosition =
        input_to_cursor.
        find_first_not_of(" \t");
    int
        rightPosition =
        0;

    // traverse through the string till leftposition or rightposition points after the last letter
    while ((leftPosition != string::npos)
           &&
           ((rightPosition =
             input_to_cursor.find_first_of(" \t;",
                                         leftPosition)) != string::npos))
    {
        // has a statement seperator been found?
        if (input_to_cursor.at(rightPosition) == ';')
        {
            // check if the word is a string
            if ((input_to_cursor.at(leftPosition) == '\'')
                || (input_to_cursor.at(leftPosition) == '"'))
            {
                // a string -> ignore ';' till the next whitespace
                rightPosition =
                    input_to_cursor.find_first_of(" \t", rightPosition + 1);

                string
                    word =
                    input_to_cursor.
                    substr(leftPosition, rightPosition - leftPosition);
                words.push_back(word);

                leftPosition =
                    input_to_cursor.find_first_not_of(" \t", rightPosition + 1);
            }
            else
            {
                // a statement separator -> empty the word list and process the next word
                words.clear();
                leftPosition =
                    input_to_cursor.find_first_not_of(" \t", rightPosition + 1);
            }
        }
        else
        {
            // a simple word
            string
                word =
                input_to_cursor.
                substr(leftPosition, rightPosition - leftPosition);
            words.push_back(word);

            leftPosition =
                input_to_cursor.find_first_not_of(" \t", rightPosition + 1);
        }
    }

    // get the last word
    if (leftPosition != string::npos)
    {
        // there was no whitespace at the end of the input
        string
            word =
            input_to_cursor.
            substr(leftPosition, rightPosition - leftPosition);
        words.push_back(word);
        last_word_start = leftPosition;
    }
    else if (rightPosition != string::npos)
    {
        // there was whitespace after the last word : this means, that the next parameter should be completed
        words.push_back("");
        last_word_start = input_to_cursor.size();
    }

    return words;
}
