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
#ifndef KEROSIN_SCANCODEMAP_H
#define KEROSIN_SCANCODEMAP_H

/*      \class ScanCodeMap
        $Id$

        ScanCodeMap

        This object can be used to translate from characters to scancodes and vice
        versa. It is necessary to allow for country specific keyboard layouts.

        NOTE:

        HISTORY:
                30.09.02 - MK
                        - Initial version

        TODO:

        TOFIX:
*/

#include <map>
#include "inputserver.h"
#include <kerosin/kerosin_defines.h>

namespace kerosin
{

class KEROSIN_API ScanCodeMap
{
    //
    // types
    //
public:
    // character representations of the scancode this struct is associated with
    // the altModifier is only really used on German keyboards
    struct TScanCodeEntry
    {
        std::string name;               // name of the key
        char            noModifier;     // no modifiers         '<'
        char            shiftModifier;  // shift modifier       '>'
        char            altModifier;    // alt modifier         '|'

        void Set(const std::string &keyName, char noMod, char shiftMod, char altMod)
        {
            name = keyName;
            noModifier = noMod;
            shiftModifier = shiftMod;
            altModifier = altMod;
        }
    };
protected:
    typedef std::map<Input::TInputCode, TScanCodeEntry*> TScanCodeEntryMap;
    //
    // functions
    //
public:
    ScanCodeMap();
    ~ScanCodeMap();

    Input::TInputCode GetCode(const std::string &name) const;
    bool TranslateCode(Input::TInputCode code, unsigned long state, char &ch) const;

    void Reset();
    void AddCode(Input::TInputCode ic, const std::string &name, char noMod, char shiftMod, char altMod);
protected:
    //! this is the map which the translation process is based on
    TScanCodeEntryMap       mScanCodes;
};

} //namespace kerosin

#endif //KEROSIN_SCANCODEMAP_H
