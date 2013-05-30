/* -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil -*-

this file is part of rcssserver3D
Fri May 9 2003
Copyright (C) 2002,2003 Koblenz University
Copyright (C) 2003 RoboCup Soccer Server 3D Maintenance Group
$Id: scancodemap.cpp 3 2008-11-21 02:38:08Z hedayat $

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
#include "scancodemap.h"

using namespace kerosin;

ScanCodeMap::ScanCodeMap()
{
}

ScanCodeMap::~ScanCodeMap()
{
}

Input::TInputCode ScanCodeMap::GetCode(const std::string &name) const
{
    // loop through all entries in the map and find the one, which matches the symbolic name
    for (TScanCodeEntryMap::const_iterator i = mScanCodes.begin(); i != mScanCodes.end(); ++i)
        {
            if ((*i).second->name == name) return (*i).first;
        }

    return 0;
}

bool ScanCodeMap::TranslateCode(Input::TInputCode code,
                                unsigned long state, char &ch) const
{
    if (
        (state & Input::eShift) &&
        (state & Input::eAlt)
        )
        {
            return false;
        }

    TScanCodeEntryMap::const_iterator entry = mScanCodes.find(code);

    if (entry != mScanCodes.end())
        {

            if (state & Input::eShift)
                {
                    ch = (*entry).second->shiftModifier;
                    return ch != 0;
                }

            if (state & Input::eAlt)
                {
                    ch = (*entry).second->altModifier;
                    return ch != 0;
                }

            ch = (*entry).second->noModifier;
            return ch != 0;
        }

    return false;
}

void ScanCodeMap::Reset()
{
    for (
         TScanCodeEntryMap::iterator i = mScanCodes.begin();
         i != mScanCodes.end();
         ++i
         )
        {
            delete (*i).second;
        }
}

void ScanCodeMap::AddCode(Input::TInputCode ic, const std::string &name,
                          char noMod, char shiftMod, char altMod)
{
    TScanCodeEntry *entry = new TScanCodeEntry();
    entry->Set(name, noMod, shiftMod, altMod);
    mScanCodes[ic] = entry;
}
