/* -*-c++-*- ***************************************************************
                           consolesettings.cc
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

#include "consolesettings.h"
#include "console.h"
#include "variables/convarserver.h"
#include "variables/convarcallback.h"

#include <forwarder.h>

using namespace std;
using namespace Utility;

ConsoleSettings::ConsoleSettings(const std::ostream* stream,
                                 InitInfo* init_info)
{
    M_callback = new ConVarCallBackConsole();

    // set the initial console settings
    initSettings(init_info);

    // initialize the call back mappings
    initCallBack();

    // create the console variables which provide access to the settings
    initVars(stream);
}

ConsoleSettings::~ConsoleSettings()
{
    delete M_callback;
}

void
ConsoleSettings::defaultInitInfo(ConsoleSettings::InitInfo& init_info)
{
    init_info.show_warning = true;
    init_info.show_error = true;
    init_info.show_debug = true;
    init_info.history_lines = 20;
    init_info.text_lines = -1;
    init_info.show_var_signature = false;
    init_info.prompt = "> ";
    init_info.color_warning = COLOR_RED;
    init_info.color_error = COLOR_RED;
    init_info.color_debug = COLOR_MAGENTA;
    init_info.color_normal = COLOR_BLUE;
    init_info.color_output = COLOR_GREEN;
    init_info.color_input = COLOR_YELLOW;
    init_info.color_interactive = COLOR_WHITE;
    init_info.color_prompt = COLOR_WHITE;
    init_info.color_cursor = COLOR_RED;
}

void
ConsoleSettings::initSettings(InitInfo* init_info)
{
    InitInfo info;
    if (init_info != 0)
    {
        info = *init_info;
    }
    else
    {
        defaultInitInfo(info);
    }

    M_show_warning = info.show_warning;
    M_show_error = info.show_error;
    M_show_debug = info.show_debug;
    M_show_var_signature = info.show_var_signature;
    M_history_lines = info.history_lines;
    M_text_lines = info.text_lines;
    M_prompt = info.prompt;
    M_color_warning = info.color_warning;
    M_color_error = info.color_error;
    M_color_debug = info.color_debug;
    M_color_normal = info.color_normal;
    M_color_output = info.color_output;
    M_color_input = info.color_input;
    M_color_interactive = info.color_interactive;
    M_color_prompt = info.color_prompt;
    M_color_cursor = info.color_cursor;
    M_base_path = info.basePath;
}

void
ConsoleSettings::initCallBack()
{
    // these
    M_callback_map[CS_CALL_BACK_WARNING] = &ConsoleSettings::updateWarning;
    M_callback_map[CS_CALL_BACK_ERROR] = &ConsoleSettings::updateError;
    M_callback_map[CS_CALL_BACK_DEBUG] = &ConsoleSettings::updateDebug;
}

void
ConsoleSettings::initVars(const std::ostream* stream)
{
    initVarsMessages(stream);
    initConstantsColor();
    initVarsColor();
    initVarsRepresentation();
    initVarsMisc();
}

void
ConsoleSettings::initVarsMessages(const std::ostream* stream)
{
    ConVarAttributes attr;
    attr.setConstant(false);
    attr.setRemovable(false);
    attr.setGroup(ConVarAttributes::S_CVG_CON_SET);

    attr.setHelp("should warning messages be printed to the console?");
    attr.setName("cs_show_warning");
    attr.setCallBack(M_callback, CS_CALL_BACK_WARNING);
    ConVarServer::instance().createRefBool(attr, &M_show_warning);
    // reinitialize the forwarder on this settings
    updateOnCallBack(CS_CALL_BACK_WARNING, stream);

    attr.setHelp("should error messages be printed to the console?");
    attr.setName("cs_show_error");
    attr.setCallBack(M_callback, CS_CALL_BACK_ERROR);
    ConVarServer::instance().createRefBool(attr, &M_show_error);
    updateOnCallBack(CS_CALL_BACK_ERROR, stream);

    attr.setHelp("should debug messages be printed to the console?");
    attr.setName("cs_show_debug");
    attr.setCallBack(M_callback, CS_CALL_BACK_DEBUG);
    ConVarServer::instance().createRefBool(attr, &M_show_debug);
    updateOnCallBack(CS_CALL_BACK_DEBUG, stream);

}

void
ConsoleSettings::initConstantsColor()
{
    ConVarAttributes attr;
    attr.setConstant(true);
    attr.setRemovable(false);
    attr.setGroup(ConVarAttributes::S_CVG_CONST_COLOR);
    attr.setHelp("A predefined RGB constant. "
                 "Usefull e.g. with cs_color_xxx variables.");

    attr.setName("cc_color_red");
    ConVarServer::instance().createVector(attr, COLOR_RED);

    attr.setName("cc_color_green");
    ConVarServer::instance().createVector(attr, COLOR_GREEN);

    attr.setName("cc_color_blue");
    ConVarServer::instance().createVector(attr, COLOR_BLUE);

    attr.setName("cc_color_yellow");
    ConVarServer::instance().createVector(attr, COLOR_YELLOW);

    attr.setName("cc_color_magenta");
    ConVarServer::instance().createVector(attr, COLOR_MAGENTA);

    attr.setName("cc_color_cyan");
    ConVarServer::instance().createVector(attr, COLOR_CYAN);

    attr.setName("cc_color_white");
    ConVarServer::instance().createVector(attr, COLOR_WHITE);
}

void
ConsoleSettings::initVarsColor()
{
    ConVarAttributes attr;
    attr.setConstant(false);
    attr.setRemovable(false);
    attr.setGroup(ConVarAttributes::S_CVG_CON_SET);
    attr.addGroup(ConVarAttributes::S_CVG_CON_SET_COLOR);

    attr.setHelp("the color of warning messages.\nA RGB value which can "
                 "e.g. be set with the cc_color_xxx constants.");
    attr.setName("cs_color_warning");
    ConVarServer::instance().createRefVector(attr, &M_color_warning);

    attr.setHelp("the color of error messages.\nA RGB value which can "
                 "e.g. be set with the cc_color_xxx constants.");
    attr.setName("cs_color_error");
    ConVarServer::instance().createRefVector(attr, &M_color_error);

    attr.setHelp("the color of debug messages.\nA RGB value which can "
                 "e.g. be set with the cc_color_xxx constants.");
    attr.setName("cs_color_debug");
    ConVarServer::instance().createRefVector(attr, &M_color_debug);

    attr.setHelp("the color of standard messages.\nA RGB value which can "
                 "e.g. be set with the cc_color_xxx constants.");
    attr.setName("cs_color_normal");
    ConVarServer::instance().createRefVector(attr, &M_color_normal);

    attr.setHelp("the color of output generated by the console.\nA RGB value "
                 "which can e.g. be set with the cc_color_xxx constants.");
    attr.setName("cs_color_output");
    ConVarServer::instance().createRefVector(attr, &M_color_output);

    attr.setHelp("the color of old user input displayed in the "
                 "history.\nA RGB value which can e.g. be set with "
                 "the cc_color_xxx constants.");
    attr.setName("cs_color_input");
    ConVarServer::instance().createRefVector(attr, &M_color_input);

    attr.setHelp("the color of the current user input line.\nA RGB value which"
                 " can e.g. be set with the cc_color_xxx constants.");
    attr.setName("cs_color_interactive");
    ConVarServer::instance().createRefVector(attr, &M_color_interactive);

    attr.setHelp("the color of the prompt.\nA RGB value which can e.g. be "
                 "set with the cc_color_xxx constants.");
    attr.setName("cs_color_prompt");
    ConVarServer::instance().createRefVector(attr, &M_color_prompt);

    attr.setHelp("the color of the console's cursor.\nA RGB value which can "
                 "e.g. be set with the cc_color_xxx constants.");
    attr.setName("cs_color_cursor");
    ConVarServer::instance().createRefVector(attr, &M_color_cursor);
}

void
ConsoleSettings::initVarsRepresentation()
{
    ConVarAttributes attr;
    attr.setConstant(false);
    attr.setRemovable(false);
    attr.setGroup(ConVarAttributes::S_CVG_CON_SET);

    attr.setHelp("should set print the signature of a "
                 "variable on its display?");
    attr.setName("cs_show_var_signature");
    ConVarServer::instance().createRefBool(attr, &M_show_var_signature);

    attr.setHelp("how many lines of the output history should be displayed?");
    attr.setName("cs_history_lines");
    ConVarServer::instance().createRefInt(attr, &M_history_lines);

    attr.setHelp("into how many lines should the output window be divided?\n"
                 "-1 tries to find a line number where the fonts look sharp.");
    attr.setName("cs_text_lines");
    ConVarServer::instance().createRefInt(attr, &M_text_lines);

    attr.setHelp("the string to display in front of the user input.");
    attr.setName("cs_prompt");
    ConVarServer::instance().createRefString(attr, &M_prompt);
}

void
ConsoleSettings::initVarsMisc()
{
    ConVarAttributes attr;
    attr.setConstant(false);
    attr.setRemovable(false);
    attr.setGroup(ConVarAttributes::S_CVG_CON_SET);

    attr.setHelp("the base path for path relative operations "
                 "(e.g. command Export).");
    attr.setName("cs_base_path");
    ConVarServer::instance().createRefString(attr, &M_base_path);
}

bool
ConsoleSettings::getShowWarning() const
{
    return M_show_warning;
}

void
ConsoleSettings::setShowWarning(bool value)
{
    M_show_warning = value;
}

bool
ConsoleSettings::getShowError() const
{
    return M_show_error;
}

void
ConsoleSettings::setShowError(bool value)
{
    M_show_error = value;
}

bool
ConsoleSettings::getShowDebug() const
{
    return M_show_debug;
}

void
ConsoleSettings::setShowDebug(bool value)
{
    M_show_debug = value;
}

bool
ConsoleSettings::getShowVarSignature() const
{
    return M_show_var_signature;
}

void
ConsoleSettings::setShowVarSignature(bool value)
{
    M_show_var_signature = value;
}

int
ConsoleSettings::getHistoryLines() const
{
    return M_history_lines;
}

void
ConsoleSettings::setHistoryLines(int value)
{
    M_history_lines = value;
}

int
ConsoleSettings::getTextLines() const
{
    return M_text_lines;
}

void
ConsoleSettings::setTextLines(int value)
{
    M_text_lines = value;
}

string
ConsoleSettings::getPrompt() const
{
    return M_prompt;
}

void
ConsoleSettings::setPrompt(const string& value)
{
    M_prompt = value;
}

Vector3
ConsoleSettings::getColorWarning() const
{
    return M_color_warning;
}

void
ConsoleSettings::setColorWarning(const Vector3& value)
{
    M_color_warning = value;
}

Vector3
ConsoleSettings::getColorError() const
{
    return M_color_error;
}

void
ConsoleSettings::setColorError(const Vector3& value)
{
    M_color_error = value;
}

Vector3
ConsoleSettings::getColorDebug() const
{
    return M_color_debug;
}

void
ConsoleSettings::setColorDebug(const Vector3& value)
{
    M_color_debug = value;
}

Vector3
ConsoleSettings::getColorNormal() const
{
    return M_color_normal;
}

void
ConsoleSettings::setColorNormal(const Vector3& value)
{
    M_color_normal = value;
}

Vector3
ConsoleSettings::getColorOutput() const
{
    return M_color_output;
}

void
ConsoleSettings::setColorOutput(const Vector3& value)
{
    M_color_output = value;
}

Vector3
ConsoleSettings::getColorInput() const
{
    return M_color_input;
}

void
ConsoleSettings::setColorInput(const Vector3& value)
{
    M_color_input = value;
}

Vector3
ConsoleSettings::getColorInteractive() const
{
    return M_color_interactive;
}

void
ConsoleSettings::setColorInteractive(const Vector3& value)
{
    M_color_interactive = value;
}

Vector3
ConsoleSettings::getColorPrompt() const
{
    return M_color_prompt;
}

void
ConsoleSettings::setColorPrompt(const Vector3& value)
{
    M_color_prompt = value;
}

Vector3
ConsoleSettings::getColorCursor() const
{
    return M_color_cursor;
}

void
ConsoleSettings::setColorCursor(const Vector3& value)
{
    M_color_cursor = value;
}

string
ConsoleSettings::getBasePath() const
{
    return M_base_path;
}

void
ConsoleSettings::setBasePath(const string& value)
{
    M_base_path = value;
}

void
ConsoleSettings::updateOnCallBack(int index, const std::ostream* stream)
{
    switch (index)
    {
        case CS_CALL_BACK_WARNING:
        {
            updateWarning(stream);
            break;
        }
        case CS_CALL_BACK_ERROR:
        {
            updateError(stream);
            break;
        }
        case CS_CALL_BACK_DEBUG:
        {
            updateDebug(stream);
            break;
        }
        default:
            smux.warning() << "ConVarCallBackConsole::CallBack: called with "
                           << "index of a not existing call back function.\n";
    }
}

void
ConsoleSettings::updateForwarder()
{
    updateWarning();
    updateError();
    updateDebug();
}

void
ConsoleSettings::update(int activate, int priority_mask,
                        const std::ostream* stream) const
{
    const std::ostream* s = stream == 0 ? &smux : stream;
    
    int priority = smux.getPriorityMask(s);

    if (activate)
    {
        priority |= priority_mask;
    }
    else
    {
        priority &= ~priority_mask;
    }

    smux.setPriorityMask(s,priority);
}

void
ConsoleSettings::updateWarning(const std::ostream* stream) const
{
    update(getShowWarning(), Forwarder::S_WARNING, stream);
}

void
ConsoleSettings::updateError(const std::ostream* stream) const
{
    update(getShowError(), Forwarder::S_ERROR, stream);
}

void
ConsoleSettings::updateDebug(const std::ostream* stream) const
{
    update(getShowDebug(), Forwarder::S_DEBUG, stream);
}
