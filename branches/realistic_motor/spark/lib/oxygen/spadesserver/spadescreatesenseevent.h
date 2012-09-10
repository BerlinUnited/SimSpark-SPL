/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   this file is part of rcssserver3D
   Fri May 9 2003
   Copyright (C) 2002,2003 Koblenz University
   Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
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
#ifndef OXYGEN_SPADESCREATESENSEEVENT_H
#define OXYGEN_SPADESCREATESENSEEVENT_H

#include <spades/CreateSenseEvent.hpp>
#include <oxygen/oxygen_defines.h>

namespace oxygen
{
#if 0
}
#endif

/** SpadesCreateSenseEvent implements the spades::CreateSenseEvent
    interface. It's purpose is to create a SenseEvent and to schedule
    it's successor SpadesCreateEvent.

    A note about the implementation: SpadesCreateSenseEvent uses the
    SpadesServer and the GameControlServer to do it's job. However it
    cannot be zeitgeist class, as this involves that the
    corresponding class object holds a shared pointer to every
    instance of this class.

    However spades is not aware of these shared pointers and destructs
    a registered ParamStorer instance using delete without notifying
    the shared pointers. This would result in dangling references.
*/
class OXYGEN_API SpadesCreateSenseEvent : public spades::CreateSenseEvent
{
 public:
  SpadesCreateSenseEvent (spades::SimTime t, spades::AgentID a)
    : CreateSenseEvent (t, a) {}

  virtual ~SpadesCreateSenseEvent () {}

  virtual void Print (std::ostream & o) const;
  virtual bool realizeEventWorldModel(spades::WorldModel* pWM);

  virtual spades::SenseEvent* createSense(spades::WorldModel* p);
};

} // namespace oxygen


#endif // OXYGEN_SPADESCREATESENSEEVENT_H
