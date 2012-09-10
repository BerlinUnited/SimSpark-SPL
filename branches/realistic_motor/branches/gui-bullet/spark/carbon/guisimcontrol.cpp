/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

   This file is part of Carbon
   Author: Patrick Geib
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

#include "guisimcontrol.h"
#include "isimcontrol.h"
#include "cutelogger\logger.h"

//--------------------------------------------------------------
// Public
//--------------------------------------------------------------

GuiSimControl::GuiSimControl()
{
}

GuiSimControl::~GuiSimControl()
{
}

void GuiSimControl::registerSimControl(boost::shared_ptr<ISimControl> simControl)
{
    bool found = false;
    for (auto it = mSimControlNodes.begin(); it != mSimControlNodes.end(); it++)
    {
        if (&**it == &*simControl)
        {
            found = true;
            break;
        }
    }

    if (!found)
    {
        LOG_INFO() << "Registered Gui-SimControl.";
        mSimControlNodes.push_back(simControl);
    }
    else
    {
        LOG_INFO() << "Gui-SimControl " << &*simControl << " already registered.";
    }
}

void GuiSimControl::unregisterSimControl(boost::shared_ptr<ISimControl> simControl)
{
    unregisterSimControl(&*simControl);
}

void GuiSimControl::unregisterSimControl(ISimControl* simControl)
{
    bool found = false;
    for (auto it = mSimControlNodes.begin(); it != mSimControlNodes.end(); it++)
    {
        if (&**it == simControl)
        {
            mSimControlNodes.erase(it);
            found = true;
            break;
        }
    }

    if (!found)
        LOG_WARNING() << "Cant unregister Gui-SimControl node " << simControl << ": not found.";
    else
        LOG_INFO() << "Unregistered Gui-SimControl.";
}

//--------------------------------------------------------------
// Protected
//--------------------------------------------------------------

void GuiSimControl::OnLink()
{
    LOG_INFO() << "OnLink.";
}

//--------------------------------------------------------------
// Private
//--------------------------------------------------------------

void GuiSimControl::InitSimulation() {for (auto it = mSimControlNodes.begin(); it != mSimControlNodes.end(); it++) (*it)->initSimulation();}
void GuiSimControl::DoneSimulation() {for (auto it = mSimControlNodes.begin(); it != mSimControlNodes.end(); it++) (*it)->doneSimulation();}
void GuiSimControl::StartCycle()     {for (auto it = mSimControlNodes.begin(); it != mSimControlNodes.end(); it++) (*it)->startCycle()    ;}
void GuiSimControl::SenseAgent()     {for (auto it = mSimControlNodes.begin(); it != mSimControlNodes.end(); it++) (*it)->senseAgent()    ;}
void GuiSimControl::ActAgent()       {for (auto it = mSimControlNodes.begin(); it != mSimControlNodes.end(); it++) (*it)->actAgent()      ;}
void GuiSimControl::EndCycle()       {for (auto it = mSimControlNodes.begin(); it != mSimControlNodes.end(); it++) (*it)->endCycle()      ;}
void GuiSimControl::WaitCycle()      {for (auto it = mSimControlNodes.begin(); it != mSimControlNodes.end(); it++) (*it)->waitCycle()     ;}