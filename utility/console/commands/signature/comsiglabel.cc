/* -*-c++-*- ***************************************************************
                           comsigany.cc
                           ------------------------
    begin                : Aug 21 2002  Oliver Obst
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

#include "comsiglabel.h"
#include <variables/convar.h>

#include <algorithm>

using namespace std;

ComSigLabel::ComSigLabel(bool optional) : ComSigBase(optional)
{
}

ComSigLabel::ComSigLabel(const string& label, bool optional) :
    ComSigBase(optional)
{
    M_labels.push_back(label);
}

ComSigLabel::ComSigLabel(const StringList& label_list, bool optional) :
    ComSigBase(optional), M_labels(label_list)
{
}

bool
ComSigLabel::validate(const ConVar* parameter) const
{
    // the parameter must be a label
    string value;
    if (!parameter->getLabel(value))
    {
        return false;
    }

    // if no label is defined, any label is valid
    if (M_labels.size() == 0)
    {
        return true;
    }

    // find a matching label
    return (find(M_labels.begin(), M_labels.end(), value) != M_labels.end());
}
