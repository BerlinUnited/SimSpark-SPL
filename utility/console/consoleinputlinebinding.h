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
#ifndef UTILITY_INPUTLINEBINDINGCONSOLE_H
#define UTILITY_INPUTLINEBINDINGCONSOLE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "consoleinputline.h"
#include <inputlinebinding.h>    // from input server

/*! \class ConsoleInputLineBinding
  $Id: consoleinputlinebinding.h,v 1.1 2002/08/21 14:11:16 fruit Exp $

    ConsoleInputLineBinding

    The bindings of a key to an action of ConsoleInputLine.  Does the
    same as the base class InputLineBindings, but the action is not
    bound to a method of InputLine but to a method of
    ConsoleInputLine. To hide this from the user the methods Execut
    and Set are overriden.
  
    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConsoleInputLineBinding:public InputLineBinding
{
protected:
    //! a pointer to a method of ConsoleInputLine
    typedef void (ConsoleInputLine::*ActionConsole) ();

public:
    ConsoleInputLineBinding();
    //! calls set with the parameters taken from initBinding and action
    ConsoleInputLineBinding(const InputLineInitBinding& init_binding,
                            ActionConsole action);

    /*! same behaviour as in InputLineBinding except that a method
        of ConsoleInputLine is executed instead of one restricted to
        InputLine
    */
    virtual void execute(InputLine* input_line);
    /*! same behaviour as in InputLineBinding except that a method of
	ConsoleInputLine is set instead of one restricted to InputLine
    */
    virtual void set(bool control, bool alt, bool shift, int key_code,
                     ActionConsole action);

protected:
    /*! which action should be executed? 
        M_action of InputLineBinding is not used. 
    */
    ActionConsole M_action_console;
};

#endif                           // UTILITY_INPUTLINEBINDINGCONSOLE_H
