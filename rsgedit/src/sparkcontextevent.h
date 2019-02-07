/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-
   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2003 Koblenz University
   $Id$

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
#ifndef SPARKCONTEXTEVENT_H__
#define SPARKCONTEXTEVENT_H__

#include <wx/event.h>
#include <wx/string.h>

extern const wxEventType wxEVT_REQUEST;

class SparkContextEvent : public wxNotifyEvent
{
public:
    enum EType
        {
            T_INVALID,
            T_SELECTION_CHANGED
        };

public:
    SparkContextEvent(int id);
    SparkContextEvent(int id, EType type, const wxString& str = wxString());

    virtual ~SparkContextEvent();

    void SetType(EType type);
    EType GetType() const;

    virtual wxEvent *Clone() const;

protected:
    EType mType;
};

typedef void (wxEvtHandler::*wxSparkContextEventFunction)(SparkContextEvent& event);

#define EVT_SPARK_CONTEXT(id, fn) \
  DECLARE_EVENT_TABLE_ENTRY( \
                            wxEVT_REQUEST, id, -1, \
                             (wxObjectEventFunction)(wxEventFunction)(wxSparkContextEventFunction)&fn, \
                             (wxObject *) NULL \
                             ),

#endif // SPARKCONTEXTEVENT_H__
