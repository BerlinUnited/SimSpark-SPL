/* -*-c++-*- ***************************************************************
                             consolehistory.cc
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
#include "consolehistory.h"
#include "console.h"
#include "consoleinputline.h"

#include <algorithm>

using namespace std;

ConsoleHistory::ConsoleHistory()
{
#if 0
    M_input_line = new ConsoleInputLine();
#endif
    // the history is empty -> see getHistoryIndex
    M_history_output_index = -1;

    //! the history command search is inactive
    M_search_command_mode = SCM_Inactive;
}

ConsoleHistory::~ConsoleHistory()
{
#if 0
    delete M_input_line;
#endif
}
#if 0
void
ConsoleHistory::processInput(const BaseInputDevice::Input& input)
{
    // is the action a button press?
    if ((input.type == BaseInputDevice::IDT_BUTTON) &&
        BaseInputDevice::KeyDown(input))
    {
        if (M_search_command_mode == SCM_Active)
        {
            M_search_command_mode = SCM_Inactivating;
        }
        else
        {
            M_search_command_mode = SCM_Inactive;
        }
    }

    // pipe the input through our inputline
    M_input_line->ProcessInput(input);
}
#endif
#if 0
string 
ConsoleHistory::getInputLineContent() const
{
    return M_input_line->getInput();
}

int
ConsoleHistory::getInputLineCursorPos() const
{
    return M_input_line->getCursorPosition();
}
#endif
const ConsoleHistory::History&
ConsoleHistory::getHistory() const
{
    return M_history;
}

int
ConsoleHistory::getHistoryIndex() const
{
    return M_history_output_index;
}

void
ConsoleHistory::setHistoryIndex(int index)
{
    M_history_output_index = index;
}

int
ConsoleHistory::getHistoryCommandIndex() const
{
    return M_history_command_index;
}

void
ConsoleHistory::setHistoryCommandIndex(int index)
{
    M_history_command_index = index;
}

void
ConsoleHistory::printCommand(const std::string& output)
{
    M_history_command.push_back(output);
    printInput(output);
}

void
ConsoleHistory::printInput(const std::string& output)
{
    printLines(OP_Input, output);
}

void
ConsoleHistory::printOutput(const std::string& output)
{
    printLines(OP_Output, output);
}

void
ConsoleHistory::printError(const std::string& output)
{
    printLines(OP_Error, output);
}

void
ConsoleHistory::printWarning(const std::string& output)
{
    printLines(OP_Warning, output);
}

void
ConsoleHistory::printNormal(const std::string& output)
{
    printLines(OP_Normal, output);
}

void
ConsoleHistory::printDebug(const std::string& output)
{
    printLines(OP_Debug, output);
}

void
ConsoleHistory::printLines(ConsoleHistory::OutputType type,
                           const std::string& output)
{
    Entry entry;
    entry.type = type;

    // does the history point to the last history line, 
    // i.e. is the user scrolling?
    bool history_is_not_scrolled = 
        (M_history_output_index == (M_history.size() - 1));

    // split the output at the linebreaks
    string::size_type left_position = 0;
    string::size_type right_position = output.find("\n", left_position);
    while (right_position != string::npos)
    {
        entry.content = 
            output.substr(left_position, right_position - left_position);
        M_history.push_back(entry);

        left_position = right_position + 1;
        right_position = output.find("\n", left_position);
    }

    // get the last entry
    entry.content = output.substr(left_position, right_position-left_position);
    // ignore a lonely newline
    if (entry.content != "")
    {
        M_history.push_back(entry);
    }

    // update the history pointer: 
    // it remains at the end or it remains at the scrolled position
    if (history_is_not_scrolled)
    {
        M_history_output_index = M_history.size() - 1;
    }
}

void
ConsoleHistory::historyUp()
{
    // scroll only if enough lines can be shown
    if (M_history_output_index >=
        (unsigned int) Console::instance().getSettings().getHistoryLines())
    {
        --M_history_output_index;
    }
}

void
ConsoleHistory::historyDown()
{
    if (M_history_output_index < (M_history.size() - 1))
    {
        ++M_history_output_index;
    }
}

void
ConsoleHistory::historyFirst()
{
    // set the history at the end of the shown output
    M_history_output_index = 
        Console::instance().getSettings().getHistoryLines() - 1;

    // if the output history is to small, jump to its end
    if (M_history_output_index >= M_history.size())
    {
        M_history_output_index = M_history.size() - 1;
    }
}

void
ConsoleHistory::historyLast()
{
    M_history_output_index = M_history.size() - 1;
}

void
ConsoleHistory::initCommandSearch()
{
    // check the current search command mode
    if (M_search_command_mode == SCM_Inactive)
    {
        // initialize the search
        // copy the inputline lowercase to M_search_command_pattern
        M_search_command_pattern = M_input_line->getInput();
        transform(M_search_command_pattern.begin(), 
                  M_search_command_pattern.end(),
                  M_search_command_pattern.begin(), tolower);

        // set the command index to the end of the command history
        M_history_command_index = M_history_command.size();
    }

    // set the search command mode to active
    M_search_command_mode = SCM_Active;
}

bool 
ConsoleHistory::comparePatternWithCommandHistoryAt(int index)
{
    string compare_part =
        M_history_command[index].substr(0, M_search_command_pattern.size());

    transform(compare_part.begin(), compare_part.end(), compare_part.begin(),
              tolower);

    return compare_part == M_search_command_pattern;
}

void
ConsoleHistory::commandBackwards()
{
    initCommandSearch();

    // backwards search -> take a step back in the command history
    int current_index = M_history_command_index - 1;

    // traverse through the history till a match is found
    while (current_index >= 0)
    {
        // compare the pattern with the cut entry from the command history
        if (comparePatternWithCommandHistoryAt(current_index))
        {
            // success -> set the history command pointer to this 
            // index and replace the input line
            M_history_command_index = current_index;

            M_input_line->setInput(M_history_command[current_index].c_str());

            return;
        }

        // failure -> try the previous entry
        --current_index;
    }

    // no previous matching entry found ->
    // we already point at the first matching entry in the history 
    // and change nothing
}

void
ConsoleHistory::commandForwards()
{
    initCommandSearch();

    // forwards search -> take a step forward in the command history
    unsigned int current_index = M_history_command_index + 1;

    while (current_index < M_history_command.size())
    {
        // compare the pattern with the cut entry from the command history
        if (comparePatternWithCommandHistoryAt(current_index))
        {
            // success -> set the history command pointer to this 
            // index and replace the input line
            M_history_command_index = current_index;

            M_input_line->setInput(M_history_command[current_index].c_str());

            return;
        }

        // failure -> try the next entry
        ++current_index;
    }

    // no matching entry found -> we are at the start of the search
    // replace the input line with the search pattern
    M_history_command_index = M_history_command.size();
    M_input_line->setInput(M_search_command_pattern.c_str());
}
