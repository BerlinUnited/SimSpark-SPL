/* -*-c++-*- ***************************************************************
                           consoleinputlinebinding.h
    bindings of a key to an action of ConsoleInputLine
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

#include "consoleinputlinebinding.h"

ConsoleInputLineBinding::ConsoleInputLineBinding() : M_action_console(0)
{
};

ConsoleInputLineBinding::ConsoleInputLineBinding(const InputLineInitBinding& 
                                                 init_binding,
                                                 ActionConsole action)
{
    set(init_binding.getControl(), 
        init_binding.getAlt(), 
        init_binding.getShift(),
        init_binding.getKeyCode(), action);
};

void
ConsoleInputLineBinding::execute(InputLine* input_line)
{
    if (M_action_console != 0)
    {
        ((*((ConsoleInputLine *) input_line)).*(M_action_console)) ();
    }
    else
    {
        smux.warning() << "ConsoleInputLineBinding::execute: "
                       << "M_action_console points to NULL.\n";
    }
}

void
ConsoleInputLineBinding::set(bool control, bool alt, bool shift, int key_code,
                             ActionConsole action)
{
    M_control = control;
    M_alt = alt;
    M_shift = shift;
    M_key_code = key_code;

    M_action_console = action;
}
