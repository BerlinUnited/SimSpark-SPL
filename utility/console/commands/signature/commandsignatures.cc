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
#include "commandsignatures.h"

using namespace std;

CommandSignatures::CommandSignatures()
{
}

CommandSignatures::~CommandSignatures()
{
    for (ConComSignature::iterator iter = M_signature.begin();
         iter != M_signature.end(); ++iter)
    {
        delete *iter;
    }
}


void
CommandSignatures::add(CommandSignature* signature)
{
    M_signature.push_back(signature);
}

int
CommandSignatures::validate(ConVar::ConVars parameters) const
{
    for (unsigned int i = 0; i < M_signature.size(); ++i)
    {
        if (M_signature[i]->validate(parameters))
        {
            return i;
        }
    }

    return -1;
}
