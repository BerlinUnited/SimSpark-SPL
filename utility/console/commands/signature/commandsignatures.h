/* -*-c++-*- ***************************************************************
                           commandsignatures.h
    Contains the complete signature of a command
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
#ifndef UTILITY_COMMANDSIGNATURES_H
#define UTILITY_COMMANDSIGNATURES_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "commandsignature.h"

/*! \class CommandSignatures
  $Id: commandsignatures.h,v 1.1 2002/08/21 08:04:14 fruit Exp $

    CommandSignatures

    Contains the complete signature of a command, i.e. a list of
    CommandSignature s.

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class CommandSignatures
{
public:
    /*! sets an empty signature, i.e. validate will only be true on 
        an empty parameter list.
    */
    CommandSignatures();
    ~CommandSignatures();

    //! add a type to the signature
    void add(CommandSignature* signature);

    /*! @returns the number of the first matching signature.

        Counting starts with 0, returns -1 if no signature matches.
    */
    int validate(ConVar::ConVars parameters) const;

protected:
    //! the container type of the signature
    typedef std::vector < CommandSignature* > ConComSignature;

    //! the signature
    ConComSignature M_signature;
};

#endif // UTILITY_COMMANDSIGNATURES_H
