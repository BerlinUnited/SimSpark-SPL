/* -*-c++-*- ***************************************************************
                           conset.h
    Console Command Set
                           ------------------------
    begin                : Aug 23 2002  Oliver Obst
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

#ifndef UTILITY_CONSET_H
#define UTILITY_CONSET_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "concommand.h"

/*! \class ConSet 
  $Id: conset.h,v 1.1 2002/08/23 14:04:13 fruit Exp $

    ConSet - Console Command Set

    Sets the value of a variable registered to the ConVarServer.
    Defines the variable, if it does not already exist.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ConSet : public ConCommand
{
public:
    ConSet();

    /*! reduces the list of ConVars to consider in combinations,
        e.g. after a CVG_CON_SET_COLOR only a CVG_CONST_COLOR is
        expected.
     */
    virtual bool complete(const StringList& parameters,
                          StringList& arguments) const;

protected:
    //! sets a ConVar of the ConVarServer
    virtual ConExecResult executeSignature(int signature,
                                           ConVar::ConVars& parameter) const;

    //! prints all variables to the console
    virtual ConExecResult displayVariables() const;
    //! prints one variable to the console
    virtual ConExecResult displayVariable(ConVar::ConVars& parameter) const;

    //! sets first argument to the bool value contained in the second argument
    virtual ConExecResult setBool(ConVar::ConVars& parameter) const;
    //! tries to set the first variable to the value of the second
    virtual ConExecResult setVariable(ConVar::ConVars& parameter) const;

    virtual ConExecResult createBool(ConVar::ConVars& parameter) const;
    virtual ConExecResult createInt(ConVar::ConVars& parameter) const;
    virtual ConExecResult createFloat(ConVar::ConVars& parameter) const;
    virtual ConExecResult createString(ConVar::ConVars& parameter) const;
    virtual ConExecResult createVector(ConVar::ConVars& parameter) const;
    virtual ConExecResult createVariable(ConVar::ConVars& parameter) const;

    virtual void examineAFailedSet(ConVar* variable) const;
};

#endif                          // UTILITY_CONSET_H
