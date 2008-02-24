/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id: sparkcontextevent.cpp,v 1.1 2008/02/24 16:20:23 rollmark Exp $

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include "sparkcontextevent.h"

const wxEventType wxEVT_REQUEST = wxNewEventType();

SparkContextEvent::SparkContextEvent(int id) : wxNotifyEvent(wxEVT_REQUEST, id)
{
    mType = T_INVALID;
}

SparkContextEvent::SparkContextEvent(int id, EType type, const wxString& str) : wxNotifyEvent(wxEVT_REQUEST, id)
{
    mType = type;
    SetString(str);
}

SparkContextEvent::~SparkContextEvent()
{
}

wxEvent* SparkContextEvent::Clone() const
{
    return new SparkContextEvent(GetId(), mType);
}

void SparkContextEvent::SetType(EType type)
{
    mType = type;
}

SparkContextEvent::EType SparkContextEvent::GetType() const
{
    return mType;
}
