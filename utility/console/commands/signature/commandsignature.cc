/* -*-c++-*- ***************************************************************
                           commandsignature.cc
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

#include "commandsignature.h"
#include "signatures.h"

CommandSignature::CommandSignature()
{
}

CommandSignature::~CommandSignature()
{
    for (ConComSignature::iterator iter = M_signature.begin();
         iter != M_signature.end(); ++iter)
    {
        delete(*iter);
    }
}


CommandSignature*
CommandSignature::add(ComSigBase* type)
{
    M_signature.push_back(type);

    return this;
}

bool
CommandSignature::validate(const ConVar::ConVars& parameters) const
{
    ConComSignature::const_iterator signature_iter = M_signature.begin();
    ConVar::ConVars::const_iterator parameters_iter = parameters.begin();
    for (;
         (signature_iter != M_signature.end()) &&
         (parameters_iter != parameters.end());)
    {
        // a special case: we allow any parameters from this point 
        // on for an elliptic signature element
        if ((*signature_iter)->isElliptic())
        {
            return true;
        }
        // the simplest case: the parameter matches the iterator
        else if ((*signature_iter)->validate(*parameters_iter))
        {
            ++signature_iter;
            ++parameters_iter;
        }
        // so the parameter was not valid.
        // is it in this signature optional?
        else if ((*signature_iter)->isOptional())
        {
            // yes, so skip it
            ++signature_iter;
        }
        // so we have a clear mismatch -> failure
        else
        {
            return false;
        }
    }

    // if we got here, all checked parameters matched

    // Therefore skip any optional and elliptic parameters
    for (; signature_iter != M_signature.end();)
    {
        // if an signature element is elliptic, we can skip the rest
        if ((*signature_iter)->isElliptic())
        {
            return true;
        }

        // is the parameter in this signature optional?
        if ((*signature_iter)->isOptional())
        {
            ++signature_iter;
        }
        else
        {
            return false;
        }
    }

    // so let's see if we have checked all parameters or if we 
    // have to much parameters
    if (parameters_iter != parameters.end())
    {
        return false;
    }

    // so we checked all parameters and the whole signature
    return true;
}
