/* -*-c++-*- ***************************************************************
                                 comsigbase.h
                           ------------------------
    begin                : Aug 20 2002  Oliver Obst
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
#ifndef UTILITY_COMSIGBASE_H
#define UTILITY_COMSIGBASE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <variables/convar.h>

/*! \class ComSigBase
  $Id: comsigbase.h,v 1.1 2002/08/21 08:04:14 fruit Exp $

    ComSigBase

    HISTORY:
    The console subsystem was taken from a student project at the AI
    Research Group, Koblenz University. Original development by Marco
    Koegler <koegler@uni-koblenz.de>, Markus Rollmann
    <rollmark@uni-koblenz.de>, Alexander Fuchs <alexf@uni-koblenz.de>,
    et.al.
*/
class ComSigBase
{
public:
    ComSigBase(bool optional = false);

    //! does the ConVar match the requirements of this parameter?
    virtual bool validate(const ConVar* parameter) const = 0;

    /*! is this parameter type optional, 
        i.e. if validate fails should we simply test the parameter on
        the next element of the signature?
    */
    virtual void setOptional(bool optional);
    virtual bool isOptional() const;

    //! returns false except for ComSigElliptic
    virtual bool isElliptic() const;

protected:
    //! is this parameter convertible to a variable of the given type?
    virtual bool validateConvertibleTo(const ConVar* parameter,
                                       ConVar::ConVarType type) const;
    //! is this parameter a label and convertible to a given type?
    virtual bool validateLabelConvertibleTo(const ConVar* parameter,
                                            ConVar::ConVarType type) const;

    //! is a pameter of this type in the current signature optional?
    bool M_optional;
};

#endif                           // UTILITY_COMSIGBASE_H
