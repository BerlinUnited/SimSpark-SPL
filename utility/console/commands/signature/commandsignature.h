/* -*-c++-*- ***************************************************************
                           commandsignature.h
    Contains one signature of a command
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
#ifndef UTILITY_COMMANDSIGNATURE_H
#define UTILITY_COMMANDSIGNATURE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <variables/convar.h>

class ComSigBase;

/*! \class ComandSignature
  $Id: commandsignature.h,v 1.1 2002/08/21 08:04:14 fruit Exp $

    ComandSignature

    Contains one signature of a command, i.e. a parameter list of
    ConVars may be checked against this signature.

    If the signature is empty an empty parameter list is excepted.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class CommandSignature
{
public:
    /*! sets an empty signature, i.e. validate will only be true on 
        an empty parameter list 
    */
    CommandSignature();

    ~CommandSignature();

    /*! Add a type to the signature.

        The matching is greedy, i.e. if an optional parameter matches
        it is never skipped.
    */
    CommandSignature* add(ComSigBase* type);

    /*! Are the parameters valid for this signature?

        An empty signature returns true on an empty parameter list.
    */
    bool validate(const ConVar::ConVars& parameters) const;

protected:
    //! the container type of a signature
    typedef std::list < ComSigBase* > ConComSignature;

    //! the signature
    ConComSignature M_signature;
};

#endif // UTILITY_COMMANDSIGNATURE_H
